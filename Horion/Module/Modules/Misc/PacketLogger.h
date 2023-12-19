#pragma once
#include "../Module.h"
class PacketLogger : public IModule {
public:
	PacketLogger();
	~PacketLogger();
	SettingEnum PacketType{this};
	bool authInput = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
