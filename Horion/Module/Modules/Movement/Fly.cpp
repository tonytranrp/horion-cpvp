#include "Fly.h"
#include "../../../../Utils/MoveUtils.h"

Fly::Fly() : IModule(0, Category::MOVEMENT, "Enables fly like in creative Mode") {
	this->registerEnumSetting("Mode", &this->selectedFlyMode, (int32_t)FlyMode::Vanilla);
	this->selectedFlyMode.addEntry(EnumEntry("Vanilla", (int32_t)FlyMode::Vanilla));
	this->selectedFlyMode.addEntry(EnumEntry("Motion", (int32_t)FlyMode::Motion));
	this->selectedFlyMode.addEntry(EnumEntry("CubeGlide", (int32_t)FlyMode::CubeGlide));
	this->registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 3.f);
}

const char* Fly::getModuleName() {
	return ("Fly");
}

std::string Fly::getModeText() {
	switch (this->getFlyMode()) {
	case FlyMode::Vanilla: return "Vanilla";
	case FlyMode::Motion: return "Motion";
	case FlyMode::CubeGlide: return "CubeGlide";
	default: return std::string{};
	}
}

void Fly::onTick(GameMode* gm) {
	auto lp = Game.getLocalPlayer();
	if (!lp) {
		return;
	}

	auto input = Game.getGameSettingsInput();

	switch (this->getFlyMode()) {
	case FlyMode::Vanilla: {
		lp->setFlying(true);
		break;
	}
	case FlyMode::Motion: {
		lp->stateVector->velocity = Vec3(0, 0, 0);
		if (GameData::isKeyDown(*input->spaceBar.key))
			lp->stateVector->velocity.y += speed;
		if (GameData::isKeyDown(*input->sneak.key))
			lp->stateVector->velocity.y -= speed;

		MoveUtils::SetMotion(speed, true, false);
		break;
	}
	case FlyMode::CubeGlide: {
		speed = 0.31f;

		Vec3 pos = *lp->getPos();
		pos.y -= 0.3f;
		{
			MovePlayerPacket pkt(lp, pos);
			pkt.onGround = false;
			Game.getLoopbackPacketSender()->sendToServer(&pkt);
		}
		lp->setOnGround(false);

		lp->stateVector->velocity = Vec3{0, 0, 0};
		if (GameData::isKeyDown(*input->spaceBar.key))
			lp->stateVector->velocity.y += speed;
		if (GameData::isKeyDown(*input->sneak.key))
			lp->stateVector->velocity.y -= speed;

		MoveUtils::SetMotion(speed, true, false);
		break;
	}
	default:
		break;
	}
}

void Fly::onDisable() {
	auto lp = Game.getLocalPlayer();
	if (!lp) return;

	if (this->getFlyMode() == FlyMode::Vanilla) {
		if (!lp->isCreative()) {
			lp->setFlying(false);
		}
		return;
	}

	lp->stateVector->velocity = Vec3(0, 0, 0);
}
