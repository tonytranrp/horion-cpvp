#include "ShulkerNesting.h"

ShulkerNesting::ShulkerNesting() : IModule(0, Category::MISC, "Allows the nesting of shulker boxes") {
	
}

ShulkerNesting::~ShulkerNesting() {
}

const char* ShulkerNesting::getModuleName() {
	return ("ShulkerNesting");
}