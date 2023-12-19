#include "TabGui.h"
#include "../../resource.h"

struct SelectedItemInformation {
	int selectedItemId = 0;
	float currentSelectedItemInterpol = 0;
	float rollbackVal = 1;

	void setSelectedItemForce(int item) {
		selectedItemId = item;
		currentSelectedItemInterpol = (float)item;
	}

	void interp() {
		currentSelectedItemInterpol += (selectedItemId - currentSelectedItemInterpol) * 0.2f;
	}

	void rollback() {
		rollbackVal *= 0.7f;
	}
	void rollin() {
		rollbackVal = 1 - ((1 - rollbackVal) * 0.7f);
	}
};

// State
int level;
SelectedItemInformation selected[4];
bool toggleCurrentSelection = false;

// Render
static float boxRad;
static float yOffset;
static float xOffset;
int renderedLevel;

struct LabelContainer {
	const char* text = 0;
	bool enabled = false;
	std::shared_ptr<IModule> mod = 0;
};

std::vector<LabelContainer> labelList;

void TabGui::renderLabel(const char* text, std::shared_ptr<IModule> mod) {
	//size_t strlength = strlen(text) + 1;
	//char* alloc = new char[strlength];
	//strcpy_s(alloc, strlength, text);
	
	//stop people from being able to turn off TabGui within the tab gui
	if (mod.get() == moduleMgr->getModule<TabGuiMod>("TabGui")) return;

	LabelContainer yikes;
	yikes.text = text;
	if (mod ) {
		yikes.enabled = mod->isEnabled();
		yikes.mod = mod;
	}

	labelList.push_back(yikes);
}

