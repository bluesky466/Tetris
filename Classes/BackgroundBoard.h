#ifndef _BACKGROUND_BOARD_H_
#define _BACKGROUND_BOARD_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Tetromino.h"
#include "Block.h"

USING_NS_CC;

class BackgroundBoard : public CCNode,public CCTouchDelegate
{
public:
	void curTetrominoMove(CCNode*);
	void setActionDistance(float actionDistance) {m_actionDistance = actionDistance;}

	virtual bool init(float blockSize,char* fnBlockTex);
	virtual void onEnter();
    virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);

	static BackgroundBoard* create(float blockSize,char* fnBlockTex);
private:
	char* m_fnBlockTex;
	float m_dropDur;
	float m_touchPosY;
	float m_actionDistance;
	bool  m_bAccAction;
	bool  m_bGameOver;
	int   m_blockSize;
	int   m_bgInfo[BACKGROUND_ROW];
	CCNode* m_blockSprRow[BACKGROUND_ROW];
	Tetromino* m_curTetromino;

	void addNewTetromino();
	bool addToBg();
	bool clearLine();
};

#endif