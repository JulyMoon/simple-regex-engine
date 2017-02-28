#include <string>

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
	if (strI >= str.size() || regexI >= regex.size())
		return false;

	switch (regex[regexI])
	{
	case '?':
		if (isMatch(str, regexI + 1, strI)) // ? is empty
			return true;

		if (isMatch(str, regexI + 1, strI + 1)) // ? is a character
			return true;

		break;

	case '*':
		for (int i = str.size() - 1; i >= strI; --i) // the search checks the cases with
			if (isMatch(str, regexI + 1, i))         // the most captured symbols first (aka greedy search)
				return true;
	}

	return false;
}

/*

a*b*c
axxbyyc

*/