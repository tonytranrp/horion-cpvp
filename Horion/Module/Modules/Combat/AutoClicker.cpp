#include "AutoClicker.h"
#include <chrono>

AutoClicker::AutoClicker() : IModule(0, Category::COMBAT, "Advanced autoclicker, automatically clicks for you.") {
	this->registerBoolSetting("Only weapons", &this->weapons, this->weapons);
	this->registerIntSetting("CPS", &this->cps, this->cps, 1, 20);
	clickEnum.addEntry(EnumEntry("Left Click", (int32_t)clickType::LeftClick));
	clickEnum.addEntry(EnumEntry("Right Click", (int32_t)clickType::RightClick));
	clickEnum.addEntry(EnumEntry("Both", (int32_t)clickType::Both));
	registerEnumSetting("Click Type", &clickEnum, 0);
	this->registerBoolSetting("Hold", &this->hold, this->hold);
	this->registerBoolSetting("Swing Sound", &this->sendSwingSound, this->sendSwingSound);

	this->registerFloatSetting("yJitter", &this->xJitter, this->xJitter, 0.f, 5.f);
	this->registerFloatSetting("xJitter", &this->yJitter, this->yJitter, 0.f, 5.f);

	this->registerBoolSetting("RandomDelay", &this->doRand, this->doRand);
	this->registerIntSetting("MinRand", &this->minRand, this->minRand, 1, 20);
	this->registerIntSetting("MaxRand", &this->maxRand, this->maxRand, 1, 20);
}

const char* AutoClicker::getModuleName() {
	return ("AutoClicker");
}

void AutoClicker::onLevelRender() {
	auto plr = Game.getLocalPlayer();
	if (!GameData::canUseMoveKeys() || !plr) {
		return;
	}
	bool isHolding = false;
	bool isLeftClick = Game.isLeftClickDown();
	bool isRightClick = Game.isRightClickDown();
	auto clickInt = this->getClickType();
	switch (this->getClickType()) {
	case clickType::LeftClick:
		isHolding = isLeftClick;
		break;
	case clickType::RightClick:
		isHolding = isRightClick;
		break;
	case clickType::Both:
		// WOOOOOOOO ITS FINALLY BACK
		isHolding = isLeftClick || isRightClick;
		break;
	}
	if (!isHolding && this->hold) {
		return;
	}

	auto ls = plr->getSelectedItem();
	if (this->weapons && (!ls || (ls->getAttackingDamageWithEnchants() < 1.f))) {
		return;
	}

	int32_t attackTime = (this->doRand ? Utils::generateRandomInt<int32_t>(this->minRand, this->maxRand) : this->cps);
	if (!TimerUtils::hasTimeElapsed(1000.f / attackTime)) {
		return;
	}

	auto gm = Game.getGameMode();
	if (xJitter > 0.f || yJitter > 0.f) {
		float randomXJitter = Utils::generateRandomReal<float>(-this->xJitter, this->xJitter);
		float randomYJitter = Utils::generateRandomReal<float>(-this->yJitter, this->yJitter);
		plr->applyTurnDelta(&Vec2(randomXJitter, randomYJitter));
	}
	auto level = Game.getLevel();
	if (clickInt == clickType::RightClick || clickInt == clickType::Both) {
		if (isRightClick || !this->hold) {
			const auto& hitResult = level->getHitResult();
			if (hitResult.type == HitResultType::Tile) {
				plr->swing(); // prevents arm just swinging for fun when staring into the beutiful sky whilst having !this->hold
				Game.getGameMode()->buildBlock(hitResult.block, hitResult.blockFace);
				Game.cpsRight.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			}
		}
	}

	if (clickInt == clickType::LeftClick || clickInt == clickType::Both) {
		if (isLeftClick || !this->hold) {
			plr->swing(); // man fuck these kids
			if (level->isLookingAtEntity()) {
				gm->attack(level->getLookingAtEntity());
				Game.cpsLeft.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			}
		}
	}

	if (this->sendSwingSound) {
		bool actuallySendPkt = false;
		HitResultType hitResult = level->getHitResult().type;
		static LevelSoundEventPacket soundPkt{};
		soundPkt.entityType = "minecraft:player";

		if (hitResult == HitResultType::Miss) {
			soundPkt.sound = LevelSoundEvent::ATTACK_NODAMAGE;
			Vec3 newPos = *plr->getPos();
			newPos.y = plr->aabbShape->aabb.centerPoint().y;  // default swing y offset
			soundPkt.pos = newPos;

			actuallySendPkt = true;
		} else if (hitResult == HitResultType::Entity) {
			soundPkt.sound = LevelSoundEvent::ATTACK_STRONG;
			soundPkt.pos = level->getHitResult().hitpos;

			actuallySendPkt = true;
		}

		if (actuallySendPkt) {
			Game.getLoopbackPacketSender()->sendToServer(&soundPkt);
		}
	}
}
