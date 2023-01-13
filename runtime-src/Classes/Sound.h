#ifndef __SOUND_H__
#define __SOUND_H__
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Define.h"
USING_NS_CC;
using namespace CocosDenshion;
class Sound
{
public:
	static Sound* getInstance();
	void init();
	void UnInit();
	void playMenuBackground();  //菜单界面背景
	void playGameBackground();  //游戏界面背景
	void playAttack();  //攻击
	void playBtn();    //按钮
	void playBtnLeft();  //左按钮
	void playBtnRight(); //右按钮
	void playDead();    //死亡
	void playDisapper();//消失
	void playDoorOpen(); //开门
	void playDoorClose();//关门
	void playFallGround();//落地
	void playGetKey();  //得到钥匙
	void playInDoor(); //进门
	void playMinStep(); //没有步数
	void playGetBei();  //得到奖杯
	void playNoGetBei(); //没有得到奖杯
	bool isBackground();
	void stopBackground();
private:
	static Sound* _instance;
};
#endif