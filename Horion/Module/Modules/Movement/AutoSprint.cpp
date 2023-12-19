#include "AutoSprint.h"
#include "../../ModuleManager.h"

AutoSprint::AutoSprint() : IModule(0x0, Category::MOVEMENT, "Automatically sprint without holding the key") {
	this->registerEnumSetting("Mode", &this->selectedSprintMode, static_cast<int32_t>(SprintMode::FORWARD_ONLY));
	this->selectedSprintMode.addEntry({"Normal", static_cast<int32_t>(SprintMode::FORWARD_ONLY)});
	this->selectedSprintMode.addEntry({"Omni", static_cast<int32_t>(SprintMode::ALL_DIRECTIONS)});
}

const char* AutoSprint::getModuleName() {
	return ("AutoSprint");
}

std::string AutoSprint::getModeText() {
	return this->selectedSprintMode.GetSelectedEntry().GetName();
}

void AutoSprint::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (input->isSneakDown) return;

	switch (this->getSprintMode()) {
	case SprintMode::FORWARD_ONLY: {
		if (input->forward && !player->isSprinting()) { // prevent spamming sprint flag to server(?)
			player->setSprinting(true);
		}
		break;
	}
	case SprintMode::ALL_DIRECTIONS: {
		auto sprintAttr = player->getAttribute(AttributeID::MovementSpeed);
		if (sprintAttr) {
			constexpr auto hasWASDInput = [](const MoveInputHandler& mih) -> bool {
				return mih.forward || mih.backward || mih.left || mih.right;
			};

			constexpr float vanillaWalkSpeed = 0.1f, vanillaSprintSpeed = 0.13f;
			if (hasWASDInput(*input)) {
				sprintAttr->currentValue = vanillaSprintSpeed;
			}
			else {	
				sprintAttr->currentValue = vanillaWalkSpeed;
			}
	
		}
		else {
#ifdef HORION_DEBUG
			logF("wtf? attribute getter broken...");
			__debugbreak();
#endif
		}
		break;
	}
	default:
		break;
	}
}