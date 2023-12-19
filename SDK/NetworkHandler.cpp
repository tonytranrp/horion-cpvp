#include "NetworkHandler.h"
#include "../Utils/Utils.h"

// NetworkHandler::getServerId()
NetworkIdentifier NetworkHandler::getNetworkIdentifier() {
	using getServerId_t = NetworkIdentifier*(__cdecl*)(NetworkHandler* _this, NetworkIdentifier* out);
	static auto func = reinterpret_cast<getServerId_t>(FindSignature(
		"48 89 5C 24 ? 48 89 7C 24 ? 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B FA 48 8B D9 E8 ? ? ? ? 84"));
	NetworkIdentifier out;
	func(this, &out);
	return out;
}

// NetworkHandler::_sendInternal(const NetworkIdentifier&, const Packet &, const std::string &)
void NetworkHandler::sendRawBinaryStreamToServer(const BinaryStream& stream, PacketID id) {
	using _sendInternal_t = void(__cdecl*)(NetworkHandler* _this, const NetworkIdentifier& netId, const Packet &dummyPkt, const std::string& payload);
	static auto _sendInternalFunc = reinterpret_cast<_sendInternal_t>(FindSignature(
		"40 53 55 56 57 41 56 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 49 8B E9 49 8B F8 4C 8B F2"));
	auto pkt = Packet::CreatePacket(id);
	_sendInternalFunc(this, this->getNetworkIdentifier(), *pkt, *(stream.bufferWritable));
}

RakNetConnector* NetworkHandler::getRakNetConnector() {  // xref: NetworkHandler::closeConnection
	auto someRandomInterface = Utils::directAccess<void*>(this, 0x58);
	return Utils::directAccess<RakNetConnector*>(someRandomInterface, 0x8);
}