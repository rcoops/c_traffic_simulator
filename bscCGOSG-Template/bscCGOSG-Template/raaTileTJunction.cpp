#include "stdafx.h"
#include "raaRoadSet.h"
#include "raaTileTJunction.h"

const float raaTileTJunction::csm_fRotation = 90.0f;

raaTileTJunction::raaTileTJunction(const unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP): raaTile(uiName, pNode, pAP)
{
	m_pOffset->setMatrix(osg::Matrix::translate(-2.0f * raaRoadSet::sm_fTileUnit, 0.0f, 0.0f));
	raaTileTJunction::addLights();
	m_pLocalRoot->addUpdateCallback(this);
}

void raaTileTJunction::addLights()
{
	// just times these by the absolute to get the right co-ordinates
	float y[3] = {-1, 1, 1};
	float x[3] = {-1, -1, 1};
	float rotation[3] = { 0.0f, -1.0f, 2.0f };
	for (unsigned int i = 0; i < 3; ++i)
	{
		raaTrafficLightUnit *pL0 = addLight(osg::Vec3f(x[i] * csm_fAbsoluteLightPosition, y[i] * csm_fAbsoluteLightPosition, csm_fRotation * rotation[i]));
		m_pLocalRoot->addChild(pL0->node());
	}
}

raaTileTJunction::~raaTileTJunction() {}
