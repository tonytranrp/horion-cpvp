#pragma once
#include "../Module.h"

class Speed : public IModule {
private:
	float speed = 1.6f;
	float boost = 3.0f;
	int stage = 0;
	int pos = 0;
	SettingEnum mode;

public:
	Speed();
	~Speed();

	// Inherited via IModule
	virtual std::string getModeText() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;

	void onSendPacket(Packet* packet);

};