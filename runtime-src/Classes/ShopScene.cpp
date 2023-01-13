#include "ShopScene.h"
#include "Sound.h"
#include "HelloWorldScene.h"
cocos2d::Scene* Shop::createScene()
{
	auto scene = Scene::create();
	auto layer = Shop::create();
	scene->addChild(layer);
	return scene;
}

bool Shop::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto _bgnode = GUIReader::getInstance()->widgetFromJsonFile("mainUi/HUD.ExportJson");
	this->addChild(_bgnode);
	auto _node = GUIReader::getInstance()->widgetFromJsonFile("mainUi/Shop_1.ExportJson");
	this->addChild(_node);

	auto Button_18 = static_cast<Button*>(Helper::seekWidgetByName(_node, "Button_18"));
	Button_18->addTouchEventListener(CC_CALLBACK_2(Shop::BackTouchEventCallback, this));

	auto btn_step = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_step"));
	btn_step->addTouchEventListener(CC_CALLBACK_2(Shop::StepTouchEventCallback, this));
	auto btn_key = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_key"));
	btn_key->addTouchEventListener(CC_CALLBACK_2(Shop::KeyTouchEventCallback, this));
	auto btn_game = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_game"));
	btn_game->addTouchEventListener(CC_CALLBACK_2(Shop::GameTouchEventCallback, this));
	auto btn_jb = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_jb"));
	btn_jb->addTouchEventListener(CC_CALLBACK_2(Shop::JbTouchEventCallback, this));

	saveBoolXML(ISNEWS, false);
	return true;
}

void Shop::BackTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("BackTouchEventCallback");
		Sound::getInstance()->playBtn();
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
}

void Shop::StepTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("StepTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (_news)
		{
			this->removeChild(_news);
			_news = NULL;
		}
		saveBoolXML(ISNEWS, true);
		_news = News::createNews(CC_CALLBACK_2(Shop::OkStepTouchEventCallback, this), 6, true);
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(0.5f, Vec2(-800, 0)));
	}
}

void Shop::KeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("KeyTouchEventCallback");
		Sound::getInstance()->playBtn();

		if (_news)
		{
			this->removeChild(_news);
			_news = NULL;
		}
		saveBoolXML(ISNEWS, true);
		_news = News::createNews(CC_CALLBACK_2(Shop::OkKeyTouchEventCallback, this), 5, true);
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(0.5f, Vec2(-800, 0)));

	}
}

void Shop::GameTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("GameTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (_news)
		{
			this->removeChild(_news);
			_news = NULL;
		}
		saveBoolXML(ISNEWS, true);
		_news = News::createNews(CC_CALLBACK_2(Shop::OkGameTouchEventCallback, this), 0, !readBoolXML(GM));
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(0.5f, Vec2(-800, 0)));
	}
}

void Shop::JbTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		log("JbTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (_news)
		{
			this->removeChild(_news);
			_news = NULL;
		}
		saveBoolXML(ISNEWS, true);
		_news = News::createNews(CC_CALLBACK_2(Shop::OkJbTouchEventCallback, this), 4, true);
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(0.5f, Vec2(-800, 0)));
	}
}

void Shop::OkStepTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkStepTouchEventCallback");
		Sound::getInstance()->playBtn();
		saveIntXML(STEP, readIntXML(STEP) + 5);
		_news->runAction(Spawn::create(
			MoveBy::create(0.5f, Vec2(800.f, 0))
			, FadeOut::create(0.5f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}

void Shop::OkKeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkKeyTouchEventCallback");
		Sound::getInstance()->playBtn();
		saveIntXML(SHOWKEY, readIntXML(SHOWKEY) + 5);
		_news->runAction(Spawn::create(
			MoveBy::create(0.5f, Vec2(800.f, 0))
			, FadeOut::create(0.5f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}

void Shop::OkGameTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkGameTouchEventCallback");
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
			MoveBy::create(0.5f, Vec2(800.f, 0))
			, FadeOut::create(0.5f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}

void Shop::OkJbTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkJbTouchEventCallback");
		Sound::getInstance()->playBtn();


		_news->runAction(Spawn::create(
			MoveBy::create(0.5f, Vec2(800.f, 0))
			, FadeOut::create(0.5f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}
