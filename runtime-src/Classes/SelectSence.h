#ifndef __SELECT_SENCE_H__
#define __SELECT_SENCE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Define.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;
#include "GameNews.h"

class Select:public Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void FirstStartTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void SecondStartTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void ThirdStartTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void HelpTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void BackTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	CREATE_FUNC(Select);

private:
	News* _news;
	TextAtlas* lab_second;
	TextAtlas* lab_third;
	ImageView* img_dark_second;
	ImageView* img_second_lock;
	ImageView* img_dark_third;
	ImageView* img_third_lock;
};
#endif