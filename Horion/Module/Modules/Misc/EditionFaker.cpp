#include "EditionFaker.h"

// https://github.com/Sandertv/gophertunnel/blob/master/minecraft/protocol/os.go

EditionFaker::EditionFaker() : IModule(0, Category::MISC, "Fakes your device edition") {
	edition.addEntry(EnumEntry("Android", 1));
	edition.addEntry(EnumEntry("iOS", 2));
	edition.addEntry(EnumEntry("Mac OS", 3));
	edition.addEntry(EnumEntry("Fire OS", 4));
	edition.addEntry(EnumEntry("Gear VR", 5));
	edition.addEntry(EnumEntry("Hololens VR", 6));
	//edition.addEntry(EnumEntry("Windows UWP (vanilla)", 7));
	edition.addEntry(EnumEntry("Windows x86", 8));
	edition.addEntry(EnumEntry("Dedicated Server", 9));
	edition.addEntry(EnumEntry("TV OS", 10));
	edition.addEntry(EnumEntry("PS4", 11));
	//edition.addEntry(EnumEntry("Switch (softlock warning)", 12));
	edition.addEntry(EnumEntry("Xbox", 13));
	edition.addEntry(EnumEntry("Windows Phone", 14));
	edition.addEntry(EnumEntry("Linux", 15));

	registerEnumSetting("Edition", &edition, 7);
}

EditionFaker::~EditionFaker() {
}

int EditionFaker::getFakedEditon() {
	return edition.GetSelectedEntry().GetValue();
}

const char* EditionFaker::getModuleName() {
	return ("EditionFaker");
}

std::string EditionFaker::getModeText() {
	return edition.GetSelectedEntry().GetName();
}