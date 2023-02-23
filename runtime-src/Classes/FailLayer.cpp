#include "FailLayer.h"
#include "GameoverLayer.h"
#include "GameScene.h"
#include "UserData.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include "GiftLayer.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;

bool FailLayer::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
	this->initWithColor(Color4B(0, 0, 0, 100));
	auto failLayer = CSLoader::createNode("FailLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	failLayer->setContentSize(frameSize);
	failLayer->setIgnoreAnchorPointForPosition(false);
	failLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	ui::Helper::doLayout(failLayer);
	failLayer->setScale(0);
	failLayer->runAction(Sequence::create(ScaleTo::create(0.2f, 1.25f), ScaleTo::create(0.1f, 1.0f), nullptr));
	addChild(failLayer);
	failLayer->setPosition(frameSize / 2);

	auto failBg = (Sprite*)failLayer->getChildByTag(77);

	auto buttonAffirm = (ui::Button*)(failBg->getChildByTag(86));
	buttonAffirm->addClickEventListener(CC_CALLBACK_1(FailLayer::menuAffirmCallBack, this));

	auto buttonCancel = (ui::Button*)(failBg->getChildByTag(87));
	buttonCancel->addClickEventListener(CC_CALLBACK_1(FailLayer::menuCancelCallBack, this));

// 	auto fail_bg = (Sprite*)(failLayer->getChildByTag(80));
// 	auto rebirthLabel = (ui::TextBMFont*)(fail_bg->getChildByTag(108));
//  	rebirthLabel->setString(StringUtils::format("%d", UserData::getInstance()->getRebirth()));
	

	return true;
}
void FailLayer::menuAffirmCallBack(Ref* Psender)
{
	auto gamescene = (GameScene*)this->getParent();
	if (UserData::getInstance()->getGold() >= 10)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Fuhuo.mp3");
		gamescene->mResume();
		UserData::getInstance()->addGold(-10);
		gamescene->_propLayer->setGoldNum();
		gamescene->_propLayer->setTime(20.0f);
		gamescene->removeChild(this);
	}
	else
	{
		auto giftLayer = GiftLayer::create(Type_Bubble);
		giftLayer->isGameScene = true;
		giftLayer->isPause = false;
		gamescene->mPause();
		gamescene->operateAllSchedulerAndActions(this, k_Operate_Resume);
		gamescene->addChild(giftLayer);
	}
}

void FailLayer::menuCancelCallBack(Ref* Psender)
{
	auto gameScene = (GameScene*)this->getParent();
	if (gameScene != nullptr)
	{
		gameScene->addGameover();
		this->setVisible(false);
	}

	//removeFromParent();
}
