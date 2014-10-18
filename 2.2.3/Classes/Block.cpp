#include "Block.h"

Block* Block::create(const char *pszFileName)
{
    Block *pobBlock = new Block();
    if (pobBlock && pobBlock->initWithFile(pszFileName))
    {
        pobBlock->autorelease();
        return pobBlock;
    }
    CC_SAFE_DELETE(pobBlock);
    return NULL;
}

bool Block::initWithFile(const char *pszFilename)
{
	if(!CCSprite::initWithFile(pszFilename))
		return false;

	m_index = FragmentEffect::getInstance()->addEffectNode(this);
	this->setAnchorPoint(ccp(1.0f,0.0f));
	return true;
}

void Block::setEffect(unsigned int effectSel)
{
	FragmentEffect::getInstance()->setEffect(m_index,effectSel);
}

void Block::setBlockSize(float size)
{
	this->setScaleX(size/m_obContentSize.width);
	this->setScaleY(size/m_obContentSize.height);	
}