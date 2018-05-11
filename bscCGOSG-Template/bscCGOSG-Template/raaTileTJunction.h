#pragma once
#include "raaTile.h"

class raaTileTJunction: public raaTile
{
public:
	raaTileTJunction(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP);
	virtual ~raaTileTJunction();
};

