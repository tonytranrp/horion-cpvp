#include "AntiEffect.h"

AntiEffect::AntiEffect() : IModule(0, Category::PLAYER, "Stop certain potion effects from affecting the player") {
	this->registerBoolSetting("Jump Boost", &this->JumpBoost, this->JumpBoost);
	this->registerBoolSetting("Nausea", &this->Nausea, this->Nausea);
	this->registerBoolSetting("Blindness", &this->Blindness, this->Blindness);
	this->registerBoolSetting("Night Vision", &this->NightVision, this->NightVision);
	this->registerBoolSetting("Levitation", &this->Levitation, this->Levitation);
	this->registerBoolSetting("Slow Falling", &this->SlowFalling, this->SlowFalling);
	this->registerBoolSetting("Darkness", &this->Darkness, this->Darkness);
}

AntiEffect::~AntiEffect() {
}

const char* AntiEffect::getModuleName() {
	return "AntiEffect";
}