#include "GameScene.h"
#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "Sound.h"
#include "scripting/deprecated/CCDeprecated.h"
#define MapWidth 64
#define MapHeight 32
cocos2d::Scene* Game::createScene(int level, int levelNum)
{
	auto scene = Scene::create();
	auto layer = Game::create(level,levelNum);
	scene->addChild(layer);
	return scene;
}

Game* Game::create(int level, int levelNum)
{
	auto game = new Game;
	if (game&&game->init(level,levelNum))
	{
		game->autorelease();
		return game;
	}
	CC_SAFE_DELETE(game);
	return NULL;
}

bool Game::init(int level, int levelNum)
{
	if (!Layer::init())
	{
		return false;
	}
	m_level = level;
	m_levelNum = levelNum;

	ArmatureDataManager::getInstance()->addSpriteFrameFromFile("palyer_stand/palyer_stand0.plist", "palyer_stand/palyer_stand0.png", "palyer_stand/palyer_stand.ExportJson");
	
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Item/Item0.png", "Item/Item0.plist", "Item/Item.ExportJson");
	
	auto winSize = Director::getInstance()->getWinSize();
	auto _bgnode = GUIReader::getInstance()->widgetFromJsonFile("mainUi/HUD.ExportJson");
	this->addChild(_bgnode);
	auto _node = GUIReader::getInstance()->widgetFromJsonFile("mainUi/hud_fg.ExportJson");
	this->addChild(_node);

	//Button
	auto btn_pause = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_pause"));
	btn_pause->addTouchEventListener(CC_CALLBACK_2(Game::PauseTouchEventCallback, this));
	//btn_pause->setVisible(false);
	auto btn_home = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_home"));
	btn_home->addTouchEventListener(CC_CALLBACK_2(Game::HomeTouchEventCallback, this));

	auto btn_sound = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_sound"));
	btn_sound->addTouchEventListener(CC_CALLBACK_2(Game::SoundTouchEventCallback, this));
	if (!readBoolXML(MUSIC))
	{
		btn_sound->setBright(false);
	}
	auto btn_restart = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_restart"));
	btn_restart->addTouchEventListener(CC_CALLBACK_2(Game::RestartTouchEventCallback, this));

	btn_left = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_left"));
	btn_left->addTouchEventListener(CC_CALLBACK_2(Game::LeftTouchEventCallback,this));

	btn_right = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_right"));
	btn_right->addTouchEventListener(CC_CALLBACK_2(Game::RightTouchEventCallback, this));

	//auto btn_tile = static_cast<Button*>(Helper::seekWidgetByName(_node, "btn_tile"));
	auto lab_levelname = static_cast<Text*>(Helper::seekWidgetByName(_node, "lab_levelname"));
	auto dictionary = Dictionary::createWithContentsOfFile("text/levelname.xml");
	char key[5] = { 0 };
	sprintf(key, "%d-%d", m_level, m_levelNum);
	auto levelname = dynamic_cast<__String*>(dictionary->objectForKey(key));
	lab_levelname->setString(levelname->getCString());

	auto lab_lv = static_cast<TextAtlas*>(Helper::seekWidgetByName(_node, "lab_lv"));
	lab_lv->setString(StringUtils::format("%d", m_levelNum));
	auto lab_sence = static_cast<TextAtlas*>(Helper::seekWidgetByName(_node, "lab_sence"));
	lab_sence->setString(StringUtils::format("%d", m_level));
	
	img_no_bei = static_cast<ImageView*>(Helper::seekWidgetByName(_node, "img_no_bei"));
	img_no_bei->setVisible(false);

	bar_step = static_cast<LoadingBar*>(Helper::seekWidgetByName(_node, "bar_step"));
	lab_setp = static_cast<TextAtlas*>(Helper::seekWidgetByName(_node, "lab_setp"));

	loadMap();



	auto addStep = Button::create("addStep.png");
	addStep->addTouchEventListener(CC_CALLBACK_2(Game::AddStepTouchEventCallback,this));
	this->addChild(addStep);
	addStep->setPosition(Vec2(addStep->getContentSize().width,winSize.height-2*addStep->getContentSize().height));
	auto showKey = Button::create("showKey.png");
	showKey->addTouchEventListener(CC_CALLBACK_2(Game::ShowKeyTouchEventCallback, this));
	this->addChild(showKey);
	showKey->setPosition(Vec2(winSize.width-showKey->getContentSize().width,addStep->getPositionY()));

	if (readBoolXML(MUSIC))
	{
		Sound::getInstance()->playGameBackground();
	}

	this->scheduleUpdate();
	this->schedule(CC_SCHEDULE_SELECTOR(Game::dropUpdate), 0.12f);
	this->schedule(CC_SCHEDULE_SELECTOR(Game::openBZUpdate), 0.02f);
	this->schedule(CC_SCHEDULE_SELECTOR(Game::faZhenUpdate), 0.08f);
	return true;
}

