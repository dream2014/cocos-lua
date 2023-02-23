#include "PropLayer.h"
#include "GameScene.h"
#include "UserData.h"
#include "GameConst.h"
#include "PauseLayer.h"
#include "GiftLayer.h"
#include "StartScene.h"

#include "editor-support/cocostudio/SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;
using namespace cocostudio;

enum NodeTag
{
	Tag_time = 100,
	Tag_score,
	tag_bestScore,
	tag_level,
	tag_currentTime
};


bool PropLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	setTime(UserData::getInstance()->getRETime());
	//UserData::getInstance()->setLevel(1);
	_propLayer = CSLoader::createNode("PropLayer.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	_propLayer->setContentSize(frameSize);
	ui::Helper::doLayout(_propLayer);
	addChild(_propLayer);

	auto visibleSize = Director::getInstance()->getVisibleSize();

// 
// 	auto bomb =  (ui::ImageView*)_propLayer->getChildByTag(24);
// 	bomb->addClickEventListener(CC_CALLBACK_1(PropLayer::menuBombCallBack,this));
// 
// 	auto bombText = (ui::TextBMFont*)bomb->getChildByTag(152);
// 	bombText->setString(StringUtils::format("%d", UserData::getInstance()->getBomb()));
// 
// 
	auto hourglass = (ui::ImageView*)_propLayer->getChildByTag(80);
	hourglass->addClickEventListener(CC_CALLBACK_1(PropLayer::menuHourglassCallBack,this));
	setGlassNum();

// 	auto colorBubble = (ui::ImageView*)_propLayer->getChildByTag(80);
// 	colorBubble->addClickEventListener(CC_CALLBACK_1(PropLayer::menuColorBubbleCallBack,this));
// 	auto colorBubbleText = (ui::TextBMFont*)colorBubble->getChildByTag(81);
// 	colorBubbleText->setString(StringUtils::format("%d", UserData::getInstance()->getColorBubble()));


	auto bomb = (ui::CheckBox*)_propLayer->getChildByTag(50);
	bomb->addEventListener(CC_CALLBACK_2(PropLayer::BombCheckBoxCallBack, this));
	setBombNum();
// 	auto bombText = (ui::TextBMFont*)bomb->getChildByTag(77);
// 	bombText->setString(StringUtils::format("%d", UserData::getInstance()->getBomb()));


	auto color = (ui::CheckBox*)_propLayer->getChildByTag(74);
	color->addEventListener(CC_CALLBACK_2(PropLayer::ColorCheckBoxCallBack, this));
	setColorBubbleNum();

// 	auto glass = (ui::CheckBox*)_propLayer->getChildByTag(75);
// 	glass->addEventListener(CC_CALLBACK_2(PropLayer::GlassCheckBoxCallBack, this));
// 	auto glassText = (ui::TextBMFont*)glass->getChildByTag(79);
// 	glassText->setString(StringUtils::format("%d", UserData::getInstance()->getHourglass()));


	auto pauseMenu = (ui::Button*)_propLayer->getChildByTag(36);
	pauseMenu->addClickEventListener(CC_CALLBACK_1(PropLayer::menuPauseCallBack, this));

	auto swapImage = (ui::ImageView*)_propLayer->getChildByTag(49);
	swapImage->addClickEventListener(CC_CALLBACK_1(PropLayer::menuSwapCallBack, this));

	setGoldNum();
	AddScoreLabel(0);
	schedule(CC_SCHEDULE_SELECTOR(PropLayer::setGoldNumDelay), 0.2f);
	scheduleUpdate();
	return true;
}
void PropLayer::setGoldNumDelay(float delay)
{
	setGoldNum();
}
void PropLayer::setBombNum()
{
	auto bomb = (ui::CheckBox*)_propLayer->getChildByTag(50);
	auto bombText = (ui::TextBMFont*)bomb->getChildByTag(146);

	auto bombNum = UserData::getInstance()->getBomb();
	auto bombBg = (Sprite*)bomb->getChildByTag(143);
	auto bombPrise = (ui::TextBMFont*)bomb->getChildByTag(77);
	std::string str = StringUtils::format("%d", bombNum);
	if (bombNum >= 100)
	{
		str == "99";
	}
	else if (bombNum <= 0)
	{
		str = "";
		bombBg->setVisible(false);
		bombPrise->setString("$8");
	}
	else
	{
		bombPrise->setString("");
		bombBg->setVisible(true);
	}
	bombText->setString(str);

}

