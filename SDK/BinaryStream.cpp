#include "BinaryStream.h"

ReadOnlyBinaryStream::ReadOnlyBinaryStream(std::string const &buffer, bool copyBuffer) {
	using readOnlyBinaryStreamConstruct_t = void *(__cdecl *)(ReadOnlyBinaryStream *, std::string const &, bool);
	static auto readOnlyBinaryStreamConstructorFunc = reinterpret_cast<readOnlyBinaryStreamConstruct_t>(
		FindSignature("48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 70 45 0F B6 E0"));

	readOnlyBinaryStreamConstructorFunc(this, buffer, copyBuffer);
}

BinaryStream::BinaryStream() {
	using binaryStreamConstruct_t = void*(__cdecl *)(BinaryStream *);
	static auto binaryStreamConstructorFunc = reinterpret_cast<binaryStreamConstruct_t>(
		FindSignature("48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 50 48 8B F9 48 89 4C 24 40 33 F6"));

	binaryStreamConstructorFunc(this);
}

// https://github.com/pmmp/BedrockProtocol/blob/99c1bb4a9139e17aef4c5ebd1459dcf482e6ea88/src/DataPacket.php#L21
// xref: Packet::writeWithHeader linux 1.14.60
void BinaryStream::prependPacketHeaderMetadata(PacketID id) {
	PacketHeader ph{};
	ph.packetId = (uint32_t)id;
	ph.recipientSubId = 0;
	ph.senderSubId = Game.getLoopbackPacketSender()->clientSubId;
	this->writeUnsignedVarInt(ph.packed);
}