#pragma once

#include "../Module.h"

class NameTags : public IModule {
public:
	std::set<std::string> nameTags;
	bool displayArmor = true;
	bool encaseItems = false;
	float opacity = 0.5f;
	NameTags();

	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;

	static std::string buildPlatformToString(BuildPlatform b);
	static std::string getSanitizedNameTag(Entity& ent);
};
