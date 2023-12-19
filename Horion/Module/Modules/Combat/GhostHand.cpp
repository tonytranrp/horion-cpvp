#include "GhostHand.h"

GhostHand::GhostHand() : IModule(0, Category::COMBAT, "Attack through walls!") {
}

GhostHand::~GhostHand() {
}

const char* GhostHand::getModuleName() {
	return ("GhostHand");
}