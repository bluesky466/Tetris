#include "RankList.h"

RankList* RankList::create(ListView* list)
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

bool RankList::init(ListView* list)
{
	if(!CCNode::init())
		return false;

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
		Widget* item = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("RankListItem/RankListItem.ExportJson");

		TextAtlas* atl = (TextAtlas*)Helper::seekWidgetByName(item,"index");
		__String* str = __String::createWithFormat("%d",i+1);
		
		atl->setString(str->getCString());
		
		atl = (TextAtlas*)Helper::seekWidgetByName(item,"score");
		str= __String::createWithFormat("%d",scores[i]);
		atl->setString(str->getCString());

		Text* lab = (Text*)Helper::seekWidgetByName(item,"nickname");
		lab->setString(nickNames[i].c_str());

		m_list->insertCustomItem(item,i);
	}
}