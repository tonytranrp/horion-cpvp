#include "UIScene.h"

bool SceneStack::pushScene(UIScene* scene, bool unk) {
	using pushScene_t = bool(__cdecl *)(SceneStack *, UIScene*, bool);
	static auto pushScene = reinterpret_cast<pushScene_t>(FindSignature("48 89 54 24 ? 53 48 83 EC ? 48 8B DA ? 44 24 ? ? 0F"));

	return pushScene(this, scene, unk);
}

UIScene* SceneFactory::createPlayScreen(PlayScreenDefaultTab tab) {
	using createPlayScreen_t = UIScene*(__cdecl *)(SceneFactory *, int64_t, PlayScreenDefaultTab, std::string const&, int64_t);
	static auto createPlayScreen = reinterpret_cast<createPlayScreen_t>(FindSignature("44 89 44 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24"));
	char input[16]{};
	int64_t unk[8]{};
	return createPlayScreen(this, (int64_t)input, tab, "", (int64_t)unk);
}

UIScene* SceneFactory::createOptionsScreen() {
	using createOptionsScreen_t = UIScene *(__cdecl *)(SceneFactory *, int64_t, int64_t, uint32_t, char, char);
	static auto createOptionsScreen = reinterpret_cast<createOptionsScreen_t>(FindSignature("44 89 4C 24 ? 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4"));
	char input[16]{};
	char levelSummary[640]{};
	return createOptionsScreen(this, (int64_t)input, (int64_t)levelSummary, 0, 1, 1);
}