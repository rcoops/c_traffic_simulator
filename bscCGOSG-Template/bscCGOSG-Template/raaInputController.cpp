#include "stdafx.h"
#include "raaInputController.h"
#include "raaTile.h"
#include "raaTrafficLightUnit.h"
#include "raaJunctionController.h"
#include "raaRoadSet.h"

bool raaInputController::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
{
	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
	{
		switch (ea.getKey())
		{
		case ']':
			rpcCollidables::instance()->adjustVehicleSpeed(rpcCollidables::csm_bSpeedUp);
			return true;
		case '[':
			rpcCollidables::instance()->adjustVehicleSpeed(rpcCollidables::csm_bSpeedDown);
			return true;
		case 'v':
		case 'V':
			rpcCollidables::instance()->toggleDetectionVisibility();
			return true;
		case 'a':
		case 'A':
			raaTile::toggleAnimPoints();
			return true;
		case 't':
		case 'T':
			rpcCollidables::instance()->toggleLightState();
			return true;
		case 'p':
		case 'P':
			rpcCollidables::instance()->pauseVehicles();
			return true;
		}
	}

	return false;
}

raaInputController::raaInputController()
{
}


raaInputController::~raaInputController()
{
}
