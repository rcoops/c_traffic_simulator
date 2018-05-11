#include "stdafx.h"

#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"

#include "raaJunctionController.h"
#include <iostream>
#include <cmath>

raaVehicles raaJunctionController::sm_lVehicles;
const double raaJunctionController::csm_dLightChangeTime = 5.0f;

raaJunctionController::raaJunctionController()
{
}

raaJunctionController::~raaJunctionController()
{
}

void raaJunctionController::toggleDetectionBoxes()
{
	raaVehicles::iterator it = sm_lVehicles.begin();
	for (; it != sm_lVehicles.end(); it++)
	{
		(*it)->toggleDetectionBox();
	}
}

void raaJunctionController::toggleDetectionVisibility()
{
	raaVehicles::iterator itVehicle;
	for (itVehicle = sm_lVehicles.begin(); itVehicle != sm_lVehicles.end(); itVehicle++)
	{
		(*itVehicle)->toggleDetectionBoxVisibility();
	}
}

void raaJunctionController::addVehicle(raaAnimatedComponent* pVehicle)
{
	if (pVehicle && std::find(sm_lVehicles.begin(), sm_lVehicles.end(), pVehicle) == sm_lVehicles.end())
		sm_lVehicles.push_back(pVehicle);
}

void raaJunctionController::addLightUnit(raaTrafficLightUnit* pLight)
{
	if(pLight && std::find(m_lLights.begin(), m_lLights.end(), pLight)==m_lLights.end())
	{
		m_lLights.push_back(pLight);
		m_itLight = m_lLights.begin();
	}
}

void raaJunctionController::operator()(osg::Node* node, osg::NodeVisitor* nv) 
{
	cycleTrafficLights(nv);
	checkDetection();
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

void raaJunctionController::checkDetection()
{
	raaVehicles::iterator itVehicle = sm_lVehicles.begin();
	raaLights::iterator itLights;
	for (; itVehicle != sm_lVehicles.end(); itVehicle++)
	{
		for (itLights = m_lLights.begin(); itLights != m_lLights.end(); itLights++)
		{
			osg::Vec3f vfGlobalDetectionPoint = (*itLights)->m_vfPosition * osg::computeLocalToWorld((*itLights)->node()->getParentalNodePaths(g_pRoot)[0]);
			rpcDetectionBox *box = (*itVehicle)->m_pDetectionBox;
			bool bIsHit = box->m_pRoot->getBound().contains(vfGlobalDetectionPoint * osg::computeWorldToLocal(box->m_pRoot->getParentalNodePaths(g_pRoot)[0]));
			if (bIsHit)
			{
				handleVehicleReactionToLight((*itLights)->eTrafficLightState, *itVehicle);
			}
			else
			{
				(*itVehicle)->setTimeMultiplier(1.0f);
			}
		}
	}
}

void raaJunctionController::handleVehicleReactionToLight(raaTrafficLightUnit::rpcTrafficLightState eState, raaAnimatedComponent* pVehicle)
{
	switch (eState)
	{
	case raaTrafficLightUnit::rpcTrafficLightState::STOP:
		pVehicle->setPause(true);
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::SLOW:
		pVehicle->setTimeMultiplier(2.0f);
	case raaTrafficLightUnit::rpcTrafficLightState::READY:
		pVehicle->setTimeMultiplier(0.1f);
	default:
		pVehicle->setPause(false);
		break;
	}
}
