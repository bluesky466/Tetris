#include "Tetromino.h"

Tetromino* Tetromino::create(int shape[4][4],float dropDistance,char* fnBlockTexture)
{
	 Tetromino *pRet = new Tetromino(); 
    if (pRet && pRet->init(shape,dropDistance,fnBlockTexture)) 
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

bool Tetromino::init(int shape[4][4],float dropDistance,char* fnBlockTexture)
{
	if(!CCNode::init())
		return false;

	for(int i = 0 ; i<4 ; ++i)
	{
		m_blockSprite[i] = CCSprite::create(fnBlockTexture);

		if(m_blockSprite[i] == 0)
			return false;
	}

	for(int i = 0 ; i<4 ; ++i)
		for(int j = 0 ;j<4 ; ++j)
			m_shape[i][j] = shape[i][j];
		
	m_dropDistance = dropDistance;

	return true;
}