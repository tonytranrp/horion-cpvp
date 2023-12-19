#pragma once
#include "../Module.h"

class EntityJesus : public IModule {
private:

public:
	EntityJesus();
	~EntityJesus();
	bool walk = false;
	bool wasInWater = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};