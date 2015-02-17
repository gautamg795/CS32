#ifndef HTTP_INCLUDED
#define HTTP_INCLUDED

// The services this header provides that you're interested in:
//
//  HTTP().set(url, pageContents);
//    Stop using the real Web and establish a pseudo-Web for testing
//    purposes.  Make it so that future calls of get will consult the
//    pseudo-Web, not the real web:  retrieval of the url from the
//    pseudo-Web will return the pageContents.  set can be called multiple
//    times to add multiple url-to-pageContents mappings to the pseudo-Web.
//
//  HTTP().get(url, pageContents)
//    If the web page at the string url can be fetched, set the string
//    pageContents to the content of the page and return true;
//    otherwise return false.  For example,
//        string s = "http://wikipedia.org/wiki/DNA";
//        string text;
//        if (HTTP().get(s, text))
//            cout << text;
//        else
//            cout << "Error fetching " << s << endl;
//    If set has been previously called, get retrieves pages from the
//    pseudo-Web, not the real Web:  If the url was added to the pseudo-web,
//    get sets the string pageContents to the content of the page and returns
//    true; otherwise, it returns false.
//
//  HTTP().normalizeLink(curURL, link)
//    Return a string that represents a normalized form of the link string
//    given the current URL string.  For example,
//        string current = "http://registrar.ucla.edu/catalog/catalog-13-14-41.htm";
//        cout << HTTP().normalizeLink(current, "catalog-curricul.htm");
//         // relative path name, so writes
//         //     http://registrar.ucla.edu/catalog/catalog-curricul.htm
//
//        cout << HTTP().normalizeLink(current, "/schedule");
//         // absolute path name, so writes
//         //     http://registrar.ucla.edu/schedule
//
//        cout << HTTP().normalizeLink(current, "http://wikipedia.org");
//         // fully qualified link, so writes
//         //     http://wikipedia.org

#ifdef _MSC_VER  // Windows

#include <windows.h>
#include <wininet.h>

#else  //  Mac OS X and LINUX

#include <cstdio>
#include <unistd.h>

#endif

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#if __cplusplus >= 201103L  // C++11
# include <unordered_map>
  using std::unordered_map; 
#else
# include <tr1/unordered_map>
  using std::tr1::unordered_map; 
#endif

const int MAX_PAGE_SIZE = 10*1000*1000;

class HTTPController
{
    typedef std::string string;
    typedef unordered_map<string,string> Webmap;

    struct Segment
    {
        Segment(size_t s, size_t ln) : start(s), len(ln) {}
        size_t start;
        size_t len;
    };

public:

      // Meyers singleton pattern
    static HTTPController& getInstance()
    {
        static HTTPController instance;
        return instance;
    }

    void set(string url, const string& pageContents)
    {
        if (url.empty())
            return;

          // Strip off trailing '\r' characters
        url.erase(url.find_last_not_of('\r')+1);
        m_webmap[url] = pageContents;
    }

    bool get(string url, string& pageContents) const
    {
        if (url.empty())
            return false;

          // Strip off trailing '\r' characters
        url.erase(url.find_last_not_of('\r')+1);

        if (!m_webmap.empty())  // using pseudo-Web
        {
            Webmap::const_iterator p = m_webmap.find(url);
            if (p == m_webmap.end())
                return false;
            pageContents = p->second;
            return true;
        }

        if (splitURL(url).scheme.empty())
            url = "http://" + url;

        // std::cerr << "Getting: " << url << std::endl;

        static char buffer[MAX_PAGE_SIZE+1];

        if (!doGet(url, buffer, sizeof(buffer)))
            return false;

        pageContents = buffer;
        return true;
    }

    string normalizeLink(string baseURL, string link)
    {
        URLParts baseParts = splitURL(baseURL);
        if (baseParts.scheme.empty())
        {
            if (baseParts.netLoc.empty())
            {
                baseURL = "http://" + baseURL;
                baseParts = splitURL(baseURL);
            }
            else
            {
                baseURL = "http:" + baseURL;
                baseParts.scheme = "http";
            }
        }

          // Normalization rules from RFC 1808
          // (Should be updated to RFC 3986)

        if (link.empty())
            return baseURL;

        URLParts linkParts = splitURL(link);

        if (!linkParts.scheme.empty())
            return link;

        linkParts.scheme = baseParts.scheme;
        if (linkParts.netLoc.empty())
        {
            linkParts.netLoc = baseParts.netLoc;
            if (linkParts.path.empty())
            {
                linkParts.path = baseParts.path;
                if (linkParts.params.empty())
                {
                    linkParts.params = baseParts.params;
                    if (linkParts.query.empty())
                        linkParts.query = baseParts.query;
                }
            }
            else if (linkParts.path[0] != '/')
            {
                size_t rightSlashPos = baseParts.path.rfind('/');
                if (rightSlashPos != string::npos)
                    linkParts.path.insert(0, baseParts.path, 0, rightSlashPos+1);
                string& path = linkParts.path;
                bool startsWithSlash = (path[0] == '/');
                bool endsWithSlash = (path.size() > 1  &&  path[path.size()-1] == '/');
                if (!endsWithSlash)
                    path += '/';
                std::vector<Segment> segStack;
                enum Action { NOTHING, PUSH, POP };
                Action action;
                size_t start = (startsWithSlash ? 1 : 0);
                while (start != path.size())
                {
                    size_t len = path.find('/', start) - start;
                    action = PUSH;
                    if (path.compare(start, len, ".") == 0)
                        action = NOTHING;
                    else if (path.compare(start, len, "..") == 0  &&  !segStack.empty())
                    {
                        const Segment& s = segStack.back();
                        if (path.compare(s.start, s.len, "..") != 0)
                            action = POP;
                    }
                    switch (action)
                    {
                      case NOTHING:
                        break;
                      case PUSH:
                        segStack.push_back(Segment(start, len));
                        break;
                      case POP:
                        segStack.pop_back();
                        break;
                    }
                    start += len + 1;
                }
                if (action != PUSH  &&  !segStack.empty())
                    endsWithSlash = true;
                string newPath = (startsWithSlash ? "/" : "");
                for (size_t k = 0; k < segStack.size(); k++)
                {
                    if (k > 0)
                        newPath += '/';
                    const Segment& s = segStack[k];
                    newPath.append(path, s.start, s.len);
                }
                if (endsWithSlash  &&  newPath[newPath.size()-1] != '/')
                    newPath += '/';
                linkParts.path = newPath;
            }
        }
        string result = linkParts.scheme + "://"+ linkParts.netLoc;
        if (!linkParts.path.empty())
        {
            if (linkParts.path[0] != '/')
                result += '/';
            result += linkParts.path;
        }
        if (!linkParts.params.empty())
            result += ";" + linkParts.params;
        if (!linkParts.query.empty())
            result += "?" + linkParts.query;
        if (!linkParts.fragment.empty())
            result += "#" + linkParts.fragment;
        return result;
    }

private:
   
#ifdef _MSC_VER
    HINTERNET m_hINet;
#endif

