#pragma once

#include <list>
#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"
#include "rpcVehicleFunctor.h"
#include "rpcTrafficLightFunctor.h"
#include "rpcVehicleFactory.h"

typedef std::list<raaTrafficLightUnit*> raaLights;
typedef std::list<raaAnimatedComponent*> raaVehicles;

class rpcCollidables
{
public:
	const static bool csm_bSpeedUp;
	const static bool csm_bSpeedDown;

	static raaLights sm_lLights;
	static raaVehicles sm_lVehicles;

	bool m_bIsGlobalPause = false;
	float m_fGlobalTimeMultiplier = 1.0f;
	raaTrafficLightUnit::rpcTrafficLightState m_eLightState = raaTrafficLightUnit::rpcTrafficLightState::off;

	static rpcCollidables* instance();

	static void addVehicle(raaAnimatedComponent *pVehicle);
	static void createRandomAnimatedComponent();
	static void createAnimatedComponent(const rpcVehicleFactory::vehicleType eVehicleType);

	static void addLight(raaTrafficLightUnit *pLight);
	static void toggleDetectionVisibility();
	void toggleLightStateManual();
	static void toggleLightStateAutomatic();
	void adjustVehicleSpeed(const bool bIsIncrease);
	void pauseVehicles();

	virtual ~rpcCollidables();
	
protected:
	rpcCollidables();

	static void performOnAllVehicles(rpcVehicleFunctor* pFunctor);
	static void performOnAllLights(rpcTrafficLightFunctor* pFunctor);
	void cycleManualState();

	static rpcCollidables* sm_pInstance;
};