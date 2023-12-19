#include "Criticals.h"

Criticals::Criticals() : IModule(0, Category::COMBAT, "Each hit is a critical hit") {}

const char* Criticals::getModuleName() {
	return ("Criticals");
}

// so we dont get recursion with onSendPacket event, ugly workaround :(
void Criticals::sendPacketAsync(/*const*/ PlayerAuthInputPacket& pkt) const {
	static BinaryStream bs{};
	bs.reset();
	bs.prependPacketHeaderMetadata(PacketID::PlayerAuthInput);
	pkt.write(&bs);
	Game.getNetworkHandler()->sendRawBinaryStreamToServer(bs, PacketID::PlayerAuthInput);
}

void Criticals::setPrevAuthInputPacket(const PlayerAuthInputPacket& pkt) {
	this->lastInputPacket = pkt;
	this->lastInputPacket.itemInteractionData = nullptr;
	this->lastInputPacket.itemStackRequestData = nullptr;
	this->lastInputPacket.blockActions.clear();
}

void Criticals::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) return;
	if (moduleMgr->getModule<Speed>("Killaura")->isEnabled()) return;

	auto pktId = packet->getId();
	if (pktId == PacketID::MovePlayer) {
		auto& movePkt = *reinterpret_cast<MovePlayerPacket*>(packet);
		movePkt.onGround = false;
	}
	else if (pktId == PacketID::PlayerAuthInput) {
		const auto& currAuthPkt = *reinterpret_cast<PlayerAuthInputPacket*>(packet);
		this->setPrevAuthInputPacket(currAuthPkt);
	}
}

void Criticals::onAttack(Entity*) {
	auto& pktToModify = this->lastInputPacket;
	if (pktToModify.clientTick >= 5) {  // make sure we have a valid packet cached
		pktToModify.moveVector = {0.f, 0.f};
		pktToModify.setFlag(PlayerAuthInputPacket::InputFlag::Jumping);
		pktToModify.setFlag(PlayerAuthInputPacket::InputFlag::JumpDown);

		this->sendPacketAsync(pktToModify);

		pktToModify.flags.reset();

		for (int32_t i = 0; i < 6; i++) {
			this->sendPacketAsync(pktToModify);
		}
	}
}