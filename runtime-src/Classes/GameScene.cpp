#include "GameScene.h"
#include "UserData.h"
#include "GameoverLayer.h"
#include "PauseLayer.h"
#include "FailLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include "GiftLayer.h"
#include "StartScene.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;


bool GameScene::init()
{

	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bubbles.plist");

	_bg = CSLoader::createNode("Background.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	_bg->setContentSize(frameSize);
	ui::Helper::doLayout(_bg);
	addChild(_bg);

	//SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bgm.mp3", true);

	//_touchLayer = TouchLayer::create();
	//addChild(_touchLayer);

	_bubbleLayer = BubbleLayer::create();
	addChild(_bubbleLayer);

	_propLayer = PropLayer::create();
	addChild(_propLayer);








	auto lisenter = EventListenerKeyboard::create();


	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	return true;
}

void GameScene::operateAllSchedulerAndActions(Node* node, OperateFlag flag)
{
	if (node->isRunning()) {
		switch (flag) {
		case k_Operate_Pause:
			node->pause();
			break;
		case k_Operate_Resume:
			node->resume();
			break;
		default:
			break;
		}
		auto children = node->getChildren();
		if (children.size() > 0) {
			for (auto child : children) {
				operateAllSchedulerAndActions(child, flag);
			}
		}
	}
}

void GameScene::mPause()
{
	operateAllSchedulerAndActions(this, k_Operate_Pause);
}

void GameScene::mResume()
{

	operateAllSchedulerAndActions(this, k_Operate_Resume);

}

void GameScene::onTouch(Point target)
{
	_bubbleLayer->onTouch(target);
}
void GameScene::setCannonAngle(Point target)
{
	_propLayer->setCannonAngle(target);
}
void GameScene::setTouchEnable(bool enable)
{
	_touchLayer->setTouchEnable(enable);
}
void GameScene::swapBubble()
{
	_bubbleLayer->swapBubble();
}
void GameScene::bombBubble(bool use)
{
	_bubbleLayer->bombBubble(use);
}
void GameScene::colorBubble(bool use)
{
	_bubbleLayer->colorBubble(use);
}
void GameScene::gameOver(bool over)
{
	UserData::getInstance()->saveData();
	operateAllSchedulerAndActions(this, k_Operate_Pause);

	if (over)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Ending.mp3");
		auto overLayer = GameoverLayer::create();
		this->addChild(overLayer);
	}
	else
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/end.mp3");
		auto failLayer = FailLayer::create();
		this->addChild(failLayer);
	}

}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode != EventKeyboard::KeyCode::KEY_BACK)
	{
		return;
	}
	operateAllSchedulerAndActions(this, k_Operate_Pause);
	// 	auto layer = PauseLayer::create();
	// 	gameSceme->addChild(layer);
	auto size = Director::getInstance()->getWinSize();
	auto giftLayer = GiftLayer::create(Type_Bubble);
	giftLayer->isGameScene = true;
	this->addChild(giftLayer);
	giftLayer->_giftNode->setPosition(Vec2(size.width / 2, size.height / 2 + 50));
	//auto buttonHome = ui::Button::create("Pause/returnHome.png", "Pause/returnHome_press.png", "Pause/returnHome_press.png");
	// 	auto buttonHome = ui::Button::create();
	// 	buttonHome->loadTextureNormal("Pause/returnHome.png");
	// 	buttonHome->loadTexturePressed("Pause/returnHome_press.png");
	auto homeImageView = MenuItemSprite::create(Sprite::createWithSpriteFrameName("return_home.png"), Sprite::createWithSpriteFrameName("return_home_press.png"), [=](Ref* Psender){
		this->_bubbleLayer->savaCurrentBoard();
		UserData::getInstance()->setRETime(this->_propLayer->getTime());
		Director::getInstance()->replaceScene(StartLayer::scene());
	});
	homeImageView->setPosition(Vec2(150, 110));

	auto continueItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("continue.png"), Sprite::createWithSpriteFrameName("continue_press.png"), [=](Ref* Psender){
		this->mResume();
		this->removeChild(giftLayer);
	});
	continueItem->setPosition(Vec2(380, 110));

	auto menu = Menu::create(homeImageView, continueItem, nullptr);
	giftLayer->addChild(menu);
	menu->setPosition(Vec2::ZERO);

}
void GameScene::addGameover()
{
	auto gameover = GameoverLayer::create();
	this->addChild(gameover);
}
