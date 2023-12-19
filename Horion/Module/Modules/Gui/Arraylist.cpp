#include "Arraylist.h"

rgb Arraylist::color = {255, 255, 255};

Arraylist::Arraylist() : IModule(0, Category::GUI, "displays a list of active modules") {
	mode.addEntry(EnumEntry("Outline", 0));
	mode.addEntry(EnumEntry("Edge Line", 1));
	mode.addEntry(EnumEntry("None", 2));
	mode.addEntry(EnumEntry("Rounded", 3));
	registerEnumSetting("Mode", &mode, 0);
	colorMode.addEntry(EnumEntry("Rainbow", 0));
	colorMode.addEntry(EnumEntry("Categorized", 1));
	colorMode.addEntry(EnumEntry("Custom", 2));
	//colorMode.addEntry(EnumEntry("Custom Swag", 3));
	registerEnumSetting("Color", &colorMode, 0);
	rainbowMode.addEntry(EnumEntry("Pastel", 0));
	rainbowMode.addEntry(EnumEntry("Saturated", 1));
	registerEnumSetting("Rainbow", &rainbowMode, 0, {0}, &colorMode.selected);
	font.addEntry(EnumEntry("Smooth", 0));
	font.addEntry(EnumEntry("Mojangles", 1));
	//font.addEntry(EnumEntry("Unicode", 2)); weird
	//font.addEntry(EnumEntry("Galactic", 3)); weird
	font.addEntry(EnumEntry("Ten", 2));
	registerEnumSetting("Font", &font, 0);
	registerColorSetting("Custom Color", &color, color, {2}, &colorMode.selected);
	registerBoolSetting("Bottom", &this->bottom, this->bottom);
	registerBoolSetting("Show Modes", &this->modes, this->modes);
	registerBoolSetting("Show Keybinds", &this->keybinds, this->keybinds);
	registerBoolSetting("ClickToggle", &this->clickToggle, this->clickToggle);
	registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.0f, 1.f);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
}

Arraylist::~Arraylist() {
}

const char* Arraylist::getModuleName() {
	return ("Arraylist");
}