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

	void addVehicle(raaAnimatedComponent *pVehicle);
	void addLight(raaTrafficLightUnit *pLight);
	void toggleDetectionVisibility();
	void checkDetection();
	void handleVehicleReactionToLight(raaTrafficLightUnit::rpcTrafficLightState eState, raaAnimatedComponent* pVehicle);
	
protected:
	rpcCollidables();

	static rpcCollidables* sm_pInstance;

};