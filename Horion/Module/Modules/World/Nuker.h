#pragma once
#include "../Module.h"

class Nuker : public IModule {
	int32_t delay = 0;
	int32_t nukerRadius = 4;
	bool veinMiner = false;
	bool autodestroy = true;

public:
	Nuker();

	inline bool isAutoMode() { return this->autodestroy; }
	inline int32_t getNukerRadius() { return this->nukerRadius; }
	inline bool isVeinMiner() { return this->veinMiner; }

	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
