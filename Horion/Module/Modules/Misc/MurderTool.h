#pragma once
#include <vector>
#include "../../ModuleManager.h"
#include "../Module.h"
class MurderTool : public IModule {
private:
	bool isMurder = false;

public:
	MurderTool();
	~MurderTool();
	float opacity = 1.f;

	// Inherited via IModule
	virtual bool isPlayerMurderer();
	virtual bool doesEntityHaveBow(Entity* ent);
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};