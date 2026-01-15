#pragma once

#if defined(_WIN32)
	#if defined(SEVEN_WONDERS_CORE_LIBRARY)
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else // macOS
	#define CORE_API __attribute__((visibility("default")))
#endif