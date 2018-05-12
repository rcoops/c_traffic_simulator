#pragma once
#include "raaTile.h"
#include "raaJunctionController.h"

class raaTileTJunction: public raaTile, public raaJunctionController
{
public:
	raaTileTJunction(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP);
	virtual ~raaTileTJunction();

protected:
	void addLights() override;
};

