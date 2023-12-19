#pragma once
#include "../Module.h"
class TestModule : public IModule {
private:
	rgb color = {255, 255, 0};
	Vec2 tv2 = {10, 10};
	Vec2i tv2i = {10, 10};
	Vec3 tv3 = {10, 10, 10};
	Vec3i tv3i = {10, 10, 10};

public:
	TestModule();
	~TestModule();

	std::string filePath;
	__int64* uniqueID = nullptr;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
};