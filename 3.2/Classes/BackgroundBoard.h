#ifndef _BACKGROUND_BOARD_H_
#define _BACKGROUND_BOARD_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Tetromino.h"
#include <list>

USING_NS_CC;

typedef void (Ref::*SEL_ClearLine)(int numLine);
#define clearLine_selector(_SELECTOR) (SEL_ClearLine)(&_SELECTOR)

typedef void (Ref::*SEL_NextBlock)(int*);
#define nextBlock_selector(_SELECTOR) (SEL_NextBlock)(&_SELECTOR)

typedef void (Ref::*SEL_GameOver)();
#define gameOver_selector(_SELECTOR) (SEL_GameOver)(&_SELECTOR)

/**
 *  底板
 */
class BackgroundBoard : public Node
{
public:
	///游戏开始,是重新开始的时候用,暂停后继续请用continueDrop
	void start();

	void pasueDrop();        ///<暂停游戏
	void continueDrop();     ///<继续游戏

	void tetrominoMove(); ///<令当前的方块往下降落,是一个回调函数
	void setNextTetromino(); ///<设置新的方块,是一个回调函数

	
	void setClearLineListener(Ref*,SEL_ClearLine);  ///<设置行消除的回调
	void setGameOverListener(Ref*,SEL_GameOver);    ///<设置GameOver的回调

	/**
	 *  设置下一个方块编号改变的回调
	 *  用于提示玩家下一个方块是什么形状的
	 */
	void setNextBlockListener(Ref*,SEL_NextBlock);   
	
	///设置下落一格的等待时间
	void setDropDelayTime(float dropDur)		 {m_dropDelayTime = dropDur;}

	bool isPause()								 {return m_isPause;}

	virtual bool init(float blockSize);
	virtual void onEnter();
    virtual void onExit();

	bool onTouchBegan(Touch *pTouch, Event *pEvent);
    void onTouchMoved(Touch *pTouch, Event *pEvent);
	void onTouchEnded(Touch *pTouch, Event *pEvent);
    void onTouchCancelled(Touch *pTouch, Event *pEvent);

	static BackgroundBoard* create(float blockSize);
private:
	float m_dropDelayTime;
	bool  m_bAccAction;
	bool  m_bGameOver;
	bool  m_isPause;
	bool  m_bAccMove;
	int   m_blockSize;
	int   m_bgInfo[BACKGROUND_ROW];

	std::list<int> m_next3Blocks;
	Point          m_touchPos;
	Node*          m_blockSprRow[BACKGROUND_ROW];
	Tetromino*     m_tetromino;
	
	Ref*           m_clearLineListener;
	SEL_ClearLine  m_clearLineCallback;
	Ref*           m_gameOverListener;
	SEL_GameOver   m_gameOverCallback;
	Ref*           m_nextBlockListener;
	SEL_NextBlock  m_nextBlockCallback;


	int  clearLine();          ///<消除满方块的行
	bool addToBg();            ///<将当前的方块加到底板标记数组中区
	void addNewTetromino();    ///<增加新的方块
	void gotoTargetPos();      ///<直接降落到最后落点
	void onGameOver();
};

inline void BackgroundBoard::setClearLineListener(Ref* pObject,SEL_ClearLine callback)
{
	m_clearLineListener   = pObject;
	m_clearLineCallback  = callback;
}

inline void BackgroundBoard::setGameOverListener(Ref* pObject,SEL_GameOver callback)
{
	m_gameOverListener  = pObject;
	m_gameOverCallback = callback;
}

inline void BackgroundBoard::setNextBlockListener(Ref* pObject,SEL_NextBlock callback)
{
	m_nextBlockListener = pObject;
	m_nextBlockCallback = callback;
}
#endif