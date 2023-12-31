#pragma once
#include "../../../DrawUtils.h"
#include "../Module.h"
class Radar : public IModule {
public:
	Radar();
	~Radar();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};
