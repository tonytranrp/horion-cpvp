#pragma once

#include "Entity.h"
#include "GameSettingsInput.h"
#include "LoopbackPacketSender.h"
#include "MinecraftUIRenderContext.h"

#include "Level.h"
#include "GuiData.h"
#include "HashedString.h"
#include "Minecraft.h"
#include "MinecraftGame.h"
#include "Tessellator.h"

class Tessellator;
class BlockTessellator;
class MoveInputHandler;
class CameraManager;
class HitDetectSystem;
class Options;
class UIScene;
class SceneFactory;
class SceneStack;
class BlockSource;

class LevelRendererPlayer {

};

class LevelRenderer {
public:
	class TextureGroup *getTextureGroup() {
		return *reinterpret_cast<class TextureGroup**>((uintptr_t)(this) + 0x58);
	}

	TexturePtr *getAtlasTexture() {
		return *reinterpret_cast<TexturePtr**>((uintptr_t)(this) + 0x160);
	}

	BlockTessellator *getBlockTess() {
		return *reinterpret_cast<BlockTessellator**>((uintptr_t)(this) + 0x218);
	}

	inline Vec3 getOrigin() {
		return *reinterpret_cast<Vec3*>((uintptr_t)(this) + 0x7CC);
	}

	inline Vec3* getOriginPtr() {
		return *reinterpret_cast<Vec3**>((uintptr_t)(this) + 0x7CC);
	}

	inline Vec3* getOriginLoader() {
		return *reinterpret_cast<Vec3**>((uintptr_t)(this) + 0xB74);
	}

	inline LevelRendererPlayer* getLevelRendererPlayer() {
		return reinterpret_cast<LevelRendererPlayer*>((uintptr_t)(this) + 0x298);
	}

	void reloadChunks();
};

enum class FontType : int32_t {
	DefaultFont,
	UIFont,
	UnicodeFont,
	OldSmoothFont,
	SmoothFontLatin,
	SmoothSmall,
	RuneFont,
	MinecraftTen,
	MinecraftTenEmoticon,
};

struct FontRepository {
public:
	char pad_0x0000[0x20];
	std::vector<std::shared_ptr<Font>> fonts;  // 0x0020
	std::unordered_map<std::string, size_t> nameToIndexMap;

	inline Font* getFont(FontType type) {
		size_t index{};
		switch (type) {
		case FontType::DefaultFont:
			index = nameToIndexMap["DefaultFont"];
			break;
		case FontType::UIFont:
			index = nameToIndexMap["UIFont"];
			break;
		case FontType::UnicodeFont:
			index = nameToIndexMap["UnicodeFont"];
			break;
		case FontType::OldSmoothFont:
			index = nameToIndexMap["OldSmoothFont"];
			break;
		case FontType::SmoothFontLatin:
			index = nameToIndexMap["SmoothFontLatin"];
			break;
		case FontType::SmoothSmall:
			index = nameToIndexMap["SmoothSmall"];
			break;
		case FontType::RuneFont:
			index = nameToIndexMap["RuneFont"];
			break;
		case FontType::MinecraftTen:
			index = nameToIndexMap["MinecraftTen"];
			break;
		case FontType::MinecraftTenEmoticon:
			index = nameToIndexMap["MinecraftTenEmoticon"];
			break;
		}
		return this->fonts[index].get();
	}
};

class ClientInstance {
public:
	inline MinecraftGame* getMinecraftGame() {
		return *reinterpret_cast<MinecraftGame**>((uintptr_t)(this) + 0xA8);
	}

	inline Minecraft* getMinecraft() {
		return *reinterpret_cast<Minecraft**>((uintptr_t)(this) + 0xC0);
	}

	inline LevelRenderer* getLevelRenderer() {
		return *reinterpret_cast<LevelRenderer**>((uintptr_t)(this) + 0xD0);
	}

	inline LoopbackPacketSender* getLoopbackPacketSender() {
		return *reinterpret_cast<LoopbackPacketSender**>((uintptr_t)(this) + 0xE0);
	}

	inline MoveInputHandler* getMoveTurnInput() {
		return *reinterpret_cast<MoveInputHandler**>((uintptr_t)(this) + 0xF8);
	}

	inline GameSettingsInput* getGameSettingsInput() {
		return (GameSettingsInput*)Utils::findMultiLvlPtr((uintptr_t)(this) + 0x100, {0x20, 0x148, 0x18, 0x0});
	}

	inline glmatrixf* getRefDef() {
		return reinterpret_cast<glmatrixf*>((uintptr_t)(this) + 0x2F0);
	}

	Vec2* getMousePos() {
		return reinterpret_cast<Vec2*>((uintptr_t)(this) + 0x458);
	}

	inline GuiData* getGuiData() {
		return *reinterpret_cast<GuiData**>((uintptr_t)(this) + 0x500);
	}

	Vec2 getFov() {
		return Vec2(
			*reinterpret_cast<float*>((uintptr_t)(this) + 0x690),
			*reinterpret_cast<float*>((uintptr_t)(this) + 0x6A4)
		);
	}

	inline BlockSource* getRegion() {
		return Utils::CallVFunc<24, BlockSource*>(this);
	};

	inline LocalPlayer* getLocalPlayer() {
		return Utils::CallVFunc<25, LocalPlayer*>(this);
	};

	inline SceneStack* getSceneStack() {
		return Utils::CallVFunc<232, SceneStack*>(this);
	}

	inline SceneFactory* getSceneFactory() {
		return Utils::CallVFunc<224, SceneFactory*>(this);
	}

	void grabCursor() {
		return Utils::CallVFunc<303, void>(this);
	}

	void releaseCursor() {
		return Utils::CallVFunc<304, void>(this);
	}

	void refocusCursor() {
		return Utils::CallVFunc<305, void>(this);
	}

	Options* getOptions() {
		return Utils::CallVFunc<176, Options*>(this);
	}

	UIScene* getActiveScene() {
		using _getActiveScene_t = UIScene*(__fastcall*)(ClientInstance*);
		static auto _getActiveScene = reinterpret_cast<_getActiveScene_t>(FindSignature("40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 01 48 8B 80 ? ? ? ? ? ? ? ? ? ? 48 8B C8"));
		return _getActiveScene(this);
	}
};
