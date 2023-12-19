#pragma once

#ifndef PERFORMANCE_TEST
//#define PERFORMANCE_TEST
#endif

#include "../Horion/Command/CommandMgr.h"
#include "../Horion/Config/ConfigManager.h"
#include "../Horion/DrawUtils.h"
#include "../Horion/Menu/ClickGui.h"
#include "../Horion/Menu/TabGui.h"
#include "../Horion/Menu/NotifMgr.h"
#include "../Horion/Module/ModuleManager.h"
#include "../SDK/BlockLegacy.h"
#include "../SDK/Camera.h"
#include "../SDK/ClientInputCallbacks.h"
#include "../SDK/ChestBlockActor.h"
#include "../SDK/GameMode.h"
#include "../SDK/MinecraftUIRenderContext.h"
#include "../SDK/MoveInputHandler.h"
#include "../SDK/Option.h"
#include "../SDK/RakNetConnector.h"
#include "../SDK/UIScene.h"
#include "../SDK/ConnectionRequest.h"
#include "../SDK/Pack.h"
#include "../Utils/TextFormat.h"
#include "GameData.h"
#include "MinHook.h"

class VMTHook;
class FuncHook;

class Hooks {
public:
	char currentScreenName[100];

public:
	bool shouldRender = true;

	static void Init();
	static void Restore();
	static void Enable();

private:
	static __int64 ClientInputCallbacks_tick(ClientInputCallbacks* _this, __int64 a2);
	static ConnectionRequest* ConnectionRequest_create(ConnectionRequest* result, PrivateKeyManager* privKeyManager, Certificate* cert, std::string* SelfSignedID, std::string* ServerAddress, uint64_t ClientRandomID, std::string* SkinID, __int64 SkinData, __int64 CapeData, SerializedSkin* Skin, std::string* DeviceID, int InputMode, int UIProfile, int GuiScale, std::string* LanguageCode, bool isEditorMode, bool IsEduMode, std::string* TenantId, int8_t ADRole, std::string* PlatformUserId, std::string* ThirdPartyName, bool ThirdPartyNameOnly, std::string* PlatformOnlineID, std::string* PlatformOfflineID, std::string* CapeID);
	static __int64 ScreenController_onHoverSlot(ScreenController* _this, std::string* a2, int slot);
	static __int64 Font_drawTransformed(Font* _this, ScreenContext* a2, std::string* text, float a4, float a5, MC_Color a6, float a7, float a8, bool a9, float a10);
	static __int64 GameMode_attack(GameMode* _this, Entity*);
	static __int64 keyHook(uint64_t key, bool isDown);
	static __int64 LevelRenderer_renderLevel(__int64 _this, ScreenContext* a2, __int64 a3);
	static __int64 LevelRendererPlayer__renderOutlineSelection(__int64* _this, ScreenContext* a2, Block* a3, BlockSource* a4, Vec3i& a5);
	static __int64 MinecraftGame_onAppSuspended(__int64 _this);
	static __int64 MinecraftScreenController_tick(ScreenController* _this);
	static __int64 MinecraftUIRenderContext_drawImage(MinecraftUIRenderContext* _this, TexturePtr* a2, Vec2& a3, Vec2& a4, Vec2& a5, Vec2& a6);
	static __int64 MinecraftUIRenderContext_drawNineslice(MinecraftUIRenderContext* _this, TexturePtr* a2, __int64* a3);
	static __int64 MoveInputHandler_tick(MoveInputHandler* _this, Entity* a2);
	static __int64 PaintingRenderer__render(__int64 _this, __int64 a2, __int64 a3);
	static __int64 ScreenView_setupAndRender(__int64 a1, MinecraftUIRenderContext* renderCtx);
	static __int64 SkinRepository___loadSkinPack(__int64 _this, Pack* pack, bool a3);
	static __int64 UIScene_render(UIScene* uiscene, __int64 screencontext);
	static bool DirectoryPackAccessStrategy__isTrusted(__int64 _this);
	static bool Actor_CanBeAffected(Entity* _this, int a1);
	static bool isOnFire(Entity* _this, void* a1);
	static bool isInvisible(void* a1);
	static bool Mob__isImmobile(Entity*);
	static bool ReturnTrue(__int64 _this);
	static bool ShulkerBlockActor_itemAllowed(ItemStack* a1);
	static float Dimension_getTimeOfDay(__int64, int a2, float a3);
	static float GameMode_getPickRange(GameMode* _this, __int64 a2, char a3);
	static float GetGamma(Options** optionList);
	static float LevelRendererPlayer_getFov(__int64 _this, float a2, bool a3);
	static float SoulSandBlock__calcGroundFriction(BlockLegacy* a1, __int64 mobMovementProxy);
	static float* Dimension_getFogColor(__int64*, float* color, __int64 brightness, float a4);
	static HitResult* HitResult_Update(HitResult* currentData, HitResult* newData);
	static int AppPlatform_getGameEdition(__int64 _this);
	static int BlockLegacy_getRenderLayer(BlockLegacy* a1);
	static MinecraftJson::Value* FlatWorldOptions_getLayers(MinecraftJson::Value* a1, __int64 a2);
	static void Actor_ascendLadder(Entity* _this);
	static void Actor_lerpMotion(Entity* _this, Vec3);
	static void Actor_startSwimming(Entity* _this);
	static void Actor_swing(Entity* _this);
	static void ChestBlockActor_tick(ChestBlockActor*, void* a);
	static void ClickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static void ClientInstanceScreenModel_sendChatMessage(void* _this, std::string* text);
	static void GameMode_startDestroyBlock(GameMode* _this, Vec3i* a2, uint8_t face, void* a4, void* a5);
	static void GameRenderer_renderCurrentFrame(void* renderer, float f);
	static void HIDController_keyMouse(HIDController* _this, void* a2, void* a3);
	static bool HurtCameraEffect(void* _this, void* a2);
	static void ItemInHandRenderer_render(__int64* _this, BaseActorRenderContext* ctx, char a3);
	static void InventoryTransactionManager__addAction(InventoryTransactionManager*, InventoryAction&, bool);
	static void JumpFromGround(Entity* _this, void* a2);
	static void LevelRendererPlayer__renderNameTags(__int64 a1, __int64 a2, std::string* name, __int64 a4, __int64 a5);
	static void LocalPlayer__updateFromCamera(__int64 a1, Camera* a2, __int64* a3, Entity* a4);
	static void LoopbackPacketSender_sendToServer(LoopbackPacketSender* a, Packet* packet);
	static void MinecraftPackets_createPacket(Packet** packet, int packetId);
	static void MultiLevelPlayer_tick(EntityList* entityList);
	static void PacketHandlerDispatcherInstance_handle(PacketHandlerDispatcherInstance* dispatcher, void* networkIdentifier, __int64* netEventCallback, Packet** packet);
	static void ChatAutoComplete(__int64 _this, __int64 a2, std::string* text, int a4);
	static bool DirectPlayerMovementProxy_isSlowedByItemUse(class DirectPlayerMovementProxy* _this);
	//static __int64 ActorRenderDispatcher_render(ActorRenderDispatcher* _this, BaseActorRenderContext* baseActorCtx, Entity* actor, Vec3 a4, Vec3 a5, Vec2 a6, char a7);
	static Block* BlockSource_getBlock(BlockSource* _this, Vec3i blockPos);
	static float GetStepHeight(void*);

