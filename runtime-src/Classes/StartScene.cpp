#include "StartScene.h"
#include "GameScene.h"
#include "TurnplateLayer.h"
#include "UserData.h"
#include "GameConst.h"
#include "ShopLayer.h"
#include "GiftLayer.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "hellocpp/PayClass.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif


using namespace cocos2d;
using namespace cocostudio::timeline;
using namespace CocosDenshion;

enum  typeTag
{
	Menu_Tag = 200,
	Vol_Tag,
};

StartLayer::StartLayer()
{
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("main_music.mp3", true);
}

StartLayer::~StartLayer()
{
	
}

Scene* StartLayer::scene()
{
    Scene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = Scene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        StartLayer *layer = StartLayer::create();
        CC_BREAK_IF(! layer);




        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	setReal(Vec2(-0.5f, 0.5f));
	_startNode = CSLoader::createNode("StartLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	_startNode->setContentSize(frameSize);
	ui::Helper::doLayout(_startNode);
	addChild(_startNode);
	_startNode->setTag(50);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bgm.mp3", true);

	auto buttonStart = (ui::Button*)_startNode->getChildByTag(3);
	buttonStart->addClickEventListener(CC_CALLBACK_1(StartLayer::menuNewGameCallback, this));
	

	auto buttonEnd = (ui::Button*)_startNode->getChildByTag(4);
	buttonEnd->addClickEventListener(CC_CALLBACK_1(StartLayer::menuCloseCallback, this));
	if (UserData::getInstance()->getBeginner())
	{
		auto armatureStart = (Armature*)buttonStart->getChildByTag(39);
		armatureStart->getAnimation()->play("kaishianniu", -1, 1);
	}
	else
	{
		auto armatureEnd = (Armature*)buttonEnd->getChildByTag(123);
		armatureEnd->getAnimation()->play("kaishianniu", -1, 1);
	}

	auto armatureStart = (Armature*)buttonStart->getChildByTag(39);
	auto shopImageView = (ui::ImageView*)_startNode->getChildByTag(79);
	shopImageView->addClickEventListener(CC_CALLBACK_1(StartLayer::menuShopCallBack, this));


	auto goldBg = (Sprite*)_startNode->getChildByTag(87);
	auto goldLabel = (ui::TextBMFont*)goldBg->getChildByTag(107);
	goldLabel->setString(StringUtils::format("%d", UserData::getInstance()->getGold()));


	auto goldImageView = (ui::ImageView*)goldBg->getChildByTag(81);
	goldImageView->addClickEventListener(CC_CALLBACK_1(StartLayer::menuShopCallBack, this));


	auto giftImageView = (ui::ImageView*)_startNode->getChildByTag(201);
	giftImageView->addClickEventListener(CC_CALLBACK_1(StartLayer::menuGiftCallBack, this));
	if (UserData::getInstance()->getNewGift())
	 {
	 		giftImageView->setVisible(true);
	}
	else
	{
		giftImageView->setVisible(false);
	}

	auto bubbleImageView = (ui::ImageView*)_startNode->getChildByTag(202);
	bubbleImageView->addClickEventListener(CC_CALLBACK_1(StartLayer::menuGiftCallBack, this));

	auto jewelImageView = (ui::ImageView*)_startNode->getChildByTag(203);
	jewelImageView->addClickEventListener(CC_CALLBACK_1(StartLayer::menuGiftCallBack, this));

	auto turnImageView = (ui::ImageView*)_startNode->getChildByTag(61);
// 	if (UserData::getInstance()->getHaveRaffle())
// 	{
// 		turnImageView->setVisible(false);
// 	}
	turnImageView->addClickEventListener(CC_CALLBACK_1(StartLayer::menuTurnCallBack, this));

	auto armature = (Armature*)_startNode->getChildByTag(10);
	armature->getAnimation()->play("Animation1");
	
	armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(StartLayer::movementCall, this));

	

	auto armatureLogo = (Armature*)_startNode->getChildByTag(13);
	_disPoint = armatureLogo->getPosition();

	
// 	auto volToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(StartLayer::menuVolCallBack, this), MenuItemImage::create("Start/vol.png", "Start/vol.png"), MenuItemImage::create("Start/vol_1.png", "Start/vol_1.png"), nullptr);
// 	volToggle->setTag(Vol_Tag);
// 	auto menu = Menu::create(volToggle,nullptr);
// 	this->addChild(menu);
// 	menu->setPosition(470.0f, 890.0f);
// 	menu->setTag(Menu_Tag);
	auto vol = (ui::CheckBox*)_startNode->getChildByTag(83);
	vol->addEventListener(CC_CALLBACK_2(StartLayer::menuVolCallBack, this));
	if (!UserData::getInstance()->getIsMusicOn())
	{
		vol->setSelected(true);
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);

	}

	auto lisenter = EventListenerKeyboard::create();
	lisenter->onKeyReleased = CC_CALLBACK_2(StartLayer::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lisenter, this);

	
	schedule(CC_SCHEDULE_SELECTOR(StartLayer::setGoldNumDelay), 0.5f);

	return true;
}

