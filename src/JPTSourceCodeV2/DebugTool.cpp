#include "stdafx.h"
#include "DebugTool.h"

void safe_printf(const char * s)
{
	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%') {
				++s;
			}
			else {
				throw std::runtime_error("invalid format string: missing arguments");
			}
		}
		std::cout << *s++;
	}
}

void xsprintf(std::string & result, const char * s)
{
	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%') {
				++s;
			}
			else {
				throw std::runtime_error("invalid format string: missing arguments");
			}
		}
		result += *s++;
	}
}
