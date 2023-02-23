#ifndef SHOP_LAYER_H_
#define SHOP_LAYER_H_


#include "cocos2d.h"
#include "UserData.h"

USING_NS_CC;

void buyGoldAddGold(const int i);
void setMusicOn(int IsOn);

class ShopLayer : public LayerColor
{

public:
	CREATE_FUNC(ShopLayer);
	virtual bool init();
	void menuReturnCallBack(Ref* pSender);
	void buyGold(Ref *Psender);
	void setGoldNum(float delay);

	Node* _shopLayer;

};




#endif
