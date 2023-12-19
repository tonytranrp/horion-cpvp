#include "TrueSight.h"

TrueSight::TrueSight() : IModule(0x0, Category::VISUAL, "Allows you to see Invisible Entities") {
}

TrueSight::~TrueSight() {
}

const char* TrueSight::getModuleName() {
	return ("TrueSight");
}