#include "DrawUtils.h"
#include "../Utils/Logger.h"
#include "../Utils/svpng.h"
#include "Module/ModuleManager.h"
#include "Config/ConfigManager.h"

MinecraftUIRenderContext* renderCtx;
GuiData* guiData;
ScreenContext* ScreenContext2D;
ScreenContext* ScreenContext3D;
Tessellator* Tessellator2D;
Tessellator* Tessellator3D;
float* colorHolder;
std::shared_ptr<glmatrixf> refdef;
Vec2 fov;
Vec2 screenSize;
Vec3 origin;
float lerpT;
TexturePtr* texturePtr = nullptr;

static MaterialPtr* uiMaterial = nullptr;
static MaterialPtr* blendMaterial = nullptr;
static MaterialPtr* entityFlatStaticMaterial = nullptr;

bool isSmoothFont = false;
bool isTenFont = false;

void DrawUtils::setCtx(MinecraftUIRenderContext* ctx, GuiData* gui) {
	LARGE_INTEGER EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - Game.getLastUpdateTime();

	ElapsedMicroseconds.QuadPart *= 1000000;
	int ticksPerSecond = 20;
	if (Game.getMinecraft())
		if (Game.getMinecraft()->timer)
			ticksPerSecond = (int)*Game.getMinecraft()->timer;
	if (ticksPerSecond < 1)
		ticksPerSecond = 1;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
	lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
	if (lerpT > 1)
		lerpT = 1;
	else if (lerpT < 0)
		lerpT = 0;

	guiData = gui;
	renderCtx = ctx;
	ScreenContext2D = reinterpret_cast<ScreenContext**>(renderCtx)[2];

	Tessellator2D = ScreenContext2D->getTessellator();
	colorHolder = ScreenContext2D->getColorHolder();

	glmatrixf* badrefdef = Game.getClientInstance()->getRefDef();

	refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	fov = Game.getClientInstance()->getFov();
	screenSize.x = gui->widthGame;
	screenSize.y = gui->heightGame;
	if (Game.getLevelRenderer())
		origin = Game.getLevelRenderer()->getOrigin();

	if (!uiMaterial ) {
		uiMaterial = new MaterialPtr("ui_fill_color");
	}
	if (!blendMaterial) {
		blendMaterial = new MaterialPtr("fullscreen_cube_overlay_blend");
	}
	if (!entityFlatStaticMaterial) {
		entityFlatStaticMaterial = new MaterialPtr("entity_flat_color_line", true);
	}
}

void DrawUtils::setCtx3D(ScreenContext* ctx) {
	ScreenContext3D = ctx;
	if (Game.getLevelRenderer())
		origin = Game.getLevelRenderer()->getOrigin();

	if (ctx) {
		LARGE_INTEGER EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - Game.getLastUpdateTime();

		ElapsedMicroseconds.QuadPart *= 1000000;
		int ticksPerSecond = 20;
		if (Game.getMinecraft())
			ticksPerSecond = (int)*Game.getMinecraft()->timer;
		if (ticksPerSecond < 1)
			ticksPerSecond = 1;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
		lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
		if (lerpT > 1)
			lerpT = 1;
		else if (lerpT < 0)
			lerpT = 0;

		Tessellator3D = ScreenContext3D->getTessellator();
	}
}

void DrawUtils::SetColor(float r, float g, float b, float a) {
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

MC_Color DrawUtils::getRainbowWave(float speed, float saturation, long index, bool invert) {
	if (invert)
		index = -index;
	long Index = (long)(index * 75.f) * 2;
	float hue = (((__int64)(TimerUtils::getTime()) + Index) % (int)(speed * 1000) / (speed * 1000.f));
	float r, g, b = 0;
	Utils::ColorConvertHSVtoRGB(hue, saturation, 1.f, r, g, b);
	return MC_Color(true, r * 255, g * 255, b * 255);
}

void DrawUtils::TessellatorBegin(VertexFormat vertexFormat, int reservedVerts) {
	return Tessellator2D->begin(vertexFormat, reservedVerts);
}
void DrawUtils::TessellatorVertex(float x, float y, float z) {
	return Tessellator2D->vertex(x, y, z);
}
void DrawUtils::TessellatorVertex3D(float x, float y, float z) {
	return Tessellator2D->vertex(x - origin.x, y - origin.y, z - origin.z);
}
void DrawUtils::TessellatorSetRotation(float angle, Vec3 pivot) {
	return Tessellator2D->setRotation(angle, pivot);
}
void DrawUtils::TessellatorResetTransform(bool a1) {
	return Tessellator2D->resetTransform(a1);
}
void DrawUtils::TessellatorColor(MC_Color& col) {
	return Tessellator2D->color(col.r, col.g, col.b, col.a);
}
void DrawUtils::FlushTessellator() {
	return Tessellator2D->renderMeshImmediately(DrawUtils::getScreenContext(), uiMaterial);
}
void DrawUtils::FlushTessellator3D() {
	return Tessellator2D->renderMeshImmediately(DrawUtils::getScreenContext(), entityFlatStaticMaterial);
}

Font* DrawUtils::getFont(DesiredUi ui) {
	static auto hudMod = moduleMgr->getModule<HudModule>("HUD");
	static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	static auto tabGuiMod = moduleMgr->getModule<TabGuiMod>("TabGui");
	static auto arraylistMod = moduleMgr->getModule<Arraylist>("Arraylist");
	int selectedEntery = 0;

	switch (ui) {
	case DesiredUi::HUD:
		selectedEntery = hudMod->font.GetSelectedEntry().GetValue();
		break;
	case DesiredUi::CLICKGUI:
		selectedEntery = clickGuiMod->font.GetSelectedEntry().GetValue();
		break;
	case DesiredUi::TABGUI:
		selectedEntery = tabGuiMod->font.GetSelectedEntry().GetValue();
		break;
	case DesiredUi::ARRAYLIST:
		selectedEntery = arraylistMod->font.GetSelectedEntry().GetValue();
		break;
	case DesiredUi::DEFAULT:
		selectedEntery = 0;
		break;
	}
	auto fontRepo = Game.getMinecraftGame()->getFontRepo();
	Font* fnt = nullptr;
	isSmoothFont = false;
	isTenFont = false;
	switch (selectedEntery) {
	case 0:
		fnt = fontRepo->getFont(FontType::OldSmoothFont);
		isSmoothFont = true;
		break;
	case 1:
		fnt = fontRepo->getFont(FontType::UIFont);
		break;
	case 2:
		isTenFont = true;
		fnt = fontRepo->getFont(FontType::MinecraftTen);
		break;
	}
	return fnt;
}

ScreenContext* DrawUtils::getScreenContext() {
	return ScreenContext2D;
}

MatrixStack* DrawUtils::getMatrixStack() {
	return reinterpret_cast<MatrixStack*>(*reinterpret_cast<__int64*>(DrawUtils::getScreenContext() + 0x18i64) + 0x30i64);
}

float DrawUtils::getTextWidth(std::string* textStr, float textSize, DesiredUi ui) {

	Font* fontPtr = getFont(ui);

	float ret = renderCtx->getLineLength(fontPtr, textStr, textSize, false);

	return ret;
}

float DrawUtils::getFontHeight(float textSize, DesiredUi ui) {
	Font* fontPtr = getFont(ui);

	float ret = fontPtr->getLineHeight() * textSize;

	return ret;
}

void DrawUtils::Flush() {
	renderCtx->flushText(0);
}

void DrawUtils::drawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
	Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

	Tessellator2D->vertex(p1.x, p1.y, 0);
	Tessellator2D->vertex(p2.x, p2.y, 0);
	Tessellator2D->vertex(p3.x, p3.y, 0);

	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
}

