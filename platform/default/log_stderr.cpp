#include <mbgl/platform/log.hpp>

#include <iostream>
#include <sstream>
#include <locale>
#include <cvt/wstring>
#include <codecvt>
#include <windows.h>

namespace mbgl {

void Log::platformRecord(EventSeverity severity, const std::string &msg) {
	std::stringstream stringg;
	stringg << "[" << EventSeverityClass(severity).str() << "] " << msg << std::endl;
	std::string buuftext = stringg.str();
	std::wstring wname;
	stdext::cvt::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	wname = converter.from_bytes(buuftext);
	OutputDebugString(wname.c_str());
}

}
