#include "stdafx.h"
#include "raaInputController.h"
#include "raaTile.h"
#include "raaTrafficLightUnit.h"
#include "raaJunctionController.h"
#include "raaRoadSet.h"

bool raaInputController::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
{
	if(ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN)
	{
		switch(ea.getKey())
		{
			case 'v':
			case 'V':
				rpcCollidables::toggleDetectionVisibility();
				return true;
			case 'a':
			case 'A':
				raaTile::toggleAnimPoints();
				return true;
			case 'p':
			case 'P':
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
