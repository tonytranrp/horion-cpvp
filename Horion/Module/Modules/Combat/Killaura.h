#pragma once
#include "../../../../Utils/Target.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class Killaura : public IModule {
private:
	int aps = 0;
	bool autoweapon = false;
	void findWeapon();
	void attack(Entity*);
	bool isValidTarget(Entity*);
	bool silent = true;

public:
	bool hasTarget = false;
	Entity* RealEnt;

	SettingEnum RotMode;
	enum struct TRotMode {
		NONE = 0,
		SILENT = 1,
		CLIENT = 2
	};
	SettingEnum RotType;
	enum struct TRotType {
		NORMAL = 0,
		PREDICT = 1,
		AVERAGE = 2
	};
	SettingEnum HitType;
	enum struct THitType {
		MULTI = 0,
		SINGLE = 1,
		SWITCH = 2
	};


	rgb color;
	Vec2 angle;
	bool isMobAura = false;
	bool teams = false;
	bool DisableOnDeath = false;
	bool preferSword = false;
	bool visualRange = false;
	bool noSlow = false;
	bool packet = false;
	float range = 6;
	int targetIndex = 0;

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual std::string getModeText() override;
	virtual const char* getModuleName() override;
	virtual void onSendPacket(Packet* packet) override;
	void doCritical();
	virtual void onLevelRender() override;
	virtual void onDisable() override;
};