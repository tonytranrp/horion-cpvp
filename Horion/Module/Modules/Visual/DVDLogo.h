#pragma once
#include "../Module.h"
class DVDLogo : public IModule {
public:
	DVDLogo();
	~DVDLogo();

	float scale = 2.0f;
	Vec2 pos;
	bool positiveX = false;
	bool positiveY = false;
	int colorR = 0;
	int colorG = 0;
	int colorB = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
};