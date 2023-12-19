#include "CoordsCommand.h"

CoordsCommand::CoordsCommand() : IMCCommand("coords", "Prints your coordinates", "") {
	registerAlias("pos");
}

CoordsCommand::~CoordsCommand() {
}

bool CoordsCommand::execute(std::vector<std::string>* args) {
	Vec3* pos = Game.getLocalPlayer()->getPos(); //pos->y = eye height
	Utils::setClipboardText(std::string("Current Position:") + " " + std::to_string((int)pos->x) + ", " + std::to_string((int)pos->y - 2) + ", " + std::to_string((int)pos->z));
	clientMessageF("Current Position: %i, %i, %i", (int)pos->x, (int)pos->y - 2, (int)pos->z);
	GuiData* guiData = Game.getGuiData();
	if (guiData) {
		guiData->displayClientMessageF("%sYour Current Location has been saved to the clipboard!", YELLOW);
	}
	return true;
}