void Game::loadMap()
{
	//Map
	if (m_map!=NULL)
	{
		m_map->removeAllChildren();
		this->removeChild(m_map);
		m_map = NULL;
		_faZhenOpen = true;
		_isBright = true;
		muTMove = false;
		_gameFail = false;
		_gameOver = false;
		_isRotate = false;
		_isHaveKey = false;
		_attack = false;
		btn_left->setBright(_isBright);
		btn_right->setBright(_isBright);

		wallArr.clear();
		ddArr.clear();
		fangArr.clear();
		enemyArr.clear();
		boxArr.clear();
		banziUArr.clear();
		banziDArr.clear();
		banziLArr.clear();
		banziRArr.clear();
		mutiaoLArr.clear();
		if (img_no_bei->isVisible())
		{
			img_no_bei->setVisible(false);
		}		
	}
	auto winSize = Director::getInstance()->getWinSize();
	char name[20] = { 0 };
	sprintf(name, "map/lv_%d_%d.tmx", m_level, m_levelNum);
	m_map = TMXTiledMap::create(name);
	m_map->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->addChild(m_map);
	m_map->setPosition(winSize.width / 2, winSize.height / 2);

	m_mapSize = m_map->getMapSize();
	mapRect = Rect(0, 0, m_mapSize.width*MapWidth, m_mapSize.height*MapWidth);
	//log("mapSize.width==%f,mapSize.height==%f", m_mapSize.width, m_mapSize.height);

	auto dict = m_map->getProperties();
	maxStep = dict["maxStep"].asInt();
	loseStep = maxStep;
	lab_setp->setString(StringUtils::format("%d", maxStep));
	bar_step->setPercent(100.f);
	auto objGroup = m_map->getObjectGroup("obj1");
	auto arrGroup = objGroup->getObjects();
	for (auto obj : arrGroup)
	{
		auto objInfo = obj.asValueMap();
		auto name = objInfo.at("name").asString();
		auto x = objInfo["x"].asFloat();
		auto y = objInfo["y"].asFloat();
		auto width = objInfo["width"].asFloat();
		auto height = objInfo["height"].asFloat();
		if (strcmp(name.c_str(), "player") == 0)
		{
			Hero = Armature::create("palyer_stand");
			Hero->getAnimation()->play("stand");
			Hero->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Game::OnHeroArmatureCallBack, this));
			m_map->addChild(Hero, MoveZorder);
			Hero->setPosition(x + width / 2, y + height / 2);
			log("Hero.x=%f,Hero.y=%f", x + width / 2, y + height / 2);
			log("Hero.x=%f,Hero.y=%f", Hero->getPositionX(), Hero->getPositionY());

		}
		if (strcmp(name.c_str(), "key") == 0)
		{
			Key = Armature::create("Item");
			Key->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Game::OnKeyArmatureCallBack, this));
			Key->getAnimation()->play("key");
			m_map->addChild(Key, StaticZorder - 1);
			Key->setPosition(x + width / 2, y + height / 2);
		}
		if (strcmp(name.c_str(), "door") == 0)
		{
			Door = Armature::create("Item");
			Door->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Game::OnDoorArmatureCallBack, this));
			Door->getAnimation()->play("door");
			m_map->addChild(Door, StaticZorder - 1);
			Door->setPosition(x + width / 2, y + height / 2);
		}
		if (strcmp(name.c_str(), "wall") == 0)
		{
			Wall = Rect(x, y, width, height);
			wallArr.push_back(Wall);
		}
		if (strcmp(name.c_str(), "ding") == 0 || strcmp(name.c_str(), "dong") == 0)
		{
			Wall = Rect(x, y, width, height);
			ddArr.push_back(Wall);
		}
		if (strcmp(name.c_str(), "torches") == 0)
		{
			auto torches = Armature::create("Item");
			torches->getAnimation()->play("torches");
			m_map->addChild(torches, StaticZorder - 1);
			torches->setPosition(x + width / 2, y + height / 2);
		}
		if (strcmp(name.c_str(), "banzi") == 0)
		{
			if (objInfo.size() == 8)
			{
				auto banzi = Sprite::create("panel_l.png");
				banzi->setAnchorPoint(Vec2(0.f, 0.5f));
				m_map->addChild(banzi);
				banzi->setPosition(x, y + height / 2);
				banziDArr.pushBack(banzi);
			}
			else if (objInfo.size() == 7)
			{
				auto banzi = Sprite::create("panel.png");
				banzi->setAnchorPoint(Vec2(1.f, 0.5f));
				m_map->addChild(banzi);
				banzi->setPosition(x + width, y + height / 2);
				banziUArr.pushBack(banzi);
			}

		}
		if (strcmp(name.c_str(), "banziL") == 0)
		{
			if (objInfo.size() == 8)
			{
				auto banzi = Sprite::create("panel_s.png");
				banzi->setAnchorPoint(Vec2(0.5f, 1.f));
				m_map->addChild(banzi);
				banzi->setPosition(x + width / 2, y + height);
				banziLArr.pushBack(banzi);
			}
			else if (objInfo.size() == 7)
			{
				auto banzi = Sprite::create("panel_x.png");
				banzi->setAnchorPoint(Vec2(0.5f, 0.f));
				m_map->addChild(banzi);
				banzi->setPosition(x + width / 2, y);
				banziRArr.pushBack(banzi);
			}
		}
		if (strcmp(name.c_str(), "enemy") == 0)
		{
			auto enemy = Armature::create("Item");
			enemy->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(Game::OnEnemyArmatureCallBack, this));
			enemy->getAnimation()->play("stand");
			m_map->addChild(enemy, StaticZorder);
			enemy->setPosition(x + width / 2, y + height / 2);
			enemyArr.pushBack(enemy);
		}
		if (strcmp(name.c_str(), "box") == 0)
		{
			auto box = Armature::create("Item");
			box->getAnimation()->play("box");
			m_map->addChild(box, StaticZorder);
			box->setPosition(x + width / 2, y + height / 2);
			boxArr.pushBack(box);
		}
		if (strcmp(name.c_str(), "mutiaoL") == 0)
		{
			auto mutiaoL = Sprite::create("mutiao_s.png");
			mutiaoL->setName("L");
			m_map->addChild(mutiaoL, StaticZorder);
			mutiaoL->setPosition(x + width / 2, y + height / 2);
			mutiaoLArr.pushBack(mutiaoL);
		}
		if (strcmp(name.c_str(), "mutiao") == 0)
		{
			auto mutiaoL = Sprite::create("mutiao.png");
			mutiaoL->setName("S");
			m_map->addChild(mutiaoL, StaticZorder);
			mutiaoL->setPosition(x + width / 2, y + height / 2);
			mutiaoLArr.pushBack(mutiaoL);
		}
		if (strcmp(name.c_str(), "fang") == 0)
		{
			Wall = Rect(x, y, width, height);
			fangArr.push_back(Wall);
		}
		if (strcmp(name.c_str(), "fazhen_A") == 0)
		{
			fazhenA = Armature::create("Item");
			fazhenA->getAnimation()->play("faZhen");
			m_map->addChild(fazhenA, StaticZorder - 1);
			fazhenA->setPosition(x + width / 2, y + height / 2);

		}
		if (strcmp(name.c_str(), "fazhen_B") == 0)
		{
			fazhenB = Armature::create("Item");
			fazhenB->getAnimation()->play("faZhen");
			m_map->addChild(fazhenB, StaticZorder - 1);
			fazhenB->setPosition(x + width / 2, y + height / 2);

		}
	}
	if (enemyArr.size() > 0)
	{
		Key->setVisible(false);
	}
	saveBoolXML(ISNEWS, false);
}

