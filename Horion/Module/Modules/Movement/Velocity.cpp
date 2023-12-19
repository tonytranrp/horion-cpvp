#include "Velocity.h"

Velocity::Velocity() : IModule(0, Category::MOVEMENT, "Don't ever take knockback again") {
	this->registerFloatSetting("Linear Modifier", &this->xModifier, this->xModifier, 0.f, 1.f);
	this->registerFloatSetting("Height Modifier", &this->yModifier, this->yModifier, 0.f, 1.f);
}

Velocity::~Velocity() {
}

const char* Velocity::getModuleName() {
	return ("Velocity");
}

std::string Velocity::getModeText() {
	return std::to_string((int)floorf((xModifier + 0.001f) * 100)) + "% | " + std::to_string((int)floorf((yModifier + 0.001f) * 100)) + "%";
}