#include "CameraNoClip.h"

CameraNoClip::CameraNoClip() : IModule(0, Category::VISUAL, "Clip the camera through walls when in 3rd person.") {
}

CameraNoClip::~CameraNoClip() {
}

const char* CameraNoClip::getModuleName() {
	return ("CameraNoClip");
}

void* targetAddress;
char ogBytes[5] = {};

void CameraNoClip::onEnable() {
	if (!targetAddress)
		targetAddress = (void*)FindSignature("F3 44 0F 51 EA 45 0F 28 CD");

	Utils::copyBytes(targetAddress, ogBytes, 5);
	Utils::nopBytes(targetAddress, 5);
}

void CameraNoClip::onDisable() {
	Utils::patchBytes(targetAddress, ogBytes, 5);
}

