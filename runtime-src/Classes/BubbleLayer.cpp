#include "BubbleLayer.h"
#include "GameScene.h"
#include "UserData.h"
#include "editor-support/cocostudio/SimpleAudioEngine.h"
#include "PropLayer.h"

using namespace CocosDenshion;
using namespace cocostudio::timeline;


static float waitTime = 0.1f;
static int sameSum = 0;
static int bubbleInitNum = 0;
static int downBubbleNum = 0;
static int baozhaNum = 0;
static int tiaoyueNum = 0;

Scene* BubbleLayer::scene()
{
		Scene *scene = Scene::create();

		BubbleLayer *layer = BubbleLayer::create();

		scene->addChild(layer);

		return scene;
}

bool BubbleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_level = UserData::getInstance()->getLevel() - 1;
	
	//UserData::getInstance()->setLevel(1);

	//Texture2D::PVRImagesHavePremultipliedAlpha(true);
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Praise.plist");

	_listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(true);
	_listener->onTouchBegan = CC_CALLBACK_2(BubbleLayer::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(BubbleLayer::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(BubbleLayer::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, this);

	initWaitPaoPao();
	initReadyPaoPao();

	armatures = Vector<Armature*>(30);
	for (int i=0; i<30; i++)
	{
		auto armature = Armature::create("baozha");
		armatures.pushBack(armature);
	}

	//schedule(schedule_selector(BubbleLayer::setDetermineLineVisible, this), 0.1f);

	return true;
}

bool BubbleLayer::initTheBoard(int level)
{


	setDisable();
	bool bRet = false;
	bubbleInitNum = 0;
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			board[i][j] = NULL;
		}
		bRet = true;
	}
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (i%2 && j == MAX_COLS - 1 || customs[level][i][j] == 0)
			{
				continue;
			}
			if (customs[level][i][j] == -1)
			{
				board[i][j] = randomPaoPao();
			}
			else if (customs[level][i][j] != 0)
			{
				board[i][j] = Bubble::initWithType((BubbleType)customs[level][i][j]);
			}
			bubbleInitNum += 1;
			bool flag = (i % 2) == 0 ? true : false;
			board[i][j]->setFlag(flag);
			addChild(board[i][j]);
			initBubbleAction(board[i][j], i, j);
		}
	}
	SimpleAudioEngine::getInstance()->playEffect("Music/start.mp3");
	auto visible = Director::getInstance()->getVisibleSize();
	auto spriteReady = Sprite::createWithSpriteFrameName("ready.png");
	this->addChild(spriteReady);
	spriteReady->setPosition(Vec2(visible.width /2, visible.height/2 + 20));
	spriteReady->setScale(0);
	spriteReady->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), CallFunc::create([=](){
		spriteReady->removeFromParent(); 
		auto goSprite = Sprite::createWithSpriteFrameName("go.png");
		this->addChild(goSprite);
		goSprite->setPosition(Vec2(visible.width / 2, visible.height / 2 + 20));
		goSprite->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.8f, 1.2f), FadeOut::create(0.8f), nullptr), CallFunc::create([=](){goSprite->removeFromParent(); setEnable(); }), nullptr));
	}), nullptr));

	return bRet;
}

Bubble * BubbleLayer::randomPaoPao()
{
	Bubble *pRet = NULL;
	BubbleType type = static_cast<BubbleType>(rand() % BUBBLE_COUNT + 1);
	pRet = Bubble::initWithType(type);
	return pRet;
}

Point BubbleLayer::getPointByRowAndCol(int row, int col)
{
	bool flag = row % 2 == 0 ? true : false;
	Size winSize = Director::getInstance()->getWinSize();
	Point pos = Point((col * 2+ 1) * (BUBBLE_RADIUS + 1.5) + (flag ? 0 : BUBBLE_RADIUS), TOUCH_TOP * winSize.height - row * (BUBBLE_RADIUS * 2 - 5) - BUBBLE_RADIUS);
	
	return pos;
}
Vec2 BubbleLayer::getRowAndColByPoint(Point target)
{
	Size winSize = Director::getInstance()->getWinSize();
	int x = (TOUCH_TOP * winSize.height - target.y) / (BUBBLE_RADIUS * 2 - 6);
	int y = ((target.x - (x % 2) * BUBBLE_RADIUS) / ((BUBBLE_RADIUS + 1.5) * 2));
	return Vec2(x, y);
}
void BubbleLayer::initWaitPaoPao()
{
	for (int i = 0; i < MAX_WAIT_PAOPAO; ++i)
	{
		Bubble *obj = randomPaoPao();
		obj->setPosition(WAIT_PAOPAO_POS);
		wait[i] = obj;
		this->addChild(obj);
	}
}
void BubbleLayer::initReadyPaoPao()
{
	ready = randomPaoPao();
	ready->setPosition(READY_PAOPAO_POS);
	this->addChild(ready);
}
void BubbleLayer::onTouch(Point target)
{
	UserData::getInstance()->setUserProp(false);
	this->real = (target - ready->getPosition()).getNormalized();
	auto gameScene = (GameScene*)this->getParent();
	auto proplayer = (PropLayer*)gameScene->_propLayer;
	switch (ready->getType())
	{
		case BUBBLE_TYPE_COLOR: /*UserData::getInstance()->addColorBubble(-1);*/ 
			proplayer->ColorNormal();
			break;
		case BUBBLE_TYPE_BOMB: /*UserData::getInstance()->addBomb(-1); */
			proplayer->BombNormal();
			break;
		default: break;
	}
	this->scheduleUpdate();
}


void BubbleLayer::setEnable(){
	_listener->setEnabled(true);
	//
}