void Game::faZhenUpdate(float dt)
{
	if (fazhenA == NULL || fazhenB == NULL || _gameFail || _gameOver || _attack)
	{
		return;
	}
	if (!isWall(getMapId(fazhenA->getPosition())) && !isWall(getMapId(fazhenB->getPosition())) && !_faZhenOpen)
	{
		_faZhenOpen = true;
	}
	if (!_faZhenOpen)
	{
		return;
	}
	setFaZhen(Hero);
	for (auto enemy:enemyArr)
	{
		setFaZhen(enemy);	
	}
	for (auto box:boxArr)
	{
		setFaZhen(box);
	}
}


void Game::update(float dt)
{
	if (_gameFail || _gameOver || _attack)
	{
		if (_isBright)
		{
			_isBright = false;
			btn_left->setBright(_isBright);
			btn_right->setBright(_isBright);
		}
		return;
	}

	if (!_isHaveKey&&!Key->isVisible())
	{
		if (enemyArr.size() <= 0)
		{
			Key->setVisible(true);
		}
	}

	if (!_isHaveKey&&Key&&Key->isVisible() && Key->getBoundingBox().intersectsRect(getCollisionRect(Hero->getPosition())))
	{
		Sound::getInstance()->playGetKey();
		_isHaveKey = true;
		Door->getAnimation()->play("doorCanIn");
		Key->getAnimation()->play("keyDisappear");
	}
	for (auto dd : ddArr)
	{
		for (auto enemy : enemyArr)
		{
			if (dd.intersectsRect(getCollisionRect(enemy->getPosition())))
			{
				Sound::getInstance()->playDead();
				enemy->stopAllActions();
				enemy->getAnimation()->play("enemyDisappear");
				enemyArr.eraseObject(enemy);
				break;
			}
		}
	}
	for (auto enemy:enemyArr)
	{
		if (enemy->getPosition().equals(Hero->getPosition()))
		{
			Sound::getInstance()->playAttack();
			Hero->getAnimation()->play("attack");
			enemy->getAnimation()->play("strike");
			_attack = true;
			break;
		}
	}
	for (auto dd : ddArr)
	{
		if (dd.intersectsRect(getCollisionRect(Hero->getPosition())))
		{
			Sound::getInstance()->playDead();
			_gameFail = true;
			Hero->stopAllActions();
			Hero->getAnimation()->play("playerDisappear");
			break;
		}
	}
	if (!mapRect.containsPoint(Hero->getPosition()))
	{
		Sound::getInstance()->playDead();
		_gameFail = true;
		Hero->stopAllActions();
		Hero->getAnimation()->play("playerDisappear");
	}
	for (auto enemy:enemyArr)
	{
		if (!mapRect.containsPoint(enemy->getPosition()))
		{
			Sound::getInstance()->playDead();
			enemyArr.eraseObject(enemy);
			enemy->stopAllActions();
			enemy->getAnimation()->play("enemyDisappear");
			break;
		}
	}
	for (auto mutiao:mutiaoLArr)
	{
		if (setMuTiao())
		{
			if (strcmp(mutiao->getName().c_str(), "S") && !muTMove)
			{
				Sound::getInstance()->playDoorOpen();
				muTMove = true;
				mutiao->runAction(MoveBy::create(0.1f, Vec2(0, MapWidth)));
			}
			else if (strcmp(mutiao->getName().c_str(), "L") && !muTMove)
			{
				Sound::getInstance()->playDoorOpen();
				muTMove = true;
				mutiao->runAction(MoveBy::create(0.1f, Vec2(MapWidth,0)));
			}
		}
		else
		{
			if (strcmp(mutiao->getName().c_str(), "S") && muTMove)
			{
				Sound::getInstance()->playDoorClose();
				muTMove = false;
				mutiao->runAction(MoveBy::create(0.1f, Vec2(0, -MapWidth)));
			}
			else if (strcmp(mutiao->getName().c_str(), "L") && muTMove)
			{
				Sound::getInstance()->playDoorClose();
				muTMove = false;
				mutiao->runAction(MoveBy::create(0.1f, Vec2(-MapWidth, 0)));
			}
		}
	}
}

