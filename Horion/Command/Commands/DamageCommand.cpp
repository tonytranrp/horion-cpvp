#include "DamageCommand.h"

#include "../../Module/ModuleManager.h"

DamageCommand::DamageCommand() : IMCCommand("damage", "Damage yourself (relies on falldamage)", "<amount>") {
	registerAlias("dmg");
}

bool DamageCommand::execute(std::vector<std::string>* args) {
	auto lp = Game.getLocalPlayer();
	assertTrue(lp);

	float amount = assertFloat(args->at(1));
	if (amount < 1.f) {
		clientMessageF("%sPlease put a number bigger than 0", RED);
		return true;
	}

	/*PlayerAuthInputPacket pkt;
	Game.getMoveInputHandler()->fillInputPacket(pkt);
	pkt.pos.y -= 10.f;
	pkt.posDelta.y -= 10.f;
	auto& inputData = *reinterpret_cast<std::bitset<37>*>((uintptr_t)&pkt + 0x68);
	inputData.set(5, true);

	auto& noFallMod = *moduleMgr->getModule<NoFall>("NoFall");
	if (noFallMod.isEnabled()) {
		noFallMod.setEnabled(false);
		Game.getLoopbackPacketSender()->sendToServer((Packet*)&pkt);
		noFallMod.setEnabled(true);
	}
	else {
		Game.getLoopbackPacketSender()->sendToServer((Packet*)&pkt);
	}*/
	return true;
}