void BubbleLayer::setDisable(){

	_listener->setEnabled(false);
	//UserData::getInstance()->setUserProp(true);


}

void BubbleLayer::update(float fDelta) {
	
	if (isCollideBorder()) 	
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Hitwall.mp3");
		real.x = -real.x; 	
	}
	setDisable();
	Point pos = ready->getPosition();
	ready->setPosition(Point(pos.x + real.x * PAOPAO_SPEED, pos.y + real.y * PAOPAO_SPEED));
	if (checCollideBoard()) 
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Hit.mp3");
		this->unscheduleUpdate();
		real = Point::ZERO;
 		correctReadyPosition();
// 		readyAction();
		return;
	}
}

bool BubbleLayer::isCollideBorder()
{
	bool bRet = false;
	Size size = Director::getInstance()->getWinSize();
	Point pos = ready->getPosition();
	if (pos.x + BUBBLE_RADIUS > size.width || pos.x - BUBBLE_RADIUS / 2 < 0)
	{
		bRet = true;
	}
	return bRet;
}

bool BubbleLayer::checCollideBoard()
{
	bool bRet = false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto point = ready->getPosition();
	if (point.y >= (TOUCH_TOP* visibleSize.height - BUBBLE_RADIUS))
	{
		if (ready->getType() == BUBBLE_TYPE_COLOR)
		{
			BubbleType type = (BubbleType)((int)(CCRANDOM_0_1() * 7 + 1));
			ready->setType(type);
			ready->initWithSpriteFrameName(StringUtils::format(BUBBLE_NAME.c_str(), type));
		}
		return true;
	}

	std::vector<Vec2> rowCol;
	rowCol.push_back(getRowAndColByPoint(Point(point.x, point.y + BUBBLE_RADIUS)));
	rowCol.push_back(getRowAndColByPoint(Point(point.x - BUBBLE_RADIUS, point.y)));
	rowCol.push_back(getRowAndColByPoint(Point(point.x + BUBBLE_RADIUS, point.y)));

	for (auto ti : rowCol)
	{
		//auto rowCol = getRowAndColByPoint(point);
		int row = ti.x;
		int col = ti.y;
		if (row < MAX_ROWS && row >= 0 && col < MAX_COLS && col >= 0 && board[row][col] != nullptr)
		{
			if (ready->getType() == BUBBLE_TYPE_COLOR)
			{
				BubbleType type = board[row][col]->getType();

				if (type == BUBBLE_TYPE_COLOR)
				{
					type = BUBBLE_TYPE1;
				}

				ready->setType(type);
				ready->initWithSpriteFrameName(StringUtils::format(BUBBLE_NAME.c_str(), type));
			}
			return true;
		}
	}

// 	for (int i = 0; i < MAX_ROWS; i++)
// 	{
// 		for (int j = 0; j < MAX_COLS; j++)
// 		{
// 			if (board[i][j]!= nullptr)
// 			{
// 				if (isCircleCollision(board[i][j]->getPosition(), BUBBLE_RADIUS, ready->getPosition(),BUBBLE_RADIUS))
// 				{
// 					if (ready->getType() == BUBBLE_TYPE_COLOR)
// 					{
// 						BubbleType type = board[i][j]->getType();
// 						ready->setType(type);
// 						ready->initWithSpriteFrameName(StringUtils::format(BUBBLE_NAME.c_str(), type));
// 					}
// 					
// 					return true;
// 				}
// 			}
// 		}
// 	}



	return false;
}

void BubbleLayer::changeWaitToReady()
{
	
	ready = wait[0];
	auto jumpAction = JumpTo::create(0.08f, READY_PAOPAO_POS, 30.0f, 1);

	auto callFunc = CallFunc::create(CC_CALLBACK_0(BubbleLayer::jumpActionCallBack, this));

	auto seq = Sequence::create(jumpAction, callFunc, nullptr);
	//setEnable();
	ready->runAction(seq);

}

void BubbleLayer::correctReadyPosition()
{
	int offX = 0, offY = 0;
	int row = 0, col = 0;
	Point pos = ready->getPosition();
	Vec2 rowCol = getRowAndColByPoint(pos);
	offX = rowCol.x == 0 ? 0 : rowCol.x - 1;
	offY = rowCol.y == 0 ? 0 : rowCol.y - 1;
	float length = FLT_MAX;
	bool flag = (int)(rowCol.x + 1) % 2 == 0 ? true : false;
	bool tempFlag = flag;
	
	for (int i = offX; i < MAX_ROWS && i < offX + 4; i++)
	{
		for (int j = offY; j < MAX_COLS && j < offY + 4; j++)
		{
			if (board[i][j] == nullptr)
			{
				if (j == MAX_COLS - 1 && !flag)
				{
					continue;
				}
				Point tPos = getPointByRowAndCol(i, j);
				if (tPos.getDistance(pos) < length)
				{
					row = i;
					col = j;
					tempFlag = flag;
					length = tPos.getDistance(pos);
				}
			}
		}
		flag = !flag;
	}
	//ready->runAction(MoveTo::create(0.1f, getPointByRowAndCol(row, col)));
	//ready->setPosition(getPointByRowAndCol(row, col));
	board[row][col] = ready;
	board[row][col]->setFlag(tempFlag);
	moveTheBubble(row, col, tempFlag, MOVE_DISTANCE, true);
// 	if (getPointByRowAndCol(row, col).y - BUBBLE_RADIUS <= TOUCH_DOWN * (Director::getInstance()->getVisibleSize().height))
// 	{
// 		gameOver(true);
// 		return;
// 	}
	//ready->setPosition(getPointByRowAndCol(row, col));
	setDetermineLineVisible(0.1f);
	//readyAction();
	ready->runAction(Sequence::create(MoveTo::create(0.1f, getPointByRowAndCol(row, col)), DelayTime::create(0.1f), CallFunc::create(CC_CALLBACK_0(BubbleLayer::readyAction, this)), nullptr));
	
}

