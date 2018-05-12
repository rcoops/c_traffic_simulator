#include "stdafx.h"
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
	float fRotation = -90.0f; // set to -90 so we can use += 90 for every loop iteration
	const float fOutsidePositionRange = 3 * csm_fAbsoluteLightPosition;
	const float fPositionStep = 2 * csm_fAbsoluteLightPosition;
	for (int y = -csm_fAbsoluteLightPosition; y < fOutsidePositionRange; y += fPositionStep) {
		for (int x = y; abs(x) < fOutsidePositionRange; x -= y * 2) {
			raaTrafficLightUnit *pL0 = new raaTrafficLightUnit();
			pL0->setTransform(x, y, fRotation += 90.0f);
			pL0->ref();
			m_pLocalRoot->addChild(pL0->node());
			rpcCollidables::addLight(pL0);
			m_lLights.push_back(pL0);
			m_itLight = m_lLights.begin();
		}
	}
}

// AnimationPathCallback.setTimeMultiplier(double) to slow down 

raaTileXJunction::~raaTileXJunction()
{
	for (raaLights::iterator itLight = m_lLights.begin(); itLight != m_lLights.end(); ++itLight)
	{
		(*itLight)->unref();
	}
	m_lLights.clear();
}
