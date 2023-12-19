#pragma once
#include "../Module.h"

class ArmorHud : public IModule {
public:
	ArmorHud();
	~ArmorHud();
	
	Vec2 position = Utils::percentToWindowPos(Vec2(0.517187f, 0.832335f));
	bool Vertical = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};