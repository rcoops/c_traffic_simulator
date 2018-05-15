#pragma once

//#include <windows.h>
#include <osg/AnimationPath>
#include <list>
#include "rpcContextAwareAnimationPath.h"

typedef std::list<unsigned int> uIntList;

// NOTE: This is a basic class that will load/save the animation data from a  file and store it in two lists
// containing the tile and actual animation point for each step in an animation. You should extend and enhance this to populate the annimation paths
// you wish to use

class raaAnimationPathBuilder
{
public:
	raaAnimationPathBuilder(rpcContextAwareAnimationPath *pAP, osg::Node* pRoot, const float fAnimationStartTime = 0.0f);
	virtual ~raaAnimationPathBuilder();

	void load(const std::string sFile);  // read animation file and populate 
	void save(const std::string sFile); // save the animation points

	void addControlPoint(const unsigned int uiCurrentTile, const unsigned int uiCurrentPoint); // manually add points
	float calculateNewAnimationTime(float fOriginalTime, osg::Vec3f vfCurrentGlobalTranslation) const;

protected:
	rpcContextAwareAnimationPath *m_pAP;
	osg::Node *m_pRoot;
	uIntList m_lTiles;
	uIntList m_lPoints;

	void addControlPoints();
	osg::Vec3f m_vfPreviousGlobalTranslation;
	float m_fAnimationTime;
	bool m_bIsBeginningOfAnimation;

	const static float csm_fFrameRate;

};