void Game::openBZUpdate(float dt)
{
	if (_isRotate||_gameFail || _gameOver)
	{
		return;
	}

	for (auto banziU : banziUArr)
	{
		auto banziId = getMapId(banziU->getPosition());
		banziId = banziId - Vec2(1, 0);
		if (isBanZiOpen(banziId) && banziU->getRotation() == 0.f)
		{
			Sound::getInstance()->playDoorOpen();
			banziU->runAction(RotateBy::create(0.1f, 90.f));
		}
		else if (!isBanZiOpen(banziId) && banziU->getRotation() == 90.f)
		{
			Sound::getInstance()->playDoorClose();
			banziU->runAction(RotateBy::create(0.1f, -90.f));
		}
	}
	for (auto banziD : banziDArr)
	{
		auto banziId = getMapId(banziD->getPosition());
		//banziId = banziId - Vec2(1, 0);
		if (isBanZiOpen(banziId) && banziD->getRotation() == 0.f)
		{
			Sound::getInstance()->playDoorOpen();
			banziD->runAction(RotateBy::create(0.1f, 90.f));
		}
		else if (!isBanZiOpen(banziId) && banziD->getRotation() == 90.f)
		{
			Sound::getInstance()->playDoorClose();
			banziD->runAction(RotateBy::create(0.1f, -90.f));
		}
	}
	for (auto banziL : banziLArr)
	{
		auto banziId = getMapId(banziL->getPosition());
		banziId = banziId - Vec2(0, 1);
		if (isBanZiOpen(banziId) && banziL->getRotation() == 0.f)
		{
			Sound::getInstance()->playDoorOpen();
			banziL->runAction(RotateBy::create(0.1f, 90.f));
		}
		else if (!isBanZiOpen(banziId) && banziL->getRotation() == 90.f)
		{
			Sound::getInstance()->playDoorClose();
			banziL->runAction(RotateBy::create(0.1f, -90.f));
		}
	}
	for (auto banziR : banziRArr)
	{
		auto banziId = getMapId(banziR->getPosition());
		//banziId = banziId - Vec2(0, 1);
		if (isBanZiOpen(banziId) && banziR->getRotation() == 0.f)
		{
			Sound::getInstance()->playDoorOpen();
			banziR->runAction(RotateBy::create(0.1f, 90.f));
		}
		else if (!isBanZiOpen(banziId) && banziR->getRotation() == 90.f)
		{
			Sound::getInstance()->playDoorClose();
			banziR->runAction(RotateBy::create(0.1f, -90.f));
		}
	}
}

void Game::HomeTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("HomeTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		Director::getInstance()->replaceScene(HelloWorld::createScene());
	}
}

void Game::PauseTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("PauseTouchEventCallback");
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
		_news = News::createNews(CC_CALLBACK_2(Game::OkTouchEventCallback, this), 3, loseStep<maxStep?true:false);
		this->addChild(_news, 100);
		_news->setPosition(800.f, 0);
		_news->runAction(MoveBy::create(1.f, Vec2(-800, 0)));
	}
}

void Game::SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
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
			Sound::getInstance()->playGameBackground();
		}
	}
}