void PropLayer::setGlassNum()
{
 	auto hourglass = (ui::ImageView*)_propLayer->getChildByTag(80);
// 	hourglass->addClickEventListener(CC_CALLBACK_1(PropLayer::menuHourglassCallBack, this));
	auto hourBg = (Sprite*)hourglass->getChildByTag(145);
	auto hourPrise = (ui::TextBMFont*)hourglass->getChildByTag(81);

	auto hourglassText = (ui::TextBMFont*)hourglass->getChildByTag(148);
	auto glassNum = UserData::getInstance()->getHourglass();
	std::string str = StringUtils::format("%d", glassNum);

	if (glassNum >= 100)
	{
		str == "99";
	}
	else if (glassNum <= 0)
	{
		str = "";
		hourBg->setVisible(false);
		hourPrise->setString("$10");
	}
	else
	{
		hourPrise->setString("");
		hourBg->setVisible(true);
	}
	hourglassText->setString(str);


}

void PropLayer::BombNormal()
{
	auto bomb = (ui::CheckBox*)_propLayer->getChildByTag(50);
	if (bomb->isSelected())
	{
		bomb->setSelected(false);
	}
}
void PropLayer::ColorNormal()
{
	auto color = (ui::CheckBox*)_propLayer->getChildByTag(74);
	if (color->isSelected())
	{
		color->setSelected(false);
	}
}



void PropLayer::BombCheckBoxCallBack(Ref* Psender, ui::CheckBox::EventType type)
{

	auto bomb = dynamic_cast<ui::CheckBox*>(Psender);
	
	if (UserData::getInstance()->getUserProp() == false)
	{
		bomb->setSelected(false);
		return;
	}


	useProp();
	
	auto gameSceme = (GameScene*)this->getParent();
	if (bomb->isSelected())
	{
		if (UserData::getInstance()->getBomb() <= 0)
		{
			if (UserData::getInstance()->getGold() >= 8)
			{
				UserData::getInstance()->addGold(-8);
				UserData::getInstance()->addBomb(1);
			}
			else
			{
				auto giftLayer = GiftLayer::create(GiftType::Type_Bubble);
				giftLayer->isGameScene = true;
				gameSceme->mPause();
				gameSceme->addChild(giftLayer);
				bomb->setSelected(false);
				setBombNum();
				setGoldNum();
				return;
			}
		}

		UserData::getInstance()->addBomb(-1);
		gameSceme->bombBubble(true);
	}
	else
	{
		UserData::getInstance()->addBomb(1);
		gameSceme->bombBubble(false);
	}

	setBombNum();
	setGoldNum();

}

void PropLayer::ColorCheckBoxCallBack(Ref* Psender, ui::CheckBox::EventType type)
{

	auto color = dynamic_cast<ui::CheckBox*>(Psender);

	if (UserData::getInstance()->getUserProp() == false)
	{
		color->setSelected(false);
		return;

	}


	useProp();

	auto gameSceme = (GameScene*)this->getParent();
	if (color->isSelected())
	{
		if (UserData::getInstance()->getColorBubble() <= 0)
		{
			if (UserData::getInstance()->getGold() >= 5)
			{
				UserData::getInstance()->addGold(-5);
				UserData::getInstance()->addColorBubble(1);
			}
			else
			{
				auto giftLayer = GiftLayer::create(GiftType::Type_Bubble);
				giftLayer->isGameScene = true;
				gameSceme->mPause();
				gameSceme->addChild(giftLayer);
				color->setSelected(false);
				setColorBubbleNum();
				setGoldNum();
				return;
			}
		}

 		UserData::getInstance()->addColorBubble(-1);
 		gameSceme->colorBubble(true);
	}
	else
	{
		UserData::getInstance()->addColorBubble(1);
		gameSceme->colorBubble(false);
	}
	setColorBubbleNum();
	setGoldNum();
}


