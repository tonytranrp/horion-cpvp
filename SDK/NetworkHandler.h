#pragma once

#include "BinaryStream.h"

class RakNetConnector;
class RakNetServerLocator;
class NetworkIdentifier {
	uint8_t pad[0xA0];
};

class NetworkHandler {
	NetworkIdentifier getNetworkIdentifier();
public:
	RakNetConnector *getRakNetConnector();

	void sendRawBinaryStreamToServer(const BinaryStream &stream, PacketID id);
};