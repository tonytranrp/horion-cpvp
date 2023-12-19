#include "ClientInputCallbacks.h"

void ClientInputCallbacks::handleDestoryOrAttackButtonPress() {   //Break Blocks / Hit
	using handleDestoryOrAttackButtonPress_t = void(__fastcall*)(ClientInputCallbacks*);
	static handleDestoryOrAttackButtonPress_t handleDestoryOrAttackButtonPress = reinterpret_cast<handleDestoryOrAttackButtonPress_t>(FindSignature("40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 48 8B C8 48 8B 10 FF ? ? ? ? 74 ? 48 8B 03 48 8B CB FF 90 ? ? ? ? ? B8 70 ? ? ? ? 74 ? 48 8B 03 48 8B CB"));
	handleDestoryOrAttackButtonPress(this);
}

void ClientInputCallbacks::handleBuildOrInteractButtonPress() {   //Build Blocks / Use
	using handleBuildOrInteractButtonPress_t = void(__fastcall*)(ClientInputCallbacks*);
	static handleBuildOrInteractButtonPress_t handleBuildOrInteractButtonPress = reinterpret_cast<handleBuildOrInteractButtonPress_t>(FindSignature("40 53 48 83 EC ? 48 8B 01 48 8B D9 FF 90 ? ? ? ? 48 8B C8 48 8B 10 FF ? ? ? ? 74 ? 48 8B 03 48 8B CB FF 90 ? ? ? ? ? B8 70 ? ? ? ? 74 ? 48 8B 03 48 8B CB FF 90 ? ? ? ? ? ? 74 ? 48 8B 03 BA ? ? ? ? 48 8B CB FF 90 ? ? ? ? 48 8B 03 48 8B CB FF 90 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? ? ? 74 ? 48 8B 03 BA 11  ? ? ? 48 8B CB"));    /* BA 11 Will change, functions are too similar. */
	handleBuildOrInteractButtonPress(this);
}

void ClientInputCallbacks::handleBuildOrDestroyActionButtonRelease() {   //Release Either Mouse Btn (They are held when above functions are ran)
	using handleBuildActionButtonRelease_t = void(__fastcall*)(ClientInputCallbacks*);
	static handleBuildActionButtonRelease_t handleBuildActionButtonRelease = reinterpret_cast<handleBuildActionButtonRelease_t>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B 01 48 8B F9 48 8B 80 ? ? ? ? ? ? ? ? ? ? 48 8B D8 48 85 C0"));
	handleBuildActionButtonRelease(this);
}