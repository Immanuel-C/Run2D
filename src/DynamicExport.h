#pragma once


#if defined(_MSC_VER) && !defined(RN_EXPORT_STATIC)

	#if defined(RN_EXPORT_DLL)
		#define RUN_API __declspec(dllexport)
	#else
		#define RUN_API __declspec(dllimport)
	#endif

#else
	#define RUN_API 
#endif