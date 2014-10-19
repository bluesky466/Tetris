#include "GameLayoutManager.h"

void GameLayoutManager::setChildrenVisible(Layout* pLayout,bool bVisible)
{
	Vector<Node*> children = pLayout->getChildren();

	auto i   = children.begin();
	auto end = children.cend();

	while(i != end)
	{
		Layout* pChild =(Layout*)(*i);
		pChild->setVisible(bVisible);
		pChild->setTouchEnabled(bVisible);

		i++;
	}
}

bool GameLayoutManager::setLayoutVisible(Layout* pLayout,bool bVisible)
{
	if(!pLayout)
		return false;

	pLayout->setVisible(bVisible);
	pLayout->setTouchEnabled(bVisible);

	setChildrenVisible(pLayout,bVisible);


	if(bVisible && m_visibleLayout != pLayout)
	{
		if(m_visibleLayout && m_visibleLayout->isVisible())
		{
			m_visibleLayout->setVisible(false);
			m_visibleLayout->setTouchEnabled(false);

			setChildrenVisible(m_visibleLayout,false);
		}

		m_visibleLayout = pLayout;
	}

	return true;
}

void GameLayoutManager::setMenuPanelVisible(bool bGameRunning,bool bVisible)
{
	setLayoutVisible(m_menuPanel,bVisible);
	
	if(bVisible)
	{
		if(bGameRunning)
		{
			Widget* pChild = (Widget*)m_menuPanel->getChildByTag(TAG_BUTTON_START);
			pChild->setVisible(false);
			pChild->setTouchEnabled(false);
		}
		else
		{
			Widget* pChild = (Widget*)m_menuPanel->getChildByTag(TAG_BUTTON_CONTINUE);
			pChild->setVisible(false);
			pChild->setTouchEnabled(false);
		}
		
	}
}

void GameLayoutManager::setGameOverPanelVisible(bool bVisible)
{
	setLayoutVisible(m_gamrOverPanel,bVisible);
}

void GameLayoutManager::setUploadScorePanelVisible(int position,bool bVisible)
{
	setLayoutVisible(m_uploadScorePanel,bVisible);

	if(bVisible)
	{
		char str[20];
		sprintf(str,"%d",position);
		((TextAtlas*)m_uploadScorePanel->getChildByTag(0))->setString(str);
	}
}

void GameLayoutManager::setRankListVisible(bool bVisible)
{
	setLayoutVisible(m_rankList,bVisible);
}