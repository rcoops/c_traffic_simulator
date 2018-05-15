#pragma once
#include <osg/AnimationPath>
#include <list>
#include "rpcContextAwareAnimationPath.h"

typedef std::map<unsigned int, std::list<class osg::AnimationPath*>> rpcAnimationPointPaths;
typedef std::map<unsigned int, rpcAnimationPointPaths*> rpcTileAnimationPointPaths;

class rpcPathSelector
{
public:

	static rpcPathSelector* instance();

	std::pair<unsigned, unsigned> retrieveIndexes(std::string sPath) const;
	void loadNewPoints(rpcContextAwareAnimationPath *pAP, const float fStartingAnimationTime, const unsigned int uiTile, const unsigned int uiPoint) const;
	static rpcContextAwareAnimationPath* createRandomPath();
	
	virtual ~rpcPathSelector();

protected:
	rpcPathSelector();
	void getPathName(std::string &psPath, const unsigned int uiTile, const unsigned int uiPoint) const;
	static void getRandomPath(std::string &psPath);

	const static unsigned int csm_uiNumberOfPaths;

	static std::string sm_sPathNames[104];
	static rpcPathSelector* sm_pInstance;
};
