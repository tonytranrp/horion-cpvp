#include "AutoDisconnect.h"

AutoDisconnect::AutoDisconnect() : IModule(0, Category::PLAYER, "Disconnects player from game apon reaching a certain health value") {
	this->registerIntSetting("Current Health", &this->HealthSlider, this->HealthSlider, 1, 19);
}

AutoDisconnect::~AutoDisconnect() {
}

const char* AutoDisconnect::getModuleName() {
	return "AutoDisconnect";
}