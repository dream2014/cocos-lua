#ifndef PROP_LAYER_H_
#define PROP_LAYER_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

class PropLayer : public Layer
{
public:
	CREATE_FUNC(PropLayer);
	virtual bool init();
	CC_SYNTHESIZE(float, _time, Time);
	void AddScoreLabel(int var);
	void setCannonAngle(Point target);
	void setCannonAction(Point target);
	void setBombNum();
	void setColorBubbleNum();
	void setGlassNum();
	void BombNormal();
	void ColorNormal();
	void setLineVisible(bool enable);
	void useProp();
	void setGoldNum();
	Node* _propLayer;

private:

	
	void timeCallFunc();
	void BombCheckBoxCallBack(Ref* Psender, ui::CheckBox::EventType type);
	
	void ColorCheckBoxCallBack(Ref* Psender, ui::CheckBox::EventType type);
	
	//void GlassCheckBoxCallBack(Ref* Psender, ui::CheckBoxEventType type);
	//void menuBombCallBack(Ref* Psender);
	void menuHourglassCallBack(Ref* Psender);
	void menuPauseCallBack(Ref* Psender);
	//void menuColorBubbleCallBack(Ref* Psender);
	void menuSwapCallBack(Ref* Psender);
	void update(float delta);
	void setGoldNumDelay(float delay);
};





#endif