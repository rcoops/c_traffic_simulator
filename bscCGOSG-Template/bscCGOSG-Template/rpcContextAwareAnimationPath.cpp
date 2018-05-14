#include "stdafx.h"
#include "rpcContextAwareAnimationPath.h"

rpcContextAwareAnimationPath::rpcContextAwareAnimationPath(): AnimationPath()// cant do this...? : _loopMode(NO_LOOPING)
{
	setLoopMode(NO_LOOPING);
}

void rpcContextAwareAnimationPath::insertPoint(double time, const osg::AnimationPath::ControlPoint& controlPoint,
	unsigned int uiTile, unsigned int uiPoint)
{
	AnimationPath::insert(time, controlPoint);
	m_uiEndTileIndex = uiTile;
	m_uiEndPointIndex = uiPoint;
}
std::pair<unsigned int, unsigned int> rpcContextAwareAnimationPath::getPoint()
{
	return std::pair<unsigned int, unsigned int>(m_uiEndTileIndex, m_uiEndPointIndex);
}

rpcContextAwareAnimationPath::~rpcContextAwareAnimationPath()
{
}
