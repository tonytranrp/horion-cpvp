#pragma once

#include "../Module.h"
#include <cstdint>

enum class Perspective : int32_t {
	UNDEFINED = -1, // not a real enumeration
	FIRST_PERSON = 0,
	THIRD_PERSON_BACK = 1,
	THIRD_PERSON_FRONT = 2,
};

// if u start freelooking in 3rd person back it snaps back when u exit freelook :/
class Freelook : public IModule {
	Perspective* perspectiveSetting = nullptr;  // for caching purposes, we assume the address never changes for the lifetime of mc
	Perspective oldPerspective = Perspective::UNDEFINED;

	void setPerspectiveOptionPtr();

public:
	Vec2 initialViewAngles{};

	Freelook();

	virtual const char* getModuleName() override;
	virtual bool isFlashMode() override { return true; }
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual bool callWhenDisabled() override { return true; }
};
