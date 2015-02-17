#ifndef TOKENIZER_INCLUDED
#define TOKENIZER_INCLUDED

#include <string>

class Tokenizer
{
public:
	Tokenizer(const std::string& text, std::string delimiters)
	{
		m_text = text;
		m_delimiters = delimiters;
		m_startPos = 0;
	}

	bool getNextToken(std::string& result)
	{
		if (m_startPos >= m_text.size())
			return false;

		size_t indexOfDelim = m_text.find_first_of(m_delimiters, m_startPos);
		if (indexOfDelim == std::string::npos)
			indexOfDelim = m_text.size();

		result = m_text.substr(m_startPos, indexOfDelim - m_startPos);
		m_startPos = m_text.find_first_not_of(m_delimiters, indexOfDelim);
		if (m_startPos == std::string::npos)
			m_startPos = m_text.size();
		
		return true;
	}

private:
	std::string m_text;
	std::string m_delimiters;
	size_t m_startPos;

};

#endif // TOKENIZER_INCLUDED
