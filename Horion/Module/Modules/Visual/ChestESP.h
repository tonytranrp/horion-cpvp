#pragma once

#include "../Module.h"

class ChestESP : public IModule {
private:
	int tickTimeout = 0;
	std::vector<AABB> bufferedChestList;
	std::mutex listLock;
	SettingEnum Mode;
	bool chests = true;
	bool trappedChests = true;
	bool enderChests = false;
	bool barrels = true;
	bool shulkers = true;
	bool doOutline = true;
	bool doShulkerColors = true;

public:
	ChestESP();
	~ChestESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	void onLevelRender() override;
	virtual void onTick(GameMode* gm) override;
};
