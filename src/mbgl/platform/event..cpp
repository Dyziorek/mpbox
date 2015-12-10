
#include <mbgl/platform/event.hpp>

namespace mbgl {
	::mbgl::EnumValue<EventSeverity> EventSeverity_names[] = {
		{ EventSeverity::Debug, "DEBUG" },
		{ EventSeverity::Info, "INFO" },
		{ EventSeverity::Warning, "WARNING" },
		{ EventSeverity::Error, "ERROR" },
		{ EventSeverity(-1), "UNKNOWN" },
	};
	using EventSeverityClass = ::mbgl::Enum<EventSeverity, EventSeverity_names, sizeof(EventSeverity_names) / sizeof(::mbgl::EnumValue<EventSeverity>)>; \
		inline std::ostream& operator<<(std::ostream& os, EventSeverity t) { return os << EventSeverityClass(t).str(); }


	::mbgl::EnumValue<Event> Event_names[] = {
		{ Event::General, "General" },
		{ Event::Setup, "Setup" },
		{ Event::Shader, "Shader" },
		{ Event::ParseStyle, "ParseStyle" },
		{ Event::ParseTile, "ParseTile" },
		{ Event::Render, "Render" },
		{ Event::Database, "Database" },
		{ Event::HttpRequest, "HttpRequest" },
		{ Event::Sprite, "Sprite" },
		{ Event::Image, "Image" },
		{ Event::OpenGL, "OpenGL" },
		{ Event::JNI, "JNI" },
		{ Event::Android, "Android" },
		{ Event::Crash, "Crash" },
		{ Event(-1), "Unknown" },
	}; \
		using EventClass = ::mbgl::Enum<Event, Event_names, sizeof(Event_names) / sizeof(::mbgl::EnumValue<Event>)>; \
		inline std::ostream& operator<<(std::ostream& os, Event t) { return os << EventClass(t).str(); }

}