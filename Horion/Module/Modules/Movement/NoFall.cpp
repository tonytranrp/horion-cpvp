#include "NoFall.h"
#include "../../ModuleManager.h"
NoFall::NoFall() : IModule(VK_NUMPAD3, Category::MOVEMENT, "Take No Fall Damage") {
	NoFallType.addEntry(EnumEntry("Vanilla", 0));
	NoFallType.addEntry(EnumEntry("NoGround", 1));
	NoFallType.addEntry(EnumEntry("Nukkit", 2));
	registerEnumSetting("Type", &NoFallType, 0);

}

NoFall::~NoFall() {
}

const char* NoFall::getModuleName() {
	return ("NoFall");
}

std::string NoFall::getModeText() {
	return NoFallType.GetSelectedEntry().GetName();
}

void NoFall::onSendPacket(Packet* packet) {
	if (NoFallType.selected == 1) {
		if (Game.getLocalPlayer() && Game.getLocalPlayer()->getFallDistance() > 3.f) {
			if (packet->getId() == PacketID::MovePlayer) {
				MovePlayerPacket* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
				movePacket->onGround = true;
			}
		}
	}
}
void NoFall::onTick(GameMode* gm) {
	auto plr = Game.getLocalPlayer();
	if (!plr) return;
	static auto FlyMod = moduleMgr->getModule<Fly>("Fly");
	static auto JetpackMod = moduleMgr->getModule<Jetpack>("Jetpack");
	static auto NoclipMod = moduleMgr->getModule<NoClip>("NoClip");
	static auto AirJumpMod = moduleMgr->getModule<AirJump>("AirJump");

	bool ShouldNoFall = !(plr->isOnGround() || plr->isCreative() || plr->isInWater() || plr->isOverWater() || plr->isInWall()) &&
						(plr->getFallDistance() > 3 || FlyMod->isEnabled() || JetpackMod->isEnabled() || NoclipMod->isEnabled() || AirJumpMod->isEnabled() && !MoveUtils::isOnground(4.f));

	if (ShouldNoFall && NoFallType.selected != 1) {
		PlayerActionPacket p;
		p.action = NoFallType.selected == 0 ? PlayerActionPacket::Action::Respawn : PlayerActionPacket::Action::StartGlide;
		p.actorRuntimeId = Game.getLocalPlayer()->getRuntimeId();
		Game.getClientInstance()->getLoopbackPacketSender()->sendToServer(&p);
	}
}