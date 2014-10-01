#ifndef _BACKGROUND_BOARD_H_
#define _BACKGROUND_BOARD_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Tetromino.h"
#include "Block.h"

USING_NS_CC;

typedef void (CCObject::*SEL_ClearLine)(int numLine);
#define clearLine_selector(_SELECTOR) (SEL_ClearLine)(&_SELECTOR)

class BackgroundBoard : public CCNode,public CCTouchDelegate
{
public:
	void curTetrominoMove(CCNode*);
	void start();
	void setActSensitivity(float actSensitivity) {m_actSensitivity = actSensitivity;}
	void setDropDur(float dropDur)				 {m_dropDur = dropDur;}
	bool isPause()								 {return m_isPause;}
	void stopDropAndAddToBg();
	void setClearLineListener(CCObject*,SEL_ClearLine);
	void pasueDrop();
	void continueDrop();
	
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
	CCObject*     m_clearLineLisener;
	SEL_ClearLine m_clearLineCallBack;

	void addNewTetromino();
	bool addToBg();
	int  clearLine();
	void onGameOver();
};

inline void BackgroundBoard::setClearLineListener(CCObject* pObject,SEL_ClearLine callback)
{
	m_clearLineLisener   = pObject;
	m_clearLineCallBack  = callback;
}

#endif