void Game::LeftTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type==Widget::TouchEventType::ENDED)
	{
		log("LeftTouchEventCallback");
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		if (_isRotate || _attack || _gameOver || _gameFail||!_isBright)
		{
			return;
		}
		Sound::getInstance()->playBtnLeft();
		_isRotate = true;
		_isBright = false;
		btn_left->setBright(_isBright);
		btn_right->setBright(_isBright);
		m_map->runAction(Sequence::create(
			RotateBy::create(0.5f, -90)
			,CallFunc::create(CC_CALLBACK_0(Game::LeftCallback,this))
			,NULL)
			);
		setBar();
	}
}

void Game::RightTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("RightTouchEventCallback");
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		if (_isRotate || _attack || _gameOver || _gameFail || !_isBright)
		{
			return;
		}
		Sound::getInstance()->playBtnRight();
		_isRotate = true;
		_isBright = false;
		btn_left->setBright(_isBright);
		btn_right->setBright(_isBright);
		m_map->runAction(Sequence::create(
			RotateBy::create(0.5f, 90)
			, CallFunc::create(CC_CALLBACK_0(Game::RightCallback, this))
			, NULL)
			);
		setBar();
	}
}

void Game::RestartTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("RestartTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (readBoolXML(ISNEWS))
		{
			return;
		}
		if( _gameOver || _gameFail)
		{
			return;
		}
		loadMap();
	}
}

void Game::setBar()
{
	if (loseStep<0)
	{
		return;
	}
	loseStep--;
	if (loseStep<0)
	{
		img_no_bei->setVisible(true);
		Sound::getInstance()->playMinStep();
		return;
	}
	lab_setp->setString(StringUtils::format("%d", loseStep));
	bar_step->setPercent(loseStep*100.f/maxStep);
}

void Game::LeftCallback()
{
// 	log("Hero.x=%f,Hero.y=%f", Hero->getPositionX(), Hero->getPositionY());
// 	log("m_map->getRotation()1=%f", m_map->getRotation());
	Hero->getAnimation()->play("jump");
	Hero->runAction(Sequence::create(
		RotateBy::create(0.01f, 90)
		, NULL)
		);
	for (auto _enemy:enemyArr)
	{
		_enemy->runAction(Sequence::create(
			RotateBy::create(0.01f, 90)
			, NULL)
			);
	}

	_isRotate = false;
	if (!_isBright)
	{
		_isBright = true;
		btn_left->setBright(_isBright);
		btn_right->setBright(_isBright);
	}
}

void Game::RightCallback()
{
// 	log("Hero.x=%f,Hero.y=%f", Hero->getPositionX(), Hero->getPositionY());
// 	log("m_map->getRotation()2=%f", m_map->getRotation());
	Hero->getAnimation()->play("jump");
	Hero->runAction(Sequence::create(
		RotateBy::create(0.01f, -90)
		, NULL)
		);
	for (auto _enemy : enemyArr)
	{
		_enemy->runAction(Sequence::create(
			RotateBy::create(0.01f, -90)
			, NULL)
			);
	}
	_isRotate = false;
	if (!_isBright)
	{
		_isBright = true;
		btn_left->setBright(_isBright);
		btn_right->setBright(_isBright);
	}
}

bool Game::isWall(Vec2 pp)
{
	//auto x = pp.x*MapWidth + MapHeight;
	////log("x====%f", x);
	//auto y = pp.y*MapWidth + MapHeight;
	////log("y====%f", y);
	//auto xy = Vec2(x, y);
	auto xy = getMapVec2(pp);
	for (auto wall:wallArr)
	{
		if (wall.containsPoint(xy))
		{
			return true;
		}
	}
	for (auto enemy:enemyArr)
	{
		if (getCollisionRect(xy).containsPoint(enemy->getPosition()))
		{
			return true;
		}
	}
	for (auto box:boxArr)
	{
		if (getCollisionRect(xy).containsPoint(box->getPosition()))
		{
			return true;
		}
	}
	for (auto mutiao:mutiaoLArr)
	{
		if (getCollisionRect(xy).containsPoint(mutiao->getPosition()))
		{
			return true;
		}
	}
	if (getCollisionRect(xy).containsPoint(Hero->getPosition()))
	{
		return true;
	}

	return false;
}

