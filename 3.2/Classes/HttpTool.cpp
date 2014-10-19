#include "HttpTool.h"

USING_NS_CC;
using namespace std;
using namespace network;

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

void HttpTool::uploadScore(const char* nickName, int score, Ref* pTarget, SEL_UploadScore pSelector)
{
	char strScore[20];
	sprintf(strScore,"%d",score);

	string url = "http://tetrisrankinglist.jd-app.com/uploadscore.php?NICK_NAME=";
	url+=nickName;
	url+="&SCORE=";
	url+=strScore;

	HttpRequest* request = new HttpRequest();  
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);  
	request->setResponseCallback(CC_CALLBACK_2(HttpTool::uploadScoreResponse,this));
	request->setTag("uploadscore");
	HttpClient::getInstance()->send(request);  
	request->release();

	m_uploadScoreTarget   = pTarget;
	m_uploadScoreSelector = pSelector;
}

void HttpTool::uploadScoreResponse(HttpClient* client, HttpResponse* response)
{ 
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

void HttpTool::getScoreList(Ref* pTarget, SEL_GetScoreList pSelector)
{
	HttpRequest* request = new HttpRequest();  
	request->setUrl("http://tetrisrankinglist.jd-app.com/getscorelist.php");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(HttpTool::getScoreListResponse,this));
	request->setTag("getScoreList");

	HttpClient::getInstance()->send(request);  
	request->release();

	m_getScoreListTarget   = pTarget;
	m_getScoreListSelector = pSelector;
}

void HttpTool::getScoreListResponse(HttpClient* client, HttpResponse* response)
{
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
			const rapidjson::Value &info = doc[strIndex];

			if (info.IsArray() && info.Size() == 2)  
			{  
				//êÇ³Æ
				const rapidjson::Value &nickName = info[(rapidjson::SizeType)0];
				if(nickName.IsString())
					nickNameList.push_back(nickName.GetString());

				//·ÖÊý
				const rapidjson::Value &score = info[(rapidjson::SizeType)1];  
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

void HttpTool::getPosition(int score, cocos2d::Ref* pTarget, SEL_GetPosition pSelector)
{
	char strScore[20];
	sprintf(strScore,"%d",score);

	string url = "http://tetrisrankinglist.jd-app.com/getposition.php?SCORE=";
	url+=strScore;

	HttpRequest* request = new HttpRequest();  
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);  
	request->setResponseCallback(CC_CALLBACK_2(HttpTool::getPositionResponse,this));
	request->setTag("getPosition");
	HttpClient::getInstance()->send(request);  
	request->release();

	m_getPositionTarget   = pTarget;
	m_getPositionSelector = pSelector;
}

void HttpTool::getPositionResponse(HttpClient* client, HttpResponse* response)
{
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