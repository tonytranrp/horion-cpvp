#pragma once
#include "../Module.h"
class AntiCrystal : public IModule {
public:
	AntiCrystal();
	~AntiCrystal();

	float posY = 1.f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onSendPacket(Packet* packet) override;
};
