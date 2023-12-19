#pragma once

class InputKey {
public:
	std::string keyName;
	int32_t* key;

private:
	char pad_0x28[0x18];
};

class GameSettingsInput {
public:
	InputKey attack;
	InputKey pickBlock;
	InputKey use;
	InputKey drop;
	InputKey hotbar1;
	InputKey hotbar2;
	InputKey hotbar3;
	InputKey hotbar4;
	InputKey hotbar5;
	InputKey hotbar6;
	InputKey hotbar7;
	InputKey hotbar8;
	InputKey hotbar9;
	InputKey inventoy;
	InputKey perspective;
	InputKey spaceBar;
	InputKey sneak;
	InputKey sprint;
	InputKey left;
	InputKey right;
	InputKey back;
	InputKey forward;
	InputKey mobEffect;
	InputKey chat;
	InputKey command;
	InputKey emote;
	InputKey menuTabLeft;
	InputKey menuTabRight;
	InputKey copyCoordinates;
	InputKey copyFacingCoordinates;
};