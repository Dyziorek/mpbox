#include <mbgl/platform/platform.hpp>
#include <mbgl/util/utf.hpp>
#include <cstring>
#include <locale>
#include <cvt/wstring>
#include <codecvt>
#include <iostream>

namespace mbgl { namespace platform {

std::string uppercase(const std::string& str)
{
    std::string output;
	char const *itr = str.c_str();
    char const *end = itr + str.length();
	std::wstringstream wstrm;
	std::wstring wname;
	stdext::cvt::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	wname = converter.from_bytes(str);
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	f.toupper(&wname[0], &wname[0] + wname.size());
	output = converter.to_bytes(wname);

    return output;

}

std::string lowercase(const std::string& str)
{
	std::string output;
	char const *itr = str.c_str();
	char const *end = itr + str.length();
	std::wstringstream wstrm;
	std::wstring wname;
	stdext::cvt::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	wname = converter.from_bytes(str);
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());
	f.tolower(&wname[0], &wname[0] + wname.size());
	output = converter.to_bytes(wname);

	return output;
}

}}
