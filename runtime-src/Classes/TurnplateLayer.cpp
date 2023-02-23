#include "TurnplateLayer.h"
#include "StartScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UserData.h"
#include <ctime>

using namespace cocostudio::timeline;

bool TurnplateLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Turnplate/Turn.plist");
	_turnplateNode = CSLoader::createNode("TurnplateLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	_turnplateNode->setContentSize(frameSize);
	ui::Helper::doLayout(_turnplateNode);

	addChild(_turnplateNode);

	auto rotateMenu = (ui::Button*)_turnplateNode->getChildByTag(16);
	rotateMenu->addClickEventListener(CC_CALLBACK_1(TurnplateLayer::menuRotateCallBack, this));

	auto returnMenu = (ui::Button*)_turnplateNode->getChildByTag(33);
	returnMenu->addClickEventListener(CC_CALLBACK_1(TurnplateLayer::menuReturnCallBack, this));

	return true;

}

void TurnplateLayer::menuRotateCallBack(Ref* Pesender)
{
//  	if (UserData::getInstance()->getHaveRaffle() || _isPress)
//  	{
//  		return;
//  	}
	if (_isPress)
	{
		return;
	}
	_isPress = true;
	auto turnplate = (Sprite*)_turnplateNode->getChildByTag(20);
	//auto angle = 720.0f + (int)(CCRANDOM_0_1()*6) * 60;
	


	//srand(unsigned int(time(0)));

	int random = (int)(CCRANDOM_0_1() * 200);
	CCLOG("%d", random);
	float angle = 0;
	if (random / 10 >= 19)
	{
		angle = 60.0f;
	}
	else if (random / 10 >= 17)
	{
		angle = 120.0f;
	}
	else if (random / 10 >= 15)
	{	
		angle = 360.0f;
	}
	else if (random / 10 >= 10)
	{
		angle = 240.0f;
	}else if (random /10 >= 7)
	{
		angle = 180.0f;
	}
	else
	{
		angle = 300.0f;
	}
	auto angleNum = 1440.0f + (int)(CCRANDOM_0_1() * 2) * 360.0f + angle;
	auto action = RotateTo::create(2.0f, angleNum);
	auto sineOut = EaseSineOut::create(action);
	auto callfunc = CallFunc::create(CC_CALLBACK_0(TurnplateLayer::addProps,this));
	auto seq = Sequence::create(sineOut, callfunc, nullptr);
	turnplate->runAction(seq);
	UserData::getInstance()->setHaveRaffle(true);
}
void TurnplateLayer::menuReturnCallBack(Ref* Psender)
{

	this->removeFromParentAndCleanup(true);
}
void TurnplateLayer::addProps()
{
	auto turnplate = (Sprite*)_turnplateNode->getChildByTag(20);
	int angle = (int)turnplate->getRotation() % 360;
	
	auto visibleSize = Director::getInstance()->getVisibleSize();

	std::string giftName = "";
	switch (angle)
	{
	case 0: UserData::getInstance()->addBomb(2); giftName = "zhadan1.png"; break;
	case 60: UserData::getInstance()->addGold(20); giftName = "jewel_2.png";  break;
	case 120: UserData::getInstance()->addColorBubble(2); giftName = "caihong1.png"; break;
	case 180: UserData::getInstance()->addHourglass(1); giftName = "shalou.png"; break;
	case 240: UserData::getInstance()->addBomb(1); giftName = "zhadan1.png"; break;
	case 300: UserData::getInstance()->addColorBubble(1); giftName = "caihong1.png"; break;
	default:
		break;
	}


	auto layerColor = LayerColor::create(Color4B(0,0,0,100));

 	this->addChild(layerColor);


 	auto gift = Sprite::createWithSpriteFrameName(giftName);


	auto node = CSLoader::createNode("Turnplate/TurnGiftNode.csb");
	layerColor->addChild(node);
	node->setPosition(visibleSize / 2);
	layerColor->addChild(gift);
	gift->setPosition(visibleSize / 2);
	if (angle == 0 || angle == 120)
	{
		auto sprite = Sprite::createWithSpriteFrameName("product.png");
		layerColor->addChild(sprite);
		sprite->setPosition(Vec2(visibleSize.width / 2 + 20, visibleSize.height / 2 + 20));
	}
	
	auto listenser = EventListenerTouchOneByOne::create();
	listenser->setSwallowTouches(true);
	listenser->onTouchBegan = [](Touch *touch, Event *unused_event){return true; };
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenser, layerColor);
 	
	auto button = (ui::Button*)node->getChildByTag(37);
	button->addClickEventListener([=](Ref *psender){this->removeFromParent(); });
// 
// 	//auto getButton = ui::Button::create("Gift/get.png", "Gift/get.png");
// 	auto getButton = ui::Button::create("get.png", "get.png");
// 	getButton->addClickEventListener([=](Ref *psender){this->removeFromParent(); });
// 	layerColor->addChild(getButton);
// 	getButton->setPosition(Vec2(visibleSize.width/ 2,visibleSize.height/ 4));

	_isPress = false;
	
}
