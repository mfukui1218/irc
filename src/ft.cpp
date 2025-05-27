#include "ft.hpp"
#include <iostream>

void ft::ltrim(std::string &str, const std::string &set) {
	size_t pos;
	
	pos = str.find_first_not_of(set);
	if (pos == std::string::npos)
		str.clear();
	else
		str.erase(0, pos);
}
