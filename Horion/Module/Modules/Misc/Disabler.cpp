#include "Disabler.h"

Disabler::Disabler() : IModule(0x0, Category::MISC, "Bypass AC or mitigate servers/certain gamemodes") {
	Mode.addEntry(EnumEntry("Lifeboat", 0));
	Mode.addEntry(EnumEntry("Cubecraft", 1));
	Mode.addEntry(EnumEntry("Mineplex", 2));
	registerEnumSetting("Type", &Mode, 0);
}

Disabler::~Disabler() {
}

const char* Disabler::getModuleName() {
	return ("Disabler");
}

std::string Disabler::getModeText() {
	return Mode.GetSelectedEntry().GetName();
}

void Disabler::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer() || !Game.getRakNetConnector()) return;

	switch (Mode.selected) {
	case 0: {
		MovePlayerPacket p(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());// bypass lifeboat ac for survival, idk about other gamemodes on lifeboat.
		p.onGround = true;
		Game.getLoopbackPacketSender()->sendToServer(&p);
		break;
	}	
	case 1: {
		MovePlayerPacket pk;
		pk.mode = 2;
		pk.onGround = false;
		Game.getLoopbackPacketSender()->sendToServer(&pk);
		break;
	}
	case 2: {
		if (Game.getLocalPlayer()->stateVector->velocity.magnitudexz() > 0.01f) {
			MovePlayerPacket pkt(Game.getLocalPlayer(), *Game.getLocalPlayer()->getPos());  // somehow makes it think u on ground and u dont get kicked for vertical flight for the most part
			pkt.mode = 2;
			pkt.onGround = true;
			Game.getLoopbackPacketSender()->sendToServer(&pkt);
			Game.getLocalPlayer()->getFallDistance() = 0.f;
		}
		break;
	}
	}
}