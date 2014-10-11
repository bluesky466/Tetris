#include "RankList.h"

RankList* RankList::create(UIListView* list)
{ 
    RankList *pRet = new RankList(); 
    if (pRet && pRet->init(list)) 
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    } 
}

bool RankList::init(UIListView* list)
{
	if(!CCNode::init())
		return false;

	HttpTool::getInstance()->getScoreList(this,getScoreList_selector(RankList::response));

	m_list = list;

	return true;
}

void RankList::response(const std::vector<std::string>& nickNames,const std::vector<int>& scores)
{
	if(!m_list)
		return ;

	m_list->removeAllItems();

	for(int i = 0 ; i<nickNames.size() ; ++i)
	{
		UIWidget* item = GUIReader::shareReader()->widgetFromJsonFile("RankListItem/RankListItem.ExportJson");

		UILabelAtlas* atl = (UILabelAtlas*)item->getChildByName("index");
		CCString str;
		str.initWithFormat("%d",i+1);
		atl->setStringValue(str.getCString());
		
		atl = (UILabelAtlas*)item->getChildByName("score");
		str.initWithFormat("%d",scores[i]);
		atl->setStringValue(str.getCString());

		UILabel* lab = (UILabel*)item->getChildByName("nickname");
		lab->setText(nickNames[i].c_str());

		m_list->insertCustomItem(item,i);
	}
}