#include "stdafx.h"

#include "rpcCollidables.h"

raaVehicles rpcCollidables::sm_lVehicles;
raaLights rpcCollidables::sm_lLights;
rpcCollidables* rpcCollidables::sm_pInstance;

extern osg::Group *g_pRoot; // TODO pass this into the controller?

rpcCollidables* rpcCollidables::instance()
{
	if (!sm_pInstance) sm_pInstance = new rpcCollidables();

	return sm_pInstance;
}

void rpcCollidables::addVehicle(raaAnimatedComponent* pVehicle)
{
	if (pVehicle && std::find(sm_lVehicles.begin(), sm_lVehicles.end(), pVehicle) == sm_lVehicles.end())
	{
		sm_lVehicles.push_back(pVehicle);
	}
}

void rpcCollidables::addLight(raaTrafficLightUnit* pLight)
{
	if (pLight && std::find(sm_lLights.begin(), sm_lLights.end(), pLight) == sm_lLights.end())
	{
		sm_lLights.push_back(pLight);
	}
}

void rpcCollidables::toggleDetectionVisibility()
{
	for (raaVehicles::iterator itVehicle = sm_lVehicles.begin(); itVehicle != sm_lVehicles.end(); ++itVehicle)
	{
		(*itVehicle)->toggleDetectionBoxVisibility();
	}
	for (raaLights::iterator itLight = sm_lLights.begin(); itLight != sm_lLights.end(); ++itLight)
	{
		(*itLight)->toggleDetectionPointVisibility();
	}
}

void rpcCollidables::checkDetection()
{
	raaVehicles::iterator itVehicle = sm_lVehicles.begin();
	raaLights::iterator itLights;
	for (; itVehicle != sm_lVehicles.end(); itVehicle++)
	{
		for (itLights = sm_lLights.begin(); itLights != sm_lLights.end(); itLights++)
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

void rpcCollidables::handleVehicleReactionToLight(raaTrafficLightUnit::rpcTrafficLightState eState, raaAnimatedComponent* pVehicle)
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

rpcCollidables::rpcCollidables()
{
}

rpcCollidables::~rpcCollidables()
{
}
