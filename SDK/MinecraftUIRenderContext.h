#pragma once


#include "../Utils/HMath.h"

struct MC_Color;
class ResourceLocation;
class TexturePtr;

class Font {
private:
	virtual void destructorFont();
	virtual void unk1();
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();

public:
	virtual float getLineLength(std::string* str, float textSize, bool unknown);
	virtual float getLineHeight();
};

struct TextMeasureData {
	float textSize;
	int idk;
	bool shadow;
	bool bool2;
};

class MinecraftUIRenderContext {
private:
	float _[140];

public:
	virtual void constructor(void);
	virtual float getLineLength(Font* font, std::string* str, float textSize, bool unknown);
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual __int64 drawDebugText(const float* pos, std::string* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(Font* font, const float* pos, std::string* text, const float* color, float alpha, unsigned int textAlignment, const TextMeasureData* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);
	virtual __int64 drawImage(const TexturePtr* texturePtr, Vec2 const& ImagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize); 
	virtual __int64 drawNineslice(TexturePtr* texturePtr, __int64* ninesliceInfo);
	virtual __int64 flushImages(MC_Color& color, float timeSinceLastFlush, class HashedString& hashedString);
	virtual void beginSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void endSharedMeshBatch(uintptr_t ComponentRenderBatch);
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth);  // line width is guessed
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	virtual void increaseStencilRef(void);
	virtual void decreaseStencilRef(void);
	virtual void resetStencilRef(void);
	virtual void fillRectangleStencil(Vec4 const&);
	virtual void enableScissorTest(Vec4 const&);
	virtual void disableScissorTest(void);
	virtual void setClippingRectangle(Vec4 const&);
	virtual void setFullClippingRectangle(void);
	virtual void saveCurrentClippingRectangle(void);
	virtual void restoreSavedClippingRectangle(void);
	virtual Vec4* getFullClippingRectangle(void);
	virtual void updateCustom(uintptr_t CustomRenderComponent);
	virtual void renderCustom(uintptr_t CustomRenderComponent, int, Vec4&);
	virtual void cleanup(void);
	virtual void removePersistentMeshes(void);
	virtual TexturePtr* getTexture(TexturePtr* texture, ResourceLocation* ResourceLocation, bool);
	virtual TexturePtr* getZippedTexture(uintptr_t Path, ResourceLocation* ResourceLocation, bool);
	virtual void unloadTexture(ResourceLocation* ResourceLocation);
	virtual __int64 getUITextureInfo(ResourceLocation* ResourceLocation, bool);
	virtual void touchTexture(ResourceLocation* ResourceLocation);
	virtual __int64 getMeasureStrategy(void);
	virtual void snapImageSizeToGrid(Vec2 const&);
	virtual void snapImagePositionToGrid(Vec2 const&);
	virtual void notifyImageEstimate(__int64);
};
