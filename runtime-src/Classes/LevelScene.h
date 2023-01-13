#ifndef __LEVEL_SCENE_H__
#define __LEVEL_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Define.h"
#include "GameNews.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;
class Level:public Layer
{
public:
	static cocos2d::Scene* createScene(int level);
	static Level* create(int level);
	virtual bool init(int level);
	void BackTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void ButtonTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void HelpTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	int getLock();
private:
	Map<std::string, ImageView*> levelButton;
	int m_level=-1;
	int m_levelNum = -1;
	News* _news;
};
#endif