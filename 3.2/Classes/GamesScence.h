#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "network/HttpClient.h"
#include "BackgroundBoard.h"
#include "Tetromino.h"
#include "GameLayoutManager.h"
#include "NumberCount.h"
#include "RankList.h"
#include <list>
#include "editor-support\cocostudio\DictionaryHelper.h"

USING_NS_CC;
using namespace ui;

class GamesScence : public Layer
{
public:
    virtual bool init();  

    static Scene* scene();

	/*按钮的回调函数*/
	///菜单按钮
	void btMenuCallback(Ref* pSender,Widget::TouchEventType type);

	///开始按钮
    void btStartCallback(Ref* pSender,Widget::TouchEventType type);

	///重新开始按钮
	void btRestartCallback(Ref* pSender,Widget::TouchEventType type);

	///继续游戏按钮
	void btContinueCallback(Ref* pSender,Widget::TouchEventType type);

	///排行榜按钮
	void btRankListCallback(Ref* pSender,Widget::TouchEventType type);

	///帮助按钮
	void btHelpCallback(Ref* pSender,Widget::TouchEventType type);

	///离开游戏按钮
	void btLeaveCallback(Ref* pSender,Widget::TouchEventType type);

	///确定按钮（上传分数）
	void btConfirmationCallback(Ref* pSender,Widget::TouchEventType type);

	///取消按钮（上传分数）
	void btCancelCallback(Ref* pSender,Widget::TouchEventType type);

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

	GameLayoutManager m_panelManager;  ///菜单、排行榜、gameover层、上传分数这些层一次只能出现一个,用它来管理
	BackgroundBoard*  m_bgBpard;       ///<游戏底板,方块的降落、左右移动、旋转等等的逻辑都在这里实现
	NumberCount       m_scoreCount;    ///<显示分数
	NumberCount       m_clearCount;    ///<提示消除了多少行
	TextAtlas*        m_highestLabel;  ///<显示最高分
	Button*           m_btMenu;        ///<菜单按钮
	TextField*        m_nickNameInput; ///<输入昵称的输入框
	ImageView*        m_imgFrame;      ///<游戏底板的纵横线
	RankList*       m_list;          ///<排行榜（功能类,与服务器的交互在这里实现）

	std::list<Tetromino*> m_next3Tetrominos; ///<下三个方块的提示（显示形状）

	float m_blockSize;      ///<方块的大小（那些正方形的边长）
	float m_dropDelayTime;  ///<方块每下降一格的延迟时间
	int   m_clearLineCount; ///<清除行的记数
	bool  m_isGgameRunning; ///<游戏是否正在运行中（用于决定是开始游戏还是继续游戏）
	Point m_nextTipPos;     ///<下一个方块的提示位置
	Size  m_nextTipSize;    ///<下一个方块的提示大小
	

	void startGame();
};

#endif 
