#include "../../../../Utils/MoveUtils.h"
#include "ElytraFly.h"

ElytraFly::ElytraFly() : IModule(0, Category::MOVEMENT, "Control your elytra with magic jap powers") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.2f, 3.f);
}

ElytraFly::~ElytraFly() {
}

const char* ElytraFly::getModuleName() {
	return "ElytraFly";
}

std::string ElytraFly::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speed);
	return speedChar;
}

void ElytraFly::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	//setting some conditions
	bool isInLiquid = player->isInLava() || player->isInWater();
	bool isRightSize = player->aabbShape->height < 1.8f; //only 2 places where your hitbox height goes below 1.8f is when swimming and using the elytra
	bool hasElytraInSlot = player->getArmor(1)->item && player->getArmor(1)->getItem()->itemId == 564;  //elytra check
	bool pressed = input->forward || input->backward || input->right || input->left;

	if (!isInLiquid && isRightSize && hasElytraInSlot) {
		if (player->isOnGround()) return;

		glideModEffective = glideMod;
		if (GameData::isKeyDown(VK_TAB))
			glideModEffective += speed;
		if (GameData::isKeyDown(VK_CONTROL))
			glideModEffective -= speed;

		player->stateVector->velocity = {0.f, 0.f, 0.f};
		player->stateVector->velocity.y = glideModEffective;
		MoveUtils::SetMotion(speed, true, false); // doesnt need pressed cause built in :)
	}
}