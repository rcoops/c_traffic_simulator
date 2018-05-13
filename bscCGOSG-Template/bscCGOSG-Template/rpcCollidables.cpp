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
	performOnAllVehicles(new rpcVehicleDetectionBoxVisibilityToggleFunctor());
	performOnAllLights(new rpcTrafficLightDetectorVisibilityToggleFunctor());
}

void rpcCollidables::toggleLightState()
{
	cycleManualState();
	performOnAllLights(new rpcToggleLightState(m_eLightState));
}

void rpcCollidables::adjustVehicleSpeed(const bool bIsIncrease)
{
	const double dAdjustment = bIsIncrease ? 2.0 : 0.5;
	m_dGlobalTimeMultiplier = m_dGlobalTimeMultiplier * dAdjustment;
	
	performOnAllVehicles(new rpcAdjustVehicleSpeed(m_dGlobalTimeMultiplier));
}

void rpcCollidables::pauseVehicles()
{
	m_bIsGlobalPause = !m_bIsGlobalPause;
	performOnAllVehicles(new rpcPauseVehicles(m_bIsGlobalPause));
}

rpcCollidables::rpcCollidables()
{
}

void rpcCollidables::performOnAllVehicles(rpcVehicleFunctor *pFunc)
{
	for (raaVehicles::iterator itVehicle = sm_lVehicles.begin(); itVehicle != sm_lVehicles.end(); ++itVehicle)
	{
		(*pFunc)(*itVehicle);
	}
}

void rpcCollidables::performOnAllLights(rpcTrafficLightFunctor* pFunctor)
{
	for (raaLights::iterator itLight = sm_lLights.begin(); itLight != sm_lLights.end(); ++itLight)
	{
		(*pFunctor)(*itLight);
	}
}

void rpcCollidables::cycleManualState()
{
	switch (m_eLightState)
	{
	case raaTrafficLightUnit::rpcTrafficLightState::OFF:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::READY;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::GO:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::SLOW;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::STOP:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::OFF;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::SLOW:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::STOP;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::READY:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::GO;
		break;
	}
	performOnAllLights(new rpcToggleLightState(m_eLightState));
}

rpcCollidables::~rpcCollidables()
{
}
