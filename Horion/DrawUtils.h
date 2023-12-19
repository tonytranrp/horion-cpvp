#pragma once

#include "../Memory/GameData.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/MinecraftUIRenderContext.h"
#include "../SDK/ScreenContext.h"
#include "../SDK/Tessellator.h"
#include "../Utils/HMath.h"
#include "../Utils/Target.h"
#include "../Utils/Utils.h"
#include "../Utils/TimerUtils.h"

enum class Fonts { 
	DEFAULT,
	UNICOD,
	SMOOTH,
	RUNE 
};

enum class DesiredUi { 
	DEFAULT,
	TABGUI,
	CLICKGUI,
	HUD,
	ARRAYLIST 
};

using mce__VertexFormat__disableHalfFloats_t = void(__fastcall*)(__int64, int, int);
using Tessellator__initializeFormat_t = void(__fastcall*)(__int64, __int64);

extern MinecraftUIRenderContext* renderCtx;
extern mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats;
extern Tessellator__initializeFormat_t Tessellator__initializeFormat;

struct MC_Color {
	union {
		struct {
			float r, g, b, a;
		};
		float arr[4];
	};
	bool shouldDelete = true;
	bool isFloat = true;
	MC_Color() {
		this->r = 1;
		this->g = 1;
		this->b = 1;
		this->a = 1;
	};

	MC_Color(const MC_Color& other) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
		this->shouldDelete = other.shouldDelete;
	}

	MC_Color(const float* arr) {
		this->arr[0] = arr[0];
		this->arr[1] = arr[1];
		this->arr[2] = arr[2];
		this->arr[3] = arr[3];
	};

	MC_Color(const float r, const float g, const float b, const float a = 1) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	};

	MC_Color(const int r, const int g, const int b, const int a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};

	MC_Color(const bool isFloat, const float r, const float g, const float b, const float a = 255) {
		this->isFloat = isFloat;
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};

	MC_Color(const float r, const float g, const float b, const float a, const bool shouldDelete) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->shouldDelete = shouldDelete;
	};

	MC_Color lerp(const MC_Color& o, float t) const;
};

struct CornerRadius {
	union {
		struct {
			float topR, topL, bottomR, bottomL;
		};
		float corners[4];
	};

	CornerRadius(const float radius) {
		this->topL = radius;
		this->topR = radius;
		this->bottomL = radius;
		this->bottomR = radius;
	};

	CornerRadius(const int radius) {
		this->topL = (float)radius;
		this->topR = (float)radius;
		this->bottomL = (float)radius;
		this->bottomR = (float)radius;
	};
	
	CornerRadius(const float topL, const float topR, const float bottomL, const float bottomR) {
		this->topL = topL;
		this->topR = topR;
		this->bottomL = bottomL;
		this->bottomR = bottomR;
	};

	CornerRadius(const int topL, const int topR, const int bottomL, const int bottomR) {
		this->topL = (float)topL;
		this->topR = (float)topR;
		this->bottomL = (float)bottomL;
		this->bottomR = (float)bottomR;
	};
};

class MatrixStack;

