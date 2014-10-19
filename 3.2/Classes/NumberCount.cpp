#include "NumberCount.h"

NumberCount::NumberCount():
	m_numberLabel(0),
	m_delayTime(0.01f),
	m_number(0),
	m_increment(0)
{
	
	CCDirector::getInstance()->getScheduler()->schedule(schedule_selector(NumberCount::onSchedule2),this,0.0f,false);
}

void NumberCount::setNumberLabel(TextAtlas* label)
{
	m_numberLabel = label;

	char str[20];
	sprintf(str,"%d",m_number);
	m_numberLabel->setString(str);
	

}

void NumberCount::onSchedule2(float dt)
{
	if(m_increment!=0)
	{
		m_timeAccumulation+=dt;

		if(m_timeAccumulation>m_delayTime)
		{
			m_timeAccumulation = 0.0f;
			m_number+=incrementReduce();

			char str[20];
			sprintf(str,"%d",m_number);
			m_numberLabel->setString(str);
		}
	}
}

void NumberCount::setNumber(int number)
{
	m_number = number;

	char str[20];
	sprintf(str,"%d",m_number);
	m_numberLabel->setString(str);
}

int NumberCount::incrementReduce()
{
	if(m_increment>0)
	{
		m_increment--;
		return 1;
	}
	else if(m_increment<0)
	{
		m_increment++;
		return -1;
	}

	return 0;
}