#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class AutoClicker : public IModule {
	bool firstTick = true;
	enum class clickType : uint8_t {
		LeftClick = 0,
		RightClick,
		Both,
	};
	int32_t cps = 0;
	bool weapons = true;
	bool hold = false;
	bool sendSwingSound = false;

	SettingEnum clickEnum;
	float xJitter = 2.f, yJitter = 2.f;

	bool doRand = true;
	int32_t minRand = 15, maxRand = 20;

public:
	AutoClicker();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onLevelRender() override;
	inline clickType getClickType() {
		return (clickType)(this->clickEnum.GetSelectedEntry().GetValue());
	}
};
