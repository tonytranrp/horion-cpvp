#pragma once
#include "../Module.h"
class CameraNoClip : public IModule {
public:
	CameraNoClip();
	~CameraNoClip();

	void onEnable() override;
	void onDisable() override;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
