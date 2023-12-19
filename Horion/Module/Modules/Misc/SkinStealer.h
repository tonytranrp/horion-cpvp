#pragma once

#include "../Module.h"
#include "../../../../Utils/Utils.h"
#include "../../../../Utils/svpng.h"
#include "../../../../SDK/MinecraftJson.h"

class SkinStealer : public IModule {
private:
	bool includeCapes = false;
	bool includeGeo = false;

public:
	SkinStealer();
	~SkinStealer();

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
