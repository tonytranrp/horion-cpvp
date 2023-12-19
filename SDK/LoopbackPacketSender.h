#pragma once

#include "Packet.h"
#include "NetworkHandler.h"

class LoopbackPacketSender {
public:
	virtual ~LoopbackPacketSender();
	virtual __int64 send(Packet* packet);
	virtual __int64 sendToServer(Packet* packet);
	virtual __int64 sendToClient(__int64 UserEntityIdentifierComponent, Packet* const&);
	virtual __int64 sendToClient(const void* networkIdentifier, const Packet* packet, int a4);
	virtual __int64 sendToClients(const void* networkIdentifier, const Packet* packet);
	virtual __int64 sendBroadcast(const Packet* packet);
	virtual __int64 sendBroadcast(const void* networkIdentifier, int a3, const Packet* packet);
	virtual __int64 flush(void* networkIdentifier, int a3);

	uint8_t clientSubId; // 0x8
	NetworkHandler* netHandler; // 0x10

	inline NetworkHandler* getNetworkHandler() {
		return this->netHandler;
	}
};