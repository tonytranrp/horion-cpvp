#include "ClickGui.h"
#include "../../Utils/Json.hpp"
#include "../../Utils/Logger.h"
#include "../../resource.h"

bool isLeftClickDown = false;
bool isRightClickDown = false;
bool shouldToggleLeftClick = false;  // If true, toggle the focused module
bool shouldToggleRightClick = false;
bool resetStartPos = true;
bool initialised = false;
int scrollingDirection = 0;

struct SavedWindowSettings {
	Vec2 pos = {-1, -1};
	bool isExtended = true;
	const char* name = "";
};

std::map<unsigned int, std::shared_ptr<ClickWindow>> windowMap;
std::map<unsigned int, SavedWindowSettings> savedWindowSettings;

bool isDragging = false;
unsigned int draggedWindow = -1;
Vec2 dragStart = Vec2();

unsigned int focusedElement = -1;
bool isFocused = false;

#define clickScale (0.7f * (Game.getGuiData()->widthGame / 550.f))

#define textPaddingX (1.0f * clickScale)
#define textPaddingY (1.0f * clickScale)
#define initialTextSize (1.0f * clickScale)
#define initialTextHeight (initialTextSize * 10.0f)
#define categoryMargin (0.5f * clickScale)
#define paddingRight (13.5f * clickScale)
#define crossSize (textHeight / 2.f)
#define crossWidth (0.3f * clickScale);

static constexpr float backgroundAlpha = 0.70f;

static MC_Color whiteColor = MC_Color(1.f, 1.f, 1.f, 1.f);
static MC_Color moduleColor = MC_Color(0.f, 0.f, 0.f);                   // background
static MC_Color selectedModuleColor = moduleColor.lerp(whiteColor, 0.08f);  // 30, 110, 200
static MC_Color enabledModuleColor = moduleColor.lerp(whiteColor, 0.04f);
static MC_Color brightModuleBlendColor = MC_Color(0, 196, 252);  // tooltip border & category divider
static MC_Color crossColor = MC_Color(255, 255, 255);
static MC_Color selectedSettingColor = MC_Color(85, 85, 85);
static MC_Color SettingColor = MC_Color(12, 12, 12);
static MC_Color sliderColor = MC_Color(85, 85, 85);

float currentYOffset = 0;
float currentXOffset = 0;

float textSize = 1.f;
float textHeight = textSize * 10.f;

int timesRendered = 0;

unsigned char mode = 0;
auto lastMode = mode;

bool isVec2f;
bool isVec3f;

void ClickGui::getModuleListByCategory(Category category, std::vector<std::shared_ptr<IModule>>* modList) {
	auto lock = moduleMgr->lockModuleList();
	std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();

	for (auto& it : *moduleList) {
		if (it->getCategory() == category)
			modList->push_back(it);
	}
}

std::shared_ptr<ClickWindow> ClickGui::getWindow(const char* name) {
	unsigned int id = Utils::getCrcHash(name);

	auto search = windowMap.find(id);
	if (search != windowMap.end()) {  // Window exists already
		return search->second;
	} else {  // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickWindow> newWindow = std::make_shared<ClickWindow>();
		newWindow->name = name;

		auto savedSearch = savedWindowSettings.find(id);
		if (savedSearch != savedWindowSettings.end()) {  // Use values from config
			newWindow->isExtended = savedSearch->second.isExtended;
			if (savedSearch->second.pos.x > 0)
				newWindow->pos = savedSearch->second.pos;
		}

		windowMap.insert(std::make_pair(id, newWindow));
		return newWindow;
	}
}

std::shared_ptr<ClickModule> ClickGui::getClickModule(std::shared_ptr<ClickWindow> window, const char* name) {
	unsigned int id = Utils::getCrcHash(name);

	auto search = window->moduleMap.find(id);
	if (search != window->moduleMap.end()) {  // Window exists already
		return search->second;
	} else {  // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickModule> newModule = std::make_shared<ClickModule>();

		window->moduleMap.insert(std::make_pair(id, newModule));
		return newModule;
	}
}

void ClickGui::renderLabel(const char* text) {
}

void ClickGui::renderTooltip(std::string* text) {
	Vec2 windowSize = Game.getGuiData()->windowSize;
	Vec2 currentTooltipPos = Vec2(5.f, windowSize.y - 15.f);
	float textWidth = DrawUtils::getTextWidth(text, textSize, DesiredUi::CLICKGUI);
	Vec2 textPos = Vec2(
		currentTooltipPos.x + textPaddingX,
		currentTooltipPos.y);
	Vec4 rectPos = Vec4(
		currentTooltipPos.x - 2.f,
		currentTooltipPos.y - 2.f,
		currentTooltipPos.x + (textPaddingX * 2) + textWidth + 2.5f,
		currentTooltipPos.y + textHeight + 2.f);
	DrawUtils::drawRoundRect(rectPos, CornerRadius(2), {moduleColor.r, moduleColor.g, moduleColor.b, backgroundAlpha});
	DrawUtils::drawRoundRect({rectPos.x, rectPos.w - 0.5f, rectPos.z, rectPos.w}, CornerRadius(0, 0, 1, 1), brightModuleBlendColor, 0.5f);
	DrawUtils::drawText(textPos, text, whiteColor, textSize, 1.f, DesiredUi::CLICKGUI);
}

