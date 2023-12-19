#include "Hitbox.h"

Hitbox::Hitbox() : IModule(0, Category::COMBAT, "Increase an entitys hitbox size") {
	this->registerFloatSetting("Height", &this->height, this->height, 1.8f, 10);
	this->registerFloatSetting("Width", &this->width, this->width, 0.6f, 10);
	registerBoolSetting("Include Mobs", &this->includeMobs, this->includeMobs);
}

Hitbox::~Hitbox() {
}

const char* Hitbox::getModuleName() {
	return ("Hitbox");
}

std::string Hitbox::getModeText() {
	char widthChar[50];
	sprintf_s(widthChar, "%.1f", width);
	char heightChar[50];
	sprintf_s(heightChar, "%.1f", height);
	return std::string(widthChar) + " | " + std::string(heightChar);
}

void Hitbox::onTick(GameMode* gm) {
	Game.forEachEntity([&](Entity* ent) {
		if (!Target::isValidTarget(ent, includeMobs, false))
			return;

		float dist = (*ent->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < range) {
			ent->aabbShape->width = width;
			ent->aabbShape->height = height;
		}
	});
}