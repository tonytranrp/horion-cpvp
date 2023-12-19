#include "GuiData.h"

void GuiData::displayClientMessage(std::string *a2) {
	using displayClientMessage = void(__thiscall*)(void*, std::string&);  
	static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 4C 8B F9 45 33 ? 4C 89 64 24"));

	std::string text = std::string(*a2);

	if (displayMessageFunc)
		displayMessageFunc(this, text);
}
void GuiData::displayClientMessageF(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	displayClientMessageVA(fmt, arg);
	va_end(arg);
}
void GuiData::displayClientMessageVA(const char *fmt, va_list lis) {
	auto lengthNeeded = _vscprintf(fmt, lis) + 1;
	if(lengthNeeded >= 300) {
		logF("A message that was %i characters long could not be fitted into the buffer", lengthNeeded);
		return;
	}

	char message[300];
	vsprintf_s(message, 300, fmt, lis);
	std::string msg(message);
	displayClientMessage(&msg);
}