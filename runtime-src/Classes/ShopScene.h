#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Define.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace std;
#include "GameNews.h"
class Shop:public Layer
{
public:
	static cocos2d::Scene* createScene();
	bool init();
	CREATE_FUNC(Shop);

	void BackTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void StepTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void OkStepTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void KeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void OkKeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void GameTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void OkGameTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void JbTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void OkJbTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

private:
	News* _news;
};
#endif