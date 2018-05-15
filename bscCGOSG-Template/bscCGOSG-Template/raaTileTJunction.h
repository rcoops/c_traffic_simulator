#pragma once
#include "raaTile.h"
#include "raaJunctionController.h"

class raaTileTJunction: public raaTile, public raaJunctionController
{
public:
	raaTileTJunction(const unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP);
	virtual ~raaTileTJunction();

protected:
	const static float csm_fRotation;

	void addLights() override;
};

