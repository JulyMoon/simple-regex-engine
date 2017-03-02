#include <string>
#include <iostream>

class Regex
{
public:
	Regex(const std::string& regex);

	bool isMatch(const std::string& str);

private:
	std::string regex;

	bool isMatch(const std::string& str, int regexI, int strI);
};

Regex::Regex(const std::string& regex)
{
	this->regex = regex;
}

bool Regex::isMatch(const std::string& str)
{
	return isMatch(str, 0, 0);
}

bool Regex::isMatch(const std::string& str, int regexI, int strI)
{
	if (strI >= str.size() && regexI >= regex.size()) // this means that the regex must match the WHOLE string
		return true;                                  // (equivalent of surrounding the regex with ^ and & in other engines)

	switch (regex[regexI])
	{
	case '?':
		if (isMatch(str, regexI + 1, strI)) // ? is empty
			return true;                    // В задании 25 сказано 'любой символ', но не уточнено, входит ли сюда нулевой символ
		                                    // я все же добавил это условие, т.к. в нормальных regex'ах '?' означает, что символ может быть опущен

		if (isMatch(str, regexI + 1, strI + 1)) // ? is a character
			return true;

		break;

	case '*':
		for (int i = str.size(); i > strI - 1; --i) // the search checks the cases with
			if (isMatch(str, regexI + 1, i))        // the most captured symbols first (aka greedy search)
				return true;

		break;

	case '+':
		for (int i = str.size(); i > strI; --i) // also greedy
			if (isMatch(str, regexI + 1, i))
				return true;

		break;

	case '(':
	{
		int depth = 1;
		int i = regexI;

		while (true)
		{
			if (++i >= regex.size())
				throw "invalid regex (no closing bracket)";

			switch (regex[i])
			{
			case '(': depth++; break;
			case ')': depth--; break;
			}

			if (depth == 0)
			{
				if (i - regexI == 1)
					throw "invalid regex (empty brackets)";

				break;
			}
		}

		switch (regex[i + 1])
		{
		case '*':
			// todo
			break;

		case '+':

			break;

		case '_':

			break;

		default:
			throw "invalid regex (wrong symbol after brackets)";
		}
	}

		break;

	default:
		if (regex[regexI] == str[strI] && isMatch(str, regexI + 1, strI + 1))
			return true;

		break;
	}

	return false;
}

// todo: add group (parentheses) support

int main()
{
	std::string regex;

	std::cout << "Regex: ";
	std::getline(std::cin, regex);

	Regex re(regex);

	while (true)
	{
		std::string str;

		std::cout << "String: ";
		std::getline(std::cin, str);

		if (str == "exit")
			break;

		std::cout << "Result: " << (re.isMatch(str) ? "match found!" : "no match found") << std::endl;
	}

	return 0;
}