void BubbleLayer::readyAction()
{
	setDisable();
	Vec2 RowAndCol = getRowAndColByPoint(ready->getPosition());
	int row = RowAndCol.x;
	int col = RowAndCol.y;
	bool tempFlag = row % 2 == 0 ? true : false;
	if (ready->getType() == BUBBLE_TYPE_BOMB)
	{
		SimpleAudioEngine::getInstance()->playEffect("Music/Bomb.mp3");
		auto armature = Armature::create("daojubaozha");
		armature->getAnimation()->play("Animation1", -1,0);
		addChild(armature);
		armature->setPosition(ready->getPosition());
		armature->getAnimation()->setMovementEventCallFunc([=](Armature* arma, MovementEventType type, const std::string name){
		if (type == COMPLETE && name == "Animation1")
		{
			arma->removeFromParent();
		}
		});
		bubbleBlast(row, col, tempFlag);
		downBubbleNum = 0;
		for (int i = 0; i < MAX_COLS; i++)
		{
			if (board[0][i] != nullptr && !board[0][i]->getIsPass())
			{
				checkDownBubble(0, i);
			}
		}
		downBubble();
	}
	else
	{
		waitTime = 0;
		sameSum = 0;
		findTheSameBubble(row, col, tempFlag, ready->getType());
		deleteTheSameBubble(row, col, tempFlag);
	}
	resetAllPass();
	downBubbleNum = 0;
// 	for (int i = 0; i < MAX_COLS; i++)
// 	{
// 		if (board[0][i] != nullptr && !board[0][i]->getIsPass())
// 		{
// 			checkDownBubble(0, i);
// 		}
// 	}
// 	downBubble();
	throwBallAction();
	changeWaitToReady();
}
bool BubbleLayer::getFirstRowFlag()  //�õ���һ���Ƿ���ȱ ��ȱΪtrue
{
	for (int i = 0; i < MAX_COLS; ++i)
	{
		if (board[0][i]) return board[0][i]->getFlag();
	}
	return true;
}

bool BubbleLayer::isCircleCollision(Point pos1, float radius1, Point pos2, float radius2)
{
	if (pos1.getDistance(pos2) >(radius1 + radius2 - 10) || pos2.y > pos1.y)
	{
		return false;
	}
	return true;
}

void BubbleLayer::findTheSameBubble(int i, int j, bool flag, BubbleType type)
{	
	setDisable();
	if (i < 0 || i >= MAX_ROWS || j < 0 || j >= MAX_COLS) 	{
		return;
	}
	if (board[i][j] == nullptr) 	{
		return;
	}
	if (board[i][j]->getType() != type) 	{
		return;
	}
	if (board[i][j]->getIsSame()) 	{
		return;
	}
	//board[i][j]->stopAllActions();
	/*
	���Ƚ��Լ�isSame����Ϊtrue����������sameSum
	�ӵ�ǰλ�����ϣ����ϣ����£����£����������������Ѱ��
	*/
	board[i][j]->setIsSame(true);
	++sameSum;

	if (flag && j > 0 && i > 0) 	{
		findTheSameBubble(i - 1, j - 1, !flag, type); //����
	}
	else if (!flag && i > 0) 	{
		findTheSameBubble(i - 1, j, !flag, type); 
	}
	if (flag && i > 0) 	{ //����
		findTheSameBubble(i - 1, j, !flag, type);
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1) 	{ 
		findTheSameBubble(i - 1, j + 1, !flag, type);
	}
	if (flag && j > 0 && i < MAX_ROWS) 	{  //����
		findTheSameBubble(i + 1, j - 1, !flag, type);
	}
	else if (!flag && i < MAX_ROWS) 	{ 
		findTheSameBubble(i + 1, j, !flag, type);
	}
	if (flag && i < MAX_ROWS) 	{  // ����
		findTheSameBubble(i + 1, j, !flag, type);
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1) 	{
		findTheSameBubble(i + 1, j + 1, !flag, type);
	}
	if (j > 0) //��
	{
		findTheSameBubble(i, j - 1, flag, type);
	}
	if (j < MAX_COLS) //��
	{
		findTheSameBubble(i, j + 1, flag, type);
	}    
// 	if (sameSum>= 3)
// 	{
// 		auto obj = board[i][j];
// 		board[i][j] = nullptr;
// 		/*obj->removeFromParent();*/
// 		waitTime += 0.01f;
// 		obj->runAction(Sequence::create(DelayTime::create(waitTime), CallFuncN::create(CC_CALLBACK_1(BubbleLayer::callbackRemoveBubble, this)), nullptr));
// 	}
}
void BubbleLayer::bubbleBlast(int i, int j, bool flag)
{
	board[i][j]->stopAllActions();
	bubbleAction(board[i][j]);
	board[i][j] = nullptr;
	ready = nullptr;

	if (flag && j > 0 && i > 0 && board[i-1][j-1]) 	{
		bubbleAction(board[i - 1][j - 1]);
		board[i - 1][j - 1] = nullptr;
	}
	else if (!flag && i > 0 && board[i-1][j]) 	{
		bubbleAction(board[i - 1][j]);
		board[i - 1][j] = nullptr;
	}
	if (flag && i > 0 && board[i - 1][j]) 	{
		bubbleAction(board[i - 1][j]);
		board[i - 1][j] = nullptr;
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1 && board[i - 1][j + 1]) 	{
		bubbleAction(board[i - 1][j+1]);
		board[i - 1][j + 1] = nullptr;
	}
	if (flag && j > 0 && i < MAX_ROWS && board[i + 1][j - 1]) 	{
		bubbleAction(board[i + 1][j - 1]);
		board[i + 1][j - 1] = nullptr;
	}
	else if (!flag && i < MAX_ROWS && board[i + 1][j]) 	{
		bubbleAction(board[i + 1][j]);
		board[i + 1][j] = nullptr;
	}
	if (flag && i < MAX_ROWS && board[i + 1][j]) 	{
		bubbleAction(board[i + 1][j]);
		board[i + 1][j] = nullptr;
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1 && board[i + 1][j + 1]) 	{
		bubbleAction(board[i + 1][j+1]);
		board[i + 1][j + 1] = nullptr;
	}
	if (j > 0 && board[i][j - 1])
	{
		bubbleAction(board[i][j - 1]);
		board[i][j - 1] = nullptr;
	}
	if (j < MAX_COLS && board[i][j + 1])
	{
		bubbleAction(board[i][j + 1]);
		board[i][j + 1] = nullptr;
	}
}