void ClickGui::renderCategory(Category category) {
	static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	const char* categoryName = ClickGui::catToName(category);

	const std::shared_ptr<ClickWindow> ourWindow = getWindow(categoryName);

	{
		auto fontRepo = Game.getMinecraftGame()->getFontRepo();
		if (DrawUtils::getFont(DesiredUi::CLICKGUI) != fontRepo->getFont(FontType::OldSmoothFont)) {
			textSize = 0.6f * (Game.getGuiData()->widthGame / 550.f);
			textHeight = textSize * 11.f;
		} else {
			textSize = 0.7f * (Game.getGuiData()->widthGame / 550.f);
			textHeight = textSize * 10.f;
		}
	}

	// Reset Windows to pre-set positions to avoid confusion
	if (resetStartPos && ourWindow->pos.x <= 0) {
		float yot = Game.getGuiData()->windowSize.x;
		ourWindow->pos.y = 4;
		switch (category) {
		case Category::COMBAT:
			ourWindow->pos.x = 10.f;
			break;
		case Category::VISUAL:
			ourWindow->pos.x = yot / 7.f;
			break;
		case Category::MOVEMENT:
			ourWindow->pos.x = yot / 7.f * 2.f;
			break;
		case Category::PLAYER:
			ourWindow->pos.x = yot / 7.f * 3.f;
			break;
		case Category::WORLD:
			ourWindow->pos.x = yot / 7.f * 4.f;
			break;
		case Category::MISC:
			ourWindow->pos.x = yot / 7.f * 5.f;
			break;
		case Category::GUI:
			ourWindow->pos.x = yot / 7.f * 6.f;
			break;
		}
	}

	const float xOffset = ourWindow->pos.x;
	const float yOffset = ourWindow->pos.y;
	Vec2* windowSize = &ourWindow->size;

	bool overflowing = false;

	currentXOffset = xOffset;
	currentYOffset = yOffset;

	// Get All Modules in our category
	std::vector<std::shared_ptr<IModule>> moduleList;
	getModuleListByCategory(category, &moduleList);

	// Get max width of all text
	{
		for (auto& it : moduleList) {
			std::string label = it->getModuleName();
			windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&label, textSize, DesiredUi::CLICKGUI) + 30);
		}
	}

	const float xEnd = currentXOffset + windowSize->x;

	Vec2 mousePos = *Game.getClientInstance()->getMousePos();

	// Convert mousePos to visual Pos
	{
		Vec2 windowSize = Game.getGuiData()->windowSize;
		Vec2 windowSizeReal = Game.getGuiData()->windowSizeReal;

		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	float categoryHeaderYOffset = currentYOffset;

	if (ourWindow->isInAnimation) {
		if (ourWindow->isExtended) {
			ourWindow->animation *= 0.85f;
			if (ourWindow->animation < 0.001f) {
				ourWindow->yOffset = 0;  // reset scroll
				ourWindow->isInAnimation = false;
			}
		} else {
			ourWindow->animation = 1 - ((1 - ourWindow->animation) * 0.85f);
			if (1 - ourWindow->animation < 0.001f)
				ourWindow->isInAnimation = false;
		}
	}

	currentYOffset += (textHeight * 1.5f) + (textPaddingY * 2.f);
	// Loop through Modules to display em
	if (ourWindow->isExtended || ourWindow->isInAnimation) {
		if (ourWindow->isInAnimation) {
			currentYOffset -= ourWindow->animation * moduleList.size() * (textHeight + (textPaddingY * 2));
		}

		const float cutoffHeight = roundf(Game.getGuiData()->heightGame * 0.88f) + 0.5f /*fix flickering related to rounding errors*/;
		int moduleIndex = 0;
		for (auto& mod : moduleList) {
			moduleIndex++;
			if (moduleIndex < ourWindow->yOffset)
				continue;
			float probableYOffset = (moduleIndex - ourWindow->yOffset) * (textHeight + (textPaddingY * 2));

			if (ourWindow->isInAnimation) {  // Estimate, we don't know about module settings yet
				if (probableYOffset > cutoffHeight) {
					overflowing = true;
					break;
				}
			} else if ((currentYOffset - ourWindow->pos.y) > cutoffHeight || currentYOffset > Game.getGuiData()->heightGame - 5) {
				overflowing = true;
				break;
			}

			std::string textStr = mod->getModuleName();

			Vec2 textPos = Vec2(
				(currentXOffset + (windowSize->x / 2)) - (DrawUtils::getTextWidth(&textStr, textSize, DesiredUi::CLICKGUI) / 2),
				currentYOffset + textPaddingY);
			Vec4 rectPos = Vec4(
				currentXOffset,
				currentYOffset,
				xEnd,
				currentYOffset + textHeight + (textPaddingY * 2));

			bool allowRender = currentYOffset >= categoryHeaderYOffset;

			Vec2 renderVisible = Vec2(
				currentXOffset + windowSize->x - 13.5f,
				currentYOffset + textPaddingY + 1.f);
			Vec4 epicRect = Vec4(renderVisible.x, renderVisible.y, renderVisible.x + 8, renderVisible.y + 8);

			bool canToggle = !epicRect.contains(&mousePos);

			if (epicRect.contains(&mousePos)) {
				if (shouldToggleLeftClick) {
					mod->setVisible(!mod->isVisibleInArrayList());
				}
			}

			// Background
			if (allowRender) {
				if (!ourWindow->isInAnimation && !isDragging && rectPos.contains(&mousePos)) {  // Is the Mouse hovering above us?
					DrawUtils::fillRect(rectPos, selectedModuleColor, backgroundAlpha);
					std::string tooltip = mod->getTooltip();
					if (clickGuiMod->showTooltips && !tooltip.empty())
						renderTooltip(&tooltip);
					if (shouldToggleLeftClick && !ourWindow->isInAnimation) {  // Are we being clicked?
						if (canToggle)
							mod->toggle();
						shouldToggleLeftClick = false;
					}
				} else {
					DrawUtils::fillRect(rectPos, mod->isEnabled() ? enabledModuleColor : moduleColor, backgroundAlpha);
				}
				
			}

			// Text
			if (allowRender)
				DrawUtils::drawText(textPos, &textStr, mod->isEnabled() ? brightModuleBlendColor : MC_Color(150, 150, 150), textSize, 1.f, DesiredUi::CLICKGUI);
				
			// Visible
			if (allowRender) {
				if (rectPos.contains(&mousePos)) {
					
					bool visible = mod->isVisibleInArrayList();
					if (visible && mod->getCategory() != Category::GUI) {
						auto textr = DrawUtils::resourceToTexturePtr(VISIBILITY_ON_DATA, "VisibilityOn");
						DrawUtils::drawImageFromTexturePtr(textr, renderVisible, Vec2(8.f * clickScale, 8.f * clickScale), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
						DrawUtils::flushImages();
					} else if (mod->getCategory() != Category::GUI) {
						auto textr = DrawUtils::resourceToTexturePtr(VISIBILITY_OFF_DATA, "VisibilityOff");
						DrawUtils::drawImageFromTexturePtr(textr, renderVisible, Vec2(8.f * clickScale, 8.f * clickScale), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
						DrawUtils::flushImages();
					}
					DrawUtils::Flush();
				}
			}

			// Settings
			{
				std::vector<SettingEntry*>* settings = mod->getSettings();
				auto size = mod->hasPositionSetting() ? 4 : 3;
				if (settings->size() > size && allowRender) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					if (rectPos.contains(&mousePos) && shouldToggleRightClick && !ourWindow->isInAnimation) {
						shouldToggleRightClick = false;
						clickMod->isExtended = !clickMod->isExtended;
						// update window size so it changes properly when closing settings
						float newX = 0;
						for (auto& it : moduleList) {
							std::string label = it->getModuleName();
							newX = fmax(newX, DrawUtils::getTextWidth(&label, textSize, DesiredUi::CLICKGUI) + 30);
						}
						windowSize->x = newX;
					}
					const float settingAnimSpeed = 0.2f;
					clickMod->mAnim += (float(clickMod->isExtended) - clickMod->mAnim) * settingAnimSpeed;
					float drawAlpha = clickMod->mAnim * clickMod->mAnim;
					if (drawAlpha <= 0.1f) {
						drawAlpha = 0.0f;
					}
					//MC_Color settWhiteColor = whiteColor;
					//settWhiteColor.a *= clickMod->mAnim;
					//MC_Color settModuleColor = moduleColor;
					//settModuleColor.a *= clickMod->mAnim;
					//MC_Color settSelModuleColor = selectedModuleColor;
					//settSelModuleColor.a *= clickMod->mAnim;
					MC_Color settBrModuleColor = crossColor;
					settBrModuleColor.a *= drawAlpha;
					DrawUtils::drawCrossLine(Vec2(
												currentXOffset + windowSize->x - (crossSize / 2) - 1.5f,
												 currentYOffset + textPaddingY + (textHeight / 2)),
											 crossColor, textSize, !clickMod->isExtended);

					currentYOffset += textHeight + (textPaddingY * 2);
					if (clickMod->mAnim > 0.01f) {
						float startYOffset = currentYOffset;
						for (auto setting : *settings) {
							if (strcmp(setting->name, "enabled") == 0 || strcmp(setting->name, "keybind") == 0 || strcmp(setting->name, "visible") == 0)
								continue;

							if (setting->dependsOnOtherSetting) {
								bool hasDependedValue = false;
								for (auto value : setting->dependentSetttingValue) {
									if (value == *setting->dependentSetttingValuePtr)
										hasDependedValue = true;
								}
								if (!hasDependedValue) continue;
							}

							Vec2 textPos = Vec2(
								currentXOffset + textPaddingX + 5,
								currentYOffset + textPaddingY);

							// Incomplete, because we dont know the endY yet
							Vec4 rectPos = Vec4(
								currentXOffset,
								currentYOffset,
								xEnd,
								0);

							if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
								overflowing = true;
								break;
							}

							switch (setting->valueType) {
							case ValueType::BOOL_T: {
								rectPos.w = currentYOffset + textHeight + (textPaddingY * 2);
								// Background of bool setting
								DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
								Vec4 selectableSurface = Vec4(
									textPos.x + textPaddingX,
									textPos.y + textPaddingY,
									xEnd - textPaddingX,
									textPos.y + textHeight - textPaddingY);

								bool isFocused = selectableSurface.contains(&mousePos);
								// Logic
								{
									if (isFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->value->_bool = !setting->value->_bool;
									}
								}
								// Checkbox
								{
									float boxHeight = textHeight - textPaddingY * 2;
									Vec4 boxPos = Vec4(
										textPos.x + textPaddingX,
										textPos.y + textPaddingY,
										textPos.x + textPaddingX + boxHeight,
										textPos.y + textPaddingY + boxHeight);

									if (drawAlpha > 0.5f) {
										auto checkbox = DrawUtils::resourceToTexturePtr(CHECKBOX_DATA, "CheckBox");
										auto box = DrawUtils::resourceToTexturePtr(BOX_DATA, "Box");
										if (setting->value->_bool)
											DrawUtils::drawImageFromTexturePtr(checkbox, Vec2(boxPos.x - 0.8f, boxPos.y - 0.8f), Vec2(boxPos.z - boxPos.x + 0.8f, boxPos.w - boxPos.y + 0.8f), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
										else
											DrawUtils::drawImageFromTexturePtr(box, Vec2(boxPos.x - 0.8f, boxPos.y - 0.8f), Vec2(boxPos.z - boxPos.x + 0.8f, boxPos.w - boxPos.y + 0.8f), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
										DrawUtils::flushImages();
									}
								}
								textPos.x += textHeight + (textPaddingX * 2);
								// Text
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x21];
									sprintf_s(name, 0x21, "%s", setting->name);
									if (name[0] )
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 10 /* because we add 10 to text padding + checkbox*/);
									DrawUtils::drawText(textPos, &elTexto, isFocused ? whiteColor : MC_Color(0.8f, 0.8f, 0.8f), textSize, drawAlpha, DesiredUi::CLICKGUI);
									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}
								break;
							}
							case ValueType::ENUM_T: {
								// Text and background
								{
									//setting name
									char name[0x22];
									sprintf_s(name, "%s:", setting->name);
									// Convert first letter to uppercase for more friendlieness
									if (name[0] )
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									
									//enum selection
									char selection[0x22];
									auto enumData = reinterpret_cast<SettingEnum*>(setting->extraData);
									sprintf_s(selection, enumData->GetSelectedEntry().GetName().c_str());
									// Convert first letter to uppercase for more friendlieness
									if (selection[0] )
										selection[0] = toupper(selection[0]);

									std::string elTextoElSelectiono = selection;
									rectPos.w = currentYOffset + textHeight + (textPaddingY * 2);
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + DrawUtils ::getTextWidth(&elTextoElSelectiono, textSize, DesiredUi::CLICKGUI) + 7 /* because we add 5 to text padding*/ + (crossSize * 3.f));


									// Background of enum setting

									if (rectPos.contains(&mousePos)) {
										DrawUtils::fillRect(rectPos, selectedModuleColor, backgroundAlpha * drawAlpha);
										if (shouldToggleRightClick && !ourWindow->isInAnimation) {
											shouldToggleRightClick = false;
											setting->minValue->_bool = !setting->minValue->_bool;
										}
										if (shouldToggleLeftClick && !ourWindow->isInAnimation) {
											shouldToggleLeftClick = false;
											setting->minValue->_bool = !setting->minValue->_bool;
										}
									} else
										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);

									DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
									DrawUtils::drawText({textPos.x + 1.2f + DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 2, textPos.y}, &elTextoElSelectiono, {0.5f, 0.5f, 0.5f, 1.f}, textSize, drawAlpha, DesiredUi::CLICKGUI);
									DrawUtils::drawCrossLine(Vec2(
																currentXOffset + windowSize->x +  - (crossSize / 2) - 1.5f,
																 currentYOffset + textPaddingY + (textHeight / 2)),
															 settBrModuleColor, textSize, !setting->minValue->_bool);

									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}
								if (setting->minValue->_bool) {
									int e = 0;
									auto enumData = reinterpret_cast<SettingEnum*>(setting->extraData);
									for (auto it = enumData->Entrys.begin();
										 it != enumData->Entrys.end(); it++, e++) {
										if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
											overflowing = true;
											break;
										}
										bool isEven = e % 2 == 0;
										rectPos.y = currentYOffset;
										rectPos.w = currentYOffset + textHeight + (textPaddingY * 2);
										EnumEntry i = *it._Ptr;
										char name[0x21];
										sprintf_s(name, 0x21, "   %s", i.GetName().c_str());
										// Convert first letter to uppercase for more friendlieness
										if (name[0] )
											name[0] = toupper(name[0]);
										std::string elTexto = name;
										windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(
																				&elTexto, textSize, DesiredUi::CLICKGUI) +
																				5);  //because we add 5 to text padding
										textPos.y = currentYOffset + textPaddingY;
										Vec4 selectableSurface = Vec4(
											textPos.x,
											rectPos.y,
											xEnd,
											rectPos.w);
										MC_Color col;
										if (setting->value->_int == e || (selectableSurface.contains(&mousePos) && !ourWindow->isInAnimation))
											col = selectedSettingColor;
										else
											col = SettingColor;
										// Background of individual enum
										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
										DrawUtils::fillRect(selectableSurface, col, col.a == 0 ? 0 : (backgroundAlpha / 2) * drawAlpha);
										DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
										// logic
										if (selectableSurface.contains(&mousePos) &&
											shouldToggleLeftClick && !ourWindow->isInAnimation) {
											shouldToggleLeftClick = false;
											setting->value->_int = e;
										}
										currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
									}
								}
								break;
							}
							case ValueType::FLOAT_T: {
								// Text and background
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x22];
									sprintf_s(name, "%s:", setting->name);
									if (name[0] )
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
									DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
									currentYOffset += (textPaddingY + textHeight) * clickMod->mAnim;
									rectPos.w = currentYOffset;
									DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
								}

								if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
									overflowing = true;
									break;
								}
								// Slider
								{
									Vec4 rect = Vec4(
										currentXOffset + textPaddingX + 5,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPaddingY * 2);
										// Background
										const bool areWeFocused = rect.contains(&mousePos);

										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);          // Background
										
										const float minValue = setting->minValue->_float;
										const float maxValue = setting->maxValue->_float - minValue;
										float value = (float)fmax(0, setting->value->_float - minValue);  // Value is now always > 0 && < maxValue
										if (value > maxValue)
											value = maxValue;
										value /= maxValue;  // Value is now in range 0 - 1
										const float endXlol = (xEnd - textPaddingX * 8.f) - (currentXOffset + textPaddingX + 5);
										value *= endXlol;  // Value is now pixel diff between start of bar and end of progress

										// Draw Int
										{
											Vec2 mid = Vec2(
												rect.x + ((rect.z - rect.x) / 2),
												rect.y - 0.2f);
											char str[10];
											sprintf_s(str, 10, "%.2f", setting->value->_float);
											std::string text = str;
											mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

											DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
										}

										// Draw Progress
										{
											Vec4 rect2 = rect;
											rect2.z = rect2.x + value;
											DrawUtils::fillRect(rect2, sliderColor, ((areWeFocused || setting->isDragging) ? 1.f : 0.8f) * drawAlpha);
										}

										DrawUtils::drawRect(rect, whiteColor, drawAlpha, 1.0f);  // Slider background

										// Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown)
													value = mousePos.x - rect.x;
												else
													setting->isDragging = false;
											} else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}

										// Save Value
										{
											value /= endXlol;  // Now in range 0 - 1
											value *= maxValue;
											value += minValue;

											setting->value->_float = value;
											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}
							} break;
							case ValueType::INT_T: {
								// Text and background
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x22];
									sprintf_s(name, "%s:", setting->name);
									if (name[0] )
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
									DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
									currentYOffset += (textPaddingY + textHeight) * clickMod->mAnim;
									rectPos.w = currentYOffset;
									DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
								}
								if ((currentYOffset - ourWindow->pos.y) > (Game.getGuiData()->heightGame * 0.75)) {
									overflowing = true;
									break;
								}
								// Slider
								{
									Vec4 rect = Vec4(
										currentXOffset + textPaddingX + 5,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPaddingY * 2);
										// Background
										const bool areWeFocused = rect.contains(&mousePos);

										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);          // Background

										const float minValue = (float)setting->minValue->_int;
										const float maxValue = (float)setting->maxValue->_int - minValue;
										float value = (float)fmax(0, setting->value->_int - minValue);  // Value is now always > 0 && < maxValue
										if (value > maxValue)
											value = maxValue;

										value /= maxValue;  // Value is now in range 0 - 1
										const float endXlol = (xEnd - textPaddingX * 8.f) - (currentXOffset + textPaddingX + 5);
										value *= endXlol;  // Value is now pixel diff between start of bar and end of progress

										// Draw Int
										{
											Vec2 mid = Vec2(
												rect.x + ((rect.z - rect.x) / 2),
												rect.y - 0.2f  // Hardcoded ghetto
											);
											char str[10];
											sprintf_s(str, 10, "%i", setting->value->_int);
											std::string text = str;
											mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

											DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
										}

										// Draw Progress
										{
											Vec4 rect2 = rect;
											rect2.z = rect2.x + value;
											DrawUtils::fillRect(rect2, sliderColor, ((areWeFocused || setting->isDragging) ? 1.f : 0.8f) * drawAlpha);
										}

										DrawUtils::drawRect(rect, whiteColor,  drawAlpha, 1.0f);  // Slider background

										// Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown)
													value = mousePos.x - rect.x;
												else
													setting->isDragging = false;
											} else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}

										// Save Value
										{
											value /= endXlol;  // Now in range 0 - 1
											value *= maxValue;
											value += minValue;

											setting->value->_int = (int)roundf(value);
											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}
							} break;
							case ValueType::COLOR_T: {
								// Text and background
								{
									// setting name
									char name[0x22];
									sprintf_s(name, "%s:", setting->name);
									// Convert first letter to uppercase for more friendlieness
									if (name[0])
										name[0] = toupper(name[0]);
									std::string elTexto = name;
									rectPos.w = currentYOffset + textHeight + (textPaddingY * 2);
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/ + crossSize);

									if (rectPos.contains(&mousePos)) {
										DrawUtils::fillRect(rectPos, selectedModuleColor, backgroundAlpha * drawAlpha);
										if (shouldToggleRightClick && !ourWindow->isInAnimation) {
											shouldToggleRightClick = false;
											setting->extended = !setting->extended;
										}
										if (shouldToggleLeftClick && !ourWindow->isInAnimation) {
											shouldToggleLeftClick = false;
											setting->extended = !setting->extended;
										}
									} else
										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);

									DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
									DrawUtils::drawCrossLine(
										Vec2(
											currentXOffset + windowSize->x - (crossSize / 2) - 1.5f,
											currentYOffset + textPaddingY + (textHeight / 2)),
										settBrModuleColor, textSize, !setting->extended);

									//color preview
									DrawUtils::drawRoundRect(
										{ 
											textPos.x + DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + (textPaddingX * 3),
											currentYOffset + (textPaddingY * 2),
											currentXOffset + windowSize->x - 8.f,	//cross line padding padding
											rectPos.w - (textPaddingY * 2)
										}, 
										CornerRadius(1), 
										{
											setting->value->Color.r, 
											setting->value->Color.g, 
											setting->value->Color.b,
											drawAlpha
										}
									);

									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}

								if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
									overflowing = true;
									break;
								}

								// Color Area and Picker
								if (setting->extended) {
									Vec4 rect = Vec4(
										currentXOffset + textPaddingX + 5,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX,
										currentYOffset - textPaddingY + textHeight + 50);

									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPaddingY * 2) + 49;

										Vec4 colorBounds;
										colorBounds.x = rect.x;
										colorBounds.y = rect.y + 10.f;
										colorBounds.z = rect.z - 4.f;
										colorBounds.w = rect.y + 55.f;

										Vec4 hueBounds;
										hueBounds.x = rect.x;
										hueBounds.y = rect.y;
										hueBounds.z = rect.z - 4.f;
										hueBounds.w = rect.y + 7.5f;

										float colorWidth = colorBounds.z - colorBounds.x;
										float colorHeight = colorBounds.w - colorBounds.y;

										float hueWidth = hueBounds.z - hueBounds.x;

										const bool pickerFocused = colorBounds.contains(&mousePos.add(1.f));
										const bool sliderFocused = hueBounds.contains(&mousePos);

										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);          // Background

										// picker position
										float x = setting->colorPicker.x;
										float y = setting->colorPicker.y;
										float z = setting->colorPicker.z;
										
										// Draw Color Area, Picker and slider
										{
											DrawUtils::SetColor(1.f, 1.f, 1.f, 1.f);

											std::array<MC_Color, 7> arr = {
												MC_Color{1.f, 0.f, 0.f, drawAlpha},
												MC_Color{1.f, 1.f, 0.f, drawAlpha},
												MC_Color{0.f, 1.f, 0.f, drawAlpha},
												MC_Color{0.f, 1.f, 1.f, drawAlpha},
												MC_Color{0.f, 0.f, 1.f, drawAlpha},
												MC_Color{1.f, 0.f, 1.f, drawAlpha},
												MC_Color{1.f, 0.f, 0.f, drawAlpha}};

											{
												Vec2 start(hueBounds.x, hueBounds.y);
												for (size_t i = 0; i < arr.size() - 1; ++i) {
													DrawUtils::TessellatorBegin(VertexFormat::QUAD);

													Vec2 end = start.add(colorWidth / 6.f, 7.5);

													DrawUtils::TessellatorColor(arr[i + 1]);
													DrawUtils::TessellatorVertex(end.x, end.y, 0.f);
													DrawUtils::TessellatorVertex(end.x, start.y, 0.f);

													DrawUtils::TessellatorColor(arr[i]);
													DrawUtils::TessellatorVertex(start.x, start.y, 0.f);
													DrawUtils::TessellatorVertex(start.x, end.y, 0.f);

													start = start.add(colorWidth / 6.f, 0.f);
												}
											}

											MC_Color hueColor;
											{
												Utils::ColorConvertHSVtoRGB(z / hueWidth, 1.f, 1.f, hueColor.r, hueColor.g, hueColor.b);
												hueColor.a = drawAlpha;
											}

											Vec2 start(colorBounds.x, colorBounds.y);
											Vec2 end(colorBounds.z, colorBounds.w);

											{
												// Left
												DrawUtils::TessellatorBegin(VertexFormat::QUAD);
												DrawUtils::TessellatorColor(hueColor);
												DrawUtils::TessellatorVertex(start.x, end.y, 0.f);
												// Right
												DrawUtils::TessellatorColor(MC_Color(1.f, 1.f, 1.f, drawAlpha));
												DrawUtils::TessellatorVertex(end.x, end.y, 0.f);
												DrawUtils::TessellatorVertex(end.x, start.y, 0.f);
												// Left again
												DrawUtils::TessellatorColor(hueColor);
												DrawUtils::TessellatorVertex(start.x, start.y, 0.f);
											}

											// Draw black gradient on top to fix weird gradient behavior
											{
												DrawUtils::TessellatorBegin(VertexFormat::QUAD);

												// Bottom
												DrawUtils::TessellatorColor(MC_Color(0.f, 0.f, 0.f, drawAlpha));
												DrawUtils::TessellatorVertex(start.x, end.y, 0.f);
												DrawUtils::TessellatorVertex(end.x, end.y, 0.f);
												// Top
												DrawUtils::TessellatorColor(MC_Color(0.f, 0.f, 0.f, 0.f));
												DrawUtils::TessellatorVertex(end.x, start.y, 0.f);
												DrawUtils::TessellatorVertex(start.x, start.y, 0.f);
											}

											// TODO: Make all of this not hardcoded

											DrawUtils::FlushTessellator();

											// picker
											DrawUtils::SetColor(1.f, 1.f, 1.f, drawAlpha);
											DrawUtils::fillRect({colorBounds.x + x, colorBounds.y + y}, {colorBounds.x + x + 2, colorBounds.y + y + 2});  // color picker dot

											// hueSlider
											DrawUtils::fillRect({hueBounds.x + z, hueBounds.y}, {hueBounds.x + z + 2, hueBounds.y + 7.5f});
											
										}

										if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
											overflowing = true;
											break;
										}

										GuiData* guiData = Game.getGuiData();

										// Hue Drag Logic
										{
											if (setting->isDragging2) {
												if (isLeftClickDown && !isRightClickDown) {
													Vec4 r = Vec4(colorBounds.x, 0, colorBounds.z, guiData->heightGame);

													if (r.contains(&mousePos))
														z = mousePos.x - hueBounds.x;
													else if (mousePos.x - hueBounds.x < 0)
														z = 0;
													else if (mousePos.x - hueBounds.z > 0)
														z = hueWidth - 0.1f;
												} else
													setting->isDragging2 = false;
											} else if (sliderFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
												shouldToggleLeftClick = false;
												setting->isDragging2 = true;
											}
										}

										// Picker Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown) {
													Vec4 r1 = Vec4(colorBounds.x, 0, colorBounds.z, guiData->heightGame);
													Vec4 r2 = Vec4(0, colorBounds.y, guiData->widthGame, colorBounds.w);

													if (r1.contains(&mousePos))
														x = mousePos.x - colorBounds.x;
													else if (mousePos.x - colorBounds.x < 0)
														x = 0;
													else if (mousePos.x - colorBounds.z > 0)
														x = colorWidth - 0.1f;

													if (r2.contains(&mousePos))
														y = mousePos.y - colorBounds.y;
													else if (mousePos.y - colorBounds.y < 0)
														y = 0;
													else if (mousePos.y - colorBounds.w > 0)
														y = colorHeight - 0.1f;
												}
												else
													setting->isDragging = false;
											} else if (pickerFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}
										

										// Save Values
										{
											setting->colorPicker.x = x;
											setting->colorPicker.y = y;
											setting->colorPicker.z = z;

											float sat = y / colorHeight;
											float vib = x / colorWidth;
											float hue = z / hueWidth;

											Utils::ColorConvertHSVtoRGB(hue, 1.f - vib, 1.f - sat, setting->value->Color.r, setting->value->Color.g, setting->value->Color.b);

											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += (textHeight + (textPaddingY * 2) + 49) * clickMod->mAnim;
								}
							} break;
							case ValueType::VEC2F_T: {
								isVec2f = true;
								goto Vec2Draw;
							} break;
							case ValueType::VEC2I_T: {
								isVec2f = false;
							Vec2Draw:

								// Text and background
								{
									// setting name
									char name[0x22];
									sprintf_s(name, "%s:", setting->name);
									// Convert first letter to uppercase for more friendlieness
									if (name[0] )
										name[0] = toupper(name[0]);
									std::string elTexto = name;
									rectPos.w = currentYOffset + textHeight + (textPaddingY * 2);
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/ + crossSize);

									if (rectPos.contains(&mousePos)) {
										DrawUtils::fillRect(rectPos, selectedModuleColor, backgroundAlpha * drawAlpha);
										if (shouldToggleRightClick && !ourWindow->isInAnimation) {
											shouldToggleRightClick = false;
											setting->minValue->_bool = !setting->minValue->_bool;
										}
										if (shouldToggleLeftClick && !ourWindow->isInAnimation) {
											shouldToggleLeftClick = false;
											setting->minValue->_bool = !setting->minValue->_bool;
										}
									} else
										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);

									DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
									DrawUtils::drawCrossLine(
										Vec2(
											currentXOffset + windowSize->x - (crossSize / 2) - 1.5f,
											currentYOffset + textPaddingY + (textHeight / 2)
										),
										settBrModuleColor, textSize, !setting->minValue->_bool
									);

									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}

								if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
									overflowing = true;
									break;
								}

								if (setting->minValue->_bool) {
									const int yOffset = 10;

									Vec4 rectX = Vec4(
										currentXOffset + textPaddingX + 12,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									currentYOffset += yOffset;

									Vec4 rectY = Vec4(
										currentXOffset + textPaddingX + 12,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									//  Text and background
									{
										{  // x
											// Convert first letter to uppercase for more friendlieness
											char name[0x22];
											sprintf_s(name, "X");
											if (name[0] )
												name[0] = toupper(name[0]);

											std::string elTexto = name;
											windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
											DrawUtils::drawText({textPos.x, rectX.y}, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
											rectPos.w = rectX.y;
											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
										}
										{  // y
											// Convert first letter to uppercase for more friendlieness
											char name[0x22];
											sprintf_s(name, "Y");
											if (name[0] )
												name[0] = toupper(name[0]);

											std::string elTexto = name;
											windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
											DrawUtils::drawText({textPos.x, rectY.y}, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
											rectPos.w = rectY.y;
											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
										}
									}

									// Sliders
									{
										// Visuals & Logic
										{
											rectPos.y = currentYOffset;
											rectPos.w += textHeight + (textPaddingY * 2);
											// Background
											const bool areWeFocusedX = rectX.contains(&mousePos);
											const bool areWeFocusedY = rectY.contains(&mousePos);

											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);  // Background

											const Vec2 minValue = isVec2f ? setting->minValue->Vec2f : 
												Vec2{setting->minValue->Vec2i.x, setting->minValue->Vec2i.y};
											const Vec2 maxValue = isVec2f ? setting->maxValue->Vec2f.sub(minValue) :
												Vec2{setting->maxValue->Vec2i.x - minValue.x, setting->maxValue->Vec2i.y - minValue.y};
											Vec2 value = {(float)fmax(0, setting->value->Vec2i.x - minValue.x), (float)fmax(0, setting->value->Vec2i.y - minValue.y)};
											if (isVec2f)
												value = {(float)fmax(0, setting->value->Vec2f.x - minValue.x), (float)fmax(0, setting->value->Vec2f.y - minValue.y)};

											if (value.x > maxValue.x)
												value.x = maxValue.x;

											if (value.y > maxValue.y)
												value.y = maxValue.y;

											value.x /= maxValue.x;  // Value is now in range 0 - 1
											value.y /= maxValue.y;

											const float endXlol = (xEnd - textPaddingX * 8.f) - (currentXOffset + textPaddingX + 12);
											value.x *= endXlol;  // Value is now pixel diff between start of bar and end of progress
											value.y *= endXlol;

											// Draw Ints
											{
												{// x
													Vec2 mid = Vec2(
														rectX.x + ((rectX.z - rectX.x) / 2),
														rectX.y - 0.2f);
													char str[10];
													if (isVec2f)
														sprintf_s(str, 10, "%.2f", setting->value->Vec2f.x);
													else
														sprintf_s(str, 10, "%i", setting->value->Vec2i.x);
													std::string text = str;
													mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

													DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
												}
												{  // y
													Vec2 mid = Vec2(
														rectY.x + ((rectY.z - rectY.x) / 2),
														rectY.y - 0.2f);
													char str[10];
													if (isVec2f)
														sprintf_s(str, 10, "%.2f", setting->value->Vec2f.y);
													else
														sprintf_s(str, 10, "%i", setting->value->Vec2i.y);
													std::string text = str;
													mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

													DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
												}
											}

											// Draw Progress
											{
												Vec4 rectX2 = rectX;
												rectX2.z = rectX2.x + value.x;
												DrawUtils::fillRect(rectX2, sliderColor, ((areWeFocusedX || setting->isDragging) ? 1.f : 0.8f) * drawAlpha);

												Vec4 rectY2 = rectY;
												rectY2.z = rectY2.x + value.y;
												DrawUtils::fillRect(rectY2, sliderColor, ((areWeFocusedY || setting->isDragging2) ? 1.f : 0.8f) * drawAlpha);
											}

											// Slider Outlines
											{
												// x slider
												DrawUtils::drawRect(rectX, whiteColor,  drawAlpha, 1.0f);  // Slider background
												// y slider
												DrawUtils::drawRect(rectY, whiteColor,  drawAlpha, 1.0f);  // Slider background
											}

											// Drag Logic
											{
												// x
												if (setting->isDragging) {
													if (isLeftClickDown && !isRightClickDown)
														value.x = mousePos.x - rectX.x;
													else
														setting->isDragging = false;
												} else if (areWeFocusedX && shouldToggleLeftClick && !ourWindow->isInAnimation) {
													shouldToggleLeftClick = false;
													setting->isDragging = true;
												}

												// y
												if (setting->isDragging2) {
													if (isLeftClickDown && !isRightClickDown)
														value.y = mousePos.x - rectY.x;
													else
														setting->isDragging2 = false;
												} else if (areWeFocusedY && shouldToggleLeftClick && !ourWindow->isInAnimation) {
													shouldToggleLeftClick = false;
													setting->isDragging2 = true;
												}
											}

											// Save Value
											{
												value.x /= endXlol;  // Now in range 0 - 1
												value.x *= maxValue.x;
												value.x += minValue.x;

												value.y /= endXlol;  // Now in range 0 - 1
												value.y *= maxValue.y;
												value.y += minValue.y;

												if (isVec2f) {
													setting->value->Vec2f.x = value.x;
													setting->value->Vec2f.y = value.y;
												}
												else {
													setting->value->Vec2i.x = (int)roundf(value.x);
													setting->value->Vec2i.y = (int)roundf(value.y);
												}

												setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
											}
										}
										currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
									}
								}
							} break;
							case ValueType::VEC3F_T: {
								isVec3f = true;
								goto Vec3Draw;
							} break;
							case ValueType::VEC3I_T: {
								isVec3f = false;
							Vec3Draw:

								// Text and background
								{
									// setting name
									char name[0x22];
									sprintf_s(name, "%s:", setting->name);
									// Convert first letter to uppercase for more friendlieness
									if (name[0] )
										name[0] = toupper(name[0]);
									std::string elTexto = name;
									rectPos.w = currentYOffset + textHeight + (textPaddingY * 2);
									windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/ + crossSize);

									if (rectPos.contains(&mousePos)) {
										DrawUtils::fillRect(rectPos, selectedModuleColor, backgroundAlpha * drawAlpha);
										if (shouldToggleRightClick && !ourWindow->isInAnimation) {
											shouldToggleRightClick = false;
											setting->minValue->_bool = !setting->minValue->_bool;
										}
										if (shouldToggleLeftClick && !ourWindow->isInAnimation) {
											shouldToggleLeftClick = false;
											setting->minValue->_bool = !setting->minValue->_bool;
										}
									} else
										DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);

									DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
									DrawUtils::drawCrossLine(
										Vec2(
											currentXOffset + windowSize->x - (crossSize / 2) - 1.5f,
											currentYOffset + textPaddingY + (textHeight / 2)
										),
										settBrModuleColor, textSize, !setting->minValue->_bool
									);

									currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
								}

								if ((currentYOffset - ourWindow->pos.y) > cutoffHeight) {
									overflowing = true;
									break;
								}

								if (setting->minValue->_bool) {
									const int yOffset = 10;

									Vec4 rectX = Vec4(
										currentXOffset + textPaddingX + 12,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									currentYOffset += yOffset;

									Vec4 rectY = Vec4(
										currentXOffset + textPaddingX + 12,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									currentYOffset += yOffset;

									Vec4 rectZ = Vec4(
										currentXOffset + textPaddingX + 12,
										currentYOffset + textPaddingY,
										xEnd - textPaddingX * 8.f,
										currentYOffset - textPaddingY + textHeight);

									//  Text and background
									{
										{  // x
											// Convert first letter to uppercase for more friendlieness
											char name[0x22];
											sprintf_s(name, "X");
											if (name[0] )
												name[0] = toupper(name[0]);

											std::string elTexto = name;
											windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
											DrawUtils::drawText({textPos.x, rectX.y}, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
											rectPos.w = rectX.y;
											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
										}
										{  // y
											// Convert first letter to uppercase for more friendlieness
											char name[0x22];
											sprintf_s(name, "Y");
											if (name[0] )
												name[0] = toupper(name[0]);

											std::string elTexto = name;
											windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
											DrawUtils::drawText({textPos.x, rectY.y}, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
											rectPos.w = rectY.y;
											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
										}
										{  // z
											// Convert first letter to uppercase for more friendlieness
											char name[0x22];
											sprintf_s(name, "Z");
											if (name[0] )
												name[0] = toupper(name[0]);

											std::string elTexto = name;
											windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);
											DrawUtils::drawText({textPos.x, rectZ.y}, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
											rectPos.w = rectZ.y;
											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);
										}
									}

									// Sliders
									{
										// Visuals & Logic
										{
											rectPos.y = currentYOffset;
											rectPos.w += textHeight + (textPaddingY * 2);
											// Background
											const bool areWeFocusedX = rectX.contains(&mousePos);
											const bool areWeFocusedY = rectY.contains(&mousePos);
											const bool areWeFocusedZ = rectZ.contains(&mousePos);

											DrawUtils::fillRect(rectPos, moduleColor, backgroundAlpha * drawAlpha);  // Background

											const Vec3 minValue = isVec3f ? setting->minValue->Vec3f : 
												Vec3{setting->minValue->Vec3i.x, setting->minValue->Vec3i.y, setting->minValue->Vec3i.z};
											const Vec3 maxValue = isVec3f ? setting->maxValue->Vec3f.sub(minValue) :
												Vec3{setting->maxValue->Vec3i.x - minValue.x, setting->maxValue->Vec3i.y - minValue.y, setting->maxValue->Vec3i.z - minValue.z};
											Vec3 value = {(float)fmax(0, setting->value->Vec3i.x - minValue.x), (float)fmax(0, setting->value->Vec3i.y - minValue.y), (float)fmax(0, setting->value->Vec3i.z - minValue.z)};
											if (isVec3f)
												value = {(float)fmax(0, setting->value->Vec3f.x - minValue.x), (float)fmax(0, setting->value->Vec3f.y - minValue.y), (float)fmax(0, setting->value->Vec3f.z - minValue.z)};

											if (value.x > maxValue.x)
												value.x = maxValue.x;

											if (value.y > maxValue.y)
												value.y = maxValue.y;

											if (value.z > maxValue.z)
												value.z = maxValue.z;

											value.x /= maxValue.x;  // Value is now in range 0 - 1
											value.y /= maxValue.y;
											value.z /= maxValue.z;

											const float endXlol = (xEnd - textPaddingX * 8.f) - (currentXOffset + textPaddingX + 12);
											value.x *= endXlol;  // Value is now pixel diff between start of bar and end of progress
											value.y *= endXlol;
											value.z *= endXlol;

											// Draw Ints
											{
												{// x
													Vec2 mid = Vec2(
														rectX.x + ((rectX.z - rectX.x) / 2),
														rectX.y - 0.2f);
													char str[10];
													if (isVec3f)
														sprintf_s(str, 10, "%.2f", setting->value->Vec3f.x);
													else
														sprintf_s(str, 10, "%i", setting->value->Vec3i.x);
													std::string text = str;
													mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

													DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
												}
												{  // y
													Vec2 mid = Vec2(
														rectY.x + ((rectY.z - rectY.x) / 2),
														rectY.y - 0.2f);
													char str[10];
													if (isVec3f)
														sprintf_s(str, 10, "%.2f", setting->value->Vec3f.y);
													else
														sprintf_s(str, 10, "%i", setting->value->Vec3i.y);
													std::string text = str;
													mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

													DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
												}
												{  // z
													Vec2 mid = Vec2(
														rectZ.x + ((rectZ.z - rectZ.x) / 2),
														rectZ.y - 0.2f);
													char str[10];
													if (isVec3f)
														sprintf_s(str, 10, "%.2f", setting->value->Vec3f.z);
													else
														sprintf_s(str, 10, "%i", setting->value->Vec3i.z);
													std::string text = str;
													mid.x -= DrawUtils::getTextWidth(&text, textSize, DesiredUi::CLICKGUI) / 2;

													DrawUtils::drawText(mid, &text, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
												}
											}

											// Draw Progress
											{
												Vec4 rectX2 = rectX;
												rectX2.z = rectX2.x + value.x;
												DrawUtils::fillRect(rectX2, sliderColor, ((areWeFocusedX || setting->isDragging) ? 1.f : 0.8f) * drawAlpha);

												Vec4 rectY2 = rectY;
												rectY2.z = rectY2.x + value.y;
												DrawUtils::fillRect(rectY2, sliderColor, ((areWeFocusedY || setting->isDragging2) ? 1.f : 0.8f) * drawAlpha);

												Vec4 rectZ2 = rectZ;
												rectZ2.z = rectZ2.x + value.z;
												DrawUtils::fillRect(rectZ2, sliderColor, ((areWeFocusedZ || setting->isDragging3) ? 1.f : 0.8f) * drawAlpha);
											}

											//Slider Outlines
											{
												// x slider
												DrawUtils::drawRect(rectX, whiteColor,  drawAlpha, 1.0f);  // Slider background
												// y slider
												DrawUtils::drawRect(rectY, whiteColor,  drawAlpha, 1.0f);  // Slider background
												// z slider
												DrawUtils::drawRect(rectZ, whiteColor,  drawAlpha, 1.0f);  // Slider background
											}

											// Drag Logic
											{
												// x
												if (setting->isDragging) {
													if (isLeftClickDown && !isRightClickDown)
														value.x = mousePos.x - rectX.x;
													else
														setting->isDragging = false;
												} else if (areWeFocusedX && shouldToggleLeftClick && !ourWindow->isInAnimation) {
													shouldToggleLeftClick = false;
													setting->isDragging = true;
												}

												// y
												if (setting->isDragging2) {
													if (isLeftClickDown && !isRightClickDown)
														value.y = mousePos.x - rectY.x;
													else
														setting->isDragging2 = false;
												} else if (areWeFocusedY && shouldToggleLeftClick && !ourWindow->isInAnimation) {
													shouldToggleLeftClick = false;
													setting->isDragging2 = true;
												}

												// z
												if (setting->isDragging3) {
													if (isLeftClickDown && !isRightClickDown)
														value.z = mousePos.x - rectZ.x;
													else
														setting->isDragging3 = false;
												} else if (areWeFocusedZ && shouldToggleLeftClick && !ourWindow->isInAnimation) {
													shouldToggleLeftClick = false;
													setting->isDragging3 = true;
												}
											}

											// Save Value
											{
												value.x /= endXlol;  // Now in range 0 - 1
												value.x *= maxValue.x;
												value.x += minValue.x;

												value.y /= endXlol;  // Now in range 0 - 1
												value.y *= maxValue.y;
												value.y += minValue.y;

												value.z /= endXlol;  // Now in range 0 - 1
												value.z *= maxValue.z;
												value.z += minValue.z;

												if (isVec2f) {
													setting->value->Vec3f.x = value.x;
													setting->value->Vec3f.y = value.y;
													setting->value->Vec3f.z = value.z;
												}
												else {
													setting->value->Vec3i.x = (int)roundf(value.x);
													setting->value->Vec3i.y = (int)roundf(value.y);
													setting->value->Vec3i.z = (int)roundf(value.z);
												}

												setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
											}
										}
										currentYOffset += (textHeight + (textPaddingY * 2)) * clickMod->mAnim;
									}
								}
							} break;
							case ValueType::POSITION_T: {
							} break;
							default: {
								char alc[100];
								sprintf_s(alc, 100, "Not implemented (%s)", setting->name);
								std::string elTexto = alc;
								// Adjust window size if our text is too  t h i c c
								windowSize->x = fmax(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize, DesiredUi::CLICKGUI) + 5 /* because we add 5 to text padding*/);

								DrawUtils::drawText(textPos, &elTexto, whiteColor, textSize, drawAlpha, DesiredUi::CLICKGUI);
								
							} break;
							}
						}
						float endYOffset = currentYOffset;
						if (endYOffset - startYOffset > textHeight + 1 || overflowing) {
							startYOffset += textPaddingY;
							endYOffset -= textPaddingY;
							float r, g, b;
							r = brightModuleBlendColor.r;
							g = brightModuleBlendColor.g;
							b = brightModuleBlendColor.b;
							DrawUtils::SetColor(r, g, b, drawAlpha);
							DrawUtils::drawLine(Vec2(currentXOffset + 2, startYOffset), Vec2(currentXOffset + 2, endYOffset), 0.5f);
						}
					}
				} else
					currentYOffset += textHeight + (textPaddingY * 2);
			}
		}

		Vec4 winRectPos = Vec4(
			xOffset,
			yOffset,
			xEnd,
			currentYOffset);

		if (winRectPos.contains(&mousePos)) {
			if (scrollingDirection > 0 && overflowing) {
				ourWindow->yOffset += scrollingDirection;
			} else if (scrollingDirection < 0) {
				ourWindow->yOffset += scrollingDirection;
			}
			scrollingDirection = 0;
			if (ourWindow->yOffset < 0) {
				ourWindow->yOffset = 0;
			}
		}
	}
		
	Vec4 rectPos = Vec4(
		currentXOffset,
		currentYOffset,
		currentXOffset + windowSize->x,
		currentYOffset + 4);
	DrawUtils::drawRoundRect(rectPos, CornerRadius(0, 0, 4, 4), {moduleColor.r, moduleColor.g, moduleColor.b, backgroundAlpha}, 1.f);

	DrawUtils::Flush();
	// Draw Category Header
	{
		std::string textStr = categoryName;
		Vec2 textPos = Vec2(
			(currentXOffset + (windowSize->x / 2.f)) - (DrawUtils::getTextWidth(&textStr, textSize * 1.4f, DesiredUi::CLICKGUI) / 2.f),
			categoryHeaderYOffset + textPaddingY);
		Vec4 rectPos = Vec4(
			currentXOffset,
			categoryHeaderYOffset - categoryMargin,
			currentXOffset + windowSize->x,
			categoryHeaderYOffset + (textHeight * 1.5f) + (textPaddingY * 2.f));

		// Extend Logic
		{
			if (rectPos.contains(&mousePos) && shouldToggleRightClick && !isDragging) {
				shouldToggleRightClick = false;
				ourWindow->isExtended = !ourWindow->isExtended;
				if (ourWindow->isExtended && ourWindow->animation == 0)
					ourWindow->animation = 0.2f;
				else if (!ourWindow->isExtended && ourWindow->animation == 1)
					ourWindow->animation = 0;
				ourWindow->isInAnimation = true;

				for (auto& mod : moduleList) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					clickMod->isExtended = false;
				}
			}
		}

		// Dragging Logic
		{
			if (isDragging && Utils::getCrcHash(categoryName) == draggedWindow) {  // WE are being dragged
				if (isLeftClickDown) {                                             // Still dragging
					Vec2 diff = Vec2(mousePos).sub(dragStart);
					ourWindow->pos = ourWindow->pos.add(diff);
					dragStart = mousePos;
				} else {  // Stopped dragging
					isDragging = false;
				}
			} else if (rectPos.contains(&mousePos) && shouldToggleLeftClick) {
				isDragging = true;
				draggedWindow = Utils::getCrcHash(categoryName);
				shouldToggleLeftClick = false;
				dragStart = mousePos;
			}
		}

		// Draw category component
		{
			// Draw Text
			std::string textStr = categoryName;
			DrawUtils::drawText(textPos, &textStr, whiteColor, textSize * 1.4f, 1.f, DesiredUi::CLICKGUI);
			DrawUtils::drawRoundRect(rectPos, CornerRadius(4, 4, 0, 0), {moduleColor.r, moduleColor.g, moduleColor.b, backgroundAlpha}, 0.5f);
		}
	}

	// anti idiot
	{
		Vec2 windowSize = Game.getGuiData()->windowSize;

		if (ourWindow->pos.x + ourWindow->size.x > windowSize.x) {
			ourWindow->pos.x = windowSize.x - ourWindow->size.x;
		}

		if (ourWindow->pos.y + ourWindow->size.y > windowSize.y) {
			ourWindow->pos.y = windowSize.y - ourWindow->size.y;
		}

		ourWindow->pos.x = (float)fmax(0, ourWindow->pos.x);
		ourWindow->pos.y = (float)fmax(0, ourWindow->pos.y);
	}

	moduleList.clear();
	DrawUtils::Flush();
}

