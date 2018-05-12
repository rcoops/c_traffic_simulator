#pragma once

#include <windows.h>
#include <osg/Node>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Switch>
#include <osg/ShapeDrawable>

#include <list>

class rpcDetectionBox
{
public:
	rpcDetectionBox(osg::Vec3f vfPosition);
	virtual ~rpcDetectionBox();

	osg::MatrixTransform* m_pRoot;
	osg::MatrixTransform* m_pScale;

	void toggleVisibility();

protected:
	rpcDetectionBox();

	osg::Switch* m_pSwitch;
	bool m_bVisible;
	osg::Matrix* m_pWorldToBound;

	osg::Material* makeMaterial();
	osg::Geode* makeGeometry();
};