void DrawUtils::drawQuad(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4) {
	Tessellator2D->begin(VertexFormat::QUAD);

	Tessellator2D->vertex(p1.x, p1.y, 0);
	Tessellator2D->vertex(p2.x, p2.y, 0);
	Tessellator2D->vertex(p3.x, p3.y, 0);
	Tessellator2D->vertex(p4.x, p4.y, 0);

	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
}

void DrawUtils::drawPolygon(const Vec2& center, const Vec2& size, int numSides) {
	const float angleIncrement = 2.0f * PI / numSides;

	std::vector<Vec2> vertices;
	for (int i = 0; i < numSides; i++) {
		float angle = i * angleIncrement;
		vertices.emplace_back(center.x + size.x * std::cos(angle), center.y + size.y * std::sin(angle));
	}

	Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

	for (int i = 0; i < numSides; i++) {
		Tessellator2D->vertex(vertices[i].x, vertices[i].y, 0);
		Tessellator2D->vertex(vertices[(i + 1) % numSides].x, vertices[(i + 1) % numSides].y, 0);
		Tessellator2D->vertex(center.x, center.y, 0);
	}
	Tessellator2D->vertex(vertices[0].x, vertices[0].y, 0);
	Tessellator2D->vertex(vertices[1].x, vertices[1].y, 0);

	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
}

void DrawUtils::drawPolygonOutline(const Vec2& center, const Vec2& size, int numSides, float thickness) {
	const float angleIncrement = 2.0f * PI / numSides;

	std::vector<Vec2> outerVertices;
	std::vector<Vec2> innerVertices;
	for (int i = 0; i < numSides; i++) {
		float angle = i * angleIncrement;
		outerVertices.emplace_back(center.x + size.x * std::cos(angle), center.y + size.y * std::sin(angle));
		innerVertices.emplace_back(center.x + (size.x - thickness) * std::cos(angle), center.y + (size.y - thickness) * std::sin(angle));
	}

	Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

	for (int i = 0; i < numSides; i++) {
		Tessellator2D->vertex(outerVertices[i].x, outerVertices[i].y, 0);
		Tessellator2D->vertex(innerVertices[i].x, innerVertices[i].y, 0);
		Tessellator2D->vertex(outerVertices[(i + 1) % numSides].x, outerVertices[(i + 1) % numSides].y, 0);
		Tessellator2D->vertex(innerVertices[(i + 1) % numSides].x, innerVertices[(i + 1) % numSides].y, 0);
	}

	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
}

void DrawUtils::drawTextInWorld(std::string* textToSay, const Vec3& location, float tsize, Vec3i tColor, Vec3i bgColor) {
	Vec2 textPos;
	Vec4 rectPos;

	float textWidth = getTextWidth(textToSay, tsize);
	
	float textHeight = DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() * tsize;
	Vec3 actualLocation = location.add(0.5f);

	if (refdef->OWorldToScreen(origin, actualLocation, textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * tsize;
		rectPos.y = textPos.y - 1.f * tsize;
		rectPos.z = textPos.x + textWidth + 1.f * tsize;
		rectPos.w = textPos.y + textHeight + 2.f * tsize;
		Vec4 subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * tsize;
		fillRect(rectPos, MC_Color(bgColor.x, bgColor.y, bgColor.z), 5.0f);

		drawText(textPos, textToSay, MC_Color(tColor.x, tColor.y, tColor.z), tsize);
	}
}

void DrawUtils::drawRoundRect(Vec4 pos, CornerRadius radius, MC_Color color, float step) {
	DrawUtils::SetColor(color.r, color.g, color.b, color.a);
	float length = pos.z - pos.x;
	float height = pos.w - pos.y;
	Vec2 topL = {pos.x + radius.topL, pos.y + radius.topL};
	Vec2 topR = {pos.x + length - radius.topR, pos.y + radius.topR};
	Vec2 bottomR = {pos.x + length - radius.bottomR, pos.y + height - radius.bottomR};
	Vec2 bottomL = {pos.x + radius.bottomL, pos.y + height - radius.bottomL};
	float x, y;
	Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);
	float angle = 0.f;
	while (angle <= 360) {
		float rad = PI * angle / 180;
		switch ((int)angle / 90) {
		case 0:
			x = bottomR.x + radius.bottomR * cos(rad);
			y = bottomR.y + radius.bottomR * sin(rad);
			Tessellator2D->vertex(x, y, 0);
			Tessellator2D->vertex(bottomR.x, bottomR.y, 0);
			break;
		case 1:
			x = bottomL.x + radius.bottomL * cos(rad);
			y = bottomL.y + radius.bottomL * sin(rad);
			Tessellator2D->vertex(x, y, 0);
			Tessellator2D->vertex(bottomL.x, bottomL.y, 0);
			break;
		case 2:
			x = topL.x + radius.topL * cos(rad);
			y = topL.y + radius.topL * sin(rad);
			Tessellator2D->vertex(x, y, 0);
			Tessellator2D->vertex(topL.x, topL.y, 0);
			break;
		case 3:
			x = topR.x + radius.topR * cos(rad);
			y = topR.y + radius.topR * sin(rad);
			Tessellator2D->vertex(x, y, 0);
			Tessellator2D->vertex(topR.x, topR.y, 0);
			break;
		}
		angle += step;
	}
	Tessellator2D->vertex(bottomR.x + radius.bottomR, bottomR.y, 0);
	Tessellator2D->vertex(bottomR.x, bottomR.y, 0);
	Tessellator2D->vertex(bottomL.x, bottomL.y, 0);
	Tessellator2D->vertex(topL.x, topL.y, 0);
	Tessellator2D->vertex(topR.x, topR.y, 0);
	Tessellator2D->vertex(bottomR.x, bottomR.y, 0);

	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
}

