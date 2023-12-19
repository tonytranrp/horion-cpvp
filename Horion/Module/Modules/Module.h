#pragma once
#include "../../../Memory/GameData.h"
#include "../../../Utils/keys.h"
#include "../../FriendList/FriendList.h"
#include "../../../SDK/Option.h"
//#include "../../DrawUtils.h"

class IModule;

enum class Category {
	COMBAT = 0,
	VISUAL = 1,
	MOVEMENT = 2,
	PLAYER = 3,
	WORLD = 4,
	MISC = 5,
	GUI = 6,
};

struct EnumEntry {
private:
	/*const */ std::string name;
	/*const */ unsigned char val;

public:
	/// <summary>Use this however you want</summary>
	void* ptr = nullptr;
	EnumEntry(const std::string _name, const unsigned char value);
	std::string GetName();
	unsigned char GetValue();
};

struct AddResult;

class SettingEnum {
private:
	IModule* owner = nullptr;

public:
	std::vector<EnumEntry> Entrys;
	int selected = -1;

	SettingEnum(std::vector<EnumEntry> entr, IModule* mod = nullptr);
	SettingEnum(IModule* mod = nullptr);
	//SettingEnum();
	SettingEnum& addEntry(EnumEntry entr);
	EnumEntry& GetEntry(int ind);
	EnumEntry& GetSelectedEntry();
	int GetCount();
};

struct rgb {
	float r;
	float g;
	float b;
};

enum class ValueType {
	FLOAT_T,
	DOUBLE_T,
	INT64_T,
	INT_T,
	BOOL_T,
	TEXT_T,
	ENUM_T,
	COLOR_T,
	VEC2F_T,
	VEC2I_T,
	VEC3F_T,
	VEC3I_T,
	POSITION_T
};

struct SettingValue {
	union {
		float _float;
		double _double;
		__int64 int64;
		int _int;
		bool _bool;
		std::string* text;
		SettingEnum* Enum;
		rgb Color;
		Vec2 Vec2f;
		Vec2i Vec2i;
		Vec3 Vec3f;
		Vec3i Vec3i;
		Vec2 Position;
	};

	SettingValue() {};
};

struct SettingEntry {
	char name[0x20] = "";
	ValueType valueType;
	SettingValue* value = nullptr;
	SettingValue* defaultValue = nullptr;
	SettingValue* minValue = nullptr;
	SettingValue* maxValue = nullptr;
	void* extraData;  // Only used by enum for now

	// ClickGui Data
	bool isDragging = false;  // This is incredibly hacky and i wanted to avoid this as much as possible but i want to get this clickgui done
	bool isDragging2 = false; // Used for multiple sliders in the same setting
	bool isDragging3 = false; // Used for multiple sliders in the same setting
	Vec3 colorPicker = {0, 0, 0}; // Only used by color		x and y = color picker		z = hue slider

	bool extended = false;	//used for controling collapsable settings

	bool dependsOnOtherSetting = false;
	std::vector<int> dependentSetttingValue = {};
	int* dependentSetttingValuePtr = nullptr;

	void makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
};

class IModule {
private:
	bool enabled = false;
	int keybind = 0x0;
	std::string modeText;
	bool extended = false;
	float ModulePos;
	float nextModulePos;
	bool visible = true;
	bool hasPosSetting = false;

private:
	Category category;
	const char* tooltip;

	std::vector<SettingEntry*> settings;

protected:
	IModule(int key, Category c, const char* tooltip);

	void registerFloatSetting(std::string name, float* floatPtr, float defaultValue, float minValue, float maxValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerIntSetting(std::string name, int* intpTr, int defaultValue, int minValue, int maxValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerEnumSetting(std::string name, SettingEnum* intPtr, int defaultValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerBoolSetting(std::string name, bool* boolPtr, bool defaultValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerColorSetting(std::string name, rgb* colorPtr, rgb defaultValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerPositionSetting(Vec2* positionPtr, Vec2 defaultValue);
	void registerVectorSetting(std::string name, Vec2* vecPtr, Vec2 defaultValue, Vec2 minValue, Vec2 maxValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerVectorSetting(std::string name, Vec2i* vecPtr, Vec2i defaultValue, Vec2i minValue, Vec2i maxValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerVectorSetting(std::string name, Vec3* vecPtr, Vec3 defaultValue, Vec3 minValue, Vec3 maxValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);
	void registerVectorSetting(std::string name, Vec3i* vecPtr, Vec3i defaultValue, Vec3i minValue, Vec3i maxValue, std::vector<int> dependentSetttingValue = {0}, int* dependentSetttingValuePtr = nullptr);

	void clientMessageF(const char* fmt, ...);

public:
	virtual ~IModule();

	const Category getCategory() { return category; };

	inline float* getPos() { return &ModulePos; };

	inline float* getNextPos() { return &nextModulePos; };

	inline std::vector<SettingEntry*>* getSettings() { return &settings; };

	virtual const char* getModuleName() = 0;
	virtual const char* getRawModuleName();
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual std::string getModeText();
	virtual bool allowAutoStart();

	virtual void onTick(GameMode*);
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onEnable();
	virtual void onDisable();
	virtual void onAttack(Entity*);
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx);
	virtual void onLevelRender();
	virtual void onClientInputCallbacksTick(ClientInputCallbacks*);
	virtual void onMove(MoveInputHandler*);
	virtual void onLoadConfig(void* conf);
	virtual void onSaveConfig(void* conf);
	virtual bool isFlashMode();
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual bool isEnabled();
	virtual void onSendPacket(Packet*);
	virtual bool callWhenDisabled();
	virtual bool isVisibleInArrayList() { return visible; }
	virtual bool hasPositionSetting() { return hasPosSetting; }
	const char* getTooltip();
	void setVisible(bool b) { visible = b; }
};
