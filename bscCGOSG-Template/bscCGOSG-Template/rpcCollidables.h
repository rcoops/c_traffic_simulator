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
	virtual ~rpcCollidables();
	static raaLights sm_lLights;
	static raaVehicles sm_lVehicles;

	bool m_bIsGlobalPause = false;
	double m_dGlobalTimeMultiplier = 1.0;
	raaTrafficLightUnit::rpcTrafficLightState m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::OFF;

	static rpcCollidables* instance();

	void addVehicle(raaAnimatedComponent *pVehicle);
	void addLight(raaTrafficLightUnit *pLight);
	void toggleDetectionVisibility();
	void toggleLightState();
	void adjustVehicleSpeed(bool bIsUp);
	void pauseVehicles();
	
protected:
	rpcCollidables();

	void performOnAllVehicles(rpcVehicleFunctor* pFunctor);
	void performOnAllLights(rpcTrafficLightFunctor* pFunctor);
	void cycleManualState();

	static rpcCollidables* sm_pInstance;
};