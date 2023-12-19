#include "InvLogger.h"

InvLogger::InvLogger() : IModule(0, Category::MISC, "Logging Transactions !") {
}

InvLogger::~InvLogger() {
}

const char* InvLogger::getModuleName() {
	return ("InvLogger");
}