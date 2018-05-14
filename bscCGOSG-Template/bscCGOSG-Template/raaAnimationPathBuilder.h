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
	raaAnimationPathBuilder(rpcContextAwareAnimationPath *pAP, osg::Node* pRoot, float fAnimationStartTime = 0.0f);
	virtual ~raaAnimationPathBuilder();

	void load(std::string sFile);  // read animation file and populate 
	void save(std::string sFile); // save the animation points

	void addControlPoint(unsigned int uiCurrentTile, unsigned int uiCurrentPoint); // manually add points
	void addControlPointToPath(float fAnimationTime, osg::Vec3f &avfGlobalTranlation, osg::Quat &aqGlobalRotation, unsigned int uiCurrentTile, unsigned int uiCurrentPoint);
	float calculateTimeChange(float fOriginalTime, osg::Vec3f vfCurrentGlobalTranslation, osg::Vec3f vfGlobalPerviousTranslation);

protected:
	rpcContextAwareAnimationPath *m_pAP;
	osg::Node *m_pRoot;
	uIntList m_lTiles;
	uIntList m_lPoints;

	unsigned int popReference(uIntList &lReferences);
	void addControlPoints();
	osg::Vec3f m_vfPreviousGlobalTranslation;
	float m_fAnimationTime;
	bool m_bIsBeginningOfAnimation;

	const static float csm_fFrameRate;

};