void BubbleLayer::moveTheBubble(int i, int j, bool flag, float distance, bool isReady)
{

	if (distance <= 1.0f)
	{
		return;
	}
	if (i < 0 || i >= MAX_ROWS || j < 0 || j >= MAX_COLS) 	{
		return;
	}
	if (board[i][j] == nullptr){
		return;
	}
	if (board[i][j]->getIsMove()) 	{
		return;
	}

	board[i][j]->setIsMove(true);
	
	if (!isReady)
	{
		auto dir = (((Vec2)getPointByRowAndCol(i, j) - ready->getPosition()).getNormalized())*distance;
		auto moveBy = MoveBy::create(0.1f, dir);
		auto seq = Sequence::create(moveBy, moveBy->reverse(), nullptr);
		board[i][j]->runAction(seq);
	}
	flag = i % 2 == 0 ? true : false;
	if (flag && j > 0 && i > 0) 	{
		moveTheBubble(i - 1, j - 1, !flag, distance - 5, false);
	}
	else if (!flag && i > 0) 	{
		moveTheBubble(i - 1, j, !flag, distance - 5, false);
	}
	if (flag && i > 0) 	{
		moveTheBubble(i - 1, j, !flag, distance - 5, false);
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1) 	{
		moveTheBubble(i - 1, j + 1, !flag, distance - 5, false);
	}
	if (flag && j > 0 && i < MAX_ROWS) 	{
		moveTheBubble(i + 1, j - 1, !flag, distance - 5, false);
	}
	else if (!flag && i < MAX_ROWS) 	{
		moveTheBubble(i + 1, j, !flag, distance - 5, false);
	}
	if (flag && i < MAX_ROWS) 	{
		moveTheBubble(i + 1, j, !flag, distance - 5, false);
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1) 	{
		moveTheBubble(i + 1, j + 1, !flag, distance - 5, false);
	}
	if (j > 0)
	{
		moveTheBubble(i, j - 1, flag, distance - 5, false);
	}
	if (j < MAX_COLS)
	{
		moveTheBubble(i, j + 1, flag, distance - 5, false);
	}
}

void BubbleLayer::deleteTheSameBubble(int i, int j, bool flag) //��������������ʱ��ͨ��Բ��ʽ�㷨�жϷ�Χ
{
	if (sameSum < 3)
	{
		for (int i = 0; i < MAX_ROWS; ++i) 	{
			for (int j = 0; j < MAX_COLS; ++j)
			{
				if (board[i][j] != nullptr && board[i][j]->getIsSame())
				{
					board[i][j]->setIsSame(false);
				}
			}
		}
		setEnable();
		if (isGameover())
		{
			gameOver(true);
			return;
		}
		
	}
	else
	{
		auto gameScene = (GameScene*)(this->getParent()); // ���ӵ���ķ���
		gameScene->_propLayer->AddScoreLabel(5 * sameSum);
		if (sameSum >= 5)
		{
			int mrand = CCRANDOM_0_1() * 4;
			string praiseName;
			switch (mrand)
			{
			case 0: praiseName = "beautiful.png"; break;
			case 1: praiseName = "ferfect.png"; break;
			case 2: praiseName = "praise.png"; break;
			case 3: praiseName = "duang.png"; break;
			default:
				praiseName = "duang.png";
				break;
			}
			//SimpleAudioEngine::getInstance()->playEffect("Music/good.mp3");

			auto praise = Sprite::createWithSpriteFrameName(praiseName);
			addChild(praise);
			praise->setPosition(getPointByRowAndCol(i, j));

			praise->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=](){praise->removeFromParent(); }), nullptr));
			tiaoyueNum = 0;
			auto armature = (Armature*)gameScene->_propLayer->_propLayer->getChildByTag(48);
			armature->getAnimation()->play("tiaoyue", -1, 2);
			armature->getAnimation()->setMovementEventCallFunc(
				[=](Armature * armature, MovementEventType type, const std::string &name)
			{
				tiaoyueNum += 1;
				if (type == LOOP_COMPLETE && name == "tiaoyue" && tiaoyueNum == 2)
				{

					armature->getAnimation()->play("daiji", -1, 1);
				}
			});
		}

		waitTime = 0;
		int DeleteBubbleNum = 0;
		baozhaNum = 0;
		for (int speed = 0; speed < MAX_ROWS; speed++)
		{
			waitTime += 0.04f;
			for (int tempX = i - speed; tempX <= i + speed; tempX++)
			{
				for (int tempY = j - speed; tempY <= j + speed; tempY++)
				{
					if (tempX < 0 || tempX >= MAX_ROWS || tempY < 0 || tempY >= MAX_COLS)
					{
						continue;
					}
					auto radius = getPointByRowAndCol(tempX, tempY).getDistance(getPointByRowAndCol(i, j));
					if (radius >= (2 * speed - 1)*BUBBLE_RADIUS && radius <= (2 * speed + 1)*BUBBLE_RADIUS)
					{
						if (board[tempX][tempY] != nullptr && board[tempX][tempY]->getIsSame())
						{
							auto label = Label::createWithBMFont("Fnt/write_fnt.fnt", "5");
							label->setScale(0.8f);
							this->addChild(label);
							auto point = getPointByRowAndCol(tempX, tempY);
							label->setPosition(point.x + 10, point.y + 10);
							label->runAction(Sequence::create(DelayTime::create(waitTime), FadeIn::create(0.2f), FadeOut::create(0.6f), CallFunc::create([=](){label->removeFromParent(); }), nullptr));
							DeleteBubbleNum++;

							Bubble *obj = board[tempX][tempY];
							board[tempX][tempY] = NULL;
							//obj->stopAllActions();
							obj->runAction(Sequence::create(DelayTime::create(waitTime), Spawn::create(FadeOut::create(0.2f), CallFuncN::create(CC_CALLBACK_1(BubbleLayer::callbackRemoveBubble, this)), nullptr), nullptr));
							//obj->runAction(Sequence::create(DelayTime::create(waitTime), FadeOut::create(0.6f), CallFunc::create([=](){obj->removeFromParent(); }), nullptr));
							if (DeleteBubbleNum >= sameSum)
							{
								//scheduleOnce(schedule_selector(BubbleLayer::dropBubble), 0.1f);

								
								auto sprite = Sprite::create();
								addChild(sprite);
								sprite->runAction(Sequence::create(DelayTime::create(waitTime + 0.15f), CallFunc::create([=](){
									downBubbleNum = 0;
									for (int i = 0; i < MAX_COLS; i++)
									{
										if (board[0][i] != nullptr && !board[0][i]->getIsPass())
										{
											checkDownBubble(0, i);
										}
									}
									downBubble();
									
								}), nullptr));

								
								return;
							}

						}

					}
				}
			}
		}
	}
}

