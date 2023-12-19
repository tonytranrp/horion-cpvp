#include "Spammer.h"

#include "../../../../Utils/Utils.h"
#include "../../../../Utils/TimerUtils.h"

Spammer::Spammer() : IModule(0, Category::MISC, "Spams a message in a specified delay") {
	registerIntSetting("delay", &this->delay, this->delay, 1, 20);
	registerBoolSetting("bypass", &this->bypass, this->bypass);
}

Spammer::~Spammer() {
}

const char* Spammer::getModuleName() {
	return ("Spammer");
}

std::string Spammer::getModeText() {
	if (bypass)
		return "Bypass";
	else
		return "Normal";
}

void Spammer::onTick(GameMode* gm) {
	if (TimerUtils::hasTimeElapsed((float)delay * 1000)) {
		TextPacket textPacket(Game.getLocalPlayer(), bypass ? (message + " | " + Utils::randomString(8)) : message);
		Game.getLoopbackPacketSender()->sendToServer(&textPacket);
	}
}