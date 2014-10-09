#include "RankList.h"

bool RankList::init()
{
	if(!CCNode::init())
		return false;

	HttpTool::getInstance()->getScoreList(this,getScoreList_selector(RankList::response));

	m_listLayer = 0;

	return true;
}

void RankList::response(const std::vector<std::string>& nickNames,const std::vector<int>& scores)
{
	if(m_listLayer)
		this->removeChild(m_listLayer);

	m_listLayer = UILayer::create();
	m_listLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("RankListUi.ExportJson"));
	this->addChild(m_listLayer);

	UIListView* list = (UIListView*)m_listLayer->getWidgetByName("ranklist");
	for(int i = 0 ; i<nickNames.size() ; ++i)
	{
		UIWidget* item;

		if(i%2)
			item = GUIReader::shareReader()->widgetFromJsonFile("DoubleLine/DoubleLine.ExportJson");
		else
			item = GUIReader::shareReader()->widgetFromJsonFile("SingleLineUi/SingleLineUi.ExportJson");

		UILabelAtlas* atl = (UILabelAtlas*)item->getChildByName("index");
		CCString str;
		str.initWithFormat("%d",i+1);
		atl->setStringValue(str.getCString());
		
		atl = (UILabelAtlas*)item->getChildByName("score");
		str.initWithFormat("%d",scores[i]);
		atl->setStringValue(str.getCString());

		UILabel* lab = (UILabel*)item->getChildByName("nickname");
		lab->setText(nickNames[i].c_str());

		list->insertCustomItem(item,i);
	}
}