cocos2d::Vec2 Game::moveTo(Vec2 pp, int rotate)
{
	auto i =(int) pp.x / MapWidth;
	auto j = (int)pp.y / MapWidth;
	auto r = rotate % 360;
	switch (r)
	{
	case 0:
		while (j - 1>0 && !isWall(Vec2(i, j - 1)) && !isBanZiU(Vec2(i, j - 1)))
		{
			j -= 1;
		}
		if (j-1<=0&&!isWall(Vec2(i,j-1)))
		{
			//_gameFail = true;
			return Vec2(pp.x, -1000);
		}
		return Vec2(pp.x, j*MapWidth + MapHeight);
		break;
	case -270:
	case 90:
		while (i + 1<m_mapSize.width&&!isWall(Vec2(i + 1, j)) && !isBanZiL(Vec2(i + 1, j)))
		{
			i += 1;
		}
		if (i + 1 >= m_mapSize.width&&!isWall(Vec2(i+1,j)))
		{
			//_gameFail = true;
			return Vec2(1000, pp.y);
		}
		return Vec2(i*MapWidth + MapHeight, pp.y);
		break;
	case -180:
	case 180:
		while (j + 1<m_mapSize.height&&!isWall(Vec2(i, j + 1)) && !isBanZiD(Vec2(i, j + 1)))
		{
			j += 1;
		}
		if (j + 1 >= m_mapSize.height-1&&!isWall(Vec2(i,j+1)))
		{
			//_gameFail = true;
			return Vec2(pp.x, 1000);
		}
		return Vec2(pp.x, j*MapWidth + MapHeight);
		break;
	case -90:
	case 270:
		while (i - 1>0 && !isWall(Vec2(i - 1, j)) && !isBanZiR(Vec2(i - 1, j)))
		{
			i -= 1;
		}
		if (i - 1 <= 0 && !isWall(Vec2(i - 1, j)))
		{
			//_gameFail = true;
			return Vec2(-1000, pp.y);
		}
		return Vec2(i*MapWidth + MapHeight, pp.y);
		break;
	}
	return Vec2::ZERO;
}

void Game::OnDoorArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID)
{
	if (movementType == COMPLETE){
		if (strcmp(movementID.c_str(), "doorOpen") == 0){
			Sound::getInstance()->playInDoor();
			_gameOver = true;
			Hero->removeFromParent();
			Door->getAnimation()->play("doorDisappear");
			Sound::getInstance()->playDoorClose();
		}
		if (strcmp(movementID.c_str(),"doorDisappear")==0)
		{
			Sound::getInstance()->playDisapper();
			Door->removeFromParent();
			gameOver();
		}
	}
	if (movementType == LOOP_COMPLETE){
		//log(movementID.c_str());
		if (strcmp(movementID.c_str(), "doorCanIn") == 0){

		}
	}
}

void Game::OnKeyArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID)
{
	if (movementType == COMPLETE){
		if (strcmp(movementID.c_str(), "keyDisappear") == 0){
			Sound::getInstance()->playDisapper();
			Key->removeFromParent();
		}
	}
	if (movementType == LOOP_COMPLETE){
		//log(movementID.c_str());
		if (strcmp(movementID.c_str(), "key") == 0){
			if (Key&&Key->isVisible()&&Key->getBoundingBox().containsPoint(Hero->getPosition()))
			{
				Sound::getInstance()->playGetKey();
				_isHaveKey = true;
				Door->getAnimation()->play("doorCanIn");
				Key->getAnimation()->play("keyDisappear");			
			}
		}
	}
}

void Game::OnHeroArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID)
{
	if (movementType == COMPLETE){
		if (strcmp(movementID.c_str(), "playerDisappear") == 0)
		{
			Sound::getInstance()->playDisapper();
			Hero->removeFromParent();
			this->gameFail();
		}
		if (strcmp(movementID.c_str(), "onGround") == 0)
		{			
			Hero->getAnimation()->play("stand");
		}
		if (strcmp(movementID.c_str(), "attack") == 0)
		{
			_attack = false;
			Hero->getAnimation()->play("jump");
			if (!_isBright)
			{
				_isBright = true;
				btn_left->setBright(_isBright);
				btn_right->setBright(_isBright);
			}
		}
	}
	if (movementType == LOOP_COMPLETE){
		//log(movementID.c_str());
		if (strcmp(movementID.c_str(), "stand") == 0){
			
		}
		if (strcmp(movementID.c_str(), "jump") == 0){
			Sound::getInstance()->playFallGround();
			armature->getAnimation()->play("stand");
		}
	}
}

void Game::OnEnemyArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID)
{
	if (movementType == COMPLETE){
		if (strcmp(movementID.c_str(), "enemyDisappear") == 0)
		{
			Sound::getInstance()->playDisapper();
			armature->removeFromParent();
		}
		if (strcmp(movementID.c_str(), "strike") == 0)
		{
			Sound::getInstance()->playDead();
			armature->getAnimation()->play("enemyDisappear");
			enemyArr.eraseObject(armature);
		}
	}
	if (movementType == LOOP_COMPLETE){
		//log(movementID.c_str());
		if (strcmp(movementID.c_str(), "stand") == 0){

		}
	}
}

void Game::gameOver()
{
	char name[15] = { 0 };
	sprintf(name, LevelBei, m_level, m_levelNum);
	if (loseStep>=0&&!readBoolXML(name))
	{
		saveBoolXML(name, true);
		saveIntXML(JBeiNum, readIntXML(JBeiNum) + 1);
	}
	sprintf(name, LevelLock, m_levelNum >= 18 && m_level<3 ? m_level + 1 : m_level, m_levelNum >= 18&&m_level<3?1:m_levelNum+1);
	if (readBoolXML(name))
	{
		saveBoolXML(name, false);
	}

	Director::getInstance()->replaceScene(GameOver::createScene(m_level, m_levelNum, loseStep));
}

