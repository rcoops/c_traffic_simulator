#pragma once

#include <list>
#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"
#include "rpcVehicleFunctor.h"
#include "rpcTrafficLightFunctor.h"

typedef std::list<raaTrafficLightUnit*> raaLights;
typedef std::list<raaAnimatedComponent*> raaVehicles;

class rpcCollidables
{
public:
	static raaLights sm_lLights;
	static raaVehicles sm_lVehicles;

	bool m_bIsGlobalPause = false;
	float m_fGlobalTimeMultiplier = 1.0f;
	raaTrafficLightUnit::rpcTrafficLightState m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::off;

	static rpcCollidables* instance();

	void addVehicle(raaAnimatedComponent *pVehicle);
	void addLight(raaTrafficLightUnit *pLight);
	void toggleDetectionVisibility();
	void toggleLightState();
	void adjustVehicleSpeed(const bool bIsIncrease);
	void pauseVehicles();

	virtual ~rpcCollidables();
	
protected:
	rpcCollidables();

	void performOnAllVehicles(rpcVehicleFunctor* pFunctor);
	void performOnAllLights(rpcTrafficLightFunctor* pFunctor);
	void cycleManualState();

	static rpcCollidables* sm_pInstance;
};