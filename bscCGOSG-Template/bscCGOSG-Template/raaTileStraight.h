#pragma once
#include "raaTile.h"

class raaTileStraight: public raaTile
{
public:
	raaTileStraight(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP);
	virtual ~raaTileStraight();
};

