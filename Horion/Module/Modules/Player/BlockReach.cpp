#include "BlockReach.h"

BlockReach::BlockReach() : IModule(0, Category::PLAYER, "Exteneded reach for placing/breaking blocks") {
	this->registerFloatSetting(std::string("reach"), &this->blockReach, this->blockReach, 7, 20);
}

BlockReach::~BlockReach() {
}

const char* BlockReach::getModuleName() {
	return ("BlockReach");
}