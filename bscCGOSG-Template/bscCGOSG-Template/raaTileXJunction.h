#pragma once
#include "raaTile.h"
#include "raaJunctionController.h"
#include "raaRoadSet.h"
#include "raaTrafficLightUnit.h"
#include "rpcCollidables.h"

class raaTileXJunction: public raaTile, public raaJunctionController 
{
public:
	raaTileXJunction(const unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP);
	virtual ~raaTileXJunction();

protected:
	void addLights() override;
};