void StartLayer::setGoldNumDelay(float delay)
{

	auto giftImageView = (ui::ImageView*)_startNode->getChildByTag(201);
	if (UserData::getInstance()->getNewGift())
	{
		giftImageView->setVisible(true);
	}
	else
	{
		giftImageView->setVisible(false);
	}



	setGoldNum();
}

void StartLayer::menuCloseCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.mp3");
// 	UserData::getInstance()->saveData();
//     Director::getInstance()->end();
//	exit(0);
	

	if (UserData::getInstance()->getIsGameOver())
	{
		UserData::getInstance()->setIsGameOver(false);
		UserData::getInstance()->newReset();
		auto gameScene = GameScene::create();
		gameScene->_bubbleLayer->initTheBoard(0);
		Director::getInstance()->pushScene(gameScene);
	}
	else
	{
		//UserData::getInstance()->setIsGameOver(false);
		auto gameScene = GameScene::create();
		gameScene->_bubbleLayer->initTheBoardWithData();
		Director::getInstance()->pushScene(gameScene);
	}
	

}

void StartLayer::menuNewGameCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.mp3");

	auto layerColor = LayerColor::create(Color4B(0,0,0,100));
	if (UserData::getInstance()->getBeginner())
	{
		UserData::getInstance()->newReset();
		auto gameScene = GameScene::create();
		gameScene->_bubbleLayer->initTheBoard();
		Director::getInstance()->pushScene(gameScene);
		return;
	}
	auto hintNode = CSLoader::createNode("Start/Hint.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	hintNode->setContentSize(frameSize);
	ui::Helper::doLayout(_startNode);
	hintNode->setPosition(frameSize/2);
	layerColor->addChild(hintNode);


 	auto listener = EventListenerTouchOneByOne::create();
 	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* even){ return true; };
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, layerColor);






	addChild(layerColor);




	auto hitBg = (Sprite*)hintNode->getChildByTag(50);

	auto buttonOn = (ui::Button*)hitBg->getChildByTag(53);
	buttonOn->addClickEventListener([=](Ref* Psender){
		UserData::getInstance()->newReset();
		auto gameScene = GameScene::create();
		gameScene->_bubbleLayer->initTheBoard();
		Director::getInstance()->pushScene(gameScene);
		return;
	});

	auto buttonOff = (ui::Button*)hitBg->getChildByTag(54);
	buttonOff->addClickEventListener([=](Ref* Psender){
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		this->removeChild(layerColor);
	});
}

void StartLayer::menuShopCallBack(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.mp3");
	auto shopLayer = ShopLayer::create();
	addChild(shopLayer);
	/*this->setTouchEnabled(false);*/
}

