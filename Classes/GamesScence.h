#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BackgroundBoard.h"
#include "FragmentEffect.h"
#include "RankList.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class GamesScence : public cocos2d::CCLayer
{
public:
    virtual bool init();  

    static CCScene* scene();
	void btMenuCallback(CCObject* pSender,TouchEventType type);
    void btStartCallback(CCObject* pSender,TouchEventType type);
	void btRestartCallback(CCObject* pSender,TouchEventType type);
	void btContinueCallback(CCObject* pSender,TouchEventType type);
	void btRankListCallback(CCObject* pSender,TouchEventType type);
	void btHelpCallback(CCObject* pSender,TouchEventType type);
	void btLeaveCallback(CCObject* pSender,TouchEventType type);
	void btConfirmationCallback(CCObject* pSender,TouchEventType type);
	void btCancelCallback(CCObject* pSender,TouchEventType type);
    void onAddScore(int numLineCleared);
	void onGameOver();
    void closeCallback(CCObject* pSender);
	void getPositionResponse(int score);
	void uploadScoreResponse(bool b);
    CREATE_FUNC(GamesScence);

private:
	BackgroundBoard* m_bgBpard;
	UILabelAtlas*    m_scoreLabel;
	UILabelAtlas*    m_highestLabel;
	UILayer*         m_uiLayer;
	UIPanel*		 m_menuPanel;
	UIPanel*		 m_gamrOverPanel;
	UIPanel*		 m_uploadScorePanel;
	UIButton*        m_btMenu;
	UIButton*        m_btStart;
	UIButton*        m_btContinue;
	UIButton*        m_btRankList;
	UIButton*        m_btHelp;
	UIButton*        m_btLeave;
	UITextField*     m_nickNameInput;
	UIImageView*     m_imgFrame;
	UIListView*		 m_listRankList;
	RankList*        m_list;

	int   m_score;
	float m_blockSize;
	bool  m_iGgameRunning;

	void setEffectMatrix();
	void setMenuVisible(bool bVisible);
	void setGameOverPanelVisible(bool bVisible);
	void setUploadScorePanelVisible(int score,bool bVisible);
};

#endif 