void PropLayer::setColorBubbleNum()
{
	auto colorBubble = (ui::CheckBox*)_propLayer->getChildByTag(74);
	auto colorBubbleText = (ui::TextBMFont*)colorBubble->getChildByTag(147);
	auto colorNum = UserData::getInstance()->getColorBubble();
	std::string str = StringUtils::format("%d", colorNum);
	auto colorBg = (Sprite*)colorBubble->getChildByTag(144);
	auto colorPrice = (ui::TextBMFont*)colorBubble->getChildByTag(78);
	if (colorNum >= 100)
	{
		str = "99";
	} else if (colorNum <= 0)
	{
		str = "";
		colorBg->setVisible(false);
		colorPrice->setString("$5");
	}
	else
	{
		colorPrice->setString("");
		colorBg->setVisible(true);
	}
	
	colorBubbleText->setString(str);
	setGlassNum();
}
// 
// void PropLayer::menuBombCallBack(Ref* Psender)
// {
// 	if (UserData::getInstance()->getBomb() <= 0)
// 	{
// 		return;
// 	}
// 	//UserData::getInstance()->addBomb(-1);
// 
// 
// 	auto gameSceme = (GameScene*)this->getParent();
// 	gameSceme->bombBubble();
// }
void PropLayer::menuHourglassCallBack(Ref* Psender)
{
	auto gameSceme = (GameScene*)this->getParent();
	if (UserData::getInstance()->getHourglass() <= 0)
	{

		if (UserData::getInstance()->getGold() >= 10)
		{
			UserData::getInstance()->addGold(-10);
			UserData::getInstance()->addHourglass(1);
		}
		else
		{
			auto giftLayer = GiftLayer::create(GiftType::Type_Bubble);
			giftLayer->isGameScene = true;
			gameSceme->mPause();
			gameSceme->addChild(giftLayer);
			setGlassNum();
			setGoldNum();
			return;
		}
	}
	UserData::getInstance()->addHourglass(-1);
	auto hourglass = (ui::ImageView*)_propLayer->getChildByTag(80);
/*	hourglass->addClickEventListener(CC_CALLBACK_1(PropLayer::menuHourglassCallBack, this));*/
// 	auto hourglassText = (ui::TextBMFont*)hourglass->getChildByTag(151);
// 	hourglassText->setString(StringUtils::format("%d", UserData::getInstance()->getHourglass()));
	setGlassNum();
	setGoldNum();
	auto time = getTime() + HOURGLASSTIME;
	if (time >= 60)
	{	
		time = 60;
	}
	setTime(time);
}

// void PropLayer::menuColorBubbleCallBack(Ref* Psender)
// {
// 	if (UserData::getInstance()->getColorBubble() <= 0)
// 	{
// 		return;
// 	}
// 	//UserData::getInstance()->addColorBubble(-1);
// 
// 
// 	auto gameSceme = (GameScene*)this->getParent();
// 	gameSceme->colorBubble();
// }
void PropLayer::menuPauseCallBack(Ref* Psender)
{

	auto gameScene = (GameScene*)this->getParent();
	gameScene->mPause();
	// 	auto layer = PauseLayer::create();
	// 	gameSceme->addChild(layer);
	auto size = Director::getInstance()->getWinSize();
	auto giftLayer = GiftLayer::create(GiftType::Type_Bubble);
	giftLayer->isGameScene = true;
	gameScene->addChild(giftLayer);
	giftLayer->_giftNode->setPosition(Vec2(size.width / 2, size.height / 2 + 50));
	//auto buttonHome = ui::Button::create("Pause/returnHome.png", "Pause/returnHome_press.png", "Pause/returnHome_press.png");
// 	auto buttonHome = ui::Button::create();
// 	buttonHome->loadTextureNormal("Pause/returnHome.png");
// 	buttonHome->loadTexturePressed("Pause/returnHome_press.png");
	//auto homeImageView = ui::ImageView::create("returnHome.png");
	//auto homeImageView = ui::Button::create("returnHome.png", "returnHome_press.png");
	auto homeImageView = MenuItemSprite::create(Sprite::createWithSpriteFrameName("return_home.png"), Sprite::createWithSpriteFrameName("return_home_press.png"), [=](Ref* Psender){
		gameScene->_bubbleLayer->savaCurrentBoard();
		UserData::getInstance()->setRETime(gameScene->_propLayer->getTime());
		Director::getInstance()->replaceScene(StartLayer::scene());
	});
	homeImageView->setPosition(Vec2(150, 110));

	auto continueItem = MenuItemSprite::create(Sprite::createWithSpriteFrameName("continue.png"), Sprite::createWithSpriteFrameName("continue_press.png"), [=](Ref* Psender){
		gameScene->mResume();
		gameScene->removeChild(giftLayer);
	});
	continueItem->setPosition(Vec2(380, 110));

	auto menu = Menu::create(homeImageView, continueItem, nullptr);
	giftLayer->addChild(menu);
	menu->setPosition(Vec2::ZERO);



}

