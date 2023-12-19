#pragma once
#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "../Utils/Logger.h"

class GuiData {
private:
	char pad_0000[48];        // 0x0000
public:
	Vec2 windowSizeReal;    // 0x0030
	Vec2 windowSizeReal2;   // 0x0038
	union {
		struct {
			float widthGame;   // 0x0040
			float heightGame;  // 0x0044
		};
		Vec2 windowSize;  // 0x0040
	};

private:
	char pad_0048[4];         // 0x0048
public:
	float scale;              // 0x004C

	void displayClientMessageVA(const char* fmt, va_list lis);
	void displayClientMessageF(const char* fmt, ...);
	void displayClientMessage(std::string* a2);
};