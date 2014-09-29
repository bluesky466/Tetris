#include "cocos2d.h"

USING_NS_CC;

class Tetromino : public CCNode
{
public:
	Tetromino();

	bool Rotate(bool bClockwise,int* bgInfo); //Ðý×ª
	bool move(bool bLeft,int* bgInfo);        //×óÓÒÒÆ¶¯
	bool drop(int* bgInfo);                   //ÏÂ½µ

	bool init(int shape[4][4],float dropDistance,char* fnBlockTexture);

	static Tetromino* create(int shape[4][4],float dropDistance,char* fnBlockTexture);

protected:
	int   m_posX;
	int   m_posY;
	int   m_rotate;
	int   m_shape[4][4];
	float m_dropDistance;
	CCSprite* m_blockSprite[4];
};

inline Tetromino::Tetromino():
m_posX(0),
m_posY(0),
m_rotate(0),
m_dropDistance(0.0f)
{
}
