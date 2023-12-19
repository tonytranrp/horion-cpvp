#include "Module.h"

#include "../../../Utils/Json.hpp"
#include "../../../Utils/Logger.h"

#include "../../Menu/NotifMgr.h"

using json = nlohmann::json;

#pragma region EnumEntry
EnumEntry::EnumEntry(const std::string _name, const unsigned char value) {
	name = _name;
	val = value;
}
std::string EnumEntry::GetName() {
	return name;
}
unsigned char EnumEntry::GetValue() {
	return val;
}
#pragma endregion
#pragma region SettingEnum
SettingEnum::SettingEnum(std::vector<EnumEntry> entr, IModule* mod) {
	Entrys = entr;
	owner = mod;
	std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
		return rhs.GetValue() < lhs.GetValue();
	});
}
SettingEnum::SettingEnum(IModule* mod) {
	owner = mod;
}

SettingEnum& SettingEnum::addEntry(EnumEntry entr) {
	auto etr = EnumEntry(entr);
	bool SameVal = false;
	for (auto it = this->Entrys.begin(); it != this->Entrys.end(); it++) {
		SameVal |= it->GetValue() == etr.GetValue();
	}
	if (!SameVal) {
		Entrys.push_back(etr);
		std::sort(Entrys.begin(), Entrys.end(), [](EnumEntry rhs, EnumEntry lhs) {
			return rhs.GetValue() < lhs.GetValue();
		});
	}
	return *this;
}
EnumEntry& SettingEnum::GetEntry(int ind) {
	return Entrys.at(ind);
}
EnumEntry& SettingEnum::GetSelectedEntry() {
	return GetEntry(selected);
}
int SettingEnum::GetCount() {
	return (int)Entrys.size();
}
#pragma endregion

IModule::IModule(int key, Category c, const char* tooltip) {
	this->keybind = key;
	this->category = c;
	this->tooltip = tooltip;
	this->registerIntSetting(std::string("keybind"), &this->keybind, this->keybind, 0, 0xFF);
	this->registerBoolSetting(std::string("enabled"), &this->enabled, false);
	this->registerBoolSetting(std::string("visible"), &this->visible, true);
	this->ModulePos = 0.0f;
}

void IModule::registerFloatSetting(std::string name, float* floatPtr, float defaultValue, float minValue, float maxValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
#ifdef HORION_DEBUG
	if (minValue > maxValue)
		__debugbreak();  // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::FLOAT_T;

	setting->value = reinterpret_cast<SettingValue*>(floatPtr);

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_float = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value
	SettingValue* minVal = new SettingValue();
	minVal->_float = minValue;
	setting->minValue = minVal;

	// Max Value
	SettingValue* maxVal = new SettingValue();
	maxVal->_float = maxValue;
	setting->maxValue = maxVal;

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerIntSetting(std::string name, int* intPtr, int defaultValue, int minValue, int maxValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
#ifdef HORION_DEBUG
	if (minValue > maxValue)
		__debugbreak();  // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::INT_T;
	setting->value = reinterpret_cast<SettingValue*>(intPtr);  // Actual Value

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value
	SettingValue* minVal = new SettingValue();
	minVal->_int = minValue;
	setting->minValue = minVal;

	// Max Value
	SettingValue* maxVal = new SettingValue();
	maxVal->_int = maxValue;
	setting->maxValue = maxVal;

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	// Name
	strcpy_s(setting->name, 19, name.c_str());

	settings.push_back(setting);  // Add to list
}

void IModule::registerEnumSetting(std::string name, SettingEnum* ptr, int defaultValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::ENUM_T;
	if (defaultValue < 0 || defaultValue >= ptr->GetCount())
		defaultValue = 0;

	// Actual Value
	setting->value = reinterpret_cast<SettingValue*>(&ptr->selected);
	setting->value->_int = defaultValue;

	// Default Value
	SettingValue* defaultVal = new SettingValue();
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value (is Extended)
	SettingValue* minVal = new SettingValue();
	minVal->_bool = false;
	setting->minValue = minVal;

	// Enum data
	setting->extraData = ptr;

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());
	settings.push_back(setting);
}

