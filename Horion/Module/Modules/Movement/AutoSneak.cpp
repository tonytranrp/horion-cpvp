#include "AutoSneak.h"

AutoSneak::AutoSneak() : IModule(0, Category::MOVEMENT, "Automatically sneak without holding the key") {
	this->registerBoolSetting("Silent", &this->doSilent, doSilent);
}

AutoSneak::~AutoSneak() {
}

const char* AutoSneak::getModuleName() {
	return ("AutoSneak");
}

std::string AutoSneak::getModeText() {
	if (doSilent)
		return "Silent";
	else
		return "Vanilla";
}

void AutoSneak::onTick(GameMode* gm) {
	if (!doSilent)
		Game.getMoveInputHandler()->isSneakDown = true;
}

void AutoSneak::onDisable() {
	if (!Game.getLocalPlayer())
		return;

	if (!doSilent) {
		Game.getMoveInputHandler()->isSneakDown = false;
		return;
	}

	PlayerActionPacket p;
	p.action = PlayerActionPacket::Action::StopSneak;
	p.actorRuntimeId = Game.getLocalPlayer()->getRuntimeId();
	Game.getLoopbackPacketSender()->sendToServer(&p);
}

void AutoSneak::onEnable() {
	if (!Game.getLocalPlayer())
		return;  //fixed crash

	if (doSilent) {
		PlayerActionPacket p;
		p.action = PlayerActionPacket::Action::StartSneak;
		p.actorRuntimeId = Game.getLocalPlayer()->getRuntimeId();
		Game.getLoopbackPacketSender()->sendToServer(&p);
	}
}