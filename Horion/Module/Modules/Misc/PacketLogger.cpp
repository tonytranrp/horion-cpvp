#include "PacketLogger.h"

PacketLogger::PacketLogger() : IModule(0, Category::MISC, "Logging Packets !") {
	PacketType.addEntry(EnumEntry("Sent", 0));
	PacketType.addEntry(EnumEntry("Received", 1));
	PacketType.addEntry(EnumEntry("Both", 2));
	registerEnumSetting("Type", &PacketType, 0);
	registerBoolSetting("InputPackets", &this->authInput, this->authInput);
}

PacketLogger::~PacketLogger() {
}

const char* PacketLogger::getModuleName() {
	return ("PacketLogger");
}