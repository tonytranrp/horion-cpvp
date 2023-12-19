#include "CustomSky.h"

rgb CustomSky::color = {255, 0, 0};

CustomSky::CustomSky() : IModule(0, Category::VISUAL, "Changes the color of the sky") {
	registerBoolSetting("Rainbow", &rainbowSky, rainbowSky);
	registerColorSetting("Color", &color, color);
}

CustomSky::~CustomSky() {
}

const char* CustomSky::getModuleName() {
	return ("CustomSky");
}
