#include "ChestStealer.h"
#include "../../ModuleManager.h"
#include "../../../../Utils/TimerUtils.h"

ChestStealer::ChestStealer() : IModule(0, Category::WORLD, "Automatically takes all items out of a chest") {
	registerIntSetting("Item Per Second", &this->takeDelay, this->takeDelay, 1, 20);
	registerIntSetting("Close Delay", &this->closeDelay, this->closeDelay, 0, 5);
	registerBoolSetting("enhanced", &this->enhanced, this->enhanced);
}

ChestStealer::~ChestStealer() {
}

const char* ChestStealer::getModuleName() {
	return ("ChestStealer");
}

std::string ChestStealer::getModeText() {
	return std::to_string(takeDelay) + " Item/s";
}

void ChestStealer::ScreenController_tick(ScreenController* c) {
	if (c->isChestContainer()) {
		std::vector<int> items = {};
		static auto invcleanerMod = moduleMgr->getModule<InventoryCleaner>("InvCleaner");
		for (int i = 0; i < 54; i++) {
			ItemStack* stack = c->getContainerItem(i);
			if (stack && stack->item != NULL)
				if (!this->enhanced || invcleanerMod->stackIsUseful(stack))
					items.push_back(i);
		}
		bool closeChest = items.empty() || Game.getInventory()->isFull();
		if (closeChest) {
			if (TimerUtils::hasTimeElapsed(1000.f * closeDelay)) {
				c->tryExit();
			}
		}
		if (!items.empty() && !closeChest) {
			for (int i : items) {
				if (TimerUtils::hasTimeElapsed(1000.f / takeDelay)) {
					c->takeContainerItem(i);
				}
			}
		}
	}
}