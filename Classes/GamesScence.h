#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BackgroundBoard.h"
#include "FragmentEffect.h"
#include "RankList.h"
#include "Tetromino.h"
#include "WidgetManager.h"
#include <list>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class GamesScence : public cocos2d::CCLayer
{
public:
    virtual bool init();  

    static CCScene* scene();

	/*按钮的回调函数*/
	///菜单按钮
	void btMenuCallback(CCObject* pSender,TouchEventType type);

	///开始按钮
    void btStartCallback(CCObject* pSender,TouchEventType type);

	///重新开始按钮
	void btRestartCallback(CCObject* pSender,TouchEventType type);

	///继续游戏按钮
	void btContinueCallback(CCObject* pSender,TouchEventType type);

	///排行榜按钮
	void btRankListCallback(CCObject* pSender,TouchEventType type);

	///帮助按钮
	void btHelpCallback(CCObject* pSender,TouchEventType type);

	///离开游戏按钮
	void btLeaveCallback(CCObject* pSender,TouchEventType type);

	///确定按钮（上传分数）
	void btConfirmationCallback(CCObject* pSender,TouchEventType type);

	///取消按钮（上传分数）
	void btCancelCallback(CCObject* pSender,TouchEventType type);

	/*BackgroundBoard的消息回调*/
	///加分（有一行或多行被消除）
    void onAddScore(int numLineCleared);

	///提示下个方块的形状
	void onNextBlock(int* next3Blocks);

	///GameOver
	void onGameOver();

	/*HttpTool的消息回调*/
	///得到排名的回调
	void getPositionResponse(int score);

	///上传分数的回调
	void uploadScoreResponse(bool b);

    CREATE_FUNC(GamesScence);

private:

	///菜单、排行榜、gameover层、上传分数这些层一次只能出现一个,用它来管理
	WidgetManager    m_panelManager;

	///<游戏底板,方块的降落、左右移动、旋转等等的逻辑都在这里实现
	BackgroundBoard* m_bgBpard;       

	UILayer*         m_uiLayer;       ///<从cocostudio加载的游戏UI
	UILabelAtlas*    m_scoreLabel;    ///<显示分数
	UILabelAtlas*    m_highestLabel;  ///<显示最高分
	UILabelAtlas*    m_numCLearLabel; ///<提示消除了多少行
	UIButton*        m_btMenu;        ///<菜单按钮
	UITextField*     m_nickNameInput; ///<输入昵称的输入框
	UIImageView*     m_imgFrame;      ///<游戏底板的纵横线
	RankList*        m_list;          ///<排行榜（功能类,与服务器的交互在这里实现）

	std::list<Tetromino*> m_next3Tetrominos; ///<下三个方块的提示（显示形状）

	int   m_score;          ///<分数
	float m_blockSize;      ///<方块的大小（那些正方形的边长）
	float m_dropDelayTime;  ///<方块每下降一格的延迟时间
	int   m_clearLineCount; ///<清除行的记数
	bool  m_isGgameRunning; ///<游戏是否正在运行中（用于决定是开始游戏还是继续游戏）
	CCPoint m_nextTipPos;       ///<下一个方块的提示位置
	CCSize m_nextTipSize;      ///<下一个方块的提示大小
	///设置各个形状的方块的颜色（用特性矩阵实现）
	void setEffectMatrix();

	void startGame();
};

#endif 
