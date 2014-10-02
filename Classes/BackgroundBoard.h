#ifndef _BACKGROUND_BOARD_H_
#define _BACKGROUND_BOARD_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Tetromino.h"
#include "Block.h"

USING_NS_CC;

typedef void (CCObject::*SEL_ClearLine)(int numLine);
#define clearLine_selector(_SELECTOR) (SEL_ClearLine)(&_SELECTOR)

typedef void (CCObject::*SEL_GameOver)();
#define gameOver_selector(_SELECTOR) (SEL_GameOver)(&_SELECTOR)

class BackgroundBoard : public CCNode,public CCTouchDelegate
{
public:
	void curTetrominoMove(CCNode*);
	void start();
	void setNextTetromino();
	void setClearLineListener(CCObject*,SEL_ClearLine);
	void setGameOverListener(CCObject*,SEL_GameOver);
	void pasueDrop();
	void continueDrop();
	
	void setActSensitivity(float actSensitivity) {m_actSensitivity = actSensitivity;}
	void setDropDur(float dropDur)				 {m_dropDur = dropDur;}
	bool isPause()								 {return m_isPause;}

	virtual bool init(float blockSize,char* fnBlockTex);
	virtual void onEnter();
    virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	static BackgroundBoard* create(float blockSize,char* fnBlockTex);
private:
	char* m_fnBlockTex;
	float m_dropDur;
	float m_accDropDur;
	float m_actSensitivity;
	bool  m_bAccAction;
	bool  m_bGameOver;
	bool  m_isPause;
	int   m_blockSize;
	int   m_bgInfo[BACKGROUND_ROW];
	CCPoint m_touchPos;
	CCNode*       m_blockSprRow[BACKGROUND_ROW];
	Tetromino*    m_curTetromino;
	CCObject*     m_clearLineListener;
	SEL_ClearLine m_clearLineCallback;
	CCObject*     m_gameOverListener;
	SEL_GameOver  m_gameOverCallback;

	void addNewTetromino();
	bool addToBg();
	int  clearLine();
	void onGameOver();
};

inline void BackgroundBoard::setClearLineListener(CCObject* pObject,SEL_ClearLine callback)
{
	m_clearLineListener   = pObject;
	m_clearLineCallback  = callback;
}

inline void BackgroundBoard::setGameOverListener(CCObject* pObject,SEL_GameOver callback)
{
	m_gameOverListener  = pObject;
	m_gameOverCallback = callback;
}

#endif