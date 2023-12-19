#pragma once
#include "../Module.h"
class InventoryViewer : public IModule {
public:
	InventoryViewer();
	~InventoryViewer();
	Vec2 position = Utils::percentToWindowPos(Vec2(0.123958f, 0.840319f));

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual bool hasPositionSetting() {return true;}
};