#include "stdafx.h"
#include "raaRoadSet.h"
#include "raaTrafficLightUnit.h"
#include "raaTileXJunction.h"
#include <cmath>


const float raaTileXJunction::csm_fAbsoluteLightPosition = 200.0f;

raaTileXJunction::raaTileXJunction(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP): raaTile(uiName, pNode, pAP)
{
	addLights();
	m_pLocalRoot->addUpdateCallback(this);
}

// TODO - extract some of this to a parent class inheritable by T and X
// TODO - make this a junction controller virtual function?
void raaTileXJunction::addLights()
{
	raaTrafficLightUnit *pL0;
	float fRotation = -90.0f; // set to -90 so we can use += 90 for every loop iteration
	const float fOutsidePositionRange = 3 * csm_fAbsoluteLightPosition;
	const float fPositionStep = 2 * csm_fAbsoluteLightPosition;
	for (int y = -csm_fAbsoluteLightPosition; y < fOutsidePositionRange; y += fPositionStep) {
		for (int x = y; abs(x) < fOutsidePositionRange; x -= y * 2) {
			pL0 = new raaTrafficLightUnit();
			pL0->setTransform(x, y, fRotation += 90.0f);
			m_pLocalRoot->addChild(pL0->node());
			addLightUnit(pL0);
		}
	}
}

// AnimationPathCallback.setTimeMultiplier(double) to slow down 

raaTileXJunction::~raaTileXJunction()
{
}
