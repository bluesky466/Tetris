#include "cocos2d.h"
#include "cocos-ext.h"

typedef void (cocos2d::CCObject::*SEL_UploadScore)(bool);
#define uploadScore_selector(_SELECTOR) (SEL_UploadScore)(&_SELECTOR)

typedef void (cocos2d::CCObject::*SEL_GetScoreList)(const std::vector<std::string>&,const std::vector<int>&);
#define getScoreList_selector(_SELECTOR) (SEL_GetScoreList)(&_SELECTOR)

typedef void (cocos2d::CCObject::*SEL_GetPosition)(int);
#define getPosition_selector(_SELECTOR) (SEL_GetPosition)(&_SELECTOR)

class HttpTool : public cocos2d::CCObject
{
public:
	static HttpTool* getInstance();
	static void destroy();

	HttpTool():
		m_uploadScoreTarget(0),
		m_uploadScoreSelector(0),
		m_getScoreListTarget(0),
		m_getScoreListSelector(0),
		m_getPositionTarget(0),
		m_getPositionSelector(0)
	{

	}

	void uploadScore(const char* nickName, int score, cocos2d::CCObject* pTarget, SEL_UploadScore pSelector);
	void uploadScoreResponse(cocos2d::CCNode*, void*);

	void getScoreList(cocos2d::CCObject* pTarget, SEL_GetScoreList pSelector);
	void getScoreListResponse(cocos2d::CCNode*, void*);
	
	void getPosition(int score, cocos2d::CCObject* pTarget, SEL_GetPosition pSelector);
	void getPositionResponse(cocos2d::CCNode*, void*);
private:
	static HttpTool* s_instane;

	cocos2d::CCObject* m_uploadScoreTarget;
	SEL_UploadScore    m_uploadScoreSelector;

	cocos2d::CCObject* m_getScoreListTarget;
	SEL_GetScoreList   m_getScoreListSelector;

	cocos2d::CCObject* m_getPositionTarget;
	SEL_GetPosition    m_getPositionSelector;
};