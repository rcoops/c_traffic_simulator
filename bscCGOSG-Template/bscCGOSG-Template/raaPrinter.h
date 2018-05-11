#pragma once

#include <windows.h>
#include <osg/NodeVisitor>
#include <iostream>

// a basic printer tool to allow console printout of scene graph sections

class raaPrinter : public osg::NodeVisitor
{
public:
	raaPrinter();
	virtual ~raaPrinter();

	virtual void apply(osg::Node& node);
};

