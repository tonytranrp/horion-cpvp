#include "Timer.h"

Timer::Timer() : IModule(0, Category::MISC, "Increase the ticks per second") {
	this->registerIntSetting("ticks per second", &this->timer, this->timer, 0, 500);
}

Timer::~Timer() {
}

const char* Timer::getModuleName() {
	return ("Timer");
}

std::string Timer::getModeText() {
	return "TPS: " + std::to_string(timer);
}

void Timer::onTick(GameMode* gm) {
	*Game.getMinecraft()->timer = static_cast<float>(this->timer);
}

void Timer::onDisable() {
	*Game.getMinecraft()->timer = 20.f;
}