void DrawUtils::drawRoundRectOutline(Vec4 pos, CornerRadius radius, MC_Color color, float lineWidth, double quality) {
	radius.bottomR -= (lineWidth / 2);
	radius.bottomL -= (lineWidth / 2);
	radius.topL -= (lineWidth / 2);
	radius.topR -= (lineWidth / 2);
	std::vector<Vec2> linesBR;
	std::vector<Vec2> linesBL;
	std::vector<Vec2> linesTL;
	std::vector<Vec2> linesTR;
	DrawUtils::SetColor(color.r, color.g, color.b, color.a);
	float length = pos.z - pos.x;
	float height = pos.w - pos.y;
	Vec2 topL = {pos.x + radius.topL + (lineWidth / 2), pos.y + radius.topL + (lineWidth / 2)};
	Vec2 topR = {pos.x + length - radius.topR - (lineWidth / 2), pos.y + radius.topR + (lineWidth / 2)};
	Vec2 bottomR = {pos.x + length - radius.bottomR - (lineWidth / 2), pos.y + height - radius.bottomR - (lineWidth / 2)};
	Vec2 bottomL = {pos.x + radius.bottomL + (lineWidth / 2), pos.y + height - radius.bottomL - (lineWidth / 2)};

	for (float i = 0; i < 360;) {
		float angle = i * PI / 180;
		if (i >= 0.f && i <= 90.f) {
			float x = bottomR.x + (float)cos(angle) * radius.bottomR;
			float y = bottomR.y + (float)sin(angle) * radius.bottomR;
			linesBR.emplace_back(x, y);
		}
		if (i >= 90.f && i <= 180.f) {
			float x = bottomL.x + (float)cos(angle) * radius.bottomL;
			float y = bottomL.y + (float)sin(angle) * radius.bottomL;
			linesBL.emplace_back(x, y);
		}
		if (i >= 180.f && i <= 270.f) {
			float x = topL.x + (float)cos(angle) * radius.topL;
			float y = topL.y + (float)sin(angle) * radius.topL;
			linesTL.emplace_back(x, y);
		}
		if (i >= 270.f && i <= 360.f) {
			float x = topR.x + (float)cos(angle) * radius.topR;
			float y = topR.y + (float)sin(angle) * radius.topR;
			linesTR.emplace_back(x, y);
		}
		i += (float)quality;
	}

	Vec2 lastBR = linesBR[0];
	for (Vec2 line : linesBR) {
		auto start = lastBR;
		auto end = line;

		float modX = 0 - (start.y - end.y);
		float modY = start.x - end.x;

		float len = sqrtf(modX * modX + modY * modY);

		modX /= len;
		modY /= len;
		modX *= lineWidth;
		modY *= lineWidth;

		Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(start.x - modX, start.y - modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(end.x + modX, end.y + modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		lastBR = line;
	}

	Vec2 lastBL = linesBL[0];
	for (Vec2 line : linesBL) {
		auto start = lastBL;
		auto end = line;

		float modX = 0 - (start.y - end.y);
		float modY = start.x - end.x;

		float len = sqrtf(modX * modX + modY * modY);

		modX /= len;
		modY /= len;
		modX *= lineWidth;
		modY *= lineWidth;

		Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(start.x - modX, start.y - modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(end.x + modX, end.y + modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		lastBL = line;
	}

	Vec2 lastTL = linesTL[0];
	for (Vec2 line : linesTL) {
		auto start = lastTL;
		auto end = line;

		float modX = 0 - (start.y - end.y);
		float modY = start.x - end.x;

		float len = sqrtf(modX * modX + modY * modY);

		modX /= len;
		modY /= len;
		modX *= lineWidth;
		modY *= lineWidth;

		Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(start.x - modX, start.y - modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(end.x + modX, end.y + modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		lastTL = line;
	}
	
	Vec2 lastTR = linesTR[0];
	for (Vec2 line : linesTR) {
		auto start = lastTR;
		auto end = line;

		float modX = 0 - (start.y - end.y);
		float modY = start.x - end.x;

		float len = sqrtf(modX * modX + modY * modY);

		modX /= len;
		modY /= len;
		modX *= lineWidth;
		modY *= lineWidth;

		Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(start.x - modX, start.y - modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
		Tessellator2D->vertex(end.x + modX, end.y + modY, 0);
		Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

		lastTR = line;
	}
	Tessellator2D->renderMeshImmediately(ScreenContext2D, blendMaterial);

	DrawUtils::fillRect(Vec4(topL.x - radius.topL - lineWidth, topL.y, bottomL.x - radius.bottomL + lineWidth, bottomL.y), MC_Color(color.r, color.g, color.b), color.a);
	DrawUtils::fillRect(Vec4(topL.x, topL.y - radius.topL - lineWidth, topR.x, topR.y - radius.topR + lineWidth), MC_Color(color.r, color.g, color.b), color.a);
	DrawUtils::fillRect(Vec4(topR.x + radius.topR - lineWidth, topR.y, bottomR.x + radius.bottomR + lineWidth, bottomR.y), MC_Color(color.r, color.g, color.b), color.a);
	DrawUtils::fillRect(Vec4(bottomL.x, bottomL.y + radius.bottomL - lineWidth, bottomR.x, bottomR.y + radius.bottomR + lineWidth), MC_Color(color.r, color.g, color.b), color.a);
}

void DrawUtils::drawLine(const Vec2& start, const Vec2& end, float lineWidth) {
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	Tessellator2D->begin(VertexFormat::TRIANGLE_STRIP);

	Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
	Tessellator2D->vertex(start.x - modX, start.y - modY, 0);
	Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

	Tessellator2D->vertex(start.x + modX, start.y + modY, 0);
	Tessellator2D->vertex(end.x + modX, end.y + modY, 0);
	Tessellator2D->vertex(end.x - modX, end.y - modY, 0);

	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
}

void DrawUtils::drawText(const Vec2& pos, std::string* textStr, const MC_Color& color, float textSize, float alpha, DesiredUi ui, bool drawShadow) {
	std::string text(*textStr);
	Font* fontPtr = getFont(ui);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;
	static ClickGuiMod* clickGui = moduleMgr->getModule<ClickGuiMod>("ClickGui");

	float posF[4];  // Vec4(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x + 1000;
	posF[2] = pos.y + (isSmoothFont ? -1 : (isTenFont ? -3 : 0));
	posF[3] = pos.y + 1000;

	TextMeasureData textMeasure{};
	memset(&textMeasure, 0, sizeof(TextMeasureData));
	textMeasure.textSize = textSize;
	textMeasure.shadow = drawShadow;

	renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &textMeasure, &caretMeasureData);
}
void DrawUtils::drawRainbowText(Vec2& pos, const std::string& textStr, float speed, float saturation, bool invert, float textSize, float alpha, DesiredUi ui, bool drawShadow) {
	int ind = 1;
	std::string afterColor;
	std::string beforeColor;
	size_t charPos = textStr.find(BASE_CHARACTER);
	if (charPos != std::string::npos) {
		afterColor = textStr.substr(charPos);
		beforeColor = textStr.substr(0, charPos);
	}
	for (char c : beforeColor) {
		ind++;
		float charWidth = DrawUtils::getTextWidth(&std::string(&c), textSize, ui);
		MC_Color Color = DrawUtils::getRainbowWave(speed, saturation, ind, invert);
		DrawUtils::drawText(pos, &std::string(&c), Color, textSize, alpha, ui, drawShadow);
		pos.x += charWidth;
	}

	DrawUtils::drawText(pos, &afterColor, MC_Color(0.f, 0.f, 0.f), textSize, alpha, ui, drawShadow);
}
void DrawUtils::drawName(Entity* ent, std::string* name, float textSize, const MC_Color& background, const MC_Color& color, float opacity, DesiredUi ui) {
	Vec2 textPos;
	Vec4 rectPos;
	float textWidth = getTextWidth(name, textSize);
	float textHeight = DrawUtils::getFont(ui)->getLineHeight() * textSize;
	Vec3 goodPos = {
		ent->getPos()->x + (ent->getPos()->x - ent->getPosOld()->x),
		ent->getPos()->y + (ent->getPos()->y - ent->getPosOld()->y),
		ent->getPos()->z + (ent->getPos()->z - ent->getPosOld()->z)};
	if (name->compare("coin") == 0)
		goodPos.y += (ent->aabbShape->aabb.upper.y - ent->aabbShape->aabb.lower.y);
	if (refdef->OWorldToScreen(origin, goodPos.add(0, 0.5f, 0), textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * textSize;
		rectPos.y = textPos.y - 1.f * textSize;
		rectPos.z = textPos.x + textWidth + 1.f * textSize;
		rectPos.w = textPos.y + textHeight + 2.f * textSize;
		drawRoundRect(Vec4(rectPos.x - 3.f, (rectPos.y), rectPos.z + 3.f, rectPos.w), CornerRadius((rectPos.w - (rectPos.y)) / 3.f), MC_Color{background.r, background.g, background.b, opacity});
		drawText(textPos, name, color, textSize,opacity);
	}
}
void DrawUtils::drawDoubleText(const Vec2& pos, std::string* FirstText, std::string* SecondText, const MC_Color& FirstColor, const MC_Color& SecondColor, float textSize, float alpha, DesiredUi ui) {
	float Spacing = DrawUtils::getTextWidth(FirstText, textSize) + 0.25f;
	Vec2 Pos2 = Vec2(pos.x + Spacing, pos.y);  //we get the pos of the second text after adding the width of the first

	drawText(pos, FirstText, FirstColor, textSize, alpha, ui);     //first string value
	drawText(Pos2, SecondText, SecondColor, textSize, alpha, ui);  //second string value
}

void DrawUtils::drawBox(const Vec3& lower, const Vec3& upper, float lineWidth, bool outline) {
	Vec3 diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	Vec3 vertices[8];
	vertices[0] = Vec3(lower.x, lower.y, lower.z);
	vertices[1] = Vec3(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = Vec3(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = Vec3(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = Vec3(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = Vec3(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = Vec3(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = Vec3(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, Vec2>> screenCords;
	for (int i = 0; i < 8; i++) {
		Vec2 screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, Vec2> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	Vec2 lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		Vec2 smallestDir;
		std::tuple<int, Vec2> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			Vec2 dir = Vec2(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw

	Vec2 lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		Vec2 curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}

		drawLine(lastVertex, curVertex, lineWidth);
		lastVertex = curVertex;
	}
}

void DrawUtils::drawImage(const std::string& filePath, Vec2& imagePos, Vec2& ImageDimension, Vec2& uvPos, Vec2& uvSize, bool external) {
	auto texturePtr = std::make_unique<TexturePtr>();
	ResourceLocation file(filePath, external);
	renderCtx->getTexture(texturePtr.get(), &file, false);
	if (texturePtr->textureData) {
		renderCtx->drawImage(texturePtr.get(), imagePos, ImageDimension, uvPos, uvSize);
		flushImages();
	}
}

void DrawUtils::drawImageFromResource(int resource, const std::string& imageName, Vec2 const& imagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize) {
	DrawUtils::resourceToTexturePtr(resource, imageName);
	DrawUtils::drawImageFromTexturePtr(DrawUtils::resourceToTexturePtr(resource, imageName), imagePos, ImageDimension, uvPos, uvSize);
}

std::string DrawUtils::resourceToFilePath(int resource, const std::string& name, const std::string& extension) {
	auto resourceFile = FindResourceA((HMODULE)Game.getDllModule(), MAKEINTRESOURCEA(resource), (LPCSTR)RT_RCDATA);
	if (resourceFile == 0) {
		logF("%s resource not found", name);
		return "NULL";
	}

	auto data = LoadResource((HMODULE)Game.getDllModule(), resourceFile);
	if (data == 0) {
		logF("%s resource not loaded", name);
		return "NULL";
	}

	auto dataPtr = LockResource(data);
	auto dataSize = SizeofResource((HMODULE)Game.getDllModule(), resourceFile);

	std::string path = Utils::getRoamingStatePath();
	path += "\\Horion\\Assets\\";
	std::filesystem::create_directory(path.c_str());
	std::string filePath = path + name + "." + extension;
	if (!(std::filesystem::exists(filePath))) {
		std::ofstream outfile;
		outfile.open(filePath, std::ofstream::binary);
		outfile.write((const char*)dataPtr, dataSize);
		outfile.close();
	}
	return filePath;
}

void DrawUtils::drawImageFromTexturePtr(std::shared_ptr<TexturePtr> texture, Vec2 const& imagePos, Vec2 const& ImageDimension, Vec2 const& uvPos, Vec2 const& uvSize) {
	if (texture->textureData) {
		renderCtx->drawImage(texture.get(), imagePos, ImageDimension, uvPos, uvSize);
	}
}

std::shared_ptr<TexturePtr> DrawUtils::rawDataToTexturePtr(const unsigned char* data, int width, int height, bool hasAlpha, const std::string& name) {
	auto txtPtr = std::make_shared<TexturePtr>();

	std::string path = Utils::getRoamingStatePath();
	path += "\\Horion\\Assets\\";
	std::filesystem::create_directory(path.c_str());
	std::string filePath = path + name + ".png";
	if (!(std::filesystem::exists(filePath))) {
		FILE* fp;
		errno_t err = fopen_s(&fp, filePath.c_str(), "wb");
		svpng(fp, width, height, data, hasAlpha);
		fclose(fp);
	}
	ResourceLocation file(filePath, true);
	renderCtx->getTexture(txtPtr.get(), &file, false);
	return txtPtr;
}

std::shared_ptr<TexturePtr> DrawUtils::resourceToTexturePtr(int resource, const std::string& name) {
	auto txtPtr = std::make_shared<TexturePtr>();
	auto resourceFile = FindResourceA((HMODULE)Game.getDllModule(), MAKEINTRESOURCEA(resource), (LPCSTR)RT_RCDATA);
	if (resourceFile == 0) {
		logF("%s resource not found", name);
		return 0;
	}

	auto data = LoadResource((HMODULE)Game.getDllModule(), resourceFile);
	if (data == 0) {
		logF("%s resource not loaded", name);
		return 0;
	}

	auto dataPtr = LockResource(data);
	auto dataSize = SizeofResource((HMODULE)Game.getDllModule(), resourceFile);

	std::string path = Utils::getRoamingStatePath();
	path += "\\Horion\\Assets\\";
	std::filesystem::create_directory(path.c_str());
	std::string filePath = path + name + ".png";
	if (!(std::filesystem::exists(filePath))) {
		std::ofstream outfile;
		outfile.open(filePath, std::ofstream::binary);
		outfile.write((const char*)dataPtr, dataSize);
		outfile.close();
	}
	ResourceLocation file(filePath, true);
	renderCtx->getTexture(txtPtr.get(), &file, false);
	return txtPtr;
}

void DrawUtils::drawNineSliceFromBase64(std::string base64, std::string imageName, Vec2& imagePos, Vec2& ImageDimension, float outlineWidth) {
	texturePtr = new TexturePtr();

	auto wide = ConfigManager::GetRoamingFolderPath();
	std::string path(wide.length(), 0);
	std::transform(wide.begin(), wide.end(), path.begin(), [](wchar_t c) {
		1;
		return (char)c;
	});
	path += "\\Horion\\Assets\\";
	std::filesystem::create_directory(path.c_str());
	std::string filePath = path + imageName + ".png";
	if (!(std::filesystem::exists(filePath))) {
		std::ofstream outfile;
		outfile.open(filePath, std::ofstream::binary);
		std::string temp = Utils::base64_decode(base64);
		outfile.write(temp.c_str(), temp.size());
		outfile.close();
	}
	ResourceLocation file(filePath, true);
	renderCtx->getTexture(texturePtr, &file, false);

	if (texturePtr->textureData) {
		//Top Left
		Vec2 posTL(imagePos.x, imagePos.y);
		Vec2 uvPosTL(0.f, 0.f);
		Vec2 uvSizeTL(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr, posTL, Vec2(outlineWidth, outlineWidth), uvPosTL, uvSizeTL);
		//Top Middle
		Vec2 posTM(imagePos.x + outlineWidth, imagePos.y);
		Vec2 uvPosTM(outlineWidth / 100.f, 0.f);
		Vec2 uvSizeTM(1.f - (outlineWidth * 2.f / 100.f), outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr, posTM, Vec2(ImageDimension.x - outlineWidth * 2.f, outlineWidth), uvPosTM, uvSizeTM);
		//Top Right
		Vec2 posTR(imagePos.x + ImageDimension.x - outlineWidth, imagePos.y);
		Vec2 uvPosTR(1.f - (outlineWidth / 100.f), 0.f);
		Vec2 uvSizeTR(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr, posTR, Vec2(outlineWidth, outlineWidth), uvPosTR, uvSizeTR);
		//Left Middle
		Vec2 posLM(imagePos.x, imagePos.y + outlineWidth);
		Vec2 uvPosLM(0.f, outlineWidth / 100.f);
		Vec2 uvSizeLM(outlineWidth / 100.f, 1.f - (outlineWidth * 2.f / 100.f));
		renderCtx->drawImage(texturePtr, posLM, Vec2(outlineWidth, ImageDimension.y - outlineWidth * 2.f), uvPosLM, uvSizeLM);
		//Right Middle
		Vec2 posRM(imagePos.x + ImageDimension.x - outlineWidth, imagePos.y + outlineWidth);
		Vec2 uvPosRM(1.f - (outlineWidth / 100.f), outlineWidth / 100.f);
		Vec2 uvSizeRM(outlineWidth / 100.f, 1.f - (outlineWidth * 2.f / 100.f));
		renderCtx->drawImage(texturePtr, posRM, Vec2(outlineWidth, ImageDimension.y - outlineWidth * 2.f), uvPosRM, uvSizeRM);
		//Bottom Left
		Vec2 posBL(imagePos.x, imagePos.y + ImageDimension.y - outlineWidth);
		Vec2 uvPosBL(0.f, 1.f - (outlineWidth / 100.f));
		Vec2 uvSizeBL(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr, posBL, Vec2(outlineWidth, outlineWidth), uvPosBL, uvSizeBL);
		//Bottom Middle
		Vec2 posBM(imagePos.x + outlineWidth, imagePos.y + ImageDimension.y - outlineWidth);
		Vec2 uvPosBM(outlineWidth / 100.f, 1.f - (outlineWidth / 100.f));
		Vec2 uvSizeBM(1.f - (outlineWidth * 2.f / 100.f), outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr, posBM, Vec2(ImageDimension.x - outlineWidth * 2.f, outlineWidth), uvPosBM, uvSizeBM);
		//Bottom Right
		Vec2 posBR(imagePos.x + ImageDimension.x - outlineWidth, imagePos.y + ImageDimension.y - outlineWidth);
		Vec2 uvPosBR(1.f - (outlineWidth / 100.f), 1.f - (outlineWidth / 100.f));
		Vec2 uvSizeBR(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr, posBR, Vec2(outlineWidth, outlineWidth), uvPosBR, uvSizeBR);
		//Middle
		Vec2 posM(imagePos.x + outlineWidth, imagePos.y + outlineWidth);
		Vec2 uvPosM(outlineWidth / 100.f, outlineWidth / 100.f);
		Vec2 uvSizeM(1.f - (outlineWidth * 2.f / 100.f), 1.f - (outlineWidth * 2.f / 100.f));
		renderCtx->drawImage(texturePtr, posM, Vec2(ImageDimension.x - outlineWidth * 2.f, ImageDimension.y - outlineWidth * 2.f), uvPosM, uvSizeM);
	}
}

void DrawUtils::drawNineSlice(std::string filePath, Vec2& imagePos, Vec2& ImageDimension, float outlineWidth) {
	auto texturePtr = std::make_unique<TexturePtr>();

	ResourceLocation file(filePath, false);
	renderCtx->getTexture(texturePtr.get(), &file, false);
	if (texturePtr->textureData) {
		//Top Left
		Vec2 posTL(imagePos.x, imagePos.y);
		Vec2 uvPosTL(0.f, 0.f);
		Vec2 uvSizeTL(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr.get(), posTL, Vec2(outlineWidth, outlineWidth), uvPosTL, uvSizeTL);
		//Top Middle
		Vec2 posTM(imagePos.x + outlineWidth, imagePos.y);
		Vec2 uvPosTM(outlineWidth / 100.f, 0.f);
		Vec2 uvSizeTM(1.f - (outlineWidth * 2.f / 100.f), outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr.get(), posTM, Vec2(ImageDimension.x - outlineWidth * 2.f, outlineWidth), uvPosTM, uvSizeTM);
		//Top Right
		Vec2 posTR(imagePos.x + ImageDimension.x - outlineWidth, imagePos.y);
		Vec2 uvPosTR(1.f - (outlineWidth / 100.f), 0.f);
		Vec2 uvSizeTR(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr.get(), posTR, Vec2(outlineWidth, outlineWidth), uvPosTR, uvSizeTR);
		//Left Middle
		Vec2 posLM(imagePos.x, imagePos.y + outlineWidth);
		Vec2 uvPosLM(0.f, outlineWidth / 100.f);
		Vec2 uvSizeLM(outlineWidth / 100.f, 1.f - (outlineWidth * 2.f / 100.f));
		renderCtx->drawImage(texturePtr.get(), posLM, Vec2(outlineWidth, ImageDimension.y - outlineWidth * 2.f), uvPosLM, uvSizeLM);
		//Right Middle
		Vec2 posRM(imagePos.x + ImageDimension.x - outlineWidth, imagePos.y + outlineWidth);
		Vec2 uvPosRM(1.f - (outlineWidth / 100.f), outlineWidth / 100.f);
		Vec2 uvSizeRM(outlineWidth / 100.f, 1.f - (outlineWidth * 2.f / 100.f));
		renderCtx->drawImage(texturePtr.get(), posRM, Vec2(outlineWidth, ImageDimension.y - outlineWidth * 2.f), uvPosRM, uvSizeRM);
		//Bottom Left
		Vec2 posBL(imagePos.x, imagePos.y + ImageDimension.y - outlineWidth);
		Vec2 uvPosBL(0.f, 1.f - (outlineWidth / 100.f));
		Vec2 uvSizeBL(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr.get(), posBL, Vec2(outlineWidth, outlineWidth), uvPosBL, uvSizeBL);
		//Bottom Middle
		Vec2 posBM(imagePos.x + outlineWidth, imagePos.y + ImageDimension.y - outlineWidth);
		Vec2 uvPosBM(outlineWidth / 100.f, 1.f - (outlineWidth / 100.f));
		Vec2 uvSizeBM(1.f - (outlineWidth * 2.f / 100.f), outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr.get(), posBM, Vec2(ImageDimension.x - outlineWidth * 2.f, outlineWidth), uvPosBM, uvSizeBM);
		//Bottom Right
		Vec2 posBR(imagePos.x + ImageDimension.x - outlineWidth, imagePos.y + ImageDimension.y - outlineWidth);
		Vec2 uvPosBR(1.f - (outlineWidth / 100.f), 1.f - (outlineWidth / 100.f));
		Vec2 uvSizeBR(outlineWidth / 100.f, outlineWidth / 100.f);
		renderCtx->drawImage(texturePtr.get(), posBR, Vec2(outlineWidth, outlineWidth), uvPosBR, uvSizeBR);
		//Middle
		Vec2 posM(imagePos.x + outlineWidth, imagePos.y + outlineWidth);
		Vec2 uvPosM(outlineWidth / 100.f, outlineWidth / 100.f);
		Vec2 uvSizeM(1.f - (outlineWidth * 2.f / 100.f), 1.f - (outlineWidth * 2.f / 100.f));
		renderCtx->drawImage(texturePtr.get(), posM, Vec2(ImageDimension.x - outlineWidth * 2.f, ImageDimension.y - outlineWidth * 2.f), uvPosM, uvSizeM);
		flushImages();
	}
}

void DrawUtils::flushImages(MC_Color color) { //Use Int colors
	static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");

	renderCtx->flushImages(color, 1.f, flushString);
}

void DrawUtils::drawNameTags(Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	Vec2 textPos;
	Vec4 rectPos;
	Vec2 itemPos;

	std::string text = NameTags::getSanitizedNameTag(*ent);

	std::string pattern("\n");
	size_t index;
	while ((index = text.find_first_of(pattern)) != std::string::npos)
		text = text.substr(0, index);

	float textWidth = getTextWidth(&text, textSize);
	float textHeight = DrawUtils::getFont(DesiredUi::HUD)->getLineHeight() * textSize;

	Vec3* start = ent->getPosOld();
	Vec3* end = ent->getPos();

	Vec3 lerped = start->lerp(end, lerpT);

	if (refdef->OWorldToScreen(origin, lerped.add(0.f, (ent->aabbShape->height / 2.f), 0.f), textPos, fov, screenSize)) {
		static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>("ClickGui");
		static auto nametagsMod = moduleMgr->getModule<NameTags>("NameTags");
		static auto noFriendsModule = moduleMgr->getModule<NoFriends>("NoFriends");

		MC_Color textCol = MC_Color(1.f, 1.f, 1.f, 1.f);
		auto col = MC_Color(12, 12, 12);
		float rectSpacing = textSize * 2.5f;
		float itemScale = textSize * 0.6f;
		float itemSpacing = itemScale + 15.f;
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - rectSpacing;
		rectPos.y = textPos.y - rectSpacing;
		rectPos.z = textPos.x + textWidth + rectSpacing;
		rectPos.w = textPos.y + textHeight +  rectSpacing;
		itemPos.y = rectPos.y - 15.f * itemScale;
		float radius = (rectPos.w - rectPos.y) / 4.5f;

		auto* player = reinterpret_cast<Player*>(ent);

		if (clickGuiMod->style.GetSelectedEntry().GetValue() == 2)
			col = MC_Color(13, 29, 48);

		if (!noFriendsModule->isEnabled() && FriendList::findPlayer(ent->getNameTag().c_str()))
			textCol = MC_Color(0.1f, 0.9f, 0.1f, 1.f);
		
		drawText(textPos, &text, textCol, textSize);

		std::vector<ItemStack*> itemDrawList;
		if (nametagsMod->displayArmor) {
			int itemCount = 0;
			for (int i = 0; i < 4; i++) {
				ItemStack* stack = player->getArmor(i);
				if (stack->item) {
					itemCount++;
					itemDrawList.push_back(stack);
				}
			}
			ItemStack* stack = player->getSelectedItem();
			if (stack->item) {
				itemCount++;
				itemDrawList.push_back(stack);
			}

			float itemWidth = itemCount * (itemScale * itemSpacing);

			itemPos.x = rectPos.x + ((rectPos.z - itemWidth) - rectPos.x) / 2.f;

			if (itemWidth > textWidth && nametagsMod->encaseItems) {
				rectPos.x = itemPos.x - rectSpacing;
				rectPos.z = itemPos.x + ((itemScale * itemSpacing) * itemCount) + rectSpacing;
			}

			if (nametagsMod->encaseItems)
				rectPos.y = itemCount > 0 ? rectPos.y - 20.f * (textSize * 0.6f) : rectPos.y;
			else
				itemPos.y -= rectSpacing;
		}

		drawRoundRect(rectPos, CornerRadius(radius), MC_Color(col.r, col.g, col.b, nametagsMod->opacity));

		if (nametagsMod->displayArmor) {
			for (auto itemStack : itemDrawList) {
				DrawUtils::drawItem(itemStack, itemPos, 1.f, itemScale, itemStack->isEnchanted());
				itemPos.x += itemScale * itemSpacing;
			}
		}
	}
}

void DrawUtils::drawCrossLine(Vec2 pos, MC_Color col, float textSize, bool secondCross) {
	textSize *= 1.5f;
	std::string plus = "+";
	std::string minus = "_";
	if (!secondCross) {
		pos.x -= DrawUtils::getTextWidth(&minus, textSize) / 2;
		pos.y -= DrawUtils::getFontHeight(textSize);
		DrawUtils::drawText(pos, &minus, col, textSize, col.a, DesiredUi::DEFAULT, false);
	} else {
		pos.x -= DrawUtils::getTextWidth(&plus, textSize) / 2;
		pos.y -= DrawUtils::getFontHeight(textSize) / 2;
		DrawUtils::drawText(pos, &plus, col, textSize, col.a, DesiredUi::DEFAULT, false);
	}
}

void DrawUtils::drawEntityBox(Entity* ent, float lineWidth, bool outline) {
	Vec3* start = ent->getPosOld();
	Vec3* end = ent->getPos();

	Vec3 lerped = start->lerp(end, lerpT);

	AABB render(lerped, ent->aabbShape->width, ent->aabbShape->height, end->y - ent->aabbShape->aabb.lower.y);
	render.upper.y += 0.1f;

	drawBox(render.lower, render.upper, lineWidth, outline);
}

Vec4 DrawUtils::getRectForAABB(const AABB& aabb) {
	Vec3 worldPoints[8];
	worldPoints[0] = Vec3(aabb.lower.x, aabb.lower.y, aabb.lower.z);
	worldPoints[1] = Vec3(aabb.lower.x, aabb.lower.y, aabb.upper.z);
	worldPoints[2] = Vec3(aabb.upper.x, aabb.lower.y, aabb.lower.z);
	worldPoints[3] = Vec3(aabb.upper.x, aabb.lower.y, aabb.upper.z);
	worldPoints[4] = Vec3(aabb.lower.x, aabb.upper.y, aabb.lower.z);
	worldPoints[5] = Vec3(aabb.lower.x, aabb.upper.y, aabb.upper.z);
	worldPoints[6] = Vec3(aabb.upper.x, aabb.upper.y, aabb.lower.z);
	worldPoints[7] = Vec3(aabb.upper.x, aabb.upper.y, aabb.upper.z);

	std::vector<Vec2> points;
	for (int i = 0; i < 8; i++) {
		Vec2 result;
		if (refdef->OWorldToScreen(origin, worldPoints[i], result, fov, screenSize))
			points.emplace_back(result);
	}
	if (points.size() < 2) return {0.0f, 0.0f, 0.0f, 0.0f};

	Vec4 resultRect = {points[0].x, points[0].y, points[0].x, points[0].y};
	for (const auto& point : points) {
		if (point.x < resultRect.x) resultRect.x = point.x;
		if (point.y < resultRect.y) resultRect.y = point.y;
		if (point.x > resultRect.z) resultRect.z = point.x;
		if (point.y > resultRect.w) resultRect.w = point.y;
	}
	return resultRect;
}

void DrawUtils::draw2D(Entity* ent, float lineWidth) {
	Vec4 rect = DrawUtils::getRectForAABB(ent->aabbShape->aabb);

	DrawUtils::drawLine(Vec2(rect.x, rect.y), Vec2(rect.x, rect.w), lineWidth);
	DrawUtils::drawLine(Vec2(rect.x, rect.y), Vec2(rect.z, rect.y), lineWidth);
	DrawUtils::drawLine(Vec2(rect.z, rect.w), Vec2(rect.z, rect.y), lineWidth);
	DrawUtils::drawLine(Vec2(rect.z, rect.w), Vec2(rect.x, rect.w), lineWidth);
}

void DrawUtils::drawItem(ItemStack* item, const Vec2& itemPos, float opacity, float scale, bool isEnchanted, int mode) {
	__int64 scnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	auto* screenCtx = reinterpret_cast<ScreenContext*>(scnCtx);
	BaseActorRenderContext baseActorRenderCtx(screenCtx, Game.getClientInstance(), Game.getMinecraftGame());
	ItemRenderer* renderer = baseActorRenderCtx.itemRenderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, mode, itemPos.x, itemPos.y, opacity, scale, isEnchanted);
}

bool DrawUtils::isMouseOver(const Vec4& pos) {
	auto clientInstance = Game.getClientInstance();
	Vec2 windowSize = Game.getGuiData()->windowSize;
	Vec2 windowSizeReal = Game.getGuiData()->windowSizeReal;
	Vec2 mousePos = *Game.getClientInstance()->getMousePos();
	mousePos = mousePos.div(windowSizeReal);
	mousePos = mousePos.mul(windowSize);
	if (mousePos.x >= pos.x && mousePos.y >= pos.y && mousePos.x < pos.z && mousePos.y < pos.w)
		return true;
	return false;
}

Vec2 DrawUtils::worldToScreen(const Vec3& world) {
	Vec2 ret{-1, -1};
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
void DrawUtils::drawLine3D(const Vec3& start, const Vec3& end, bool onUi) {
	Tessellator3D->begin(VertexFormat::LINE_LIST);

	auto start1 = start.sub(origin);
	auto end1 = end.sub(origin);

	Tessellator3D->vertex(start1.x, start1.y, start1.z);
	Tessellator3D->vertex(end1.x, end1.y, end1.z);

	Tessellator3D->renderMeshImmediately(ScreenContext3D, onUi ? uiMaterial : entityFlatStaticMaterial);
}
void DrawUtils::drawBox3D(const Vec3& lower, const Vec3& upper) {
	Tessellator3D->begin(VertexFormat::LINE_LIST);

	Vec3 diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	auto newLower = lower.sub(origin);

	Vec3 vertices[8];
	vertices[0] = Vec3(newLower.x, newLower.y, newLower.z);
	vertices[1] = Vec3(newLower.x + diff.x, newLower.y, newLower.z);
	vertices[2] = Vec3(newLower.x, newLower.y, newLower.z + diff.z);
	vertices[3] = Vec3(newLower.x + diff.x, newLower.y, newLower.z + diff.z);

	vertices[4] = Vec3(newLower.x, newLower.y + diff.y, newLower.z);
	vertices[5] = Vec3(newLower.x + diff.x, newLower.y + diff.y, newLower.z);
	vertices[6] = Vec3(newLower.x, newLower.y + diff.y, newLower.z + diff.z);
	vertices[7] = Vec3(newLower.x + diff.x, newLower.y + diff.y, newLower.z + diff.z);

#define line(m, n)                      \
	Tessellator3D->vertex(m.x, m.y, m.z); \
	Tessellator3D->vertex(n.x, n.y, n.z);

#define li(m, n) line(vertices[m], vertices[n]);

	li(0, 1);
	li(1, 3);
	li(3, 2);
	li(2, 0);

	li(4, 5);
	li(5, 7);
	li(7, 6);
	li(6, 4);

	li(0, 4);
	li(1, 5);
	li(2, 6);
	li(3, 7);

#undef li
#undef line

	Tessellator3D->renderMeshImmediately(ScreenContext3D, entityFlatStaticMaterial);
}

void DrawUtils::fillRect(const Vec4& pos, const MC_Color& col, float alpha) {
	DrawUtils::SetColor(col.r, col.g, col.b, alpha);
	DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
}

float DrawUtils::getLerpTime() {
	return lerpT;
}

Vec3 DrawUtils::getOrigin() {
	return origin;
}

void DrawUtils::drawMinimap(Minimap* mod) {
#ifdef HORION_DEBUG
	auto& blocks = mod->blocks;
	Vec2 pos = {10, 100};
	bool showChunkBorders = moduleMgr->getModule<Minimap>("Minimap")->chunkBorders;
	SetColor(0.1f, 0.1f, 0.1f, 1);
	//fillRect(pos, pos.add({Minimap::map_size, Minimap::map_size}));
	SetColor(1.f, 1.f, 1.f, 1);
	int i = 0;
	for (int x = 0; x < (float)Minimap::map_size; x++) {
		for (int y = 0; y < (float)Minimap::map_size; y++) {
			auto& bc = blocks[i];
			if (bc.exists) {
				auto& col = bc.color;
				Vec2 p = pos.add((float)x, (float)y);

				if (showChunkBorders) {
					Vec2 chunkOrigin = {
						(bc.worldPos.x >> 4) * 16,
						(bc.worldPos.z >> 4) * 16
					};
					if (bc.worldPos.x == chunkOrigin.x || bc.worldPos.z == chunkOrigin.y) {
						bc.color[0] = 1;
						bc.color[1] = 0;
						bc.color[2] = 0;
						bc.color[3] = 1;
					}
				}

				Tessellator2D->begin(VertexFormat::QUAD);
				Tessellator2D->color(bc.color[0], bc.color[1], bc.color[2], 1);
				Tessellator2D->vertex(p.x, p.y + 1, 0);
				Tessellator2D->vertex(p.x + 1, p.y + 1, 0);
				Tessellator2D->vertex(p.x + 1, p.y, 0);
				Tessellator2D->vertex(p.x, p.y, 0);
			}
			i++;
		}
	}
	Tessellator2D->renderMeshImmediately(ScreenContext2D, uiMaterial);
#endif
}

void DrawUtils::drawLinestrip3D(const std::vector<Vec3>& points) {
	Tessellator3D->begin(VertexFormat::LINE_STRIP);

	for (const auto& p : points) {
		auto pD = p.sub(origin);
		Tessellator3D->vertex(pD.x, pD.y, pD.z);
	}

	Tessellator3D->renderMeshImmediately(ScreenContext3D, entityFlatStaticMaterial);
}

MC_Color MC_Color::lerp(const MC_Color& o, float t) const {
	return MC_Color(Utils::lerp(r, o.r, t), Utils::lerp(g, o.g, t), Utils::lerp(b, o.b, t), Utils::lerp(a, o.a, t));
}