#ifndef __S_HELPER__
#define __S_HELPER__

#include <algorithm>
#include <functional>
#include <sstream>
#include <vector>
#include <cctype>
#include <locale>

// Code taken from Stack Overflow
//
// trim functions:
// http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
//
// split functions:
// http://stackoverflow.com/questions/236129/splitting-a-string-in-c

// Trim whitespace from the start of a string
static inline std::string& ltrim(std::string& s)
{
	s.erase(s.begin(), 
			std::find_if(s.begin(), s.end(), std::not1(
					std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// Trim whitespace from the end of a string
static inline std::string& rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), 
				std::not1(std::ptr_fun<int, int>(
						std::isspace))).base(), s.end());
	return s;
}

// Trim whitespace from the beginning and end of a string
static inline std::string& trim(std::string& s)
{
	return ltrim(rtrim(s));
} 

// Split a string using a delimiter
static inline std::vector<std::string>& split(
		const std::string& s, 
		char delim, 
	  	std::vector<std::string>& elems) 
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

// Split a string using a delimiter
static inline std::vector<std::string> split(const std::string& s, 
											 char delim) 
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

#endif
