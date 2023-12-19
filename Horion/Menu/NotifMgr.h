#pragma once

#include "../../Memory/GameData.h"
#include "../DrawUtils.h"
#include "../Module/ModuleManager.h"

class Notif {
public:
	enum class DefTexture {
		SCheckmark,
		SCross,
		SWarn,
		SNotif,
		SLogo
	};
	Notif(int time, MC_Color color);
	~Notif();
	void setName(MC_Color color, char *format, ...);
	void setText(MC_Color color, char *format, ...);
	void setTexture(std::shared_ptr<TexturePtr> texdata);
	void setDefTexture(DefTexture tex);
	int mTotalTime;
	int mTimeLeft;
	std::shared_ptr<TexturePtr> mTexture;
	MC_Color mNameColor;
	MC_Color mTextColor;
	MC_Color mBGColor;
	char mName[256];
	char mText[256];
	int namelen;
	int textlen;
	float mAnim;
};
class NotifMgr {
public:
	static NotifMgr *sInstance;
	NotifMgr();
	void onCalc();
	void onCalcDraw();
	Notif *makeNotif(int time, MC_Color color);
	static Notif *makeNotifStatic(int time, MC_Color color);
	void addNotif(Notif *notif);
	static MC_Color getBgColor(MC_Color og);
	std::mutex notifMutex;
	std::vector<Notif *> mNotifications;
	std::shared_ptr<TexturePtr> mCheckmark;
	std::shared_ptr<TexturePtr> mCross;
	std::shared_ptr<TexturePtr> mWarn;
	std::shared_ptr<TexturePtr> mLogo;
	std::shared_ptr<TexturePtr> mNotif;
};