    Webmap m_webmap;

    HTTPController();
    ~HTTPController();
    HTTPController(const HTTPController&);
    HTTPController& operator=(const HTTPController&);

    bool doGet(string url, char buffer[], unsigned long maxLength) const;

    struct URLParts
    {
        URLParts(string s, string nl, string pth, string prm, string q, string f)
         : scheme(s), netLoc(nl), path(pth), params(prm), query(q), fragment(f)
        {}
        string scheme;
        string netLoc;
        string path;
        string params;
        string query;
        string fragment;
    };

    const URLParts splitURL(string url) const
    {
        string scheme;
        string netLoc;
        string path;
        string params;
        string query;
        string fragment;

        size_t fragPos = url.find('#');
        if (fragPos != string::npos)
        {
            fragment = url.substr(fragPos+1);
            url.erase(fragPos); // erases to end of string
        }
        size_t schemePos = url.find(':', 1);
        if (schemePos != string::npos)
        {
            size_t k;
            for (k = 0; k != schemePos; k++)
                if (!isascii(url[k]) || (!isalpha(url[k]) && url[k] != '+' &&
                                             url[k] != '-' && url[k] != '.'))
                    break;
            if (k == schemePos)
            {
                scheme = url.substr(0, schemePos);
                url.erase(0, schemePos+1);
                for (size_t n = 0; n != scheme.size(); n++)
                    scheme[n] = tolower(scheme[n]);
            }
        }
        if (url.compare(0, 2, "//") == 0)
        {
            size_t netLocPos = url.find('/', 2);
            if (netLocPos == string::npos)
            {
                netLoc.assign(url, 2, string::npos);
                url.clear();
            }
            else
            {
                netLoc.assign(url, 2, netLocPos-2);
                url.erase(0, netLocPos);
            }
        }
        size_t queryPos = url.find('?');
        if (queryPos != string::npos)
        {
            query = url.substr(queryPos+1);
            url.erase(queryPos);
        }
        size_t paramsPos = url.find(';');
        if (paramsPos != string::npos)
        {
            params = url.substr(paramsPos+1);
            url.erase(paramsPos);
        }
        return URLParts(scheme, netLoc, url, params, query, fragment);
    }
};

inline HTTPController& HTTP()
{
    return HTTPController::getInstance();
}

#ifdef _MSC_VER  // Windows

inline HTTPController::HTTPController()
{
    m_hINet = InternetOpen("CS32Proj4", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
}

inline HTTPController::~HTTPController()
{
    InternetCloseHandle(m_hINet);
}

inline bool HTTPController::doGet(string url, char buffer[], unsigned long maxLength) const
{
    if (maxLength == 0)
        return false;

    HINTERNET wininetHandle = InternetOpenUrl(m_hINet, url.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0) ;
    if ( wininetHandle == NULL )
        return false;

    bool result;
    for (;;)
    {
        unsigned long bytesRead;
        result = InternetReadFile(wininetHandle, buffer, maxLength-1, &bytesRead) ? true : false;
        if (bytesRead == 0)
        {
            buffer[0] = '\0';
            break;
        }
        buffer += bytesRead;
        maxLength -= bytesRead;
    }
    InternetCloseHandle(wininetHandle);
    return result;
}

#else  //  MacOS and LINUX

inline HTTPController::HTTPController()
{
}

inline HTTPController::~HTTPController()
{
}

inline bool HTTPController::doGet(string url, char buffer[], unsigned long maxLength) const
{
    if (maxLength == 0)
        return false;

    bool isFile = (url.compare(0, 7, "file://") == 0);
    FILE* f;
    if (isFile)
        f = fopen(url.substr(7).c_str(), "r");
    else
    {
        for (size_t k = 0; k < url.size(); k++)
            if (!isascii(url[k]) || !isprint(url[k]) || url[k] == '\'' || url[k] == '\\')
                return false;
        string cmd = "cmd='curl -s'; { /usr/bin/which curl | grep '^[/.~]'; } >/dev/null 2>&1 || "
                     "cmd='wget -q -O -'; $cmd '" + url + "'";
        f = popen(cmd.c_str(), "r");
    }
    if (f == NULL)
        return false;
    size_t length = fread(buffer, 1, maxLength-1, f);
    if (isFile)
        fclose(f);
    else if (pclose(f) != 0)
        return false;
    buffer[length] = '\0';
    return true;
}

#endif // _MSC_VER

#endif // HTTP_INCLUDED