void ClickGui::render() {
	static auto mod = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	mode = mod->style.GetSelectedEntry().GetValue();

	if (lastMode != mode) {
		if (mode == 0) {                                   //dark accented
			moduleColor = MC_Color(0, 0, 0);                   // background
			selectedModuleColor = moduleColor.lerp(whiteColor, 0.08f);  // 30, 110, 200
			enabledModuleColor = moduleColor.lerp(whiteColor, 0.04f);
			brightModuleBlendColor = MC_Color(0, 196, 252);  // tooltip border & category divider
			crossColor = MC_Color(0, 196, 252);
			selectedSettingColor = MC_Color(85, 85, 85);
			SettingColor = MC_Color(0, 0, 0, 0);
			sliderColor = MC_Color(85, 85, 85);
		} else if (mode == 1) {                            // dark white
			moduleColor = MC_Color(0, 0, 0);                   // background
			selectedModuleColor = moduleColor.lerp(whiteColor, 0.08f);  // 30, 110, 200
			enabledModuleColor = moduleColor.lerp(whiteColor, 0.04f);
			brightModuleBlendColor = MC_Color(255, 255, 255);  // tooltip border & category divider
			crossColor = MC_Color(150, 150, 150);
			selectedSettingColor = MC_Color(85, 85, 85);
			SettingColor = MC_Color(0, 0, 0, 0);
			sliderColor = MC_Color(85, 85, 85);
		} else if (mode == 2) {                            // classic
			moduleColor = MC_Color(13, 29, 48);            // background
			selectedModuleColor = MC_Color(30, 110, 200);  // 30, 110, 200
			enabledModuleColor = moduleColor.lerp(whiteColor, 0.04f);
			brightModuleBlendColor = whiteColor;  // tooltip border & category divider
			crossColor = whiteColor;
			selectedSettingColor = MC_Color(20, 100, 195);
			SettingColor = MC_Color(10, 25, 45);
			sliderColor = MC_Color(28, 107, 201);
		}
		lastMode = mode;
	}

	if (!moduleMgr->isInitialized())
		return;

	if (timesRendered < 10)
		timesRendered++;

	// Fill Background
	{
		DrawUtils::fillRect(Vec4(0,0,Game.getGuiData()->widthGame,Game.getGuiData()->heightGame),MC_Color(0, 0, 0), 0.6f);

		auto vint = DrawUtils::resourceToTexturePtr(VINTAGE_DATA, "Vintage");
		DrawUtils::drawImageFromTexturePtr(vint, Vec2(0.f, 0.f), Vec2(Game.getGuiData()->widthGame, Game.getGuiData()->heightGame), Vec2(0.f, 0.f), Vec2(1.f, 1.f));
		DrawUtils::flushImages(MC_Color(255, 255, 255));
	}

	// Render all categorys
	renderCategory(Category::COMBAT);
	renderCategory(Category::VISUAL);
	renderCategory(Category::MOVEMENT);
	renderCategory(Category::PLAYER);
	renderCategory(Category::WORLD);
	renderCategory(Category::MISC);
	renderCategory(Category::GUI);

	shouldToggleLeftClick = false;
	shouldToggleRightClick = false;
	resetStartPos = false;

	DrawUtils::Flush();
}

