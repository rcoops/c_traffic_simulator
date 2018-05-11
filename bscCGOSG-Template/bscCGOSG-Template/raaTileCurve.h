#pragma once
#include "raaTile.h"

class raaTileCurve: public raaTile
{
public:
	raaTileCurve(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP);
	virtual ~raaTileCurve();
};

