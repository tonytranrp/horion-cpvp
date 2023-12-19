#include "NotifMgr.h"
#include "../../resource.h"

NotifMgr *NotifMgr::sInstance = NULL;
Notif::Notif(int time, MC_Color color) {
	mTexture = NotifMgr::sInstance->mLogo;
	mBGColor = color;
	mNameColor = MC_Color(1.0f, 1.0f, 1.0f, 1.0f);
	mTextColor = MC_Color(1.0f, 1.0f, 1.0f, 1.0f);
	mTotalTime = time;
	mTimeLeft = time;
	memset(mName, 0, 256);
	memset(mText, 0, 256);
	namelen = 0;
	textlen = 0;
	mAnim = 0.0f;
}
void Notif::setName(MC_Color color, char *format, ...) {
	va_list args;
	char buff[512];
	memset(buff, 0, sizeof(buff));
	va_start(args, format);
	namelen = vsnprintf(buff, sizeof(buff), format, args);
	memset(mName, 0, 256);
	memcpy(mName, buff, namelen);
	va_end(args);
	mNameColor = color;
}
void Notif::setText(MC_Color color, char *format, ...) {
	va_list args;
	char buff[512];
	memset(buff, 0, sizeof(buff));
	va_start(args, format);
	textlen = vsnprintf(buff, sizeof(buff), format, args);
	memset(mText, 0, 256);
	memcpy(mText, buff, textlen);
	va_end(args);
	mTextColor = color;
}
void Notif::setDefTexture(Notif::DefTexture tex) { // default textures
	switch (tex) {
	case DefTexture::SLogo:
		mTexture = NotifMgr::sInstance->mLogo;
		break;
	case DefTexture::SNotif:
		mTexture = NotifMgr::sInstance->mNotif;
		break;
	case DefTexture::SCheckmark:
		mTexture = NotifMgr::sInstance->mCheckmark;
		break;
	case DefTexture::SCross:
		mTexture = NotifMgr::sInstance->mCross;
		break;
	case DefTexture::SWarn:
		mTexture = NotifMgr::sInstance->mWarn;
		break;
	};
}
void Notif::setTexture(std::shared_ptr<TexturePtr> texdata) {  // your own textures
	mTexture = texdata;
}
Notif::~Notif() {}
NotifMgr::NotifMgr() {
	//sInstance = this;
	// Textures should be 1:1 ratio
	mLogo = DrawUtils::resourceToTexturePtr(LOGO_DATA, "HorionLogo");
	mNotif = DrawUtils::resourceToTexturePtr(NOTIF_DATA, "Notification");
	mCheckmark = DrawUtils::resourceToTexturePtr(CHECK_DATA, "Checkmark");
	mCross = DrawUtils::resourceToTexturePtr(CROSS_DATA, "Cross");
	mWarn = DrawUtils::resourceToTexturePtr(WARN_DATA, "Warning");
}
void NotifMgr::addNotif(Notif *notif) {
	std::lock_guard g(this->notifMutex);
	mNotifications.push_back(notif);
}
Notif *NotifMgr::makeNotif(int time, MC_Color color) {
	Notif *notif = new Notif(time, color);
	this->addNotif(notif);
	return notif;
}
Notif *NotifMgr::makeNotifStatic(int time, MC_Color color) {
	return sInstance->makeNotif(time, color);
}
void NotifMgr::onCalc() {
	std::lock_guard g(this->notifMutex);
	auto it = mNotifications.begin();
	while(it < mNotifications.end()) {
		auto ita = *it;
		if (ita->mTimeLeft <= 0) {
			delete ita;
			it = mNotifications.erase(it);
			continue;
		}
		ita->mTimeLeft -= 1;
		if (ita->mTimeLeft < 20 or ita->mTotalTime - ita->mTimeLeft < 20) {
			ita->mAnim += (0.0f - ita->mAnim) * 0.2f;
		} else {
			ita->mAnim += (1.0f - ita->mAnim) * 0.2f;
		}
		it++;
	}
}
MC_Color NotifMgr::getBgColor(MC_Color og) {
	Utils::ColorConvertRGBtoHSV(og.r, og.g, og.b, og.r, og.g, og.b);
	og.b *= 50.0f / 100.0f;
	Utils::ColorConvertHSVtoRGB(og.r, og.g, og.b, og.r, og.g, og.b);
	//og.a = 0.95f;
	return og;
}
void NotifMgr::onCalcDraw() {
	mLogo = DrawUtils::resourceToTexturePtr(LOGO_DATA, "HorionLogo");
	mNotif = DrawUtils::resourceToTexturePtr(NOTIF_DATA, "Notification");
	mCheckmark = DrawUtils::resourceToTexturePtr(CHECK_DATA, "Checkmark");
	mCross = DrawUtils::resourceToTexturePtr(CROSS_DATA, "Cross");
	mWarn = DrawUtils::resourceToTexturePtr(WARN_DATA, "Warning");
	Vec2 windowSize = Game.getGuiData()->windowSize;
	float SCREEN_W = windowSize.x;
	float SCREEN_H = windowSize.y;
	int i = 0;
	const float mainScale = 0.6f; // maybe add as an option somewhere
	float nrmlTextScale = 12.0f * mainScale;
	float nameTextScale = 16.0f * mainScale;
	std::string pad = "K";
	float csmallpad = DrawUtils::getTextWidth(&pad, nrmlTextScale * 0.1f, DesiredUi::HUD);
	float cpadding = DrawUtils::getTextWidth(&pad, nameTextScale * 0.1f, DesiredUi::HUD);
	float h = nrmlTextScale * 1.2f + nameTextScale * 1.4f;
	float height = SCREEN_H - SCREEN_H * (115.0f / 1280.0f) - h - nrmlTextScale * 0.5f;  // initial offset from bottom
	float texhw = nrmlTextScale * 1.2f + nameTextScale;
	{
		std::lock_guard g(this->notifMutex);
		for (auto &ita : mNotifications) {
			if (height > SCREEN_H) {
				break;
			}
			MC_Color color = NotifMgr::getBgColor(ita->mBGColor);
			// color.a *= ita->mAnim;
			float time = ita->mTimeLeft / 60.f;
			char timeChar[50];
			sprintf_s(timeChar, " (%.1fs)", time);
			std::string timeStr = timeChar;
			std::string nameStr = ita->mName + timeStr;
			std::string textStr = ita->mText;
			float width = fmaxf(DrawUtils::getTextWidth(&nameStr, nameTextScale * 0.1f, DesiredUi::HUD), DrawUtils::getTextWidth(&textStr, nrmlTextScale * 0.1f, DesiredUi::HUD));
			float ogwidth = width + cpadding;
			width += 40.0f * mainScale + csmallpad * 3;
			Vec2 startpos = {SCREEN_W - width * ita->mAnim, height};
			DrawUtils::drawRoundRect({startpos.x, startpos.y, startpos.x + width, startpos.y + h}, CornerRadius(h * 0.2f, 0.f, h * 0.2f, 0.f), color);

			DrawUtils::SetColor(1.0f, 1.0f, 1.0f, ita->mAnim);
			DrawUtils::drawImageFromTexturePtr(ita->mTexture, {startpos.x + csmallpad + (3.3f * mainScale), startpos.y + nameTextScale * 0.2f + (3.3f * mainScale)}, {texhw - (6.6f * mainScale), texhw - (6.6f * mainScale)}, Vec2(0.f, 0.f), Vec2(1.f, 1.f));
			DrawUtils::flushImages();
			DrawUtils::drawText(Vec2(startpos.x + 40.0f * mainScale, height + nameTextScale * 1.2f + nrmlTextScale * 1.2f - nrmlTextScale),
								&textStr, ita->mTextColor, nrmlTextScale * 0.1f, 1.f, DesiredUi::HUD);

			DrawUtils::drawText(Vec2(startpos.x + 40.0f * mainScale, height + nameTextScale * 1.2f - nrmlTextScale * 0.2f - nrmlTextScale),
								&nameStr, ita->mNameColor, nameTextScale * 0.1f, 1.f, DesiredUi::HUD);

			height -= h * ita->mAnim + nrmlTextScale * 0.5f;
			i++;
		}
	}
	
	DrawUtils::flushImages(); // do i do it here idk?
	DrawUtils::Flush();
}