void TabGui::renderLevel() {
	auto tabGuiMod = moduleMgr->getModule<TabGuiMod>("TabGui");
	// 0: dark
	// 1: classic

	// Parameters
	float textSize = tabGuiMod->scale;
	float textHeight = 10.f * textSize;
	float alphaVal = 1.0f;
	std::string arrowSymbol(">");
	float arrowWidth = DrawUtils::getTextWidth(&arrowSymbol, textSize, DesiredUi::TABGUI);

	// First loop: Get the maximum text length
	float maxLength = 0.f;
	int labelListLength = 0;
	for (auto it = labelList.begin(); it != labelList.end(); ++it) {
		labelListLength++;
		std::string label = it->text;
		maxLength = fmax(maxLength, DrawUtils::getTextWidth(&label, textSize, DesiredUi::TABGUI));
	}

	if (renderedLevel == 0) { // get rounded box radius from first level
		boxRad = float(labelListLength) * textHeight * 0.06f;
	}

	if (selected[renderedLevel].selectedItemId < 0)
		selected[renderedLevel].selectedItemId += labelListLength;
	if (selected[renderedLevel].selectedItemId >= labelListLength)
		selected[renderedLevel].selectedItemId -= labelListLength;

	selected[renderedLevel].interp();  // Converge to selected item
	if (renderedLevel < level)
		selected[renderedLevel].rollbackVal = 1;  // Speed up animation when we are in the next menu already

	// Second loop: Render everything
	int i = 0;
	float wid = maxLength + 5.0f + arrowWidth * 5.0f;
	Vec4 rectPos = Vec4(
		xOffset - 0.5f,                                           // Off screen / Left border not visible
		yOffset - textHeight * 0.5f, 
		xOffset + maxLength + 4.5f + arrowWidth *5.0f,
		yOffset + textHeight * (float(labelListLength) + 0.5f + float(renderedLevel == 0) * 1.5f)); // Space for logo
		if (tabGuiMod->style.selected == 0) {
			DrawUtils::drawRoundRect(rectPos, CornerRadius(boxRad * float(level < 0 or renderedLevel != 1), boxRad, boxRad, boxRad* float(level < 0 or renderedLevel )), MC_Color(12, 12, 12, (int)(tabGuiMod->opacity * 255)));
		} else if (tabGuiMod->style.selected == 1) {
			DrawUtils::drawRoundRect(rectPos, CornerRadius(boxRad * float(level < 0 or renderedLevel != 1), boxRad, boxRad, boxRad* float(level < 0 or renderedLevel )), MC_Color(20, 35, 55, (int)(tabGuiMod->opacity * 255)));
	} 
	if (renderedLevel == 0) { // Logo
		//const float scaleMul = 0.6f; // Scale compared to menu width
		float imageWidth = 385.f;
		float imageHeight = 166.f * (wid / imageWidth);
		imageWidth = wid;
		float scaleMul = ((textHeight * 1.8f) / imageHeight);
		// Centered
		auto texture = DrawUtils::resourceToTexturePtr(HORION_BANNER_DATA, "HorionBanner"); // needs to be a variable
		DrawUtils::drawImageFromTexturePtr(texture, Vec2(rectPos.x + imageWidth * (1.0f - scaleMul) * 0.5f, yOffset - textHeight * 0.3f), Vec2(imageWidth * scaleMul, imageHeight * scaleMul), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
		DrawUtils::flushImages();
		yOffset+=textHeight * 1.5f;
	}
	float selectedYOffset = yOffset;
	float startYOffset = yOffset;
	for (auto it = labelList.begin(); it != labelList.end(); ++it, i++) {
		auto label = *it;

		MC_Color color = MC_Color(180, 180, 180);
		const float textAnimSpeed = 0.2f;
		float moduleTextAnim = abs(selected[renderedLevel].currentSelectedItemInterpol - float(i));
		if (moduleTextAnim > 1.0f)
			moduleTextAnim = 1.0f;
		moduleTextAnim = 1.0f - moduleTextAnim;
		moduleTextAnim *= selected[renderedLevel].rollbackVal;

		if (selected[renderedLevel].selectedItemId == i && renderedLevel == level) {  // Are we actually in the menu we are drawing right now?
			static bool lastVal = toggleCurrentSelection;

			if (toggleCurrentSelection) {
				if (label.mod->isFlashMode()) {
					label.mod->setEnabled(true);
				} else {
					toggleCurrentSelection = false;
					label.mod->toggle();
				}
			} else if (toggleCurrentSelection != lastVal && label.mod->isFlashMode())
				label.mod->setEnabled(false);
			lastVal = toggleCurrentSelection;
		} else {
		}
		//selectedYOffset = yOffset;

		std::string tempLabel(label.text);
		DrawUtils::drawText(Vec2(xOffset + arrowWidth + arrowWidth * 3.0f * moduleTextAnim, yOffset + 0.5f), &tempLabel, label.enabled ? MC_Color(1.f, 1.f, 1.f) : color, textSize, 1.f, DesiredUi::TABGUI);
		if (moduleTextAnim > 0.02f) {
		DrawUtils::drawText(Vec2(xOffset - arrowWidth * 0.5f + arrowWidth * 2.5f * moduleTextAnim, yOffset + 0.5f), &arrowSymbol, label.enabled ? MC_Color(1.f, 1.f, 1.f) : color, textSize, moduleTextAnim, DesiredUi::TABGUI);
		}
		yOffset += textHeight;
	}

	// Draw selected item
	{
		selectedYOffset = startYOffset + textHeight * selected[renderedLevel].currentSelectedItemInterpol;
		Vec4 selectedPos = Vec4(
			xOffset - 0.5f,  // Off screen / Left border not visible
			selectedYOffset,
			xOffset + maxLength + 4.5f + arrowWidth * 5.0f,
			selectedYOffset + textHeight);

		float diff = selectedPos.z - selectedPos.x;
		selectedPos.z = selectedPos.x + diff * selected[renderedLevel].rollbackVal;

		if (renderedLevel > level) {
			selected[renderedLevel].rollback();
		} else
			selected[renderedLevel].rollin();
		if (tabGuiMod->isSlider)
			if (tabGuiMod->style.selected == 0) {
				DrawUtils::fillRect(selectedPos, MC_Color(85, 85, 85), alphaVal);
			} else if (tabGuiMod->style.selected == 1) {
				DrawUtils::fillRect(selectedPos, MC_Color(28, 107, 201), alphaVal);
			}
	}

	// Cleanup
	DrawUtils::Flush();
	labelList.clear();
	xOffset += maxLength + 4.5f + arrowWidth * 5.0f;
	yOffset = selectedYOffset;
	renderedLevel++;
}

void TabGui::render() {

	if (!moduleMgr->isInitialized())
		return;
	if (!GameData::canUseMoveKeys())
		level = -1;
	renderedLevel = 0;
	xOffset = 2.f;
	yOffset = 14.f;

	// Render all categorys
	renderLabel("Combat");
	renderLabel("Visual");
	renderLabel("Movement");
	renderLabel("Player");
	renderLabel("World");
	renderLabel("Misc");
	renderLabel("Gui");
	renderLevel();

	// Render all modules
	if (level >= 0) {
		auto lock = moduleMgr->lockModuleList();

		std::vector<std::shared_ptr<IModule>>* modules = moduleMgr->getModuleList();
		for (std::vector<std::shared_ptr<IModule>>::iterator it = modules->begin(); it != modules->end(); ++it) {
			auto mod = *it;
			if (selected[0].selectedItemId == static_cast<int>(mod->getCategory())) {
				auto name = mod->getModuleName();
				renderLabel(name, mod);
			}
		}

		renderLevel();
	}
}

void TabGui::init() {
	level = 0;
	xOffset = 0;
	yOffset = 0;
	renderedLevel = 0;
}

void TabGui::onKeyUpdate(int key, bool isDown) {
	if (!GameData::canUseMoveKeys()) {
		level = -1;
		return;
	}

	if (!isDown) {
		if (key == VK_RIGHT)
			toggleCurrentSelection = false;
		return;
	}

	switch (key) {
	case VK_LEFT:  // Leave menus
		if (level > -1) {
			level--;
		}
		if (level == -1)
			selected[1].rollbackVal = 0;
		return;
	case VK_RIGHT:
		if (level < 1) {
			level++;
			selected[level].setSelectedItemForce(0);
			selected[level].rollbackVal = 0;
		} else
			toggleCurrentSelection = true;
		return;
	case VK_UP:
		if (level >= 0)
			selected[level].selectedItemId--;
		else
			level = 0;
		break;
	case VK_DOWN:
		if (level >= 0)
			selected[level].selectedItemId++;
		else
			level = 0;
		break;
	};

	if (level < 3)
		selected[level + 1].setSelectedItemForce(0);
}