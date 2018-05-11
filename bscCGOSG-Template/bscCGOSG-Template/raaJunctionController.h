#pragma once

#include <windows.h>
#include <osg/NodeCallback>

#include <list>

class raaTrafficLightUnit;
class raaAnimatedComponent;

typedef std::list<raaTrafficLightUnit*>raaLights;
typedef std::list<raaAnimatedComponent*>raaVehicles;

extern osg::Group *g_pRoot; // TODO pass this into the controller?

// a basic junction controller. This will manage the lights as a single junction. You should develop and extend this to manage a set of lights 
// as a single junction. 

class raaJunctionController: public osg::NodeCallback
{
public:
	raaJunctionController();
	virtual ~raaJunctionController();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	static void addVehicle(raaAnimatedComponent *pVehicle);
	void addLightUnit(raaTrafficLightUnit *pLight);
	static void toggleDetectionBoxes();
	static void toggleDetectionVisibility();

protected:
	static const double csm_dLightChangeTime;
	static raaVehicles sm_lVehicles;
	raaLights m_lLights;
	raaLights::iterator m_itLight;
	double m_dLastChangeTime = 0.0;

	void cycleTrafficLights(osg::NodeVisitor* nv);
	void checkDetection();
	void handleVehicleReactionToLight(raaTrafficLightUnit::rpcTrafficLightState eState, raaAnimatedComponent* pVehicle);
};

