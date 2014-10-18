#ifndef _WIDGET_MANAGER_H_
#define _WIDGET_MANAGER_H_

#include "cocos2d.h"
#include "cocos-ext.h"


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocostudio;
using namespace ui;

USING_NS_CC;
USING_NS_CC_EXT;



/**
 * 每次只允许一个Widget可见
 * 当设置Widget是否可见的时候，他和他的第一层孩子的是否可见和是否可触碰会同时设置
 */
class WidgetManager
{
public:
	WidgetManager():m_visibleWidget(0){}

	void setUploadScorePanel( * uploadScorePanel) {m_uploadScorePanel = uploadScorePanel;}
	void setMenuPanel(Layout* menuPanel)               {m_menuPanel     = menuPanel;}
	void setGameOverPanel(UIPanel* gamrOverPanel)       {m_gamrOverPanel = gamrOverPanel;}
	void setRankList(UIListView* rankList)              {m_rankList      = rankList;}

	void setMenuPanelVisible(bool bGameRunning, bool visible);
	void setGameOverPanelVisible(bool visible);
	void setUploadScorePanelVisible(int position,bool visible);
	void setRankListVisible(bool visible);
private:
	Widget*  m_visibleWidget;

	UIPanel* m_menuPanel;
	UIPanel* m_gamrOverPanel;
	UIPanel* m_uploadScorePanel;
	UIListView* m_rankList;

	///设置是否可见,是否可触碰会同时设置
	bool setWidgetVisible(Widget* widget,bool visible);
};

#endif