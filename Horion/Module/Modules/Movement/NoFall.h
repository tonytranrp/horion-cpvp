#pragma once

#include "../../../DrawUtils.h"
#include "../Module.h"

class NoFall : public IModule {
public:
	NoFall();
	~NoFall();
	SettingEnum NoFallType{this};

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet) override;
};
