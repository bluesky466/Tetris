#include "WidgetManager.h"

bool WidgetManager::setWidgetVisible(Widget* widget,bool visible)
{
	if(!widget)
		return false;

	widget->setVisible(visible);
	widget->setTouchEnabled(visible);

	CCArray* arr = widget->getChildren();
	CCObject* pObj;

	CCARRAY_FOREACH(arr,pObj)
	{
		
		Widget* pChild =(Widget*)pObj;
		pChild->setVisible(visible);
		pChild->setTouchEnabled(visible);
	}

	if(visible && m_visibleWidget!=widget)
	{
		if(m_visibleWidget && m_visibleWidget->isVisible())
		{
			m_visibleWidget->setVisible(false);
			m_visibleWidget->setTouchEnabled(false);

			CCArray* arr = m_visibleWidget->getChildren();
			CCObject* pObj;

			CCARRAY_FOREACH(arr,pObj)
			{
				Widget* pChild =(Widget*)pObj;
				pChild->setVisible(false);
				pChild->setTouchEnabled(false);
			}
		}

		m_visibleWidget = widget;
	}

	return true;
}

void WidgetManager::setMenuPanelVisible(bool bGameRunning,bool visible)
{
	setWidgetVisible(m_menuPanel,visible);
	
	if(visible)
	{
		if(bGameRunning)
		{
			UIWidget* pChild = (UIWidget*)m_menuPanel->getChildByTag(0);
			pChild->setVisible(false);
			pChild->setTouchEnabled(false);
		}
		else
		{
			UIWidget* pChild = (UIWidget*)m_menuPanel->getChildByTag(1);
			pChild->setVisible(false);
			pChild->setTouchEnabled(false);
		}
	}
}

void WidgetManager::setGameOverPanelVisible(bool visible)
{
	setWidgetVisible(m_gamrOverPanel,visible);
}

void WidgetManager::setUploadScorePanelVisible(int position,bool visible)
{
	setWidgetVisible(m_uploadScorePanel,visible);

	if(visible)
	{
		char str[20];
		sprintf(str,"%d",position);
		((UILabelAtlas*)m_uploadScorePanel->getChildByTag(0))->setStringValue(str);
	}
}

void WidgetManager::setRankListVisible(bool visible)
{
	setWidgetVisible(m_rankList,visible);
}