void Game::gameFail()
{
	Director::getInstance()->replaceScene(GameOver::createScene(m_level, m_levelNum, -1));
}

Rect Game::getCollisionRect(Vec2 pp)
{
	return Rect(pp.x - MapHeight + 2, pp.y - MapHeight + 2, MapWidth - 4, MapWidth - 4);
}

cocos2d::Vec2 Game::getMapId(Vec2 pp)
{
	auto x = (int)pp.x / MapWidth;
	auto y = (int)pp.y / MapWidth;
	return Vec2(x, y);
}

cocos2d::Vec2 Game::getMapVec2(Vec2 id)
{
	auto x = id.x*MapWidth + MapHeight;
	//log("x====%f", x);
	auto y = id.y*MapWidth + MapHeight;
	//log("y====%f", y);
	return Vec2(x, y);
}

void Game::dropUpdate(float dt)
{
	if (_isRotate || _gameOver || _gameFail )
	{
		return;
	}

	if (Hero->getNumberOfRunningActions() == 0 && !_attack)
	{
		Hero->runAction(Sequence::create(
			MoveTo::create(0.1f, moveTo(Hero->getPosition(), m_map->getRotation()))
			, DelayTime::create(0.12f)
			, CallFunc::create(CC_CALLBACK_0(Game::isAttack, this))
			, NULL)
			);
	}

	for (auto _enemy : enemyArr)
	{
		//auto _enemy = static_cast<Armature*>(enemy);
		if (_enemy->getNumberOfRunningActions()==0)
		{
			_enemy->runAction(Sequence::create(
				MoveTo::create(0.1f, moveTo(_enemy->getPosition(), m_map->getRotation()))
				, NULL)
				);
		}	
	}

	for (auto box : boxArr)
	{
		//auto _enemy = static_cast<Armature*>(enemy);
		box->runAction(Sequence::create(
			MoveTo::create(0.1f, moveTo(box->getPosition(), m_map->getRotation()))
			, NULL)
			);
	}

	if (_isHaveKey&&!_isRotate&&getCollisionRect(Hero->getPosition()).containsPoint(Door->getPosition()))
	{
		Sound::getInstance()->playDoorOpen();
		_gameOver = true;
		Door->getAnimation()->play("doorOpen");		
	}

}

bool Game::isEnemy(Vec2 pp)
{
	auto xy = getMapVec2(pp);
	for (auto enemy:enemyArr)
	{
		if (getCollisionRect(xy).containsPoint(enemy->getPosition()))
		{
			Sound::getInstance()->playAttack();
			Hero->getAnimation()->play("attack");
			enemy->getAnimation()->play("strike");
			_attack = true;
			return true;
		}
	}
//	Hero->getAnimation()->play("stand");
	return false;
}

cocos2d::Vec2 Game::isFoot()
{
	auto r = (int)Hero->getRotation();
	r %= 360;
	//log("Hero->getRotation()===%d", r);
	auto xy = Hero->getPosition();
	auto x = (int)xy.x / MapWidth;
	auto y = (int)xy.y / MapWidth;
	switch (r)
	{
	case 0:
		y -= 1;
		break;
	case -270:
	case 90:
		x-=1;
		break;
	case -180:
	case 180:
		y += 1;
		break;
	case -90:
	case 270:
		x += 1;
		break;
	}
	return Vec2(x, y);
}

void Game::isAttack()
{
	isEnemy(isFoot());
}

bool Game::isBanZiU(Vec2 pp)
{
	for (auto banziU : banziUArr)
	{
		auto banziId = getMapId(banziU->getPosition());
		banziId = banziId - Vec2(1, 0);
		if (banziU->getRotation() == 0.f && pp.x == banziId.x&&banziId.y == pp.y&&!isBanZiOpen(banziId))
		{
			return true;
		}
	}
	return false;
}

bool Game::isBanZiL(Vec2 pp)
{
	for (auto banziL : banziLArr)
	{
		auto banziId = getMapId(banziL->getPosition());
		banziId = banziId - Vec2(0, 1);
		if (banziL->getRotation() == 0.f && pp.x == banziId.x && banziId.y == pp.y&&!isBanZiOpen(banziId))
		{
			return true;
		}
	}
	return false;
}

bool Game::isBanZiD(Vec2 pp)
{
	for (auto banziD : banziDArr)
	{
		auto banziId = getMapId(banziD->getPosition());
		if (banziD->getRotation() == 0.f && pp.x == banziId.x && banziId.y == pp.y&&!isBanZiOpen(banziId))
		{
			return true;
		}
	}
	return false;
}

