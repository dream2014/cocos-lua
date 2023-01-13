#ifndef __GAME_OVER_SCENE_H_
#define __GAME_OVER_SCENE_H_
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Define.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
class GameOver:public Layer
{
public:
	static cocos2d::Scene* createScene(int level, int levelNum, int loseStep);
	static GameOver* create(int level, int levelNum, int loseStep);
	virtual bool init(int level, int levelNum, int loseStep);

	void HomeTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void RestartTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void NextTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
private:
	int _loseStep=-1;
	int m_level = -1;
	int m_levelNum = -1;
};
#endif
