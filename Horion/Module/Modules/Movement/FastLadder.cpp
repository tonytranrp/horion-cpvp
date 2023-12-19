#include "FastLadder.h"

FastLadder::FastLadder() : IModule(0, Category::MOVEMENT, "Climb up ladders faster") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 1.0f);
}

FastLadder::~FastLadder() {
}

const char* FastLadder::getModuleName() {
	return ("FastLadder");
}

std::string FastLadder::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speed);
	return speedChar;
}