void StartLayer::menuGiftCallBack(Ref* pSender)
{
	auto image = dynamic_cast<ui::ImageView*>(pSender);
	

	UserData::getInstance()->setBeginner(false);
	auto tag = image->getTag();
	auto type = (GiftType)(tag - 201);
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.mp3");
	auto giftLayer = GiftLayer::create(type);
	addChild(giftLayer);
	
}
void StartLayer::menuTurnCallBack(Ref* psender)
{
	SimpleAudioEngine::getInstance()->playEffect("Music/Click.mp3");
	auto turnPlate = TurnplateLayer::create();
	this->addChild(turnPlate);
}
void StartLayer::movementCall(Armature * armature, MovementEventType type, const std::string &name)
{
	if (type == COMPLETE){
		
		if (name == "Animation1")
		{
			haveRun = true;
			scheduleUpdate();
			armature->getAnimation()->play("Animation1_Copy1");
			auto moveActionBoy = MoveBy::create(1.5f, Vec2(-5, 15));
			
			auto seqBoy = Sequence::create(moveActionBoy, moveActionBoy->reverse(), nullptr);
			auto repeatBoy = RepeatForever::create(seqBoy);
			auto spriteBoy = (Sprite*)_startNode->getChildByTag(12);
			spriteBoy->runAction(repeatBoy);

			auto moveActionCat = MoveBy::create(1.5f, Vec2(15, 8));
			auto seqCat = Sequence::create(moveActionCat, moveActionCat->reverse(), nullptr);
			auto repeatCat = RepeatForever::create(seqCat);
			auto spriteCat = (Sprite*)_startNode->getChildByTag(41);
			spriteCat->runAction(repeatCat);


			auto moveActionArm = MoveBy::create(1.5f, Vec2(8, 8));
			auto seqArm = Sequence::create(moveActionArm, moveActionArm->reverse(), nullptr);
			auto repeatArm = RepeatForever::create(seqArm);
			armature->runAction(repeatArm);
		}
	}
}
void StartLayer::update(float delta)
{
	auto ball = (Sprite*)_startNode->getChildByTag(43);
	ball->setPosition(ball->getPosition().x + getReal().x * 20, ball->getPosition().y + getReal().y * 20);
	if (ball->getPosition().x <= BUBBLE_RADIUS)
	{
		setReal((_disPoint - ball->getPosition()).getNormalized());
	}
	if (ball->getPosition().x >= _disPoint.x && ball->getPosition().y >= _disPoint.y)
	{
		ball->removeAllChildren();
		auto scale = ScaleTo::create(0.5f, 1.3f);
		ball->runAction(scale);
		unscheduleUpdate();
		ball->setPosition(_disPoint);
		auto armatureLogo = (Armature*)_startNode->getChildByTag(13);
		armatureLogo->getAnimation()->play("Animation1");
		armatureLogo->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(StartLayer::movementLogoCall,this));
	}

}
void StartLayer::movementLogoCall(Armature * armature, MovementEventType type, const std::string &name)
{

	if (type == COMPLETE){
		if (name == "Animation1")
		{
			armature->getAnimation()->play("Animation2",-1,5);
		}
	}
}
void StartLayer::menuVolCallBack(Ref* pSender, ui::CheckBox::EventType type)
{
	
	if (type == ui::CheckBox::EventType::SELECTED)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
		UserData::getInstance()->setIsMusicOn(false);
		//SimpleAudioEngine::getInstance()->setEffectsVolume(0.0f);
	}
	else
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		SimpleAudioEngine::getInstance()->setEffectsVolume(1);
		UserData::getInstance()->setIsMusicOn(true);
		//SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	}


}
void StartLayer::setGoldNum()
{

	auto goldBg = (Sprite*)_startNode->getChildByTag(87);
	auto goldLabel = (ui::TextBMFont*)goldBg->getChildByTag(107);
	goldLabel->setString(StringUtils::format("%d", UserData::getInstance()->getGold()));

}
void StartLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK && isExit == false)
	{
// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 		onKeyDown(4);
// #endif
		isExit = true;
		auto layer = LayerColor::create(Color4B(0,0,0,100));
		addChild(layer);

		auto exitLayer = CSLoader::createNode("ExitLayer.csb");
		this->addChild(exitLayer);
		auto listenr = EventListenerTouchOneByOne::create();
		listenr->setSwallowTouches(true);
		listenr->onTouchBegan = [=](Touch* touch, Event* event){return true;};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenr, exitLayer);

		auto buttonYes = (ui::Button*)exitLayer->getChildByTag(14);
		buttonYes->addClickEventListener([=](Ref* Psender){ Director::getInstance()->end(); });
		auto buttonNo = (ui::Button*)exitLayer->getChildByTag(13);
		buttonNo->addClickEventListener([=](Ref* Psender){exitLayer->removeFromParent(); layer->removeFromParent(); isExit = false; });

	}



}
