#include "SelectSence.h"
#include "HelloWorldScene.h"
#include "LevelScene.h"
#include "Sound.h"
cocos2d::Scene* Select::createScene()
{
	auto scene = Scene::create();
	auto layer = Select::create();
	scene->addChild(layer);
	return scene;
}

bool Select::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	auto _node = GUIReader::getInstance()->widgetFromJsonFile("mainUi/selectSence.ExportJson");
	this->addChild(_node);

	auto btn_frist_start = static_cast<Button*>(Helper::seekWidgetByName(_node,"btn_frist_start"));
	btn_frist_start->addTouchEventListener(CC_CALLBACK_2(Select::FirstStartTouchEventCallback, this));

	auto btn_back = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_back"));
	btn_back->addTouchEventListener(CC_CALLBACK_2(Select::BackTouchEventCallback, this));

	auto btn_help = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_help"));
	btn_help->addTouchEventListener(CC_CALLBACK_2(Select::HelpTouchEventCallback, this));

	auto btn_sound = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_sound"));
	btn_sound->addTouchEventListener(CC_CALLBACK_2(Select::SoundTouchEventCallback,this));
	if (!readBoolXML(MUSIC))
	{
		btn_sound->setBright(false);
	}
	auto btn_second_start = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_second_start"));
	btn_second_start->addTouchEventListener(CC_CALLBACK_2(Select::SecondStartTouchEventCallback, this));

	auto btn_third_start = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_third_start"));
	btn_third_start->addTouchEventListener(CC_CALLBACK_2(Select::ThirdStartTouchEventCallback, this));

	auto jBNum = readIntXML(JBeiNum);
	
	if (jBNum>=10)
	{
		if (!readBoolXML(SecondLevel))
		{
			saveBoolXML(SecondLevel, true);
			saveBoolXML("levelLock2_1", false);
		}
	}
	if (jBNum>=20)
	{
		if (!readBoolXML(ThirdLevel))
		{
			saveBoolXML(ThirdLevel, true);
			saveBoolXML("levelLock3_1", false);
		}
	}

	lab_second = static_cast<TextAtlas*>(Helper::seekWidgetByName(_node, "lab_second"));
	lab_second->setString(String::createWithFormat("%d", jBNum > 10 ? 10 : jBNum)->getCString());
	lab_third = static_cast<TextAtlas*>(Helper::seekWidgetByName(_node, "lab_third"));
	lab_third->setString(String::createWithFormat("%d", jBNum > 20 ? 20 : jBNum)->getCString());

	img_dark_second = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_dark_second"));
	img_dark_second->setVisible(!readBoolXML(SecondLevel));
	img_second_lock = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_second_lock"));
	img_second_lock->setVisible(!readBoolXML(SecondLevel));

	img_dark_third = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_dark_third"));
	img_dark_third->setVisible(!readBoolXML(ThirdLevel));
	img_third_lock = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_third_lock"));
	img_third_lock->setVisible(!readBoolXML(ThirdLevel));

	saveBoolXML(ISNEWS, false);
	return true;
}

void Select::FirstStartTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("FirstStartTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		Director::getInstance()->replaceScene(Level::createScene(1));
	}
}

void Select::BackTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("FirstStartTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
}

void Select::HelpTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
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
		_news = News::createNews(CC_CALLBACK_2(Select::OkTouchEventCallback, this), 1, true);
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(1.f, Vec2(-800, 0)));

	}
}

void Select::SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("SoundTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
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

void Select::ThirdStartTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("ThirdStartTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		
		if (readBoolXML(ThirdLevel))
		{
			Sound::getInstance()->playBtn();
			Director::getInstance()->replaceScene(Level::createScene(3));
		}		
	}
}

void Select::SecondStartTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("ScondStartTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}		
		if (readBoolXML(SecondLevel))
		{
			Sound::getInstance()->playBtn();
			Director::getInstance()->replaceScene(Level::createScene(2));
		}
	}
}

void Select::OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkTouchEventCallback");
		Sound::getInstance()->playBtn();
		auto jbnum = readIntXML(JBeiNum);
		saveIntXML(JBeiNum, jbnum + 1);
		auto jBNum = readIntXML(JBeiNum);
		if (jBNum >= 10)
		{
			if (!readBoolXML(SecondLevel))
			{
				saveBoolXML(SecondLevel, true);
				saveBoolXML("levelLock2_1", false);
			}
		}
		if (jBNum >= 20)
		{
			if (!readBoolXML(ThirdLevel))
			{
				saveBoolXML(ThirdLevel, true);
				saveBoolXML("levelLock3_1", false);
			}
		}
		lab_second->setString(String::createWithFormat("%d", jBNum > 10 ? 10 : jBNum)->getCString());
		lab_third->setString(String::createWithFormat("%d", jBNum > 20 ? 20 : jBNum)->getCString());
		img_third_lock->setVisible(!readBoolXML(ThirdLevel));
		img_dark_third->setVisible(!readBoolXML(ThirdLevel));
		img_dark_second->setVisible(!readBoolXML(SecondLevel));
		img_second_lock->setVisible(!readBoolXML(SecondLevel));

		_news->runAction(Spawn::create(
			MoveBy::create(1.f, Vec2(800.f, 0))
			, FadeOut::create(1.f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}
