#pragma once
#include "../Module.h"
class Breadcrumbs : public IModule {
public:
	Breadcrumbs();
	~Breadcrumbs();

	std::vector<Vec3> linePoints;
	Vec3 startPos;
	Vec3 startPosTop;

	//settings
	bool clearOndisable = true;
	bool stopBreadCrumbs = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onLevelRender() override;
	virtual void onDisable() override;
};