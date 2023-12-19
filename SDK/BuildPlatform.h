#pragma once

#include <cstdint>

// https://github.com/Sandertv/gophertunnel/blob/a072417a428c601176d9f7d2172eec5bfde38c07/minecraft/protocol/os.go#L7
enum class BuildPlatform : int32_t {
	Unknown = 0,
	Android,
	iOS,
	FireOS,
	GearVR,
	Hololens,
	WinUWP,
	Win32,
	DedicatedServer,
	TVOS,
	PS4,
	NintendoSwitch,
	Xbox,
	WindowsPhone,
	Linux,
};
