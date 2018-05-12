#include "stdafx.h"

#include "rpcCollidables.h"
#include "rpcVehicleDetectionBoxVisibilityToggleFunctor.h"
#include "rpcTrafficLightDetectorVisibilityToggleFunctor.h"
#include <osg/ShapeDrawable>

raaVehicles rpcCollidables::sm_lVehicles;
raaLights rpcCollidables::sm_lLights;
rpcCollidables* rpcCollidables::sm_pInstance;

extern osg::Group *g_pRoot; // TODO pass this into the controller?

class rpcAdjustVehicleSpeed: public rpcVehicleFunctor
{
public:
	rpcAdjustVehicleSpeed(float fMultiplier): m_fMultiplier((fMultiplier)) {}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		printf("Vehicle speed (before): %f\n", pVehicle->getTimeMultiplier());
		pVehicle->setTimeMultiplier(m_fMultiplier);
		printf("Vehicle speed (after): %f\n", pVehicle->getTimeMultiplier());
	}

	virtual ~rpcAdjustVehicleSpeed() {};

protected:
	float m_fMultiplier;
};

class rpcPauseVehicles : public rpcVehicleFunctor
{
public:
	rpcPauseVehicles(bool bIsPause)
	{
		m_bIsPause = bIsPause;
	}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		pVehicle->setPause(m_bIsPause);
	}

	virtual ~rpcPauseVehicles() {};
protected:
	bool m_bIsPause;
};

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

void rpcCollidables::checkDetection()
{
	raaLights::iterator itLights;
	for (raaVehicles::iterator itVehicle = sm_lVehicles.begin(); itVehicle != sm_lVehicles.end(); ++itVehicle)
	{
		for (itLights = sm_lLights.begin(); itLights != sm_lLights.end(); ++itLights)
		{
			osg::Vec3f vfGlobalDetectionPoint = (*itLights)->csm_vfPosition * osg::computeLocalToWorld((*itLights)->m_pRotation->getParentalNodePaths(g_pRoot)[0]);
			rpcDetectionBox *box = (*itVehicle)->m_pDetectionBox;
			osg::BoundingSphere sphere = box->m_pScale->computeBound();
			osg::Vec3f vfDetectionPointLocalToLight = vfGlobalDetectionPoint * osg::computeWorldToLocal(box->m_pRoot->getParentalNodePaths(g_pRoot)[0]);
			bool bIsHit = sphere.contains(vfDetectionPointLocalToLight);
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
		pVehicle->setPause(m_bIsGlobalPause);
		break;
	}
}

void rpcCollidables::adjustVehicleSpeed(float fMultiplier)
{
	performOnAllVehicles(new rpcAdjustVehicleSpeed(fMultiplier));
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

rpcCollidables::~rpcCollidables()
{
}
