#include "stdafx.h"
#include "rpcContextAwareAnimationPath.h"

rpcContextAwareAnimationPath::rpcContextAwareAnimationPath(): AnimationPath() // cant do this...? , _loopMode(NO_LOOPING)
{
	setLoopMode(NO_LOOPING); // we only want each path to execute once
}

void rpcContextAwareAnimationPath::insertPoint(const double dAnimationTime, const ControlPoint& cPoint,
											   const unsigned int uiTile, const unsigned int uiPoint)
{
	insert(dAnimationTime, cPoint); // do what would be done anyway
	// set the points
	m_uiEndTileIndex = uiTile;
	m_uiEndPointIndex = uiPoint;
}

std::pair<unsigned int, unsigned int> rpcContextAwareAnimationPath::getFinalPoint()
{
	return std::pair<unsigned int, unsigned int>(m_uiEndTileIndex, m_uiEndPointIndex);
}

bool rpcContextAwareAnimationPath::isEndOfAnimation(const double dCurrentTime) const
{
	return dCurrentTime >= getLastTime();
}

rpcContextAwareAnimationPath::~rpcContextAwareAnimationPath() {}
