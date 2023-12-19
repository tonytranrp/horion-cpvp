#pragma once

class UIControl {
public:
	std::string getName() {
		return *reinterpret_cast<std::string *>((uintptr_t)(this) + 0x18);
	}

	Vec2& getSize() {
		return *reinterpret_cast<Vec2 *>((uintptr_t)(this) + 0x40);
	}

	Vec2& getMinSize() {
		return *reinterpret_cast<Vec2 *>((uintptr_t)(this) + 0x48);
	}

	Vec2& getMaxSize() {
		return *reinterpret_cast<Vec2 *>((uintptr_t)(this) + 0x50);
	}

	Vec2& getPosition() {
		return *reinterpret_cast<Vec2 *>((uintptr_t)(this) + 0x78);
	}
};

class VisualTree {
public:
	UIControl* getRootControl() {
		return *reinterpret_cast<UIControl **>(this + 0x8);
	}
};

class ScreenView {
public:
	ScreenController* getController() {
		return *reinterpret_cast<ScreenController **>(this + 0x38);
	}

	VisualTree* getVisualTree() {
		return *reinterpret_cast<VisualTree **>(this + 0x48);
	}
};

class UIScene {
public:
	ScreenView* getScreenView() {
		return *reinterpret_cast<ScreenView **>((uintptr_t)(this) + 0x30);
	}

	std::string getScreenName() {
		return this->getScreenView()->getVisualTree()->getRootControl()->getName();
	}

	ScreenController* getController() {
		return this->getScreenView()->getController();
	}
};

class SceneStack {
public:
	bool pushScene(UIScene* scene, bool unk = false);
};

enum class PlayScreenDefaultTab : int32_t {
	WorldTab,
	FriendsTab,
	ServersTab,
};

class SceneFactory {
public:
	UIScene* createPlayScreen(PlayScreenDefaultTab tab);
	UIScene* createOptionsScreen();
};