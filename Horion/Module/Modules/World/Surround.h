#pragma once
#include "../Module.h"
class Surround : public IModule {

public:
	Surround();
	~Surround();

	bool centre = true;
	bool disableOnComplete = true;
	bool swap = false;
	std::vector<Vec3i> PotentialSurroundBlocks = {
		Vec3i(0, -1, 0),
		Vec3i(1, -1, 0),
		Vec3i(1, 0, 0),
		Vec3i(0, -1, 1),
		Vec3i(0, 0, 1),
		Vec3i(-1, -1, 0),
		Vec3i(-1, 0, 0),
		Vec3i(0, -1, -1),
		Vec3i(0, 0, -1)
	};

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};