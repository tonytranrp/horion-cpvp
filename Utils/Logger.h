#pragma once

#include "xorstr.hpp"

#pragma comment(lib, "runtimeobject")

#ifndef logF
#define logF(x, ...) Logger::WriteLogFileF(xorstr_(x), __VA_ARGS__)
#endif

#ifndef debugLogF

#ifdef HORION_DEBUG
#define debugLogF(x, ...) Logger::WriteLogFileF(xorstr_(x), __VA_ARGS__)
#else
#define debugLogF(x, ...) (void)0
#endif

#endif

struct TextForPrint {
	char time[20];
	char text[100];
};

struct TextForPrintBig {
	char time[20];
	char text[2900];
};

class Logger {
public:
	static bool isActive();
	static std::wstring GetRoamingFolderPath();
	static void WriteLogFileF(volatile char* fmt, ...);
	static void WriteBigLogFileF(size_t maxSize, const char* fmt, ...);
	static void SendToConsoleF(const char* msg);
	static std::vector<TextForPrint>* GetTextToPrint();
	static std::lock_guard<std::mutex> GetTextToPrintLock();
	//static std::vector<TextForPrint*> stringPrintVector;
	static void Disable();
};
