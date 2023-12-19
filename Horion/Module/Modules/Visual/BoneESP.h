#pragma once
#include "../Module.h"

class BoneESP : public IModule {
private:
	std::vector<std::shared_ptr<TexturePtr>> textures;
	bool hasInit = false;
	int ind = 0;
	float indOverflow = 0;
	int fps = 30;
	std::chrono::steady_clock::time_point lastRender;

public:
	BoneESP();
	~BoneESP() = default;

	// Inherited via IModule
	void doInit();
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual const char* getModuleName() override;
};
