#pragma once
#include "../Module.h"
class Targethud : public IModule {
private:
	rgb color = {1.f, 1.f, 1.f};

public:
	Targethud();
	~Targethud();

	std::string filePath;
	int64_t uniqueID = -1;
	Vec2 position = Utils::percentToWindowPos(Vec2(0.513542f, 0.426148f));

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual bool hasPositionSetting() { return true; }
};