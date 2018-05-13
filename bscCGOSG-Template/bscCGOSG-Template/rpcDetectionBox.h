#pragma once

#include <windows.h>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Switch>

class rpcDetectionBox
{
public:
	rpcDetectionBox(osg::Vec3f vfPosition, osg::Vec3f vfSize = vfDefaultSize);
	virtual ~rpcDetectionBox();

	osg::MatrixTransform* m_pRoot;
	osg::MatrixTransform* m_pScale;

	void toggleVisibility();

protected:

	osg::Switch* m_pSwitch;
	bool m_bVisible;
	osg::Matrix* m_pWorldToBound;

	osg::Material* makeMaterial();
	osg::Geode* makeGeometry();
	static const osg::Vec3f vfDefaultSize;
};