	std::unique_ptr<FuncHook> isInvisibleHook;
	std::unique_ptr<FuncHook> Actor_CanBeAffectedHook;
	std::unique_ptr<FuncHook> Actor_ascendLadderHook;
	std::unique_ptr<FuncHook> Actor_lerpMotionHook;
	std::unique_ptr<FuncHook> Actor_startSwimmingHook;
	std::unique_ptr<FuncHook> Actor_swingHook;
	//std::unique_ptr<FuncHook> ActorRenderDispatcher_renderHook;
	std::unique_ptr<FuncHook> AppPlatform_getGameEditionHook;
	std::unique_ptr<FuncHook> BlockLegacy_getRenderLayerHook;
	std::unique_ptr<FuncHook> BlockSource_getBlockHook;
	std::unique_ptr<FuncHook> ChestBlockActor_tickHook;
	std::unique_ptr<FuncHook> ClickFuncHook;
	std::unique_ptr<FuncHook> ClientInputCallbacks_tickHook;
	std::unique_ptr<FuncHook> ClientInstanceScreenModel_sendChatMessageHook;
	std::unique_ptr<FuncHook> ConnectionRequest_createHook;
	std::unique_ptr<FuncHook> ScreenController_onHoverSlotHook;
	std::unique_ptr<FuncHook> Dimension_getFogColorHook;
	std::unique_ptr<FuncHook> Dimension_getTimeOfDayHook;
	std::unique_ptr<FuncHook> DirectoryPackAccessStrategy__isTrustedHook;
	std::unique_ptr<FuncHook> FlatWorldOptions_getLayersHook;
	std::unique_ptr<FuncHook> Font_drawTransformedHook;
	std::unique_ptr<FuncHook> GameMode_attackHook;
	std::unique_ptr<FuncHook> GameMode_getPickRangeHook;
	std::unique_ptr<FuncHook> GameMode_startDestroyBlockHook;
	std::unique_ptr<FuncHook> GameRenderer_renderCurrentFrameHook;
	std::unique_ptr<FuncHook> GetGammaHook;
	std::unique_ptr<FuncHook> HIDController_keyMouseHook;
	std::unique_ptr<FuncHook> HitResult_UpdateHook;
	std::unique_ptr<FuncHook> HurtCameraEffectHook;
	std::unique_ptr<FuncHook> InventoryTransactionManager__addActionHook;
	std::unique_ptr<FuncHook> isOnFireHook;
	std::unique_ptr<FuncHook> ItemInHandRenderer_renderHook;
	std::unique_ptr<FuncHook> Item_getMaxUseDurationHook;
	std::unique_ptr<FuncHook> JumpFromGroundHook;
	std::unique_ptr<FuncHook> keyHookHook;
	std::unique_ptr<FuncHook> LevelRenderer_renderLevelHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer__renderNameTagsHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer__renderOutlineSelectionHook;
	std::unique_ptr<FuncHook> LevelRendererPlayer_getFovHook;
	std::unique_ptr<FuncHook> LocalPlayer__updateFromCameraHook;
	std::unique_ptr<FuncHook> LoopbackPacketSender_sendToServerHook;
	std::unique_ptr<FuncHook> MinecraftGame_onAppSuspendedHook;
	std::unique_ptr<FuncHook> MinecraftPackets_createPacketHook;
	std::unique_ptr<FuncHook> MinecraftScreenController_tickHook;
	std::unique_ptr<FuncHook> MinecraftUIRenderContext_drawImageHook;
	std::unique_ptr<FuncHook> MinecraftUIRenderContext_drawNinesliceHook;
	std::unique_ptr<FuncHook> Mob__isImmobileHook;
	std::unique_ptr<FuncHook> MoveInputHandler_tickHook;
	std::unique_ptr<FuncHook> MultiLevelPlayer_tickHook;
	std::unique_ptr<FuncHook> PaintingRenderer__renderHook;
	std::unique_ptr<FuncHook> ChatAutoCompleteHook;
	std::unique_ptr<FuncHook> RakNetInstance_tickHook;
	std::unique_ptr<FuncHook> ScreenView_setupAndRenderHook;
	std::unique_ptr<FuncHook> ShulkerBlockActor_itemAllowedHook;
	std::unique_ptr<FuncHook> SkinRepository___checkSignatureFileInPack;
	std::unique_ptr<FuncHook> SkinRepository___loadSkinPackHook;
	std::unique_ptr<FuncHook> SoulSandBlock__calcGroundFrictionHook;
	std::unique_ptr<FuncHook> UIScene_renderHook;
	std::unique_ptr<FuncHook> UIScene_setupAndRenderHook;
	std::unique_ptr<FuncHook> ZipPackAccessStrategy__isTrustedHook;
	std::unique_ptr<FuncHook> ChatLoggerHook;
	std::unique_ptr<FuncHook> isSlowedByItemUseHook;
	std::unique_ptr<FuncHook> GetStepHeightHook;


