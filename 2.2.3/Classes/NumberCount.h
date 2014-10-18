#ifndef _NUMBER_COUNT_H_
#define _NUMBER_COUNT_H_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class NumberCount : public CCObject
{
public:
	NumberCount();

	void setNumberLabel(UILabelAtlas* label);
	void onSchedule2(float dt);
	void setNumber(int number);

	int getNumber()                  {return m_number;}
	void setDelayTime(int delayTime) {m_delayTime  = delayTime;}
	void increaseBy(int number)      {m_increment += number;m_timeAccumulation = 0.0f;}
private:
	UILabelAtlas* m_numberLabel;
	int           m_number;
	int           m_increment;
	float         m_delayTime;
	float         m_timeAccumulation;

	int incrementReduce();
};


#endif