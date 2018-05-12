#include "stdafx.h"

#include <iostream>
#include <cmath>

#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"
#include "raaJunctionController.h"
#include "rpcCollidables.h"

const double raaJunctionController::csm_dLightChangeTime = 5.0f;
const float raaJunctionController::csm_fAbsoluteLightPosition = 200.0f;

raaJunctionController::raaJunctionController(): m_lLights()
{
}

raaJunctionController::~raaJunctionController()
{
}

void raaJunctionController::operator()(osg::Node* node, osg::NodeVisitor* nv) 
{
	cycleTrafficLights(nv);
	rpcCollidables::instance()->checkDetection();
	nv->traverse(*node);
}

void raaJunctionController::cycleTrafficLights(osg::NodeVisitor* pNodeVisitor)
{
	if (!(*m_itLight)) return;

	const double dSimulationTime = round(pNodeVisitor->getFrameStamp()->getSimulationTime());
	const double c_dTimeSinceStatusChange = dSimulationTime - m_dLastChangeTime;
	
	if (c_dTimeSinceStatusChange > csm_dLightChangeTime)
	{
		(*m_itLight)->setLightState(raaTrafficLightUnit::SLOW);
		raaLights::iterator itLights = m_itLight;
		if (++itLights == m_lLights.end()) itLights = m_lLights.begin();
		(*itLights)->setLightState(raaTrafficLightUnit::READY);
	}
	if (c_dTimeSinceStatusChange > csm_dLightChangeTime + 2.0f)
	{
		(*m_itLight)->setLightState(raaTrafficLightUnit::STOP);
		if (++m_itLight == m_lLights.end()) m_itLight = m_lLights.begin();
		(*m_itLight)->setLightState(raaTrafficLightUnit::GO);

		//counter reset to zero
		m_dLastChangeTime = dSimulationTime;
	}
}

raaTrafficLightUnit* raaJunctionController::addLight(osg::Vec3f vfPositionRotation)
{
	raaTrafficLightUnit *pL0 = new raaTrafficLightUnit();
	pL0->setTransform(vfPositionRotation[0], vfPositionRotation[1], vfPositionRotation[2]);
	pL0->ref();
	rpcCollidables::instance()->addLight(pL0);
	m_lLights.push_back(pL0);
	m_itLight = m_lLights.begin();
	return pL0;
}
