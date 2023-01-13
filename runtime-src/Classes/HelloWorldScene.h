#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#include "GameNews.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();  
   
	void StartTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void HelpTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void CloseTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void VipTouchEventCallback(Ref *pSender, Widget::TouchEventType type);


	void OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

    CREATE_FUNC(HelloWorld);
private:
	News* _news;
};

#endif // __HELLOWORLD_SCENE_H__
