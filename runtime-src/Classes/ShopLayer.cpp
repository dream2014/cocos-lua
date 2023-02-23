#include "ShopLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include "GameConst.h"
#include "StartScene.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "hellocpp/PayClass.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

using namespace CocosDenshion;

bool ShopLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initWithColor(Color4B(0, 0, 0, 100));


	_shopLayer = CSLoader::createNode("ShopLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	_shopLayer->setContentSize(frameSize);
	_shopLayer->setIgnoreAnchorPointForPosition(false);
	_shopLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	_shopLayer->setScale(0);
	_shopLayer->runAction(Sequence::create(ScaleTo::create(0.2f, 1.25f), ScaleTo::create(0.1f, 1.0f), nullptr));

	
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch *touch, Event *unused_event){return true; };
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);



	ui::Helper::doLayout(_shopLayer);
	addChild(_shopLayer);
	_shopLayer->setPosition(frameSize / 2);

 	auto shop_bg = (Sprite*)_shopLayer->getChildByTag(40);
// 
// 	auto goldLabel = (ui::TextBMFont*)shop_bg->getChildByTag(130);
// 	goldLabel->setString(StringUtils::format("%d", UserData::getInstance()->getGold()));

	
	int tag = 41;
	for (int i = 0; i < 8; i ++)
	{
		auto imageView = (ui::ImageView*)(shop_bg->getChildByTag(tag++));
		imageView->addClickEventListener(CC_CALLBACK_1(ShopLayer::buyGold, this));
	}

	auto buttonReturn = (ui::Button*)shop_bg->getChildByTag(50);
	buttonReturn->addClickEventListener(CC_CALLBACK_1(ShopLayer::menuReturnCallBack, this));

	schedule(CC_SCHEDULE_SELECTOR(ShopLayer::setGoldNum), 0.3f);

	return true;
}
void ShopLayer::setGoldNum(float delay)
{
	auto shop_bg = (Sprite*)_shopLayer->getChildByTag(40);
	auto goldLabel = (ui::TextBMFont*)shop_bg->getChildByTag(130);
	goldLabel->setString(StringUtils::format("%d", UserData::getInstance()->getGold()));
}

void ShopLayer::menuReturnCallBack(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}
void ShopLayer::buyGold(Ref* Psender)
{
	auto imageView = dynamic_cast<ui::ImageView*>(Psender);
	auto tag = imageView->getTag();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 	// payGold(tag - 39);
#endif

}
void buyGoldAddGold(const int i)
{
	if (i == 1)
	{
		UserData::getInstance()->setNewGift(false);
	}
	

	Director::getInstance()->startAnimation();
	UserData::getInstance()->addGold(GoldRMB[i]);
}
void setMusicOn(int IsOn)
{
/*	if(IsOn == 1){
		UserData::getInstance()->addGold(12);
	}else{
		UserData::getInstance()->addGold(13);
	}*/
	//UserData::getInstance()->addGold(IsOn);
	bool isOn = (bool)IsOn;
	UserData::getInstance()->setIsMusicOn(isOn);

}


