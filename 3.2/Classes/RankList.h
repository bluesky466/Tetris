#ifndef _RANK_LIST_H_
#define _RANK_LIST_H_


#include "cocos2d.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"
#include "HttpTool.h"

USING_NS_CC;
using namespace ui;

class RankList : public Node
{
public:
	bool init(ListView* list);
	void response(const std::vector<std::string>&,const std::vector<int>&);

	void downloadRankList()
	{
		HttpTool::getInstance()->getScoreList(this,getScoreList_selector(RankList::response));
	}

	static RankList* create(ListView* list);

private:
	ListView* m_list;
};

#endif