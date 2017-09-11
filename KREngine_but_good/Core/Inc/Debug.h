#ifndef INCLUDED_CORE_DEBUG_H
#define INCLUDED_CORE_DEBUG_H

#if defined(_CONSOLE)
	#define LOGPRINT printf
#else 
	#define LOGPRINT OutputDebugStringA


#if defined(_DEBUG)
#define LOG(format, ...)\
	{\
		char buffer[1024];\
		int ret = _snprintf_s(buffer, _countof(buffer), _TRUNCATE, (#format), __VA_ARGS__);\
		LOGPRINT(buffer);\
		if (ret == -1) OutputDebugStringA("** message truncated **\n");\
		LOGPRINT("\n");\
	}

#define ASSERT(condition, format, ...)\
	{\
		if (!(condition))\
		{\
			LOG(format, __VA_ARGS__)\
			DebugBreak();\
		}\
	}

#define VERIFY(condition, format, ...)\
	ASSERT(condition, format, __VA_ARGS__)
#else
#define LOG(format, ...)
#define ASSERT(condition, format, ...)
#define VERIFY(condition, format, ...) condition;
#endif
#endif // #if defined(_DEBUG)

#endif // #ifndef INCLUDED_CORE_DEBUG_H