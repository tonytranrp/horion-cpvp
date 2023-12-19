#pragma once
#include "../Module.h"

class JavaInv : public IModule {
public:
	JavaInv();
	~JavaInv();

	bool usePlayerKeybinds = true;
	bool animated = false;
	std::string screenName;
	int hoverSlot = 0;
	int slot = 0;
	bool shouldSwitch = false;
	int timer = 0;

	int key1;
	int key2;
	int key3;
	int key4;
	int key5;
	int key6;
	int key7;
	int key8;
	int key9;

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void ScreenController_tick(ScreenController* c);
};