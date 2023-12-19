#pragma once
#include "../Module.h"
class AutoSneak : public IModule {
private:
public:
	bool doSilent = false;
	AutoSneak();
	~AutoSneak();

	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
};
