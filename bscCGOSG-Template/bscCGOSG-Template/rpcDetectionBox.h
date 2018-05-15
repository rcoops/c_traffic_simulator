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

	osg::MatrixTransform* node() const;
	bool contains(const osg::Vec3f pvfGlobalCoordinates, osg::Group *pRoot) const;

	virtual ~rpcDetectionBox();

protected:
	static const osg::Vec3f vfDefaultSize;

	osg::MatrixTransform* m_pTransform;
	osg::Matrix* m_pWorldToBound;

	static osg::Material* makeMaterial();

	osg::Geode* makeGeometry() const;
};