void BubbleLayer::dropBubble(float dat)
{
	downBubbleNum = 0;
	for (int i = 0; i < MAX_COLS; i++)
	{
		if (board[0][i] != nullptr && !board[0][i]->getIsPass())
		{
			checkDownBubble(0, i);
		}
	}
	downBubble();
}

// void BubbleLayer::deleteTheSameBubble(int i, int j, bool flag)
// {
// 	if (sameSum >= 3){
// 		for (int i = 0; i < MAX_ROWS; ++i) 	{
// 			for (int j = 0; j < MAX_COLS; ++j)
// 			{
// 				if (board[i][j] != nullptr && board[i][j]->getIsSame())
// 				{
// 					auto obj = board[i][j];
// 					board[i][j] = nullptr;
// 					/*obj->removeFromParent();*/
// 					obj->runAction(Sequence::create(DelayTime::create(waitTime), CallFuncN::create(CC_CALLBACK_1(BubbleLayer::callbackRemoveBubble, this)), nullptr));
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for (int i = 0; i < MAX_ROWS; ++i) 	{
// 			for (int j = 0; j < MAX_COLS; ++j)
// 			{
// 				if (board[i][j] != nullptr && board[i][j]->getIsSame())
// 				{
// 					board[i][j]->setIsSame(false);
// 				}
// 			}
// 		}
// 	}
// 
// }
void BubbleLayer::bubbleAction(Bubble *obj)
{
	auto gameSceme = (GameScene*)this->getParent();
	gameSceme->_propLayer->AddScoreLabel(5);
	SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");
	obj->runAction(Sequence::create(FadeOut::create(waitTime), CallFunc::create([=](){obj->removeFromParent(); setEnable(); }), NULL));
}

void BubbleLayer::callbackRemoveBubble(cocos2d::Node *obj)
{
	baozhaNum++;
	auto bubble = dynamic_cast<Bubble*>(obj);
	if (bubble != nullptr)
	{	
		bubble->runAction(FadeOut::create(0.2f));
		SimpleAudioEngine::getInstance()->playEffect("Music/Remove.mp3");
		Armature* armature = Armature::create("baozha");

		armature->getAnimation()->play("daojubaozha");
		this->addChild(armature);
		armature->setPosition(bubble->getPosition());
		//armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(BubbleLayer::moveParantCallBack, this));
		armature->getAnimation()->setMovementEventCallFunc([=](Armature * armature, MovementEventType type, const std::string &name){
			if (type == COMPLETE){
			{
				if (name == "daojubaozha")
				{
					this->removeChild(armature,true);
					//auto bubble = (Bubble*)armature->getParent();
					bubble->removeFromParentAndCleanup(true);
					//setEnable();
// 					if (baozhaNum >= sameSum)
// 					{
// 						downBubbleNum = 0;
// 						for (int i = 0; i < MAX_COLS; i++)
// 						{
// 							if (board[0][i] != nullptr && !board[0][i]->getIsPass())
// 							{
// 								checkDownBubble(0, i);
// 							}
// 						}
// 						downBubble();
// 					}
				}
			}
		}});
	}

}
void BubbleLayer::movementPassCallBack(Armature * armature, MovementEventType type, const std::string &name)
{
	if (type == COMPLETE){
		if (name == "gongxiguoguan")
		{
			this->removeChild(armature);
			_level++;
			if (_level >= MAX_CUS)
			{
				_level = 8;
			}
			this->initTheBoard(_level);
			UserData::getInstance()->addLevel(1);
			_havePass = false;
			auto gameScene = (GameScene*)this->getParent();
			gameScene->_propLayer->setTime(61);
			gameScene->_propLayer->scheduleUpdate();
		}
	}
}

void BubbleLayer::moveParantCallBack(Armature * armature, MovementEventType type, const std::string &name)
{
	if (type == COMPLETE){
		{
			if (name == "daojubaozha")
			{
				auto bubble = (Bubble*)armature->getParent();
				bubble->removeFromParentAndCleanup(true);
				//setEnable();
			}
			
		}
	}
}
void BubbleLayer::jumpActionCallBack()
{
	for (int i = 0; i < MAX_WAIT_PAOPAO - 1; ++i)
	{
		wait[i] = wait[i + 1];
	}
	wait[MAX_WAIT_PAOPAO - 1] = randomPaoPao();
	this->addChild(wait[MAX_WAIT_PAOPAO - 1], -1);
	for (int i = 0; i < MAX_WAIT_PAOPAO; ++i)
	{
		wait[i]->setPosition(Point(WAIT_PAOPAO_POS));
	}

	UserData::getInstance()->setUserProp(true);
}
void BubbleLayer::resetAllPass()
{
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (board[i][j])
			{
				board[i][j]->setIsPass(false);
				board[i][j]->setIsMove(false);
			}
		}
	}
}

