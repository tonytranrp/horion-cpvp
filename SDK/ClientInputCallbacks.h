#pragma once
#include "../Utils/Utils.h"
#include "ClientInstance.h"

class ClientInputCallbacks {
public:
	void handleBuildOrInteractButtonPress();
	void handleDestoryOrAttackButtonPress();
	void handleBuildOrDestroyActionButtonRelease();
};