#include "Speed.h"
#include "../../../../Utils/MoveUtils.h"

Speed::Speed() : IModule(VK_NUMPAD2, Category::MOVEMENT, "Speed Up!") {
	mode = (*new SettingEnum(this))
			   .addEntry(EnumEntry("Motion", 0))
			   .addEntry(EnumEntry("Boost", 1))
			   .addEntry(EnumEntry("Nether", 2));
	registerEnumSetting("Type", &mode, 0);
	registerFloatSetting("speed", &this->speed, 1, 0.1f, 3.f);
	registerFloatSetting("velocity", &this->boost, 2.5f, 2.5f, 5.0f);
}

Speed::~Speed() {
}

const char* Speed::getModuleName() {
	return ("Speed");
}
std::string Speed::getModeText() {
	return mode.GetSelectedEntry().GetName();
}
void Speed::onTick(GameMode* gm) {
	auto* player = Game.getLocalPlayer();
	if (!player || moduleMgr->getModule<Fly>("Fly")->isEnabled()) return;

	switch (mode.selected) {
	default:
		break;
	case 0: {
		MoveUtils::SetMotion(speed, true, false);
	} break;
	case 1: {
		if (player->isOnGround()) {
			this->stage++;
			switch (stage) {
			case 1:
				player->stateVector->velocity.x *= this->boost;
				player->stateVector->velocity.z *= this->boost;
				break;
			case 2:
				player->stateVector->velocity.x *= 0.795f;
				player->stateVector->velocity.z *= 0.795f;
				break;
			case 3:
				player->stateVector->velocity.x *= 0.69f;
				player->stateVector->velocity.z *= 0.69f;
				break;
			case 4:
				player->stateVector->velocity.x *= 0.75f;
				player->stateVector->velocity.z *= 0.75f;
				break;
			case 5:
				this->stage = 0;
			}
		} else {
			this->stage = 1;
		}
	} break;
	case 2: {
		if (player->isOnGround()) {
			player->jumpFromGround();
			player->stateVector->velocity.x *= (1 + this->speed);
			player->stateVector->velocity.z *= (1 + this->speed);
			player->stateVector->velocity.y = 0.01f;
		}
	} break;
	}
}

void Speed::onSendPacket(Packet* packet) {
	//Leaving this here in case its needed
}