void BubbleLayer::checkDownBubble(int i, int j)
{
	if (i < 0 || i >= MAX_ROWS || j < 0 || j >= MAX_COLS)
	{
		return;
 	}

	if ((board[i][j] == nullptr || board[i][j]->getIsPass()))
	{
		return;
	}
	//board[i][j]->stopAllActions();
	board[i][j]->setIsPass(true);

	auto flag = i % 2 == 0 ? true : false;
	if (flag && j > 0 && i > 0) 	{//����
		checkDownBubble(i - 1, j - 1);
	}
	else if (!flag && i > 0) 	{
		checkDownBubble(i - 1, j);
	}
	if (flag && i > 0) 	{ //����
		checkDownBubble(i - 1, j);
	}
	else if (!flag && i > 0 && j < MAX_COLS - 1) 	{
		checkDownBubble(i - 1, j + 1);
	}
	if (flag && j > 0 && i < MAX_ROWS) 	{ // ����
		checkDownBubble(i + 1, j - 1);
	}
	else if (!flag && i < MAX_ROWS) 	{
		checkDownBubble(i + 1, j);
	}
	if (flag && i < MAX_ROWS) 	{ //����
		checkDownBubble(i + 1, j);
	}
	else if (!flag && i < MAX_ROWS && j < MAX_COLS - 1) 	{
		checkDownBubble(i + 1, j + 1);
	}
	if (j > 0)
	{
		checkDownBubble(i, j - 1); // ��
	}
	if (j < MAX_COLS)
	{
		checkDownBubble(i, j + 1); // ��
	}  
}

void BubbleLayer::downBubble()
{
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (board[i][j] && !(board[i][j]->getIsPass()))
			{
				Bubble *obj = board[i][j];
				board[i][j] = NULL;
				obj->stopAllActions();
				downBubbleAction(obj);
			}
		}
	}
	if (isGameover())
	{
		gameOver(true);
		return;
	}

	if (isPass() && _havePass == false)
	{
		auto gameScene = (GameScene*)this->getParent();
		gameScene->_propLayer->unscheduleUpdate();

		_havePass = true;
		auto sprite = Sprite::create();
		addChild(sprite);
		auto visibleSize = Director::getInstance()->getVisibleSize();
		sprite->setPosition(visibleSize/2);
		sprite->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
			Armature* armature = Armature::create("paopaolong");
			armature->getAnimation()->play("gongxiguoguan");
			this->addChild(armature);
			SimpleAudioEngine::getInstance()->playEffect("Music/Guoguan.mp3");
			armature->setPosition(270, 580);
			armature->getAnimation()->setMovementEventCallFunc(CC_CALLBACK_3(BubbleLayer::movementPassCallBack, this));
			this->removeChild(sprite);
		}), nullptr));
	}
	else
	{
		setEnable();
	}
}

void BubbleLayer::downBubbleAction(Bubble *obj)
{

	auto gameScene = (GameScene*)(this->getParent()); // ���ӵ���ķ���
	gameScene->_propLayer->AddScoreLabel(10);
	//obj->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=](){obj->removeFromParent(); }), nullptr));
	

	auto duration = (obj->getPosition().y - 200.0) / 600.0;
	obj->runAction(Sequence::create(EaseSineIn::create(Spawn::create(ScaleTo::create(duration, 0.5), MoveTo::create(duration, Vec2(obj->getPosition().x, 200.0f)), nullptr)), CallFuncN::create(CC_CALLBACK_1(BubbleLayer::downBubbleActionCallBack, this)), NULL));
}

void BubbleLayer::downBubbleActionCallBack(Node *obj)
{


	auto bubble = dynamic_cast<Bubble*>(obj);

	auto label = Label::createWithBMFont("Fnt/write_fnt.fnt", "10");
	label->setScale(0.8f);
	this->addChild(label);
	auto point = obj->getPosition();
	label->setPosition(point.x + 10, point.y + 10);
	label->runAction(Sequence::create(Spawn::create(FadeIn::create(0.6f),MoveBy::create(0.6f, Vec2(0,10.0f)), nullptr), FadeOut::create(0.2f), CallFunc::create([=](){label->removeFromParent(); }), nullptr));

 	auto particle = ParticleSystemQuad::create("Particle/luoxia_lizi.plist");
 	particle->setPosition(bubble->getPosition());
 	this->addChild(particle);
	bubble->removeFromParent();
}
void BubbleLayer::initBubbleAction(Bubble *obj, int i, int j)
{
	setDisable();
	bubbleInitNum--;
	auto point = getPointByRowAndCol(i, j);
	auto start = Point(point.x, 300.0f - i * BUBBLE_RADIUS * 2);
	obj->setPosition(start);
	auto moveTo = MoveTo::create(0.4f + j * 0.1f, point);
	obj->runAction(Sequence::create(moveTo,nullptr));
}

