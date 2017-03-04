#include <string>
#include <vector>
#include <iostream>

class Regex
{
public:
	Regex(const std::string& regex);

	bool isMatch(const std::string& str);

private:
	std::string regex;

	bool isMatch(const std::string& str, int regexI, int strI);

	void getCapturePermutations(const std::string& str, int leftI, int matches, std::vector<int>& matchesStrI, std::vector<int>& matchCount);
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

		char type = regex[closingI + 1];

		switch (type)
		{
		case '*': break;
		case '+': break;
		case '_': break;
		default: throw "invalid regex (wrong symbol after brackets)";
		}

		std::string inbetween = regex.substr(regexI + 1, closingI - regexI - 1);
		Regex subRegex(inbetween);

		std::vector<int> matchesStrI;
		std::vector<int> matchCount;

		subRegex.getCapturePermutations(str, strI, 0, matchesStrI, matchCount);

		int nextLeftI = closingI + 2;
		switch (type)
		{
		case '*':
			for (int i = 0; i < matchesStrI.size(); ++i)
				if (isMatch(str, nextLeftI, matchesStrI[i]))
					return true;

			return isMatch(str, nextLeftI, strI);

		case '+':
			for (int i = 0; i < matchesStrI.size(); ++i)
				if (isMatch(str, nextLeftI, matchesStrI[i]))
					return true;

			break;

		case '_':
			for (int i = 0; i < matchesStrI.size(); ++i)
				if (matchCount[i] < 2 && isMatch(str, nextLeftI, matchesStrI[i]))
					return true;

			return isMatch(str, nextLeftI, strI);

		default:
			throw "this should never be thrown";
		}
	}

		break;

	default:
		return regex[regexI] == str[strI] && isMatch(str, regexI + 1, strI + 1);

		break;
	}
	return false;
}

void Regex::getCapturePermutations(const std::string& str, int leftI, int matches, std::vector<int>& matchesStrI, std::vector<int>& matchCount)
{
	for (int rightI = leftI - 1; rightI < static_cast<int>(str.size()); ++rightI)
	{
		std::string newStr = str.substr(leftI, rightI - leftI + 1);
		if (isMatch(newStr, 0, 0))
		{
			int newLeftI = rightI + 1;
			int newMatches = matches + 1;
			matchesStrI.push_back(newLeftI);
			matchCount.push_back(newMatches);
			getCapturePermutations(str, newLeftI, newMatches, matchesStrI, matchCount);
		}
	}
}

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