bool Game::isBanZiR(Vec2 pp)
{
	for (auto banziR : banziRArr)
	{
		auto banziId = getMapId(banziR->getPosition());
		if (banziR->getRotation()==0.f&&pp.x == banziId.x && banziId.y == pp.y&&!isBanZiOpen(banziId))
		{
			return true;
		}
	}
	return false;
}

bool Game::isBanZiOpen(Vec2 pp)
{
	auto xy = getMapVec2(pp);
	for (auto enemy:enemyArr)
	{
		if (getCollisionRect(xy).intersectsRect(getCollisionRect(enemy->getPosition())))
		{
			return true;
		}
	}
	for (auto box : boxArr)
	{
		if (getCollisionRect(xy).intersectsRect(getCollisionRect(box->getPosition())))
		{
			return true;
		}
	}
	if (getCollisionRect(xy).intersectsRect(getCollisionRect(Hero->getPosition())))
	{
		return true;
	}
	return false;
}

void Game::setFaZhen(Node* node)
{
	//auto aId = getMapId(fazhenA->getPosition());
	//auto bId = getMapId(fazhenB->getPosition());
	//auto nId = getMapId(node->getPosition());
	if (!isWall(getMapId(fazhenA->getPosition())) && !isWall(getMapId(fazhenB->getPosition())) && !_faZhenOpen)
	{
		_faZhenOpen = true;
	}
	if (!_faZhenOpen)
	{
		return;
	}
	//auto a_b = fazhenA->getPosition().equals(node->getPosition());
	//auto b_a = fazhenB->getPosition().equals(node->getPosition());
	if (getCollisionRect(fazhenA->getPosition()).containsPoint(node->getPosition()))
	{
		_faZhenOpen = false;
		node->stopAllActions();
		node->setPosition(fazhenB->getPosition());
		return;
	}
	if (getCollisionRect(fazhenB->getPosition()).containsPoint(node->getPosition()))
	{
		_faZhenOpen = false;
		node->stopAllActions();
		node->setPosition(fazhenA->getPosition());
		return;
	}
}

bool Game::setMuTiao()
{
	for (auto fang:fangArr)
	{
		for (auto box:boxArr)
		{
			if (fang.containsPoint(box->getPosition()))
			{
				return true;
			}
		}
	}
	return false;
}

void Game::OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("OkTouchEventCallback");
		Sound::getInstance()->playBtn();
		if (loseStep<maxStep)
		{
			loseStep = loseStep < 0 ? 1 : loseStep + 1;
			if (loseStep >= 0&&img_no_bei->isVisible())
			{
				img_no_bei->setVisible(false);
			}
			lab_setp->setString(StringUtils::format("%d", loseStep));
			bar_step->setPercent(loseStep*100.f / maxStep);
		}
		_news->runAction(Spawn::create(
			MoveBy::create(1.f, Vec2(800.f, 0))
			, FadeOut::create(1.f)
			, NULL));
		saveBoolXML(ISNEWS, false);
	}
}

void Game::AddStepTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("AddStepTouchEventCallback");
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
		if (readIntXML(STEP)<=0)
		{
			saveBoolXML(ISNEWS, true);
			_news = News::createNews(CC_CALLBACK_2(Game::OkStepTouchEventCallback, this), 6, true);
			this->addChild(_news, 100);
			_news->setPosition(800.f, 0);
			_news->runAction(MoveBy::create(0.5f, Vec2(-800, 0)));
		}
		if (readIntXML(STEP) > 0)
		{
			saveIntXML(STEP, readIntXML(STEP) - 1);
			if (loseStep < maxStep)
			{
				loseStep = loseStep < 0 ? 1 : loseStep + 1;
				if (loseStep >= 0 && img_no_bei->isVisible())
				{
					img_no_bei->setVisible(false);
				}
				lab_setp->setString(StringUtils::format("%d", loseStep));
				bar_step->setPercent(loseStep*100.f / maxStep);
			}
		}
	}
}

void Game::ShowKeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		log("ShowKeyTouchEventCallback");
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
		if (readIntXML(SHOWKEY) <= 0)
		{
			saveBoolXML(ISNEWS, true);
			_news = News::createNews(CC_CALLBACK_2(Game::OkKeyTouchEventCallback, this), 5, true);
			this->addChild(_news, 100);
			_news->setPosition(800.f, 0);
			_news->runAction(MoveBy::create(0.5f, Vec2(-800, 0)));
		}
		if (readIntXML(SHOWKEY) > 0)
		{
			saveIntXML(SHOWKEY, readIntXML(SHOWKEY) - 1);
			if (!Key->isVisible())
			{
				Vector<Armature*> _enemy;
				for (auto enemy : enemyArr)
				{
					Sound::getInstance()->playDead();
					_enemy.pushBack(enemy);
					enemy->stopAllActions();
					enemy->getAnimation()->play("enemyDisappear");
				}
				for (auto enemy:_enemy)
				{
					enemyArr.eraseObject(enemy);
				}
			}
		}
	}
}

void Game::OkKeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
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

void Game::OkStepTouchEventCallback(Ref *pSender, Widget::TouchEventType type)
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








