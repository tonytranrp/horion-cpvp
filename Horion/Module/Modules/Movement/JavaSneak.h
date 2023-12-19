#pragma once
#include "../Module.h"

class JavaSneak : public IModule {
public:
	JavaSneak();
	~JavaSneak();

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPostRender(MinecraftUIRenderContext* ctx) override;
};