#include "TabGuiMod.h"

TabGuiMod::TabGuiMod() : IModule(0, Category::GUI, "The TabGui - Toggle everything using arrow keys!"){
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
	registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.0f, 1.f);
	font.addEntry(EnumEntry("Smooth", 0));
	font.addEntry(EnumEntry("Mojangles", 1));
	//font.addEntry(EnumEntry("Unicode", 2)); weird
	//font.addEntry(EnumEntry("Galactic", 3)); weird
	font.addEntry(EnumEntry("Ten", 2));
	registerEnumSetting("Font", &font, 0);
	style.addEntry(EnumEntry("Dark", 0));
	style.addEntry(EnumEntry("Classic", 1));
	registerEnumSetting("Style", &style, 0);
	registerBoolSetting("Slider", &isSlider, 0);
	}

TabGuiMod::~TabGuiMod() {
}

const char* TabGuiMod::getModuleName() {
	return ("TabGui");
}