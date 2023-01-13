#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Define.h"
#include "GameNews.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace std;
class Game:public Layer
{
public:
	static cocos2d::Scene* createScene(int level, int levelNum);
	static Game* create(int level, int levelNum);
	virtual bool init(int level, int levelNum);
	
	void HomeTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void PauseTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void SoundTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void LeftTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void RightTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void RestartTouchEventCallback(Ref *pSender, Widget::TouchEventType type);



	void update(float dt);
	void setBar();

	void LeftCallback();
	void RightCallback();

	bool isWall(Vec2 pp);
	bool isBanZiU(Vec2 pp);
	bool isBanZiOpen(Vec2 pp);
	bool isBanZiD(Vec2 pp);
	bool isBanZiL(Vec2 pp);
	bool isBanZiR(Vec2 pp);
	Vec2 moveTo(Vec2 pp,int rotate);

	void OnKeyArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID);
	void OnHeroArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID);
	void OnDoorArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID);
	void OnEnemyArmatureCallBack(Armature* armature, MovementEventType movementType, const std::string &movementID);
	void gameOver();
	void gameFail();
	Rect getCollisionRect(Vec2 pp);
	Vec2 getMapId(Vec2 pp);
	Vec2 getMapVec2(Vec2 id);
	void dropUpdate(float dt);
	void openBZUpdate(float dt);
	void faZhenUpdate(float dt);
	bool isEnemy(Vec2 pp);
	Vec2 isFoot();
	void isAttack();
	void setFaZhen(Node* node);
	bool setMuTiao();
	void loadMap();

	void OkTouchEventCallback(Ref *pSender, Widget::TouchEventType type);

	void AddStepTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void ShowKeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void OkKeyTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
	void OkStepTouchEventCallback(Ref *pSender, Widget::TouchEventType type);
private:
	
	Button* btn_left;
	Button* btn_right;
	bool _faZhenOpen = true;
	bool _isBright = true;
	bool muTMove = false;
	bool _gameFail = false;
	bool _gameOver = false;
	bool _isRotate=false;
	bool _isHaveKey = false;
	bool _attack = false;
	Rect mapRect;
	TMXTiledMap* m_map=NULL;
	Size m_mapSize;
	int m_level=-1;
	int m_levelNum=-1;
	int maxStep = 0;
	int loseStep = 0;
	LoadingBar* bar_step;
	TextAtlas* lab_setp;
	ImageView* img_no_bei;
	Armature* Hero;
	Armature* Key;
	Armature* Door;
	Armature* fazhenA=NULL;
	Armature* fazhenB=NULL;

	Rect Wall;
	vector<Rect> wallArr;
	vector<Rect> ddArr;
	vector<Rect> fangArr;
	Vector<Armature*> enemyArr;
	Vector<Armature*> boxArr;
	Vector<Sprite*> banziUArr;
	Vector<Sprite*> banziDArr;
	Vector<Sprite*> banziLArr;
	Vector<Sprite*> banziRArr;
	Vector<Sprite*> mutiaoLArr;

	News* _news;
};
#endif