#pragma once

#include <windows.h>
#include <osgGA/GUIEventHandler>

// the basic input controller. You should extend and enhance this for your user input

class raaInputController: public osgGA::GUIEventHandler
{
public:
	virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&);
	raaInputController();
	virtual ~raaInputController();
};

