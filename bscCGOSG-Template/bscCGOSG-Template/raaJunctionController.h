#pragma once

#include <windows.h>
#include <osg/NodeCallback>
#include <list>

#include "raaTrafficLightUnit.h"
#include "rpcCollidables.h"

class raaTrafficLightUnit;
class raaAnimatedComponent;
class rpcCollidables;

// a basic junction controller. This will manage the lights as a single junction. You should develop and extend this to manage a set of lights 
// as a single junction. 

class raaJunctionController: public osg::NodeCallback
{
public:
	raaJunctionController();
	virtual ~raaJunctionController();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
	static const double csm_dLightChangeTime;
	std::list<raaTrafficLightUnit*> m_lLights;
	std::list<raaTrafficLightUnit*>::iterator m_itLight;
	std::list<raaAnimatedComponent*>::iterator m_itVehicle;
	double m_dLastChangeTime = 0.0;

	void cycleTrafficLights(osg::NodeVisitor* nv);
};

