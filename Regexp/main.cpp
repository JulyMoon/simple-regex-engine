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
	if (strI >= str.size() && regexI >= regex.size())
		return true;

	switch (regex[regexI])
	{
	case '?':
		if (isMatch(str, regexI + 1, strI))
			return true;

		if (isMatch(str, regexI + 1, strI + 1))
			return true;

		break;

	case '*':
		for (int i = str.size(); i > strI - 1; --i)
			if (isMatch(str, regexI + 1, i))
				return true;

		break;

	case '+':
		for (int i = str.size(); i > strI; --i)
			if (isMatch(str, regexI + 1, i))
				return true;

		break;

	case '(':
	{
		int depth = 1;
		int closingI = regexI;

		while (true)
		{
			if (++closingI >= regex.size())
				throw "invalid regex (no closing bracket)";

			switch (regex[closingI])
			{
			case '(': depth++; break;
			case ')': depth--; break;
			}

			if (depth == 0)
			{
				if (closingI - regexI == 1)
					throw "invalid regex (empty brackets)";

				break;
			}
		}

		switch (regex[closingI + 1])
		{
		case '*': break;
		case '+': break;
		case '_': break;
		default: throw "invalid regex (wrong symbol after brackets)";
		}

		std::string inbetween = str.substr(regexI + 1, closingI - regexI - 1);
		Regex subRegex(inbetween);

		int leftI = strI;
		int matches = 0;
		while (true) // checkpoint loop
		{
			bool matchFound = false;
			int rightI;
			for (rightI = str.size() - 1; rightI >= leftI - 1; --rightI)
			{
				std::string newStr = str.substr(leftI, rightI - leftI + 1);
				if (subRegex.isMatch(newStr, 0, leftI))
				{
					matchFound = true;
					break;
				}
			}

			if (!matchFound)
				break;

			leftI = rightI + 1;
			matches++;
		}

		bool symbolCondition;
		switch (regex[closingI + 1])
		{
		case '*': symbolCondition = true; break;
		case '+': symbolCondition = matches > 0; break;
		case '_': symbolCondition = matches < 2; break;
		default: throw "this should never be thrown";
		}

		return symbolCondition && isMatch(str, closingI + 2, leftI);
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