#include "LevelScene.h"
#include "SelectSence.h"
#include "GameScene.h"
#include "Sound.h"
cocos2d::Scene* Level::createScene(int level)
{
	auto scene = Scene::create();
	auto layer = Level::create(level);
	scene->addChild(layer);
	return scene;
}

Level* Level::create(int level)
{
	auto L = new Level;
	if (L&&L->init(level))
	{
		L->autorelease();
		return L;
	}
	CC_SAFE_DELETE(L);
	return NULL;
}

bool Level::init(int level)
{
	if (!Layer::init())
	{
		return false;
	}

	m_level = level;

	auto _node = GUIReader::getInstance()->widgetFromJsonFile("mainUi/selectLevel.ExportJson");
	this->addChild(_node);

	for (int i = 1; i < 19;i++)
	{
		char name[20] = { 0 };
		sprintf(name, "btn_%d", i);
		auto button= static_cast<Button*>(Helper::seekWidgetByName(_node, name));
		button->addTouchEventListener(CC_CALLBACK_2(Level::ButtonTouchEventCallback, this));
		auto img_lock = static_cast<ImageView*>(button->getChildByName("img_lock"));
		sprintf(name, LevelLock, m_level, i);
		img_lock->setVisible(readBoolXML(name));
		levelButton.insert(name, img_lock);
		auto img_bei = static_cast<ImageView*>(button->getChildByName("img_bei"));
		sprintf(name, LevelBei, m_level, i);
		img_bei->setVisible(readBoolXML(name));
		
	}

	auto btn_back = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_back"));
	btn_back->addTouchEventListener(CC_CALLBACK_2(Level::BackTouchEventCallback,this));

	auto btn_help=static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_help"));
	btn_help->addTouchEventListener(CC_CALLBACK_2(Level::HelpTouchEventCallback, this));

	auto btn_sound = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_sound"));
	btn_sound->addTouchEventListener(CC_CALLBACK_2(Level::SoundTouchEventCallback, this));
	if (!readBoolXML(MUSIC))
	{
		btn_sound->setBright(false);
	}

	saveBoolXML(ISNEWS, false);
	return true;
}

void Level::ButtonTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{	
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		auto button = dynamic_cast<Button*>(pSender);
		auto name = button->getName();
		if (sscanf(name.c_str(), "btn_%d", &m_levelNum) == 1)
		{
			char level[20] = { 0 };
			sprintf(level, LevelLock, m_level, m_levelNum);
			//log(level);
			if (!readBoolXML(level))
			{
				Director::getInstance()->replaceScene(Game::createScene(m_level, m_levelNum));
			}			
		}		
	}
}

void Level::BackTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("BackTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		Director::getInstance()->replaceScene(Select::createScene());
	}
}

void Level::HelpTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
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
		
		_news = News::createNews(CC_CALLBACK_2(Level::OkTouchEventCallback, this), 2, getLock()!=0?true:false);
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(1.f, Vec2(-800, 0)));
	}
}

void Level::SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
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

void Level::OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkTouchEventCallback");
		Sound::getInstance()->playBtn();
		auto lockNum = getLock();
		if (lockNum>0)
		{			
			char level[20] = { 0 };
			sprintf(level, LevelLock,m_level, lockNum);
			saveBoolXML(level, false);
			auto lock = levelButton.at(level);
			lock->setVisible(readBoolXML(level));
		}
		
		_news->runAction(Spawn::create(
			MoveBy::create(1.f, Vec2(800.f, 0))
			, FadeOut::create(1.f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}

int Level::getLock()
{
	for (int i = 1; i < 19;i++)
	{
		char level[20] = { 0 };
		sprintf(level, LevelLock, m_level,i);
		if (readBoolXML(level))
		{
			return i;
		}
	}
	return 0;
}
