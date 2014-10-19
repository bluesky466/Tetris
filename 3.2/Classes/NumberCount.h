#ifndef _NUMBER_COUNT_H_
#define _NUMBER_COUNT_H_

#include "cocos2d.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class NumberCount : public Ref
{
public:
	NumberCount();

	void setNumberLabel(TextAtlas* label);
	void onSchedule2(float dt);
	void setNumber(int number);

	int getNumber()                  {return m_number;}
	void setDelayTime(int delayTime) {m_delayTime  = delayTime;}
	void increaseBy(int number)      {m_increment += number;m_timeAccumulation = 0.0f;}
private:
	TextAtlas*    m_numberLabel;
	int           m_number;
	int           m_increment;
	float         m_delayTime;
	float         m_timeAccumulation;

	int incrementReduce();
};


#endif