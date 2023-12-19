#pragma once
#include "../Module.h"

class Damage : public IModule {
private:
	bool done = false;

public:
	Damage();
	~Damage();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	void onDisable();
	void onSendPacket(Packet* packet);
};
