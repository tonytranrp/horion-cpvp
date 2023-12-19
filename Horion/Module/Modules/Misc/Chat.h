#pragma once
#include "../Module.h"
class Chat : public IModule {
public:
	Chat();
	~Chat();

	SettingEnum chatTypeModes;
	SettingEnum ChatModes;
	SettingEnum prefixModes;
	SettingEnum suffixModes;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onTick(GameMode* gm) override;
	std::string ChangeMeDaddy(std::string* string);
	std::string UWUFY(std::string* string);
	std::string LEETME(std::string* string);
};
