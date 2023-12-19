#pragma once

#include "../Module.h"
#include <SDK/Packet.h>

class Criticals : public IModule {
	PlayerAuthInputPacket lastInputPacket{};

	void sendPacketAsync(/*const*/ PlayerAuthInputPacket& pkt) const;
	void setPrevAuthInputPacket(const PlayerAuthInputPacket& pkt);
public:
	Criticals();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onAttack(Entity*) override;
};