#pragma once
#include "../Module.h"

class Nbt : public IModule {
private:
	std::string lastCopy;
	bool prettyFormatting = false;

public:
	Nbt();
	~Nbt();

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
