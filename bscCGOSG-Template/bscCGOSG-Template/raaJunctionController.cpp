#include "stdafx.h"

#include <iostream>
#include <cmath>

#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"
#include "raaJunctionController.h"
#include "rpcCollidables.h"

const double raaJunctionController::csm_dLightChangeTime = 5.0f;
const float raaJunctionController::csm_fAbsoluteLightPosition = 200.0f;

extern osg::Group *g_pRoot; // TODO pass this into the controller?

raaJunctionController::raaJunctionController(): m_lLights()
{
}

raaJunctionController::~raaJunctionController()
{
}

void raaJunctionController::operator()(osg::Node* node, osg::NodeVisitor* nv) 
{
	cycleTrafficLights(nv);
	checkDetection();
	nv->traverse(*node);
}

void raaJunctionController::cycleTrafficLights(osg::NodeVisitor* pNodeVisitor)
{
	if (!(*m_itLight) || (*m_itLight)->m_bIsManual) return; // if we've turned off light changes

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

bool raaJunctionController::isHit(raaAnimatedComponent* pVehicle, raaTrafficLightUnit* pLight)
{
	return pVehicle->canSee(pLight->getDetectionPointRelativeTo(g_pRoot), g_pRoot);
}

void raaJunctionController::checkDetection()
{
	raaLights::iterator itLight; raaVehicles::iterator itVehicle;
	// Iterate through all vehicles
	for (itVehicle = rpcCollidables::sm_lVehicles.begin(); itVehicle != rpcCollidables::sm_lVehicles.end(); ++itVehicle)
	{
		// Iterate through all lights for the junction IF the vehicle has no 'current' hit light
		// So we can avoid this extra processing if we know there's already a hit on the car
		for (itLight = m_lLights.begin(); itLight != m_lLights.end() && !(*itVehicle)->m_pLightDetected; ++itLight)
		{
			// If we have a hit, set the car's detected light to that hit
			if (isHit(*itVehicle, *itLight)) (*itVehicle)->m_pLightDetected = (*itLight);
		}
		if((*itVehicle)->m_pLightDetected) // If the car has a detected light already
		{
			if (isHit(*itVehicle, (*itVehicle)->m_pLightDetected)) // Ift it's still detected
			{
				(*itVehicle)->handleVehicleReactionToLight(rpcCollidables::instance()->m_bIsGlobalPause); // React
			}
			else
			{
				(*itVehicle)->setSpeed(1.0f); // If the speed's been sped during an orange, it needs to go back to normal
				(*itVehicle)->m_pLightDetected = nullptr; // Reset the car's light to nothing
			}
		}
	}
}