	std::array<std::unique_ptr<FuncHook>, static_cast<size_t>(PacketID::COUNT)> packetHooks{};
};

extern Hooks g_Hooks;

class FuncHook {
public:
	void* funcPtr;
	void* funcReal;

	FuncHook(void* func, void* hooked) {
		funcPtr = func;

		MH_STATUS ret = MH_CreateHook(func, hooked, &funcReal);
		if (ret == MH_OK && (__int64)func > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	FuncHook(uintptr_t func, void* hooked) {
		funcPtr = reinterpret_cast<void*>(func);

		MH_STATUS ret = MH_CreateHook(funcPtr, hooked, &funcReal);
		if (ret == MH_OK && (__int64)funcPtr > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	void enableHook(bool enable = true) {
		if (funcPtr) {
			int ret = enable ? MH_EnableHook(funcPtr) : MH_DisableHook(funcPtr);
			if (ret != MH_OK)
				logF("MH_EnableHook = %i", ret);
		} else
			logF("enableHook() called with nullptr func!");
	}

	~FuncHook() {
		Restore();
	}

	void Restore() {
		if (funcPtr)
			MH_DisableHook(funcPtr);
	}

	template <typename TRet, typename... TArgs>
	inline auto* GetFastcall() {
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(funcReal);
	};
};