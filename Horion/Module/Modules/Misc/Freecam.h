#pragma once
#include "../Module.h"
#include "../../../../SDK/Camera.h"
class Freecam : public IModule {
private:
	float speed = 0.325f;

	int* perspectiveSetting = nullptr;
	int oldPerspective;
	bool gotInfo = false;

public:
	Camera* cam = nullptr;

	Freecam();
	~Freecam();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};