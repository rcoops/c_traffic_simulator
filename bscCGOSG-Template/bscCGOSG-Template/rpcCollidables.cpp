#include "stdafx.h"

#include "rpcCollidables.h"
#include "rpcPathSelector.h"

raaVehicles rpcCollidables::sm_lVehicles;
raaLights rpcCollidables::sm_lLights;
rpcCollidables* rpcCollidables::sm_pInstance;

extern osg::Group *g_pRoot; // TODO pass this into the controller?

const bool rpcCollidables::csm_bSpeedUp = true;
const bool rpcCollidables::csm_bSpeedDown = false;

rpcCollidables::rpcCollidables() {}

rpcCollidables* rpcCollidables::instance()
{
	if (!sm_pInstance) sm_pInstance = new rpcCollidables();
	return sm_pInstance;
}

void rpcCollidables::addVehicle(raaAnimatedComponent* pVehicle)
{
	if (pVehicle && find(sm_lVehicles.begin(), sm_lVehicles.end(), pVehicle) == sm_lVehicles.end())
	{
		sm_lVehicles.push_back(pVehicle);
	}
}

void rpcCollidables::addLight(raaTrafficLightUnit* pLight)
{
	if (pLight && find(sm_lLights.begin(), sm_lLights.end(), pLight) == sm_lLights.end())
	{
		sm_lLights.push_back(pLight);
	}
}

void rpcCollidables::toggleDetectionVisibility()
{
	performOnAllVehicles(new rpcVehicleDetectionBoxVisibilityToggleFunctor());
	performOnAllLights(new rpcTrafficLightDetectorVisibilityToggleFunctor());
}

void rpcCollidables::toggleLightStateManual()
{
	cycleManualState();
	performOnAllLights(new rpcToggleLightState(m_eLightState));
}

void rpcCollidables::toggleLightStateAutomatic()
{
	performOnAllLights(new rpcRevertToAutomaticState());
}

void rpcCollidables::adjustVehicleSpeed(const bool bIsIncrease)
{
	const float fAdjustment = bIsIncrease ? 2.0f : 0.5f;
	m_fGlobalTimeMultiplier = m_fGlobalTimeMultiplier * fAdjustment;
	raaAnimatedComponent::setManualMultiplier(m_fGlobalTimeMultiplier);
}

void rpcCollidables::pauseVehicles()
{
	m_bIsGlobalPause = !m_bIsGlobalPause;
	performOnAllVehicles(new rpcPauseVehicles(m_bIsGlobalPause));
}

void rpcCollidables::performOnAllVehicles(rpcVehicleFunctor *pFunc)
{
	raaVehicles::iterator itVehicle = sm_lVehicles.begin();
	for (; itVehicle != sm_lVehicles.end(); ++itVehicle) (*pFunc)(*itVehicle);
}

void rpcCollidables::performOnAllLights(rpcTrafficLightFunctor* pFunctor)
{
	raaLights::iterator itLight = sm_lLights.begin;
	for (; itLight != sm_lLights.end(); ++itLight) (*pFunctor)(*itLight);
}

void rpcCollidables::cycleManualState()
{
	switch (m_eLightState)
	{
	case raaTrafficLightUnit::rpcTrafficLightState::off:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::ready;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::go:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::slow;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::stop:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::off;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::slow:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::stop;
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::ready:
		m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::go;
		break;
	}
	performOnAllLights(new rpcToggleLightState(m_eLightState));
}


void rpcCollidables::createRandomAnimatedComponent()
{
	createAnimatedComponent(raaAnimatedComponent::getRandomType());
}

void rpcCollidables::createAnimatedComponent(const raaAnimatedComponent::vehicleType eVehicleType)
{
	if (!g_pRoot) return;
	rpcContextAwareAnimationPath *pAP = rpcPathSelector::instance()->createRandomPath();
	raaAnimatedComponent *pAnim = raaAnimatedComponent::vehicleFactory(eVehicleType, pAP);
	g_pRoot->addChild(pAnim->root());

	addVehicle(pAnim);
}

rpcCollidables::~rpcCollidables() {}
