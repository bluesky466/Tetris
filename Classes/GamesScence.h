#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BackgroundBoard.h"
#include "FragmentEffect.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class GamesScence : public cocos2d::CCLayer
{
public:
    virtual bool init();  

    static CCScene* scene();
    void startCallback(CCObject* pSender,TouchEventType type);
    void onAddScore(int numLineCleared);
	void onGameOver();
    void closeCallback(CCObject* pSender);

    CREATE_FUNC(GamesScence);

private:
	BackgroundBoard* m_bgBpard;
	UILabelAtlas*    m_scoreLabel;
	UILabelAtlas*    m_highestLabel;
	UIButton*        m_btStart;

	int   m_score;
	float m_blockSize;
	bool  m_iGgameRunning;

	void setEffectMatrix();
};

#endif 
