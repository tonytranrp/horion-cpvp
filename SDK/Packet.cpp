#include "Packet.h"
#include "../Utils/Utils.h"

std::string Packet::getName() {
	return *Utils::CallVFunc<2, std::string*>(this, std::string());
}

PacketID Packet::getId() {
	return Utils::CallVFunc<1, PacketID>(this);
}

void Packet::read(ReadOnlyBinaryStream& bs) {
	Utils::CallVFunc<6, void>(this, bs);
}

void Packet::write(BinaryStream* bs) {
	Utils::CallVFunc<3, void>(this, bs);
}

uintptr_t** Packet::GetPacketVTable(PacketID id) {
	auto packet = CreatePacket(id);
	return packet.get()->vTable;
}

std::shared_ptr<Packet> Packet::CreatePacket(PacketID id) {
	using createPacket_t = std::shared_ptr<Packet>(__cdecl*)(PacketID);
	static createPacket_t createPacket = Utils::FuncFromSigOffset<createPacket_t>(FindSignature("E8 ? ? ? ? 90 48 83 BD ? ? ? ? ? 0F ? ? ? ? ? ? ? ? ? ? ? 48 8B D8"), 1);

	return createPacket(id);
}