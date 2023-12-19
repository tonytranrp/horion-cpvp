#pragma once
#include "../Module.h"

class Fly : public IModule {
private:
	float speed = 1.5f;
	SettingEnum selectedFlyMode;
public:
	enum class FlyMode : uint8_t {
		Vanilla = 0,
		Motion = 1,
		CubeGlide = 2,
	};

 	Fly();

	virtual std::string getModeText() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;

	inline FlyMode getFlyMode() {
		return (FlyMode)(this->selectedFlyMode.GetSelectedEntry().GetValue());
	}
};