void IModule::registerBoolSetting(std::string name, bool* boolPtr, bool defaultValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::BOOL_T;

	setting->value = reinterpret_cast<SettingValue*>(boolPtr);  // Actual value

	SettingValue* defaultVal = new SettingValue();  // Default Value
	defaultVal->_bool = defaultValue;
	setting->defaultValue = defaultVal;

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerColorSetting(std::string name, rgb* colorPtr, rgb defaultValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
	SettingEntry* setting = new SettingEntry();
	setting->valueType = ValueType::COLOR_T;

	setting->value = reinterpret_cast<SettingValue*>(colorPtr);  // Actual value

	SettingValue* defaultVal = new SettingValue();  // Default Value
	defaultVal->Color = defaultValue;
	setting->defaultValue = defaultVal;
	setting->value->Color = defaultValue;

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerVectorSetting(std::string name, Vec2* vecPtr, Vec2 defaultValue, Vec2 minValue, Vec2 maxValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
#ifdef HORION_DEBUG
	if (minValue.x > maxValue.x || minValue.y > maxValue.y)
		__debugbreak();  // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	SettingValue* defaultVal = new SettingValue();
	SettingValue* minVal = new SettingValue();
	SettingValue* maxVal = new SettingValue();

	setting->valueType = ValueType::VEC2F_T;

	defaultVal->Vec2f = defaultValue;
	minVal->Vec2f = minValue;
	maxVal->Vec2f = maxValue;

	setting->value = reinterpret_cast<SettingValue*>(vecPtr);
	setting->defaultValue = defaultVal;
	setting->minValue = minVal;
	setting->maxValue = maxVal;

	setting->minValue->_bool = false; //fixes some weird default collapse bug

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerVectorSetting(std::string name, Vec2i* vecPtr, Vec2i defaultValue, Vec2i minValue, Vec2i maxValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
#ifdef HORION_DEBUG
	if (minValue.x > maxValue.x || minValue.y > maxValue.y)
		__debugbreak();  // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	SettingValue* defaultVal = new SettingValue();
	SettingValue* minVal = new SettingValue();
	SettingValue* maxVal = new SettingValue();

	setting->valueType = ValueType::VEC2I_T;

	defaultVal->Vec2i = defaultValue;
	minVal->Vec2i = minValue;
	maxVal->Vec2i = maxValue;

	setting->value = reinterpret_cast<SettingValue*>(vecPtr);
	setting->defaultValue = defaultVal;
	setting->minValue = minVal;
	setting->maxValue = maxVal;

	setting->minValue->_bool = false;  // fixes some weird default collapse bug

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerVectorSetting(std::string name, Vec3* vecPtr, Vec3 defaultValue, Vec3 minValue, Vec3 maxValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
#ifdef HORION_DEBUG
	if (minValue.x > maxValue.x || minValue.y > maxValue.y || minValue.z > maxValue.z)
		__debugbreak();  // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	SettingValue* defaultVal = new SettingValue();
	SettingValue* minVal = new SettingValue();
	SettingValue* maxVal = new SettingValue();

	setting->valueType = ValueType::VEC3F_T;

	defaultVal->Vec3f = defaultValue;
	minVal->Vec3f = minValue;
	maxVal->Vec3f = maxValue;

	setting->value = reinterpret_cast<SettingValue*>(vecPtr);
	setting->defaultValue = defaultVal;
	setting->minValue = minVal;
	setting->maxValue = maxVal;

	setting->minValue->_bool = false;  // fixes some weird default collapse bug

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerVectorSetting(std::string name, Vec3i* vecPtr, Vec3i defaultValue, Vec3i minValue, Vec3i maxValue, std::vector<int> dependentSetttingValue, int* dependentSetttingValuePtr) {
#ifdef HORION_DEBUG
	if (minValue.x > maxValue.x || minValue.y > maxValue.y || minValue.z > maxValue.z)
		__debugbreak();  // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	SettingValue* defaultVal = new SettingValue();
	SettingValue* minVal = new SettingValue();
	SettingValue* maxVal = new SettingValue();

	setting->valueType = ValueType::VEC3I_T;

	defaultVal->Vec3i = defaultValue;
	minVal->Vec3i = minValue;
	maxVal->Vec3i = maxValue;

	setting->value = reinterpret_cast<SettingValue*>(vecPtr);
	setting->defaultValue = defaultVal;
	setting->minValue = minVal;
	setting->maxValue = maxVal;

	setting->minValue->_bool = false;  // fixes some weird default collapse bug

	// Dependent Setting
	if (dependentSetttingValuePtr) {
		setting->dependsOnOtherSetting = true;
		setting->dependentSetttingValue = dependentSetttingValue;
		setting->dependentSetttingValuePtr = dependentSetttingValuePtr;
	}

	strcpy_s(setting->name, 19, name.c_str());  // Name

	settings.push_back(setting);  // Add to list
}

void IModule::registerPositionSetting(Vec2* positionPtr, Vec2 defaultValue) {
	SettingEntry* setting = new SettingEntry();
	SettingValue* defaultVal = new SettingValue();

	setting->valueType = ValueType::POSITION_T;

	defaultVal->Vec2f = defaultValue;
	setting->value = reinterpret_cast<SettingValue*>(positionPtr);
	setting->defaultValue = defaultVal;

	strcpy_s(setting->name, 19, "Position");  // Name

	settings.push_back(setting);  // Add to list
} 

IModule::~IModule() {
	for (auto it = this->settings.begin(); it != this->settings.end(); it++) {
		delete *it;
	}
	this->settings.clear();
}

const char* IModule::getModuleName() {
	return "Module";
}

const char* IModule::getRawModuleName() {
	return getModuleName();
}

int IModule::getKeybind() {
	return this->keybind;
}

void IModule::setKeybind(int key) {
	this->keybind = key;
}

std::string IModule::getModeText() {
	return "";
}

bool IModule::allowAutoStart() {
	return true;
}

void IModule::onTick(GameMode*) {
}

void IModule::onKeyUpdate(int key, bool isDown) {
	if (key == getKeybind()) {
		if (isFlashMode())
			setEnabled(isDown);
		else if (isDown)
			toggle();
	}
}

void IModule::onEnable() {
}

void IModule::onDisable() {
}

void IModule::onPreRender(MinecraftUIRenderContext* renderCtx) {
}

void IModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
}

void IModule::onSendPacket(Packet*) {
}

void IModule::onLoadConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	if (conf->contains(this->getRawModuleName())) {
		auto obj = conf->at(this->getRawModuleName());
		if (obj.is_null())
			return;
		for (auto it = this->settings.begin(); it != this->settings.end(); ++it) {
			SettingEntry* sett = *it;
			if (obj.contains(sett->name)) {
				auto value = obj.at(sett->name);
				if (value.is_null())
					continue;
				try {
					switch (sett->valueType) {
					case ValueType::FLOAT_T:
						sett->value->_float = value.get<float>();
						break;
					case ValueType::DOUBLE_T:
						sett->value->_double = value.get<double>();
						break;
					case ValueType::INT64_T:
						sett->value->int64 = value.get<__int64>();
						break;
					case ValueType::INT_T:
						sett->value->_int = value.get<int>();
						break;
					case ValueType::BOOL_T:
						sett->value->_bool = value.get<bool>();
						break;
					case ValueType::TEXT_T:
						sett->value->text = new std::string(value.get<std::string>());
						break;
					case ValueType::ENUM_T:
						try {
							sett->value->_int = value.get<int>();
						} catch (const std::exception& e) {
							logF("Config Load Error(Enum) (%s): %s ", this->getRawModuleName(), e.what());
						}
						break;
					case ValueType::COLOR_T:
						sett->value->Color.r = value.at("Red").get<float>();
						sett->value->Color.g = value.at("Green").get<float>();
						sett->value->Color.b = value.at("Blue").get<float>();

						sett->colorPicker.x = value.at("PickerX").get<float>();
						sett->colorPicker.y = value.at("PickerY").get<float>();
						sett->colorPicker.z = value.at("HueX").get<float>();

						break;
					case ValueType::VEC2F_T:
						sett->value->Vec2f.x = value.at("X").get<float>();  
						sett->value->Vec2f.y = value.at("Y").get<float>();
						break;
					case ValueType::VEC2I_T:
						sett->value->Vec2i.x = value.at("X").get<int>();
						sett->value->Vec2i.y = value.at("Y").get<int>();
						break;
					case ValueType::VEC3F_T:
						sett->value->Vec3f.x = value.at("X").get<float>();
						sett->value->Vec3f.y = value.at("Y").get<float>();
						sett->value->Vec3f.z = value.at("Z").get<float>();
						break;
					case ValueType::VEC3I_T:
						sett->value->Vec3i.x = value.at("X").get<int>();
						sett->value->Vec3i.y = value.at("Y").get<int>();
						sett->value->Vec3i.z = value.at("Z").get<int>();
						break;
					case ValueType::POSITION_T:
						sett->value->Position.x = value.at("X").get<float>();
						sett->value->Position.y = value.at("Y").get<float>();
						break;
					}
					sett->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
					continue;
				} catch (std::exception e) {
					logF("Config Load Error (%s): %s", this->getRawModuleName(), e.what());
				}
			}
		}
		if (this->enabled)
			this->onEnable();
	}
}

#pragma warning(push)
#pragma warning(disable : 26444)
void IModule::onSaveConfig(void* confVoid) {
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = getRawModuleName();
	json obj = (*conf)[modName];

	//auto obj = conf->at(modName);
	for (auto sett : this->settings) {
		switch (sett->valueType) {
		case ValueType::FLOAT_T:
			obj[sett->name] = sett->value->_float;
			break;
		case ValueType::DOUBLE_T:
			obj[sett->name] = sett->value->_double;
			break;
		case ValueType::INT64_T:
			obj[sett->name] = sett->value->int64;
			break;
		case ValueType::INT_T:
			obj[sett->name] = sett->value->_int;
			break;
		case ValueType::BOOL_T:
			obj[sett->name] = sett->value->_bool;
			break;
		case ValueType::TEXT_T:
			obj[sett->name] = *sett->value->text;
			break;
		case ValueType::ENUM_T:
			obj[sett->name] = sett->value->_int;
			break;
		case ValueType::COLOR_T: {
			json c_obj = {}; //color value in rgb
			c_obj.emplace("Red", sett->value->Color.r);
			c_obj.emplace("Green", sett->value->Color.g);
			c_obj.emplace("Blue", sett->value->Color.b);

			//color picker and slider positions
			c_obj.emplace("PickerX", sett->colorPicker.x);
			c_obj.emplace("PickerY", sett->colorPicker.y);
			c_obj.emplace("HueX", sett->colorPicker.z);

			obj[sett->name] = c_obj;
		} break;
		case ValueType::VEC2F_T: {
			json c_obj = {};
			c_obj.emplace("X", sett->value->Vec2f.x);
			c_obj.emplace("Y", sett->value->Vec2f.y);

			obj[sett->name] = c_obj;
		} break;
		case ValueType::VEC2I_T: {
			json c_obj = {};
			c_obj.emplace("X", sett->value->Vec2i.x);
			c_obj.emplace("Y", sett->value->Vec2i.y);

			obj[sett->name] = c_obj;
		} break;
		case ValueType::VEC3F_T: {
			json c_obj = {};
			c_obj.emplace("X", sett->value->Vec3f.x);
			c_obj.emplace("Y", sett->value->Vec3f.y);
			c_obj.emplace("Z", sett->value->Vec3f.z);

			obj[sett->name] = c_obj;
		} break;
		case ValueType::VEC3I_T: {
			json c_obj = {};
			c_obj.emplace("X", sett->value->Vec3i.x);
			c_obj.emplace("Y", sett->value->Vec3i.y);
			c_obj.emplace("Z", sett->value->Vec3i.z);

			obj[sett->name] = c_obj;
		} break;
		case ValueType::POSITION_T: {
			json c_obj = {};
			c_obj.emplace("X", sett->value->Position.x);
			c_obj.emplace("Y", sett->value->Position.y);

			obj[sett->name] = c_obj;
		} break;
		}
	}
	(*conf)[modName] = obj;
}

#pragma warning(pop)

bool IModule::isFlashMode() {
	return false;
}

void IModule::setEnabled(bool enabled) {
	if (this->enabled != enabled) {
		this->enabled = enabled;
			//logF("%s %s", enabled ? "Enabled" : "Disabled", this->getModuleName());
#ifdef HORION_DEBUG
		if (NotifMgr::sInstance) {
			Notif* notif = NotifMgr::makeNotifStatic(3 * 60, MC_Color(0.0f, 0.0f, 0.0f, 0.65f));
			notif->setName(MC_Color(1.0f, 1.0f, 1.0f), "Notification");
			notif->setText(MC_Color(1.0f, 1.0f, 1.0f), "%s %s", enabled ? "Enabled" : "Disabled", this->getModuleName());
			notif->setDefTexture(Notif::DefTexture::SNotif);
		}
#endif
		
		if (enabled)
			this->onEnable();
		else
			this->onDisable();
	}
}

void IModule::toggle() {
	setEnabled(!this->enabled);
}

bool IModule::isEnabled() {
	return this->enabled;
}

const char* IModule::getTooltip() {
	return this->tooltip;
}
void IModule::onAttack(Entity*) {
}
bool IModule::callWhenDisabled() {
	return false;
}
void IModule::onMove(MoveInputHandler*) {
}
void IModule::onLevelRender() {
}
void IModule::onClientInputCallbacksTick(ClientInputCallbacks*) {
}
void IModule::clientMessageF(const char* fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	static char msgBuf[300]{};
	vsprintf_s(msgBuf, sizeof(msgBuf), fmt, arg);
	GameData::log("%s[%sHorion %s- %s%s%s] - %s%s", GRAY, AQUA, GRAY, AQUA, this->getModuleName(), GRAY, RESET, msgBuf);
	va_end(arg);
}

void SettingEntry::makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt() {
	switch (valueType) {
	case ValueType::ENUM_T:
		value->_int = std::max(0, std::min(reinterpret_cast<SettingEnum*>(extraData)->GetCount() - 1, value->_int));
		break;
	case ValueType::BOOL_T:
		break;
	case ValueType::INT64_T:
		value->int64 = std::max(minValue->int64, std::min(maxValue->int64, value->int64));
		break;
	case ValueType::DOUBLE_T:
		value->_double = std::max(minValue->_double, std::min(maxValue->_double, value->_double));
		break;
	case ValueType::FLOAT_T:
		value->_float = std::max(minValue->_float, std::min(maxValue->_float, value->_float));
		break;
	case ValueType::INT_T:
		value->_int = std::max(minValue->_int, std::min(maxValue->_int, value->_int));
		break;
	case ValueType::COLOR_T:
		break;
	case ValueType::VEC2F_T:
		value->Vec2f.x = std::max(minValue->Vec2f.x, std::min(maxValue->Vec2f.x, value->Vec2f.x));
		value->Vec2f.y = std::max(minValue->Vec2f.y, std::min(maxValue->Vec2f.y, value->Vec2f.y));
		break;
	case ValueType::VEC2I_T:
		value->Vec2i.x = std::max(minValue->Vec2i.x, std::min(maxValue->Vec2i.x, value->Vec2i.x));
		value->Vec2i.y = std::max(minValue->Vec2i.y, std::min(maxValue->Vec2i.y, value->Vec2i.y));
		break;
	case ValueType::VEC3F_T:
		value->Vec3f.x = std::max(minValue->Vec3f.x, std::min(maxValue->Vec3f.x, value->Vec3f.x));
		value->Vec3f.y = std::max(minValue->Vec3f.y, std::min(maxValue->Vec3f.y, value->Vec3f.y));
		value->Vec3f.z = std::max(minValue->Vec3f.z, std::min(maxValue->Vec3f.z, value->Vec3f.z));
		break;
	case ValueType::VEC3I_T:
		value->Vec3i.x = std::max(minValue->Vec3i.x, std::min(maxValue->Vec3i.x, value->Vec3i.x));
		value->Vec3i.y = std::max(minValue->Vec3i.y, std::min(maxValue->Vec3i.y, value->Vec3i.y));
		value->Vec3i.z = std::max(minValue->Vec3i.z, std::min(maxValue->Vec3i.z, value->Vec3i.z));
		break;
	case ValueType::TEXT_T:
		break;
	case ValueType::POSITION_T:
		break;
	default:
		logF("unrecognized value %i", valueType);
		break;
	}
}