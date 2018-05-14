#pragma once
#include <osg/AnimationPath>

class rpcContextAwareAnimationPath : public osg::AnimationPath
{
public:
	rpcContextAwareAnimationPath();

	unsigned int m_uiEndTileIndex;
	unsigned int m_uiEndPointIndex;

	void insertPoint(double time, const osg::AnimationPath::ControlPoint& controlPoint, unsigned int uiTile, unsigned int uiPoint);

	~rpcContextAwareAnimationPath();
};

