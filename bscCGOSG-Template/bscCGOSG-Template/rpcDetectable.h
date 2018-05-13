#pragma once
#include <osg/Vec3f>
#include <osg/Node>

class rpcDetectable
{
public:
	virtual osg::Vec3f getDetectionPointRelativeTo(osg::Node *pRoot) = 0;
	virtual ~rpcDetectable() {}
};