class DrawUtils {
public:
	static void setCtx(MinecraftUIRenderContext* ctx, GuiData* guiData);
	static void setCtx3D(ScreenContext* ctx);
	static void Flush();
	static void FlushTessellator();
	static void FlushTessellator3D();
	static void TessellatorVertex(float x, float y, float z);
	static void TessellatorVertex3D(float x, float y, float z);
	static void TessellatorSetRotation(float a1, Vec3 rotation);
	static void TessellatorResetTransform(bool a1);
	static void TessellatorColor(MC_Color& col);
	static void TessellatorBegin(VertexFormat vertexFormat, int reservedVerts = 0);
	static void SetColor(float r, float g, float b, float a);
	static inline void SetColor(MC_Color color) { SetColor(color.r, color.g, color.b, color.a); }
	static MC_Color getRainbowWave(float speed, float saturation, long index, bool invert);
	static Font* getFont(DesiredUi ui = DesiredUi::DEFAULT);
	static ScreenContext* getScreenContext();
	static MatrixStack* getMatrixStack();
	static float getTextWidth(std::string* textStr, float textSize = 1, DesiredUi ui = DesiredUi::DEFAULT);
	static float getFontHeight(float textSize = 1, DesiredUi ui = DesiredUi::DEFAULT);
	static void drawCrossLine(Vec2 pos, MC_Color col, float textSize, bool secondCross);
	static void drawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3);
	static void drawQuad(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4);
	static void drawPolygon(const Vec2& center, const Vec2& size, int numSides);
	static void drawPolygonOutline(const Vec2& center, const Vec2& size, int numSides, float thickness);
	static void drawRoundRect(Vec4 pos, CornerRadius radius, MC_Color color, float step = 5.0f);
	static void drawRoundRectOutline(Vec4 pos, CornerRadius radius, MC_Color color, float lineWidth = 1.f, double quality = 0.1);
	static void drawLine(const Vec2& start, const Vec2& end, float lineWidth);
	static void drawLinestrip3D(const std::vector<Vec3>& points);
	static void drawLine3D(const Vec3& start, const Vec3& end, bool onUi = false);
	static void drawBox3D(const Vec3& lower, const Vec3& upper);
	static void fillRect(const Vec4& pos, const MC_Color& col, float alpha);
	static bool isMouseOver(const Vec4& pos);
	static Vec4 getRectForAABB(const AABB& aabb);
	static inline void fillRect(const Vec2& start, const Vec2& end) { DrawUtils::drawQuad({start.x, end.y}, {end.x, end.y}, {end.x, start.y}, {start.x, start.y}); }
	static inline void drawRect(const Vec2& start, const Vec2& end, float lineWidth = 1.0f) {
		lineWidth /= 2;
		fillRect({start.x - lineWidth, start.y - lineWidth}, {end.x + lineWidth, start.y + lineWidth});  // TOP
		fillRect({start.x - lineWidth, start.y}, {start.x + lineWidth, end.y});                          // LEFT
		fillRect({end.x - lineWidth, start.y}, {end.x + lineWidth, end.y});                              //
		fillRect({start.x - lineWidth, end.y - lineWidth}, {end.x + lineWidth, end.y + lineWidth});
	}
	static inline void drawRect(const Vec4& pos, const MC_Color& col, float alpha, float lineWidth = 1.0f) {
		lineWidth /= 2;
		fillRect(Vec4(pos.x - lineWidth, pos.y - lineWidth, pos.z + lineWidth, pos.y + lineWidth), col, alpha);  // TOP
		fillRect(Vec4(pos.x - lineWidth, pos.y, pos.x + lineWidth, pos.w), col, alpha);                          // LEFT
		fillRect(Vec4(pos.z - lineWidth, pos.y, pos.z + lineWidth, pos.w), col, alpha);                          //
		fillRect(Vec4(pos.x - lineWidth, pos.w - lineWidth, pos.z + lineWidth, pos.w + lineWidth), col, alpha);
	}
	static void drawImage(const std::string& filePath, Vec2& ImagePos, Vec2& ImageDimension, Vec2& uvPos, Vec2& uvSize, bool external = false);
	static void drawImageFromResource(int resource, const std::string& imageName, Vec2 const& imagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize);
	static void drawImageFromTexturePtr(std::shared_ptr<TexturePtr> texture, Vec2 const& imagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize);
	
	// Must delete the TexturePtr yourself
	static std::shared_ptr<TexturePtr> rawDataToTexturePtr(const unsigned char* data, int width, int height, bool hasAlpha, const std::string& name);
	static std::shared_ptr<TexturePtr> resourceToTexturePtr(int resource, const std::string& name);
	static std::string resourceToFilePath(int resource, const std::string& name, const std::string& extension);
	static void drawNineSlice(std::string filePath, Vec2& imagePos, Vec2& ImageDimension, float outlineWidth = 10.f);
	static void drawNineSliceFromBase64(std::string base64, std::string imageName, Vec2& imagePos, Vec2& ImageDimension, float outlineWidth = 10.f);
	static void flushImages(MC_Color color = MC_Color(1.f, 1.f, 1.f));
	static void drawName(Entity* ent, std::string* name, float textSize, const MC_Color& background, const MC_Color& color, float opacity = 1.f, DesiredUi ui = DesiredUi::DEFAULT);
	static void drawText(const Vec2& pos, std::string* text, const MC_Color& color, float textSize = 1.f, float alpha = 1.f, DesiredUi ui = DesiredUi::DEFAULT, bool drawShadow = true);
	static void drawRainbowText(Vec2& pos, const std::string& textStr, float speed, float saturation, bool invert, float textSize = 1.f, float alpha = 1.f, DesiredUi ui = DesiredUi::DEFAULT, bool drawShadow = true);
	static void drawBox(const Vec3& lower, const Vec3& upper, float lineWidth, bool outline = false);
	static void drawEntityBox(Entity* ent, float lineWidth, bool outline);
	static void draw2D(Entity* ent, float lineWidth);
	static void drawNameTags(Entity* ent, float textSize, bool drawHealth = false, bool useUnicodeFont = false);
	static void drawItem(ItemStack* item, const Vec2& ItemPos, float opacity, float scale, bool isEnchanted, int mode = 0);
	static void drawDoubleText(const Vec2& pos, std::string* FirstText, std::string* SecondText, const MC_Color& FirstColor, const MC_Color& SecondColor, float textSize = 1, float alpha = 1, DesiredUi ui = DesiredUi::DEFAULT);
	static float getLerpTime();
	static Vec3 getOrigin();
	static void drawMinimap(class Minimap* mod);
	static Vec2 worldToScreen(const Vec3& world);
	inline static float lerp(double a1, double a2, double a3) { return (float)(a1 + (a2 - a1) * a3); }
	inline static int lerpInt(int a1, int a2, double a3) { return (int)lerp(a1, a2, (float)a3); }
	inline static MC_Color lerpColor(MC_Color color1, MC_Color color2, float amount) {
		amount = std::min(1.f, std::max(0.f, amount));
		return MC_Color(lerpInt((int)(color1.r * 255.f), (int)(color2.r * 255.f), (int)amount), lerpInt((int)(color1.g * 255.f), (int)(color2.g * 255.f), (int)amount), lerpInt((int)(color1.b * 255.f), (int)(color2.b * 255.f), (int)amount), lerpInt((int)(color1.a * 255.f), (int)(color2.a * 255.f), (int)amount));
	}

	inline static MC_Color lerpColors(int speed, int index, MC_Color start, MC_Color end) {
		int angle = (int)(((TimerUtils::getCurrentMs()) / speed + (index * 50)) % 360);
		return DrawUtils::lerpColor(start, end, ((angle >= 180 ? 360 - angle : angle) * 2) / 360.0f);
	}
};
