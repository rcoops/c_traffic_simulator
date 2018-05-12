#include "stdafx.h"

#include <iostream>
#include <cmath>

#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"
#include "raaJunctionController.h"
#include "rpcCollidables.h"

const double raaJunctionController::csm_dLightChangeTime = 5.0f;

raaJunctionController::raaJunctionController(): m_lLights()
{
}

raaJunctionController::~raaJunctionController()
{
}

void raaJunctionController::operator()(osg::Node* node, osg::NodeVisitor* nv) 
{
	cycleTrafficLights(nv);
	rpcCollidables::checkDetection();
	nv->traverse(*node);
}

void raaJunctionController::cycleTrafficLights(osg::NodeVisitor* pNodeVisitor)
{
	const double dSimulationTime = round(pNodeVisitor->getFrameStamp()->getSimulationTime());
	const double c_dTimeSinceStatusChange = dSimulationTime - m_dLastChangeTime;
	
	if (c_dTimeSinceStatusChange > csm_dLightChangeTime)
	{
		(*m_itLight)->setLightState(raaTrafficLightUnit::SLOW);
		raaLights::iterator it = m_itLight;
		if (++it == m_lLights.end()) it = m_lLights.begin();
		(*it)->setLightState(raaTrafficLightUnit::READY);
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
