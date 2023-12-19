#include "HighJump.h"

HighJump::HighJump() : IModule(0, Category::MOVEMENT, "Jump higher than ever before") {
	registerFloatSetting("Jump Height", &this->jumpPower, 1, 0.5f, 5.f);
}

HighJump::~HighJump() {
}

const char* HighJump::getModuleName() {
	return ("HighJump");
}

std::string HighJump::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Power: %.1f", jumpPower);
	return speedChar;
}