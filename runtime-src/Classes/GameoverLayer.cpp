#include "GameoverLayer.h"
#include "StartScene.h"
#include "GameScene.h"
#include "UserData.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;

bool GameoverLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	UserData::getInstance()->setIsGameOver(true);
	auto visible = Director::getInstance()->getVisibleSize();

	this->initWithColor(Color4B(0, 0, 0, 100));
	auto gameoverLayer = CSLoader::createNode("GameoverLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	gameoverLayer->setContentSize(frameSize);
	ui::Helper::doLayout(gameoverLayer);
	gameoverLayer->setIgnoreAnchorPointForPosition(false);
	gameoverLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	gameoverLayer->setScale(0);
	gameoverLayer->runAction(Sequence::create(ScaleTo::create(0.2f, 1.25f), ScaleTo::create(0.1f, 1.0f), nullptr));
	addChild(gameoverLayer);
	gameoverLayer->setPosition(visible / 2);

	auto buttonStart = (ui::Button*)(gameoverLayer->getChildByTag(78));
	buttonStart->addClickEventListener(CC_CALLBACK_1(GameoverLayer::menuStartCallBack, this));

	auto buttonOver = (ui::Button*)(gameoverLayer->getChildByTag(77));
	buttonOver->addClickEventListener(CC_CALLBACK_1(GameoverLayer::menuOverCallBack, this));

	auto gameover_bg = (Sprite*)gameoverLayer->getChildByTag(68);

	auto scoreText = (ui::TextBMFont*)(gameover_bg->getChildByTag(106));
	scoreText->setString(StringUtils::format("%d", UserData::getInstance()->getScore()));

	auto bestScoreText = (ui::TextBMFont*)(gameover_bg->getChildByTag(105));
	bestScoreText->setString(StringUtils::format("%d", UserData::getInstance()->getBestScore()));

// 	auto goldText = (ui::TextBMFont*)(gameover_bg->getChildByTag(104));
// 	goldText->setString(StringUtils::format("%d", UserData::getInstance()->getGold()));



	if (scoreText->getString() == bestScoreText->getString())
	{
		auto recordSprite = (Sprite*)gameover_bg->getChildByTag(71);
		if (!recordSprite->isVisible())
		{
			recordSprite->setVisible(true);
		}
	}
	return true;
}
void GameoverLayer::menuStartCallBack(Ref* Psender)
{
	UserData::getInstance()->newReset();
	auto gameScene = GameScene::create();
	gameScene->_bubbleLayer->initTheBoard();
	Director::getInstance()->replaceScene(gameScene);
}

void GameoverLayer::menuOverCallBack(Ref* Psender)
{
	Director::getInstance()->replaceScene(StartLayer::scene());
}