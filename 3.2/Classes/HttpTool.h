#include "cocos2d.h"
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"

typedef void (cocos2d::Ref::*SEL_UploadScore)(bool);
#define uploadScore_selector(_SELECTOR) (SEL_UploadScore)(&_SELECTOR)

typedef void (cocos2d::Ref::*SEL_GetScoreList)(const std::vector<std::string>&,const std::vector<int>&);
#define getScoreList_selector(_SELECTOR) (SEL_GetScoreList)(&_SELECTOR)

typedef void (cocos2d::Ref::*SEL_GetPosition)(int);
#define getPosition_selector(_SELECTOR) (SEL_GetPosition)(&_SELECTOR)

class HttpTool : public cocos2d::Ref
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

	void uploadScore(const char* nickName, int score, cocos2d::Ref* pTarget, SEL_UploadScore pSelector);
	void uploadScoreResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

	void getScoreList(cocos2d::Ref* pTarget, SEL_GetScoreList pSelector);
	void getScoreListResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
	
	void getPosition(int score, cocos2d::Ref* pTarget, SEL_GetPosition pSelector);
	void getPositionResponse(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
private:
	static HttpTool* s_instane;

	cocos2d::Ref* m_uploadScoreTarget;
	SEL_UploadScore    m_uploadScoreSelector;

	cocos2d::Ref* m_getScoreListTarget;
	SEL_GetScoreList   m_getScoreListSelector;

	cocos2d::Ref* m_getPositionTarget;
	SEL_GetPosition    m_getPositionSelector;
};