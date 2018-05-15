#include "stdafx.h"
#include "raaTileXJunction.h"
#include <cmath>


raaTileXJunction::raaTileXJunction(const unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP): raaTile(uiName, pNode, pAP)
{
	raaTileXJunction::addLights();
	m_pLocalRoot->addUpdateCallback(this);
}

void raaTileXJunction::addLights()
{
	float fRotation = -90.0f; // set to -90 so we can use += 90 for every loop iteration (start at 0)
	const float fOutsidePositionRange = 3 * csm_fAbsoluteLightPosition; // 600
	const float fPositionStep = 2 * csm_fAbsoluteLightPosition; // 400
	// we want bottom-left, bottom-right top-right, top-left (to go round in a circle)
	for (int y = -csm_fAbsoluteLightPosition; y < fOutsidePositionRange; y += fPositionStep) { // -200, 200, break (600)
		for (int x = y; abs(x) < fOutsidePositionRange; x -= y * 2) { // -200, 200, break (600) , 200, -200, break (-600)
			raaTrafficLightUnit *pL0 = addLight(osg::Vec3f(x, y, fRotation += 90.0f)); // start at 0 and then rotate 90 each time
			m_pLocalRoot->addChild(pL0->node());
		}
	}
}

raaTileXJunction::~raaTileXJunction() {}
