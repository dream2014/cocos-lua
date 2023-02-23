#ifndef GIFT_LAYER_H_
#define GIFT_LAYER_H_

#include "cocos2d.h"


USING_NS_CC;

enum GiftType
{
	Type_New,
	Type_Bubble,
	Type_Jewel,
};

class GiftLayer : public LayerColor
{
public:
	static GiftLayer* create(GiftType type = Type_New);
	virtual bool init(GiftType type);
	bool isGameScene = false;
	bool isPause = true;
	CC_SYNTHESIZE(GiftType, _type, Type);
	Node* _giftNode;
private:
	
	void getButtonCallBack(Ref* Pesender);
	void returnButtonCallBack(Ref* Pesender);

};















#endif