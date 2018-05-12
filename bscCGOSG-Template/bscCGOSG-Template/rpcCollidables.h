#pragma once

#include <list>
#include "raaTrafficLightUnit.h"
#include "raaAnimatedComponent.h"
#include "raaJunctionController.h"

typedef std::list<raaTrafficLightUnit*> raaLights;
typedef std::list<raaAnimatedComponent*> raaVehicles;

class rpcCollidables
{
public:
	virtual ~rpcCollidables();
	static raaLights sm_lLights;
	static raaVehicles sm_lVehicles;

	static rpcCollidables* instance();

	static void addVehicle(raaAnimatedComponent *pVehicle);
	static void addLight(raaTrafficLightUnit *pLight);
	static void toggleDetectionVisibility();
	static void checkDetection();
	static void handleVehicleReactionToLight(raaTrafficLightUnit::rpcTrafficLightState eState, raaAnimatedComponent* pVehicle);
	
protected:
	rpcCollidables();

	static rpcCollidables* sm_pInstance;

};