void ClickGui::init() { initialised = true; }

void ClickGui::onMouseClickUpdate(int key, bool isDown) {
	static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>("ClickGui");
	if (clickGuiMod->isEnabled() && Game.isInGame())
		switch (key) {
		case 1:  // Left Click
			isLeftClickDown = isDown;
			if (isDown)
				shouldToggleLeftClick = true;
			break;
		case 2:  // Right Click
			isRightClickDown = isDown;
			if (isDown)
				shouldToggleRightClick = true;
			break;
		}
}

void ClickGui::onWheelScroll(bool direction) {
	if (!direction)
		scrollingDirection++;
	else
		scrollingDirection--;
}

void ClickGui::onKeyUpdate(int key, bool isDown) {
	if (!initialised)
		return;
	static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>("ClickGui");

	if (!isDown)
		return;

	if (!clickGuiMod->isEnabled()) {
		timesRendered = 0;
		return;
	}

	if (timesRendered < 10)
		return;
	timesRendered = 0;

	if (clickGuiMod->isEnabled()) {
		GameSettingsInput* input = Game.getGameSettingsInput();
		if (GameData::isKeyDown(*input->inventoy.key) || GameData::isKeyDown(*input->chat.key)) {
			clickGuiMod->setEnabled(false);  // prevents a glitch were you are able to have clickgui open and are able to turn, but unable to click modules.
		}
	}

	switch (key) {
	case VK_ESCAPE:
		clickGuiMod->setEnabled(false);
		return;
	default:
		if (key == clickGuiMod->getKeybind())
			clickGuiMod->setEnabled(false);
	}
}
using json = nlohmann::json;
#include <unordered_map>