void PropLayer::menuSwapCallBack(Ref* Psender)
{
	auto gameScene = (GameScene*)this->getParent();
	gameScene->swapBubble();
}

void PropLayer::update(float delta)
{
	setTime(getTime() - 1.0/60.0);
	float time = getTime();
	if (time >= 60.0)
	{
		time = 60;
	}
	auto timeLabel = (ui::TextBMFont*)_propLayer->getChildByTag(131);
 	timeLabel->setString(StringUtils::format("%d", (int)time));


	auto loadingBar = (ui::LoadingBar*)_propLayer->getChildByTag(29);
	loadingBar->setPercent(getTime()/60.0 * 100);

	auto armature = (Armature*)loadingBar->getChildByTag(125);
	float wight = loadingBar->getContentSize().width;
	armature->setPosition(211- wight*(1 - getTime() / 60.0), 13);

	auto levelLabel = (ui::TextBMFont*)_propLayer->getChildByTag(122);
	auto level = StringUtils::format("%d", UserData::getInstance()->getLevel());
	auto timeSptite = (Sprite*)_propLayer->getChildByTag(46);

	if (levelLabel->getString() != level)
	{
		levelLabel->setString(level);
	}
	if (getTime() <= 0)
	{
		auto gameSceme = (GameScene*)this->getParent();
		gameSceme->gameOver();
	}
	else if (getTime() <= 5)
	{
		
		auto currentTime = (ui::TextBMFont*)timeSptite->getChildByTag(132);
		if (currentTime->getString() != StringUtils::format("%d", (int)getTime()))
		{
			
			SimpleAudioEngine::getInstance()->playEffect("Music/Clock.mp3");
			currentTime->setString(StringUtils::format("%d", (int)getTime()));
		}
		
		if (!timeSptite->isVisible())
		{
 			auto point = timeSptite->getPosition();
			timeSptite->setPosition(650, point.y);
 			timeSptite->setVisible(true);
			auto moveAction = MoveTo::create(0.5f, point);
			auto speedAction = EaseSineOut::create(moveAction);
			auto callFunc = CallFunc::create(CC_CALLBACK_0(PropLayer::timeCallFunc, this));
			auto seq = Sequence::create(speedAction, callFunc, nullptr);
 			timeSptite->runAction(seq);
		}
	}
	else
	{
		if (timeSptite->isVisible())
		{
			timeSptite->setVisible(false);
		}
	}
}
void PropLayer::AddScoreLabel(int var)
{
	auto scoreLabel = (ui::TextBMFont*)_propLayer->getChildByTag(123);
	UserData::getInstance()->addScore(var);
	scoreLabel->setString(StringUtils::format("%d", UserData::getInstance()->getScore()));
}
void PropLayer::setCannonAngle(Point target)
{
	auto cannon = (Sprite*)_propLayer->getChildByTag(38);
	auto angle = (target - READY_PAOPAO_POS).getAngle(Vec2(0, 1));
	cannon->setRotation(CC_RADIANS_TO_DEGREES(angle));
}
void PropLayer::setCannonAction(Point target)
{
	auto cannon = (Sprite*)_propLayer->getChildByTag(38);
	auto angle = cannon->getRotation();

	auto dir = -10 * (target - READY_PAOPAO_POS).getNormalized();
	auto moveBy = MoveBy::create(0.1f, dir);
	auto seq = Sequence::create(moveBy, moveBy->reverse(), nullptr);
	cannon->runAction(seq);
}
void PropLayer::timeCallFunc()
{
	auto timeSptite = (Sprite*)_propLayer->getChildByTag(46);
	auto time_bg = (Sprite*)timeSptite->getChildByTag(45);
	auto rotateAction = RotateBy::create(1.0f, -360.0f);
	auto repeatAction = RepeatForever::create(rotateAction);
	time_bg->runAction(repeatAction);
}
void PropLayer::setLineVisible(bool enable)
{
	auto armature = (Armature*)_propLayer->getChildByTag(40);
	armature->setVisible(enable);
}
void PropLayer::useProp()
{
	auto sprite = (Sprite*)_propLayer->getChildByTag(38);
	auto armature = (Armature*)sprite->getChildByTag(124);
	armature->getAnimation()->play("shandian");
}
void PropLayer::setGoldNum()
{
	auto goltFnt = (ui::TextBMFont*)_propLayer->getChildByTag(114);
	goltFnt->setString(StringUtils::format("$%d", UserData::getInstance()->getGold()));
}
