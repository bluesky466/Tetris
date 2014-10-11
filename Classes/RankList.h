#ifndef _RANK_LIST_H_
#define _RANK_LIST_H_


#include "cocos2d.h"
#include "cocos-ext.h"
#include "HttpTool.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

class RankList : public CCNode
{
public:
	bool init(UIListView* list);
	void response(const std::vector<std::string>&,const std::vector<int>&);

	void downloadRankList()
	{
		HttpTool::getInstance()->getScoreList(this,getScoreList_selector(RankList::response));
	}

	static RankList* create(UIListView* list);

private:
	UIListView* m_list;
};

#endif