#ifndef  USER_DATA_H_
#define  USER_DATA_H_

#include "cocos2d.h"
#include "GameConst.h"

USING_NS_CC;

#define CC_SYNTHESIZE_ADD(varType, varName, funName)\
protected: varType varName; \
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }\
public: virtual void add##funName(varType var){ varType temp = varName; varName = var + temp; saveData(); }\


class UserData
{
public:
	static UserData* getInstance();
	bool init();
	void reset();
	void saveData();
	void addScore(int var);
	void newReset();

	CC_SYNTHESIZE(int, _score, Score);
	CC_SYNTHESIZE(bool, _isBeginner, IsBeginner);
	CC_SYNTHESIZE(bool, _beginner, Beginner);
	CC_SYNTHESIZE_ADD(int, _bomb, Bomb); //Õ¨µ¯
	CC_SYNTHESIZE_ADD(int, _colorBubble, ColorBubble); //¿É±äÅÝÅÝ
	CC_SYNTHESIZE_ADD(int, _hourglass, Hourglass); //É³Â©
	CC_SYNTHESIZE_ADD(int, _gold, Gold);  //½ð±Ò
	CC_SYNTHESIZE_ADD(int, _rebirth, Rebirth); //¸´»îÒ©
	CC_SYNTHESIZE_ADD(int, _bestScore, BestScore);
	CC_SYNTHESIZE_ADD(unsigned int, _level, Level);
	CC_SYNTHESIZE(bool, _raffle, Raffle);
	CC_SYNTHESIZE(bool, _haveRaffle, HaveRaffle);
	CC_SYNTHESIZE(int, _remainingTime, RETime);
	CC_SYNTHESIZE(bool, _isGameOver, IsGameOver);
	CC_SYNTHESIZE(bool, _newGift, NewGift);
	CC_SYNTHESIZE(bool, _userProp, UserProp);
//	CC_SYNTHESIZE(bool, _isMusicOn, IsMusicOn);
    bool _isMusicOn;
    void setIsMusicOn(const bool IsOn){ _isMusicOn = IsOn;}
    bool getIsMusicOn(){ return _isMusicOn; }
	int _boradArray[MAX_ROWS][MAX_COLS];

private:
	UserData() = default;
	~UserData();
};


#endif
