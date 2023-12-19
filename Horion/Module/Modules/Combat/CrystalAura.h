#pragma once
#include "../../../../Utils/Target.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class CrystalAura : public IModule {
public:
	CrystalAura();
	~CrystalAura();

	int placeDelay = 0;
	int attackDelay = 0;
	int packets = 1;
	float range = 6.f;
	bool autoPlace = true;
	bool multiPlace = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onEnable() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet);
};