#pragma once

// base function which stops the recursion.
void safe_printf(const char *s);
void xsprintf(std::string& result, const char *s);


// variadic recursive function
template<typename T, typename... Args>
void safe_printf(const char *s, T value, Args... args);

template<typename T, typename... Args>
void xsprintf(std::string& result, const char *s, T value, Args... args);

template<typename... Args>
void DebugPrint(const char *s, Args... args);

// convert any nummeric typy to string and reverse
template <class T>
T FromString(std::string s);

template <class T>
std::string ToString(T val);

/// main of template
template<typename T, typename ...Args>
inline void safe_printf(const char * s, T value, Args ...args)
{
	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%') {
				++s;
			}
			else {
				std::cout << value;
				safe_printf(s + 1, args...); // call even when *s == 0 to detect extra arguments
				return;
			}
		}
		std::cout << *s++;
	}
	throw std::logic_error("extra arguments provided to printf");
}

template<typename T, typename ...Args>
inline void xsprintf(std::string & result, const char * s, T value, Args ...args)
{
	while (*s) {
		if (*s == '%') {
			if (*(s + 1) == '%') {
				++s;
			}
			else {
				std::stringstream stream;
				stream << value;
				result += stream.str();
				xsprintf(result, s + 1, args...); // call even when *s == 0 to detect extra arguments
				return;
			}
		}
		result += *s++;
	}
	throw std::logic_error("extra arguments provided to printf");
}

template<typename ...Args>
inline void DebugPrint(const char * s, Args ...args)
{
#ifdef _DEBUG
	std::string result = "";
	xsprintf(result, s, args...);

	OutputDebugStringA(result.c_str());
#endif
}

// convert any nummeric typy to string and reverse
template<class T>
inline T FromString(std::string s)
{
	T result;
	std::stringstream str;
	str << s;
	str >> result;
	return result;
}

template<class T>
inline std::string ToString(T val)
{
	std::string s;
	std::stringstream str;
	str << d;
	str >> s;
	return s;
}
