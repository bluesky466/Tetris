#include "HttpTool.h"

USING_NS_CC;
USING_NS_CC_EXT;  
using namespace std;
using namespace rapidjson;

HttpTool* HttpTool::s_instane = 0;

HttpTool* HttpTool::getInstance()
{
	if(!s_instane)
		s_instane = new HttpTool;

	return  s_instane;
}

void HttpTool::destroy()
{
	if(s_instane)
	{
		delete s_instane;
		s_instane =0;
	}
}

void HttpTool::uploadScore(const char* nickName, int score, CCObject* pTarget, SEL_UploadScore pSelector)
{
	char strScore[20];
	sprintf(strScore,"%d",score);

	string url = "http://tetrisrankinglist.jd-app.com/uploadscore.php?NICK_NAME=";
	url+=nickName;
	url+="&SCORE=";
	url+=strScore;

	CCHttpRequest* request = new CCHttpRequest();  
	request->setUrl(url.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);  
	request->setResponseCallback(this,callfuncND_selector(HttpTool::uploadScoreResponse));
	request->setTag("uploadscore");
	CCHttpClient::getInstance()->send(request);  
	request->release();

	m_uploadScoreTarget   = pTarget;
	m_uploadScoreSelector = pSelector;
}

void HttpTool::uploadScoreResponse(CCNode* pObject, void* pData)
{
	CCHttpResponse *response = (CCHttpResponse*)pData;  

    if(!response ||!response->isSucceed())
	{
		if(m_uploadScoreTarget && m_uploadScoreSelector)
		{
			(m_uploadScoreTarget->*m_uploadScoreSelector)(false);
			m_uploadScoreTarget   = 0;
			m_uploadScoreSelector = 0;
		}
        return;
	}
  
    std::vector<char> *buffer = response->getResponseData();   
	std::string result;
    for (unsigned int i = 0; i < buffer->size(); i++)  
		result+=(*buffer)[i];

	if(!strcmp("0",result.c_str()))
	{
		if(m_uploadScoreTarget && m_uploadScoreSelector)
		{
			(m_uploadScoreTarget->*m_uploadScoreSelector)(true);
			m_uploadScoreTarget   = 0;
			m_uploadScoreSelector = 0;
		}
	}
	else
	{
		if(m_uploadScoreTarget && m_uploadScoreSelector)
		{
			(m_uploadScoreTarget->*m_uploadScoreSelector)(false);
			m_uploadScoreTarget   = 0;
			m_uploadScoreSelector = 0;
		}
	}
}

void HttpTool::getScoreList(CCObject* pTarget, SEL_GetScoreList pSelector)
{
	CCHttpRequest* request = new CCHttpRequest();  
	request->setUrl("http://tetrisrankinglist.jd-app.com/getscorelist.php");
	request->setRequestType(CCHttpRequest::kHttpGet);  
	request->setResponseCallback(this,callfuncND_selector(HttpTool::getScoreListResponse));
	request->setTag("getScoreList");

	CCHttpClient::getInstance()->send(request);  
	request->release();

	m_getScoreListTarget   = pTarget;
	m_getScoreListSelector = pSelector;
}

void HttpTool::getScoreListResponse(CCNode* pObject, void* pData)
{
	CCHttpResponse *response = (CCHttpResponse*)pData;

    if(!response || !response->isSucceed())
        return;
  
	string json;
    vector<char> *buffer = response->getResponseData();   
    for (unsigned int i = 0; i < buffer->size(); i++)  
		json+=(*buffer)[i];
	
    rapidjson::Document doc;       
	doc.Parse<0>(json.c_str());     

    if (doc.HasParseError())  
        return;

	vector<string> nickNameList;
	vector<int>    scoreList;

	char strIndex[3];
	for(int i = 1 ; i<11 ; ++i)
	{
		sprintf(strIndex,"%d",i);
		if (doc.HasMember(strIndex))
		{
			const Value &info = doc[strIndex];

			if (info.IsArray() && info.Size() == 2)  
			{  
				//êÇ³Æ
				const Value &nickName = info[(SizeType)0];
				if(nickName.IsString())
					nickNameList.push_back(nickName.GetString());

				//·ÖÊý
				const Value &score = info[(SizeType)1];  
				if (score.IsInt())
					scoreList.push_back(score.GetInt());
			} 
		}
		else
			break;
	}

	if(m_getScoreListTarget && m_getScoreListSelector)
	{
		(m_getScoreListTarget->*m_getScoreListSelector)(nickNameList,scoreList);
		m_getScoreListTarget   = 0;
		m_getScoreListSelector = 0;
	}
}

void HttpTool::getPosition(int score, cocos2d::CCObject* pTarget, SEL_GetPosition pSelector)
{
	char strScore[20];
	sprintf(strScore,"%d",score);

	string url = "http://tetrisrankinglist.jd-app.com/getposition.php?SCORE=";
	url+=strScore;

	CCHttpRequest* request = new CCHttpRequest();  
	request->setUrl(url.c_str());
	request->setRequestType(CCHttpRequest::kHttpGet);  
	request->setResponseCallback(this,callfuncND_selector(HttpTool::getPositionResponse));
	request->setTag("getPosition");
	CCHttpClient::getInstance()->send(request);  
	request->release();

	m_getPositionTarget   = pTarget;
	m_getPositionSelector = pSelector;
}

void HttpTool::getPositionResponse(CCNode* pObject, void* pData)
{
	CCHttpResponse *response = (CCHttpResponse*)pData;  

    if(!response ||!response->isSucceed())
	{
		if(m_uploadScoreTarget && m_uploadScoreSelector)
		{
			(m_getPositionTarget->*m_getPositionSelector)(-1);
			m_getPositionTarget   = 0;
			m_getPositionSelector = 0;
		}
        return;
	}
  
    std::vector<char> *buffer = response->getResponseData();   
	std::string result;
    for (unsigned int i = 0; i < buffer->size(); i++)  
		result+=(*buffer)[i];

	
	if(m_getPositionTarget && m_getPositionSelector)
	{
		(m_getPositionTarget->*m_getPositionSelector)(atoi(result.c_str()));
		m_getPositionTarget   = 0;
		m_getPositionSelector = 0;
	}
	
}