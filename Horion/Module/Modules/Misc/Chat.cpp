#include "Chat.h"

Chat::Chat() : IModule(0x0, Category::MISC, "Allows you to modify the chat messages you sent") {
	registerEnumSetting("Types", &chatTypeModes, 0);
	chatTypeModes = SettingEnum(this)
					  .addEntry(EnumEntry("None", 0))
					  .addEntry(EnumEntry("UWUFY", 1))
				      .addEntry(EnumEntry("LEET", 2));

	registerEnumSetting("Prefix Options", &prefixModes, 0);
	prefixModes = SettingEnum(this)
					  .addEntry(EnumEntry("None", 0))
					  .addEntry(EnumEntry("Green Text", 1));

	registerEnumSetting("Suffix Options", &suffixModes, 0);
	suffixModes = SettingEnum(this)
					  .addEntry(EnumEntry("None", 0))
					  .addEntry(EnumEntry("Horion", 1))
					  .addEntry(EnumEntry(":^)", 2))
					  .addEntry(EnumEntry("discord", 3))
					  .addEntry(EnumEntry("website", 4));

	registerEnumSetting("Font Options", &ChatModes, 0);
	ChatModes = SettingEnum(this)
					.addEntry(EnumEntry("None", 0))
					.addEntry(EnumEntry("Circle Text", 1))
					.addEntry(EnumEntry("Funky Text", 2))
					.addEntry(EnumEntry("Fancy Text", 3))
					.addEntry(EnumEntry("Bold Text", 4))
					.addEntry(EnumEntry("Bubble Text", 5));
}

Chat::~Chat() {
}

const char* Chat::getModuleName() {
	return ("Chat");
}

void Chat::onSendPacket(Packet* packet) {
	if (packet->getId() == PacketID::Text) {
		TextPacket* funi = reinterpret_cast<TextPacket*>(packet);
		std::string build;
		std::string endText;
		std::string startText;

		if (suffixModes.selected > 0) endText = suffixModes.selected == 1 ? " | Horion" : suffixModes.selected == 2 ? " :^)" : suffixModes.selected == 3 ? " | horion.download/discord" : suffixModes.selected == 4 ? " | horion.download" : "";
		if (prefixModes.selected > 0) startText = prefixModes.selected == 1 ? "> " : "";
		build = startText + funi->message.c_str() + endText;
		if (chatTypeModes.selected > 0) chatTypeModes.selected == 1 ? build = UWUFY(&build) : chatTypeModes.selected == 2 ? build = LEETME(&build) : build;
		if (ChatModes.selected > 0) build = ChangeMeDaddy(&build);
		funi->message = build;
	}
}

void Chat::onTick(GameMode* gm) {
}

std::string Chat::LEETME(std::string* string) {
	std::string build = *string;

	for (unsigned int i = 0; i < build.length(); i++) {
		if (build.at(i) == *"a" || build.at(i) == *"A") {
			build.replace(i, 1, "4");
		}
		if (build.at(i) == *"e" || build.at(i) == *"E") {
			build.replace(i, 1, "3");
		}
		if (build.at(i) == *"g" || build.at(i) == *"G") {
			build.replace(i, 1, "6");
		}
		if (build.at(i) == *"l" || build.at(i) == *"L" || build.at(i) == *"i" || build.at(i) == *"I") {
			build.replace(i, 1, "1");
		}
		if (build.at(i) == *"o" || build.at(i) == *"O") {
			build.replace(i, 1, "0");
		}
		if (build.at(i) == *"s" || build.at(i) == *"S") {
			build.replace(i, 1, "$");
		}
		if (build.at(i) == *"t" || build.at(i) == *"T") {
			build.replace(i, 1, "7");
		}
	}

	return build;
}

std::string Chat::UWUFY(std::string* string) {
	std::string build = *string;
	std::string owomoji[13]{
		" ^~^",
		" UwU",
		" OwO",
		" O_O",
		" O_o",
		" oWo",
		" OvO",
		" UvU",
		" *~*",
		" :3",
		" =3",
		" <(^V^<)",
		" UmU"
	};

	//had to do this because my old method only works with one
	size_t pos = -1;
	while ((pos = build.find("the")) != -1) {
		build.replace(pos, sizeof("the") - 1, "da");
	}
	while ((pos = build.find("The")) != -1 || (pos = build.find("THE")) != -1) {
		build.replace(pos, sizeof("The") - 1, "Da");
	}
	while ((pos = build.find("th")) != -1) {
		build.replace(pos, sizeof("th") - 1, "f");
	}
	while ((pos = build.find("Th")) != -1 || (pos = build.find("TH")) != -1) {
		build.replace(pos, sizeof("Th") - 1, "F");
	}
	while ((pos = build.find("you")) != -1) {
		build.replace(pos, sizeof("you") - 1, "u");
	}
	while ((pos = build.find("too")) != -1) {
		build.replace(pos, sizeof("too") - 1, "2");
	}

	for (unsigned int i = 0; i < build.length(); i++) {
		if (build.at(i) == *".") {
			build.replace(i, 1, "!!!");
		}
		if (build.at(i) == *"W") {
			build.replace(i, 1, "w-w");
		}
		if (build.at(i) == *"l" || build.at(i) == *"r") {
			build.replace(i, 1, "w");
		}
		if (build.at(i) == *"L" ||  build.at(i) == *"R") {
			build.replace(i, 1, "W");
		}
	}
	build = build + owomoji[(rand() % 13)];
	return build;
}