void BubbleLayer::gameOver(bool over)
{
	auto gameSceme = (GameScene*)this->getParent();
	gameSceme->gameOver(over);
}

void BubbleLayer::swapBubble()
{
	auto readyPoint = ready->getPosition();
	auto waitPoint = wait[0]->getPosition();

	ready->runAction(MoveTo::create(0.1f, waitPoint));
	wait[0]->runAction(MoveTo::create(0.1f, readyPoint));

	auto temp = ready;
	ready = wait[0];
	wait[0] = temp;

	throwBallAction();
}
void BubbleLayer::colorBubble(bool use)
{
	if (ready == nullptr || wait[0] == nullptr)
	{
		return;
	}
	if (use)
	{
		if (ready->getType() == BUBBLE_TYPE_BOMB)
		{
			//����BombBubble������һ��״̬Ϊ����

			UserData::getInstance()->addBomb(1);
			auto propLayer = ((GameScene*)(this->getParent()))->_propLayer;
			propLayer->BombNormal();
			propLayer->setBombNum();

		}
		else
		{
			ready->setPastType(ready->getType());
		}
		ready->setType(BUBBLE_TYPE_COLOR);
		ready->setSpriteFrame(BUBBLE_COLOR_NAME);
	}
	else
	{
		if (ready->getType() == BUBBLE_TYPE_COLOR)
		{
			ready->setType(ready->getPastType());
			ready->setSpriteFrame(ready->getStringByType(ready->getType()));
		}
		else
		{
			wait[0]->setType(wait[0]->getPastType());
			CCLOG("%d", wait[0]->getType());
			wait[0]->setSpriteFrame(wait[0]->getStringByType(wait[0]->getType()));
		}
	}
}
void BubbleLayer::bombBubble(bool use)
{
	if (ready == nullptr || wait[0] == nullptr)
	{
		return;
	}
	if (use)
	{
		if (ready->getType() == BUBBLE_TYPE_COLOR)
		{
			UserData::getInstance()->addColorBubble(1);
			auto propLayer = ((GameScene*)(this->getParent()))->_propLayer;
			propLayer->ColorNormal();
			propLayer->setColorBubbleNum();

		}
		else
		{
			ready->setPastType(ready->getType());
		}
		ready->setType(BUBBLE_TYPE_BOMB);
		ready->setSpriteFrame(BUBBLE_BOMB_NAME);
	}
	else
	{
		if (ready->getType() == BUBBLE_TYPE_BOMB)
		{
			ready->setType(ready->getPastType());
			ready->setSpriteFrame(ready->getStringByType(ready->getType()));
		}
		else
		{
			wait[0]->setType(wait[0]->getPastType());
			wait[0]->setSpriteFrame(wait[0]->getStringByType(wait[0]->getType()));
		}
	}

}
void BubbleLayer::auxiliaryLine(Point tagrat)
{	
	auto node = Node::create();
	addChild(node);
	node->setTag(100);
	
	auto lineName = StringUtils::format("line_%d.png", (int)ready->getType());
	auto speed = 36.0f;
	auto position = Point(READY_PAOPAO_POS.x + real.x * speed, READY_PAOPAO_POS.y + real.y * speed);

	while (position.y < TOUCH_TOP * Director::getInstance()->getVisibleSize().height - BUBBLE_RADIUS)
	{
		std::vector<Vec2> rowCol;
		rowCol.push_back(getRowAndColByPoint(Point(position.x, position.y + BUBBLE_RADIUS)));
		rowCol.push_back(getRowAndColByPoint(Point(position.x - BUBBLE_RADIUS + 5.0f, position.y)));
		rowCol.push_back(getRowAndColByPoint(Point(position.x + BUBBLE_RADIUS - 5.0f, position.y)));
		for (auto &ti : rowCol)
		{
			if (board[int(ti.x)][int(ti.y)] != nullptr && (int)ti.x < MAX_ROWS && (int)ti.y < MAX_COLS)
			{
				return;
			}
		}
		if (position.x <= BUBBLE_RADIUS || position.x >= Director::getInstance()->getVisibleSize().width - BUBBLE_RADIUS)
		{
			real.x = -real.x;
		}
		position = Point(position.x + real.x * speed, position.y + real.y * speed);
		auto line = Sprite::createWithSpriteFrameName(lineName);
		node->addChild(line);
		line->setPosition(position);
	}
}
void BubbleLayer::setReadyAngle(Point target)
{
	auto angle = (target - READY_PAOPAO_POS).getAngle(Vec2(0, 1));
	ready->setRotation(CC_RADIANS_TO_DEGREES(angle));
}
void BubbleLayer::throwBallAction()
{
	auto gameScene = (GameScene*)this->getParent();

	auto bgNode = (Node*)gameScene->_bg;
	auto bgArmature = (Armature*)bgNode->getChildByTag(63);
	bgArmature->getAnimation()->play("huanqiu");

	auto propLayer = (PropLayer*)gameScene->_propLayer;
	auto propNode = (Node*)propLayer->_propLayer;
	auto propArmature = (Armature*)propNode->getChildByTag(68);
	propArmature->getAnimation()->play("huanqiushou");
}

