#pragma once
#include "../Module.h"
class EditionFaker : public IModule {
public:
	EditionFaker();
	~EditionFaker();

	SettingEnum edition{this};
	int getFakedEditon();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
};
