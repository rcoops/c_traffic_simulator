#include "stdafx.h"
#include "raaInputController.h"
#include "raaTile.h"
#include "raaTrafficLightUnit.h"
#include "raaJunctionController.h"
#include "raaRoadSet.h"

unsigned int uiSpeed = 10;

bool raaInputController::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
{
	if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
	{
		switch (ea.getKey())
		{
		case '+':
			uiSpeed = uiSpeed > 10 ? 10 : uiSpeed;
			rpcCollidables::instance()->adjustVehicleSpeed((++uiSpeed) / 10.0f);
			return true;
		case '-':
			uiSpeed = uiSpeed < 0 ? 0 : uiSpeed;
			rpcCollidables::instance()->adjustVehicleSpeed(uiSpeed ? (--uiSpeed) / 10.0f : 0.0f);
			return true;
		case 'v':
		case 'V':
			rpcCollidables::instance()->toggleDetectionVisibility();
			return true;
		case 'a':
		case 'A':
			raaTile::toggleAnimPoints();
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
