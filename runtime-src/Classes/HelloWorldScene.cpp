#include "HelloWorldScene.h"
#include "SelectSence.h"
#include "Sound.h"
#include "ShopScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto winSize = Director::getInstance()->getWinSize();

	auto _node = SceneReader::getInstance()->createNodeWithSceneFile("UIScene.json");
	if (_node)
	{
		this->addChild(_node);
	}
	
	auto uiRender = static_cast<ComRender*>(_node->getChildByTag(10014)->getComponent("mainUI"));

	auto uiWidget = static_cast<Widget*>(uiRender->getNode());

	auto btnStart = static_cast<Button*>(Helper::seekWidgetByName(uiWidget,"btnStart"));
	btnStart->addTouchEventListener(CC_CALLBACK_2(HelloWorld::StartTouchEventCallback,this));
	
	auto btn_help = static_cast<Button*>(Helper::seekWidgetByName(uiWidget, "btn_help"));
	btn_help->addTouchEventListener(CC_CALLBACK_2(HelloWorld::HelpTouchEventCallback, this));
	
	auto btn_sound = static_cast<Button*>(Helper::seekWidgetByName(uiWidget, "btn_sound"));
	btn_sound->addTouchEventListener(CC_CALLBACK_2(HelloWorld::SoundTouchEventCallback, this));
	if (!readBoolXML(MUSIC))
	{
		btn_sound->setBright(false);
	}
	auto btn_close = static_cast<Button*>(Helper::seekWidgetByName(uiWidget, "btn_close"));
	btn_close->addTouchEventListener(CC_CALLBACK_2(HelloWorld::CloseTouchEventCallback, this));


	auto vip = Button::create("vipIcon.png");
	vip->setAnchorPoint(Vec2(0.5f, 0.f));
	vip->addTouchEventListener(CC_CALLBACK_2(HelloWorld::VipTouchEventCallback, this));
	this->addChild(vip,10);
	vip->setPosition(Vec2(vip->getContentSize().width,vip->getContentSize().height));
	vip->runAction(RepeatForever::create(
		Sequence::create(
		ScaleTo::create(0.5f, 1.f, 0.7f)
		, Spawn::create(MoveBy::create(0.5f, Vec2(0.f, 30.f)), ScaleTo::create(0.5f, 1.f, 1.f), NULL)
		, DelayTime::create(0.2f)
		, Spawn::create(MoveBy::create(0.5f, Vec2(0.f, -30.f)), ScaleTo::create(0.5f, 1.f, 0.7f), NULL)
		, DelayTime::create(0.1f)
		, ScaleTo::create(0.5f, 1.f, 1.f)
		, DelayTime::create(0.5f)
		, NULL)
		));

	if (readBoolXML(MUSIC))
	{
		Sound::getInstance()->playMenuBackground();
	}
	saveBoolXML(ISNEWS, false);
	return true;
}

void HelloWorld::StartTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{	
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("StartTouchEventCallback");
		Sound::getInstance()->playBtn();
		Director::getInstance()->replaceScene(Select::createScene());	
	}
}

void HelloWorld::HelpTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{	
	
	if (type == Widget::TouchEventType::ENDED)
	{
		log("HelpTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		if (_news)
		{
			this->removeChild(_news);
			_news = NULL;
		}
		saveBoolXML(ISNEWS, true);
		_news = News::createNews(CC_CALLBACK_2(HelloWorld::OkTouchEventCallback, this), 0, !readBoolXML(GM));
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(1.f, Vec2(-800, 0)));
	}
}

void HelloWorld::SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type==Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("SoundTouchEventCallback");
		Sound::getInstance()->playBtn();
		auto button = (Button*)pSender;
		button->setBright(!button->isBright());
		if (readBoolXML(MUSIC))
		{
			saveBoolXML(MUSIC, false);
			Sound::getInstance()->stopBackground();
		}
		else
		{
			saveBoolXML(MUSIC, true);
			Sound::getInstance()->playMenuBackground();
		}
	}
		
}

void HelloWorld::CloseTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("CloseTouchEventCallback");
		Sound::getInstance()->playBtn();
		Director::getInstance()->end();
	}
}

void HelloWorld::OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (!readBoolXML(GM))
		{
			saveBoolXML(GM, true);
			for (int i = 1; i < 4; i++)
			{
				for (int j = 1; j < 19; j++)
				{
					char name[15] = { 0 };
					sprintf(name, LevelLock, i, j);
					saveBoolXML(name, false);
				}
			}
			saveBoolXML(SecondLevel, true);
			saveBoolXML(ThirdLevel, true);
		}
		_news->runAction(Spawn::create(
			MoveBy::create(1.f, Vec2(800.f, 0))
			, FadeOut::create(1.f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}

void HelloWorld::VipTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("VipTouchEventCallback");
		Sound::getInstance()->playBtn();
		Director::getInstance()->replaceScene(Shop::createScene());
	}
}
