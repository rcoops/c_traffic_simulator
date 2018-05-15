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
	
	if (c_dTimeSinceStatusChange > csm_dLightChangeTime) // every 5 seconds
	{
		(*m_itLight)->setLightState(raaTrafficLightUnit::slow); // current light
		raaLights::iterator itLights = m_itLight;
		if (++itLights == m_lLights.end()) itLights = m_lLights.begin(); // 'next' one on
		(*itLights)->setLightState(raaTrafficLightUnit::ready);
	}
	if (c_dTimeSinceStatusChange > csm_dLightChangeTime + 2.0f) // 2 seconds later
	{
		(*m_itLight)->setLightState(raaTrafficLightUnit::stop); // current light
		if (++m_itLight == m_lLights.end()) m_itLight = m_lLights.begin(); // move iterator to next light
		(*m_itLight)->setLightState(raaTrafficLightUnit::go);

		m_dLastChangeTime = dSimulationTime; //counter reset to zero
	}
}

// only needs x y translation, and rotation
raaTrafficLightUnit* raaJunctionController::addLight(osg::Vec3f vfPositionRotation)
{
	raaTrafficLightUnit *pL0 = new raaTrafficLightUnit();
	pL0->setTransform(vfPositionRotation[0], vfPositionRotation[1], vfPositionRotation[2]);
	rpcCollidables::instance()->addLight(pL0); // add ref to collidables list
	m_lLights.push_back(pL0); // add to our own list
	m_itLight = m_lLights.begin(); // reset iterator
	return pL0;
}

void raaJunctionController::checkDetection()
{
	if (!g_pRoot) return;
	raaLights::iterator itLight; raaVehicles::iterator itVehicle;
	// Iterate through all vehicles
	for (itVehicle = rpcCollidables::sm_lVehicles.begin(); itVehicle != rpcCollidables::sm_lVehicles.end(); ++itVehicle)
	{
		// Iterate through all lights for the junction IF the vehicle has no 'current' hit light
		// So we can avoid this extra processing if we know there's already a hit on the car
		for (itLight = m_lLights.begin(); itLight != m_lLights.end() && !(*itVehicle)->m_pLightDetected; ++itLight)
		{
			// If we have a hit, set the car's detected light to that hit
			if ((*itVehicle)->canSee(*itLight)) (*itVehicle)->m_pLightDetected = (*itLight);
		}
		if((*itVehicle)->m_pLightDetected) // If the car has a detected light already
		{
			if ((*itVehicle)->canSee((*itVehicle)->m_pLightDetected)) // Ift it's still detected
			{
				(*itVehicle)->reactToLightInSights(); // React
			}
			else
			{
				(*itVehicle)->setSpeed(1.0f); // If the speed's been sped during an orange, it needs to go back to normal
				(*itVehicle)->m_pLightDetected = nullptr; // Reset the car's light to nothing
			}
		}
	}
}

raaJunctionController::~raaJunctionController()
{
	for (raaLights::iterator itLight = m_lLights.begin(); itLight != m_lLights.end(); ++itLight) (*itLight)->unref();
	m_lLights.clear();
}
