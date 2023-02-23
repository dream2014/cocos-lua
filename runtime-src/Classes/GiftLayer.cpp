#include "GiftLayer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UserData.h"
#include "GameScene.h"
#include "StartScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "hellocpp/PayClass.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif



using namespace cocostudio;

GiftLayer* GiftLayer::create(GiftType type)
{
	auto giftLayer = new GiftLayer();
	if (giftLayer != nullptr && giftLayer->init(type))
	{
		giftLayer->autorelease();
		return giftLayer;
	}
	else
	{
		delete giftLayer;
		giftLayer = nullptr;
		return nullptr;
	}
}

bool GiftLayer::init(GiftType type)
{
	if (!LayerColor::init())
	{
		return false;
	}



	this->initWithColor(Color4B(0, 0, 0, 100));
	_eventDispatcher->setEnabled(true);
	_giftNode = CSLoader::createNode("GiftLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	_giftNode->setContentSize(frameSize);
	_giftNode->setIgnoreAnchorPointForPosition(false);
	_giftNode->setAnchorPoint(Vec2(0.5f, 0.5f));
	ui::Helper::doLayout(_giftNode);
	_giftNode->setScale(0);
	_giftNode->runAction(Sequence::create(ScaleTo::create(0.2f, 1.25f), ScaleTo::create(0.1f, 1.0f), nullptr));
	addChild(_giftNode);
	_giftNode->setPosition(frameSize / 2);

	auto giftBg = (Sprite*)_giftNode->getChildByTag(16);


	setType(type);

	auto giftName = (Sprite*)giftBg->getChildByTag(20);
	auto giftFrameName = StringUtils::format("gift_%d.png", (int)type + 1);
	giftName->setSpriteFrame(giftFrameName);

	auto giftNameBG = (Sprite*)giftBg->getChildByTag(17);
	auto giftFrameNameBG = StringUtils::format("gift_bg%d.png", (int)type + 1);
	giftNameBG->setSpriteFrame(giftFrameNameBG);





	auto getButton = (ui::Button*)giftBg->getChildByTag(19);
	getButton->addClickEventListener(CC_CALLBACK_1(GiftLayer::getButtonCallBack, this));

	
	auto returnButton = (ui::Button*)giftBg->getChildByTag(148);
	returnButton->addClickEventListener(CC_CALLBACK_1(GiftLayer::returnButtonCallBack, this));

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *unused_event){return true; };
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GiftLayer::getButtonCallBack(Ref* Pesender)
{
	return;


// 	auto giftBg = (Sprite*)_giftNode->getChildByTag(16);
// 	auto armature = (Armature*)giftBg->getChildByTag(35);
// 	armature->pause();
// 	_giftNode->stopAllActions();
// 	int payID = 0;
// 	switch(getType()){
// 	case Type_New: payID = 1; break;
// 	case Type_Bubble : payID = 11; break;
// 	case Type_Jewel : payID = 10; break;
// 	default : payID = 0;
// 	}

// #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 	Director::getInstance()->stopAnimation();
//  	// payGold(payID);
// #endif

// 	if (this->isGameScene)
// 	{
// 		auto gameScene = (GameScene*)this->getParent();
// 		if (isPause)
// 		{
// 			gameScene->mResume();
// 			//gameScene->_propLayer->setGoldNum();
// 		}
// 	}
// 	else
// 	{
// 		auto startScene = (StartLayer*)this->getParent();
// 		//startScene->setGoldNum();
// 	}
	
// 	this->runAction(Sequence::create(ScaleTo::create(0.1f, 0), CallFunc::create([=](){this->removeFromParent(); }), nullptr));


}

void GiftLayer::returnButtonCallBack(Ref* Pesender)
{

	if (this->isGameScene)
	{
		auto gameScene = (GameScene*)this->getParent();
		if (isPause)
		{
			gameScene->mResume();
			//gameScene->_propLayer->setGoldNum();
		}
	}

	this->removeFromParent();

}
