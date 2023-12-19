#include "ICommand.h"

IMCCommand::IMCCommand(const char* command, const char* description, const char* usage) {
	this->_command = command;
	this->_description = description;
	this->_usage = usage;
	registerAlias(command);
}

IMCCommand::~IMCCommand() {
}

void IMCCommand::clientMessageF(const char* fmt, ...) {
	static char msgBuf[300]{};

	va_list arg;
	va_start(arg, fmt);

	int32_t lengthNeeded = _vscprintf(fmt, arg) + 1;
	if (lengthNeeded >= 300) {
		logF("A message that was %i characters long could not be fitted into the buffer", lengthNeeded);
		return;
	}

	vsprintf_s(msgBuf, sizeof(msgBuf), fmt, arg);
	va_end(arg);

	Game.getGuiData()->displayClientMessageF("%s[%sHorion%s] - %s%s", GRAY, AQUA, GRAY, RESET, msgBuf);
}

void IMCCommand::registerAlias(const char* str) {
	std::string ss = str;
	std::transform(ss.begin(), ss.end(), ss.begin(), ::tolower);
	aliasList.push_back(ss);
}