#ifndef TEST_INCLUDED
#define TEST_INCLUDED

#include "Database.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
 
class Test
{
public:
    bool loadAndRunTestScript(std::string filename, std::string& problem)
    {
        std::ifstream scriptf(filename.c_str());
         
        if (!scriptf)
        {
            problem = "cannot open script file " + filename;
            return false;
        }

        std::string line;
        while (std::getline(scriptf, line))
        {
            if (!line.empty())
            {
                if (!parseAndExecute(line, problem))
                    return false;
            }
        }

        return true;
    }

private:

    enum Command {
        cmd_empty, cmd_error, cmd_schema, cmd_url, cmd_file, cmd_add,
        cmd_query_param, cmd_sort_param, cmd_execute
    };

    bool parseAndExecute(std::string line, std::string& problem)
    {
        std::vector<std::string> tokens;    
        Command cmd = tokenize(line, tokens);
        switch (cmd)
        {
            default:
                return false;
            case cmd_empty:
                return true;
            case cmd_schema:
                if (!setSchema(tokens))
                {
                    problem = "invalid schema";
                    return false;
                }
                m_searchCriteria.clear();
                m_sortCriteria.clear();
                return true;
            case cmd_url:
                if (!m_db.loadFromURL(tokens[0]))
                {
                    problem = "problem loading from URL " + tokens[0];
                    return false;
                }
                return true;
            case cmd_file:
                if (!m_db.loadFromFile(tokens[0]))
                {
                    problem = "problem loading from file " + tokens[0];
                    return false;
                }
                return true;
            case cmd_add:
                if (!m_db.addRow(tokens))
                {
                    problem = "row does not match schema";
                    return false;
                }
                return true;
            case cmd_query_param:
                {
                    if (tokens.size() < 3)
                    {
                        problem = "invalid number of query params";
                        return false;
                    }
                    Database::SearchCriterion sc;
                    sc.fieldName = tokens[0];
                    sc.minValue = tokens[1];
                    sc.maxValue = tokens[2];
                    m_searchCriteria.push_back(sc);
                    return true;
                }
            case cmd_sort_param:
                {
                    if (tokens.size() < 2)
                    {
                        problem = "invalid number of sort params";
                        return false;
                    }
                    Database::SortCriterion sc;
                    sc.fieldName = tokens[0];
                    if (tokens[1] == "ascending")
                        sc.ordering = Database::ot_ascending;
                    else if (tokens[1] == "descending")
                        sc.ordering = Database::ot_descending;
                    else
                    {
                        problem = "invalid sort direction for field " + tokens[0];
                        return false;
                    }
                    m_sortCriteria.push_back(sc);
                    return true;
                }
            case cmd_execute:
                {
                    std::vector<int> rows;
                    int result = m_db.search(m_searchCriteria, m_sortCriteria, rows);
                    m_searchCriteria.clear();
                    m_sortCriteria.clear();
                    if (result == Database::ERROR_RESULT)
                    {
                        problem = "error during search";
                        return false;
                    }
                    printRows(rows);
                    std::cout << std::string(60, '-') << std::endl;
                    return true;
                }
        }

        return true;
    }

    void printRows(const std::vector<int>& rowNums) const
    {   
        for (size_t i = 0;i < rowNums.size(); i++)
        {
            std::vector<std::string> row;
            if (m_db.getRow(rowNums[i], row))
            {
                for (size_t i = 0; i < row.size(); i++)
                {
                    if (i != 0)
                        std::cout << ", ";
                    std::cout << row[i];
                }
                std::cout << std::endl;
            }
        }
    }

    bool setSchema(const std::vector<std::string>& tokens)
    {
        std::vector<Database::FieldDescriptor> schema;

        for (size_t i = 0;i < tokens.size(); i++)
        {
            Database::FieldDescriptor fd;

            if (tokens[i].find('*') != std::string::npos)
            {
                fd.name = tokens[i].substr(0,tokens[i].size()-1);
                fd.index = Database::it_indexed;
            }
            else
            {
                fd.name = tokens[i];
                fd.index = Database::it_none;
            }

            schema.push_back(fd);
        }

        return m_db.specifySchema(schema);
    }

    Command tokenize(std::string line, std::vector<std::string>& tokens)
    {
        trim(line);
        if (line.empty())
            return cmd_empty;

        if (line == "execute")
            return cmd_execute;

        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos)
            return cmd_error;

          // Determine command
        std::string command = line.substr(0, colonPos);
        Command cmd;
    
        if (command == "schema")
            cmd = cmd_schema;
        else if (command == "url")
            cmd = cmd_url;
        else if (command == "file")
            cmd = cmd_file;
        else if (command == "add")
            cmd = cmd_add;
        else if (command == "qparam")
            cmd = cmd_query_param;
        else if (command == "sparam")
            cmd = cmd_sort_param;
        else
            return cmd_error;

          // Determine arguments
        std::string args = line.substr(colonPos+1);
        trim(args);

        tokens.clear();
        if (cmd == cmd_url)
            tokens.push_back(args); // no tokenization for URL param
        else
        {
            size_t begin = 0;
            for (;;)
            {
                size_t commaPos = args.find(',', begin);
                if (commaPos == std::string::npos)
                {
                    tokens.push_back(args.substr(begin));
                    break;
                }
                tokens.push_back(args.substr(begin, commaPos - begin));
                begin = commaPos + 1;
            }
        }

        return cmd;
    }

    void trim(std::string& s)  // trim leading and trailing whitespace
    {
        size_t b;
        for (b = 0; b != s.size()  &&  isspace(s[b]); b++)
            ;
        size_t e;
        for (e = s.size(); e != b  &&  isspace(s[e-1]); e--)
            ;
        s = s.substr(b, e-b);
    }

private:
    Database                                m_db;
    std::vector<Database::SearchCriterion>  m_searchCriteria;
    std::vector<Database::SortCriterion>    m_sortCriteria;
};

#endif // TEST_INCLUDED