bool BubbleLayer::isPass()
{
	for (int i = 0; i < MAX_COLS; i++)
	{
		if (board[0][i] != nullptr)
		{
			return false;
		}
	}
	return true;
}

bool BubbleLayer::onTouchBegan(Touch *touch, Event *unused_event)
{

	real = (touch->getLocation() - READY_PAOPAO_POS).getNormalized();
	if (real.x > 0.98 || real.x < -0.98 || real == Vec2::ZERO || real.y <= 0)
	{
		return false;

	}
	return true;
}

void BubbleLayer::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto gameScene = (GameScene*)this->getParent();
	this->removeChildByTag(100);
	if (touch->getLocation().y >= TOUCH_TOP*Director::getInstance()->getVisibleSize().height)
	{
		return;
	}
	real = (touch->getLocation() - READY_PAOPAO_POS).getNormalized();
	if (real.x <= 0.98 && real.x >= -0.98 && real != Vec2::ZERO && real.y > 0)
	{
		this->auxiliaryLine(touch->getLocation());
		this->setReadyAngle(touch->getLocation());
		gameScene->setCannonAngle(touch->getLocation());
	}

}

void BubbleLayer::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto gameScene = (GameScene*)this->getParent();
	this->removeChildByTag(100);
//  	if (Rect(85,150,120,120).containsPoint(touch->getLocation()))
//  	{
//  		//this->swapBubble();
//  		return;
//  	}
	if (touch->getLocation().y >= TOUCH_TOP*Director::getInstance()->getVisibleSize().height)
	{
		return;
	}
	real = (touch->getLocation() - READY_PAOPAO_POS).getNormalized();
	if (real.x <= 0.98 && real.x >= -0.98 && real != Vec2::ZERO && real.y > 0)
	{
		_listener->setEnabled(true);
		gameScene->setCannonAngle(touch->getLocation());
		this->onTouch(touch->getLocation());
		gameScene->_propLayer->setCannonAction(touch->getLocation());

	}

}
void BubbleLayer::savaCurrentBoard()
{
	int(*boradArray)[MAX_COLS] = UserData::getInstance()->_boradArray;
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			if (board[i][j] != nullptr)
			{
				boradArray[i][j] = (int)board[i][j]->getType();
			}
			else
			{
				boradArray[i][j] = 0;
			}
		}
	}
 	auto gameScene = (GameScene*)(this->getParent());
 	UserData::getInstance()->setRETime(gameScene->_propLayer->getTime());
}
bool BubbleLayer::initTheBoardWithData()
{
	bool bRet = false;
	bubbleInitNum = 0;
	int(*boradData)[MAX_COLS] = UserData::getInstance()->_boradArray;
	for (int i = 0; i < MAX_ROWS; ++i)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			board[i][j] = nullptr;
		}
		bRet = true;
	}
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; ++j)
		{
			if (i % 2 && j == MAX_COLS - 1 || boradData[i][j] == 0)
			{
				continue;
			}
			else
			{
				board[i][j] = Bubble::initWithType((BubbleType)boradData[i][j]);
			}
			bubbleInitNum += 1;
			bool flag = (i % 2) == 0 ? true : false;
			board[i][j]->setFlag(flag);
			addChild(board[i][j]);
			initBubbleAction(board[i][j], i, j);
		}
	}
	SimpleAudioEngine::getInstance()->playEffect("Music/start.mp3");
	auto visible = Director::getInstance()->getVisibleSize();
	auto spriteReady = Sprite::createWithSpriteFrameName("ready.png");
	this->addChild(spriteReady);
	spriteReady->setPosition(Vec2(visible.width / 2, visible.height / 2 + 20));
	spriteReady->setScale(0);
	spriteReady->runAction(Sequence::create(ScaleTo::create(0.5f, 1.0f), CallFunc::create([=](){
		spriteReady->removeFromParent();
		auto goSprite = Sprite::createWithSpriteFrameName("go.png");
		this->addChild(goSprite);
		goSprite->setPosition(Vec2(visible.width / 2, visible.height / 2 + 20));
		goSprite->runAction(Sequence::create(Spawn::create(ScaleTo::create(0.8f, 1.2f), FadeOut::create(0.8f), nullptr), CallFunc::create([=](){goSprite->removeFromParent(); setEnable(); }), nullptr));
	}), nullptr));
	bRet = true;
	return bRet;
}

void BubbleLayer::addReadGo()
{
	auto node = CSLoader::createNode("ReadyGoNode.csb");
	auto frameSize = Director::getInstance()->getVisibleSize();
	ui::Helper::doLayout(node);
	addChild(node);
	node->setPosition(frameSize / 2);
}
void BubbleLayer::setDetermineLineVisible(float delat)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gameScene = (GameScene*)this->getParent();
	for (int i = MAX_ROWS - 1; i >= 0; i--)
	{
		if (getPointByRowAndCol(i,0).y >= (TOUCH_DOWN* visibleSize.height + 2*BUBBLE_RADIUS))
		{
			for (int j = 0; j < MAX_COLS; j++)
			{
				if (board[i][j] != nullptr)
				{
					gameScene->_propLayer->setLineVisible(true);
					return;
				}
			}
			gameScene->_propLayer->setLineVisible(false);
			return;
		}
	}



}

bool BubbleLayer::isGameover()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	for (int i = MAX_ROWS - 1; i >= 0; i--)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			if (board[i][j] == nullptr)
			{
				continue;
			}
			if (getPointByRowAndCol(i, 0).y <= TOUCH_DOWN * visibleSize.height)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}