#pragma once

#include "../Module.h"

class AutoSprint : public IModule {
public:
	SettingEnum selectedSprintMode;

	enum class SprintMode {
		FORWARD_ONLY = 0,
		ALL_DIRECTIONS = 1,
	};

	AutoSprint();

	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onMove(MoveInputHandler* input) override;

	inline SprintMode getSprintMode() {
		return static_cast<SprintMode>(this->selectedSprintMode.GetSelectedEntry().GetValue());
	}
};
