#include "GameOverScene.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
#include "Sound.h"
#include "scripting/deprecated/CCDeprecated.h"
cocos2d::Scene* GameOver::createScene(int level, int levelNum, int loseStep)
{
	auto scene = Scene::create();
	auto layer = GameOver::create(level, levelNum,loseStep);
	scene->addChild(layer);
	return scene;
}

GameOver* GameOver::create(int level, int levelNum, int loseStep)
{
	auto gameOver = new GameOver;
	if (gameOver&&gameOver->init(level,levelNum,loseStep))
	{
		gameOver->autorelease();
		return gameOver;
	}
	CC_SAFE_DELETE(gameOver);
	return NULL;
}

bool GameOver::init(int level, int levelNum, int loseStep)
{
	if (!Layer::init())
	{
		return false;
	}
	_loseStep = loseStep;
	m_level = level;
	m_levelNum = levelNum;
	auto _bgnode = GUIReader::getInstance()->widgetFromJsonFile("mainUi/HUD.ExportJson");
	this->addChild(_bgnode);
	auto _node = GUIReader::getInstance()->widgetFromJsonFile("mainUi/endMenu.ExportJson");
	this->addChild(_node);

	auto btn_home = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_home"));
	btn_home->addTouchEventListener(CC_CALLBACK_2(GameOver::HomeTouchEventCallback, this));

	auto btn_reStart = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_reStart"));
	btn_reStart->addTouchEventListener(CC_CALLBACK_2(GameOver::RestartTouchEventCallback, this));

	auto btn_next = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_next"));
	btn_next->addTouchEventListener(CC_CALLBACK_2(GameOver::NextTouchEventCallback, this));

	auto lab_endTxt = static_cast<Text*>(Helper::seekWidgetByName(_node, "lab_endTxt"));
	lab_endTxt->setColor(Color3B::MAGENTA);
	log("FontName==========%s", lab_endTxt->getFontName().c_str());
	auto img_err = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_err"));
	auto img_err1 = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_err1"));
	auto img_lost_bei = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_lost_bei"));
	auto img_get_bei = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_get_bei"));
	if (_loseStep<0)
	{
		Sound::getInstance()->playNoGetBei();
		img_get_bei->setVisible(false);
		auto dictionary = Dictionary::createWithContentsOfFile("text/gamefail.xml");
		auto nameArr = static_cast<__Array*>(dictionary->objectForKey("loseGame"));
		auto levelname = static_cast<__String*>(nameArr->getRandomObject());
		lab_endTxt->setString(levelname->getCString());
		img_lost_bei->runAction(MoveBy::create(0.2f, Vec2(0, 70.f)));
	}
	else
	{
		Sound::getInstance()->playGetBei();
		img_err->setVisible(false);
		img_err1->setVisible(false);
		img_lost_bei->setVisible(false);
		auto dictionary = Dictionary::createWithContentsOfFile("text/gamewin.xml");
		char key[5] = { 0 };
		sprintf(key, "%d-%d", m_level, m_levelNum);
		auto levelname = dynamic_cast<__String*>(dictionary->objectForKey(key));
		lab_endTxt->setString(levelname->getCString());
		img_get_bei->runAction(MoveBy::create(0.2f, Vec2(0, 70.f)));
		
	}

	if (readBoolXML(MUSIC))
	{
		Sound::getInstance()->playMenuBackground();
	}

	return true;
}

void GameOver::HomeTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("HomeTouchEventCallback");
		Sound::getInstance()->playBtn();
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
}

void GameOver::RestartTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("RestartTouchEventCallback");
		Sound::getInstance()->playBtn();
		Director::getInstance()->replaceScene(Game::createScene(m_level, m_levelNum));
	}
}

void GameOver::NextTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("NextTouchEventCallback");
		Sound::getInstance()->playBtn();
		char name[15] = { 0 };
		if (m_levelNum>=18&&m_level<3)
		{
			sprintf(name, LevelLock, m_level+1, m_levelNum+1);
			if (!readBoolXML(name))
			{
				m_level += 1;
				m_levelNum = 1;
				Director::getInstance()->replaceScene(Game::createScene(m_level, m_levelNum));
			}
			else
			{
				auto button = (Button*)pSender;
				button->setBright(false);
			}
		}
		else if (m_levelNum<18)
		{
			sprintf(name, LevelLock, m_level, m_levelNum + 1);
			if (!readBoolXML(name))
			{
				m_levelNum += 1;
				Director::getInstance()->replaceScene(Game::createScene(m_level, m_levelNum));
			}
			else
			{
				auto button = (Button*)pSender;
				button->setBright(false);
			}
		}	
	}
}