std::string Chat::ChangeMeDaddy(std::string* string) {  //ah yes optimisation
	std::string build = *string;
	static std::string VanillaChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	//reason why all unicode chars are in arrays is because if they are together like VanillaChars it counts as one big letter rather than 62
	static std::string Circles[]{
		"ⓐ", "ⓑ", "ⓒ", "ⓓ", "ⓔ", "ⓕ", "ⓖ", "ⓗ", "ⓘ", "ⓙ", "ⓚ", "ⓛ",
		"ⓜ", "ⓝ", "ⓞ", "ⓟ", "ⓠ", "ⓡ", "ⓢ", "ⓣ", "ⓤ", "ⓥ", "ⓦ", "ⓧ",
		"ⓨ", "ⓩ", "Ⓐ", "Ⓑ", "Ⓒ", "Ⓓ", "Ⓔ", "Ⓕ", "Ⓖ", "Ⓗ", "Ⓘ", "Ⓙ",
		"Ⓚ", "Ⓛ", "Ⓜ", "Ⓝ", "Ⓞ", "Ⓟ", "Ⓠ", "Ⓡ", "Ⓢ", "Ⓣ", "Ⓤ", "Ⓥ",
		"Ⓦ", "Ⓧ", "Ⓨ", "Ⓩ", "①", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⓪"};

	static std::string ChingChongChars[]{
		"卂", "乃", "匚", "ᗪ", "乇", "千", "Ꮆ",
		"卄", "丨", "ﾌ", "Ҝ", "ㄥ", "爪", "几", "ㄖ", "卩", "Ɋ", "尺", "丂", "ㄒ",
		"ㄩ", "ᐯ", "山", "乂", "ㄚ", "乙", "卂", "乃", "匚", "ᗪ", "乇", "千", "Ꮆ",
		"卄", "丨", "ﾌ", "Ҝ", "ㄥ", "爪", "几", "ㄖ", "卩", "Ɋ", "尺", "丂", "ㄒ",
		"ㄩ", "ᐯ", "山", "乂", "ㄚ", "乙", "1", "2", "3", "4", "5", "6", "7", "8",
		"9", "0"};

	static std::string FancyFontChars[]{
		"ａ", "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ",
		"ｈ", "ｉ", "ｊ", "ｋ", "ｌ", "ｍ", "ｎ", "ｏ", "ｐ", "ｑ", "ｒ", "ｓ", "ｔ",
		"ｕ", "ｖ", "ｗ", "ｘ", "ｙ", "ｚ", "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ",
		"Ｈ", "Ｉ", "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "Ｐ", "Ｑ", "Ｒ", "Ｓ", "Ｔ",
		"Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "Ｚ", "１", "２", "３", "４", "５", "６", "７",
		"８", "９", "０"};

	static std::string BoldTextChars[]{
		"ᴀ", "ʙ", "ᴄ", "ᴅ", "ᴇ", "ꜰ", "ɢ", "ʜ", "ɪ", "ᴊ", "ᴋ", "ʟ", "ᴍ", "ɴ", "ᴏ", "ᴘ", 
		"ǫ", "ʀ", "ꜱ", "ᴛ", "ᴜ", "ᴠ", "ᴡ", "x", "ʏ", "ᴢ", "ᴀ", "ʙ", "ᴄ", "ᴅ", "ᴇ", "ꜰ", 
		"ɢ", "ʜ", "ɪ", "ᴊ", "ᴋ", "ʟ", "ᴍ", "ɴ", "ᴏ", "ᴘ", "ǫ", "ʀ", "ꜱ", "ᴛ", "ᴜ", "ᴠ", 
		"ᴡ", "x", "ʏ", "ᴢ", "₁", "₂", "₃", "₄", "₅", "₆", "₇", "₈", "₉", "₀"};

	static std::string BubbleTextChars[]{
		"ᗩ", "ᗷ", "ᑕ", "ᗪ", "E", "ᖴ", "G", "ᕼ", "I", "ᒍ", "K", "ᒪ", "ᗰ", "ᑎ", "O", "ᑭ",
		"ᑫ", "ᖇ", "ᔕ", "T", "ᑌ", "ᐯ", "ᗯ", "᙭", "Y", "ᘔ", "ᗩ", "ᗷ", "ᑕ", "ᗪ", "E", "ᖴ",
		"G", "ᕼ", "I", "ᒍ", "K", "ᒪ", "ᗰ", "ᑎ", "O", "ᑭ", "ᑫ", "ᖇ", "ᔕ", "T", "ᑌ", "ᐯ",
		"ᗯ", "᙭", "Y", "ᘔ", "１", "２", "３", "４", "５", "６", "７", "８", "９", "０"};

	//i = position in string, i2 = characters in VanillaChars/replacement string
	for (int i = 0; i < build.length(); i++) {
		for (int i2 = 0; i2 < VanillaChars.length(); i2++) {
			if (build.at(i) == VanillaChars.at(i2)) {
				build.replace(i, 1, ChatModes.selected == 2 ? ChingChongChars[i2] : ChatModes.selected == 3 ? FancyFontChars[i2] : ChatModes.selected == 4 ? BoldTextChars[i2] : ChatModes.selected == 5 ? BubbleTextChars[i2] : Circles[i2]);
			}
		}
	}
	return build;
}