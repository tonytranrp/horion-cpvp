#pragma once
#include "../Module.h"
class Step : public IModule {
private:
	bool reverse = false, packets = false, smoothing = true;
	int smoothTicks = 0;
	int revHeight = 3;

public:
	SettingEnum mode;
	float height = 2;

	Step();
	~Step();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onDisable() override;
};
