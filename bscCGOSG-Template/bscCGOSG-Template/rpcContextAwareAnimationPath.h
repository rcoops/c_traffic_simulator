#pragma once
#include <osg/AnimationPath>

class rpcContextAwareAnimationPath : public osg::AnimationPath
{
public:
	rpcContextAwareAnimationPath();

	unsigned int m_uiEndTileIndex;
	unsigned int m_uiEndPointIndex;

	void insertPoint(const double dAnimationTime, const ControlPoint& controlPoint, const unsigned int uiTile, const unsigned int uiPoint);
	std::pair<unsigned int, unsigned int> getFinalPoint();
	bool isEndOfAnimation(const double dCurrentTime) const;

	~rpcContextAwareAnimationPath();
};