void ClickGui::onLoadConfig(void* confVoid) {
	savedWindowSettings.clear();
	windowMap.clear();

	json* conf = reinterpret_cast<json*>(confVoid);
	if (conf->contains("ClickGui")) {
		auto obj = conf->at("ClickGui");
		for (int i = 0; i <= static_cast<int>(Category::GUI); i++) {
			auto catName = ClickGui::catToName(static_cast<Category>(i));
			if (obj.contains(catName)) {
				try {
												auto& value = obj.at(catName);
												SavedWindowSettings windowSettings;

												if (value.contains("pos")) {
													auto& posVal = value.at("pos");
													if (posVal.is_object() && posVal.contains("x") && posVal.contains("y")) {
														windowSettings.pos = {posVal["x"].get<float>(), posVal["y"].get<float>()};
													}
												}

												if (value.contains("isExtended") && value["isExtended"].is_boolean()) {
													windowSettings.isExtended = value["isExtended"].get<bool>();
												}

												windowSettings.name = catName;
												savedWindowSettings[std::hash<std::string>{}(catName)] = windowSettings;
				} catch (const std::exception& e) {
												logF("Config Load Error (ClickGuiMenu): %s", e.what());
				}
			}
		}
	}
}

void ClickGui::onSaveConfig(void* confVoid, void* objVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	json& obj = *(json*)objVoid;

	for (const auto& wind : windowMap) {
		savedWindowSettings[std::hash<std::string>{}(wind.second->name)] = {wind.second->pos, wind.second->isExtended, wind.second->name};
	}

	if (conf->contains("ClickGui")) {
		conf->erase("ClickGui");
	}

	for (const auto& wind : savedWindowSettings) {
		obj[wind.second.name]["pos"]["x"] = wind.second.pos.x;
		obj[wind.second.name]["pos"]["y"] = wind.second.pos.y;
		obj[wind.second.name]["isExtended"] = wind.second.isExtended;
	}
}
