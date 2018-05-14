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
	void loadNewPoints(rpcContextAwareAnimationPath *pAP, float fStartingAnimationTime, unsigned int uiTile, unsigned int uiPoint);
	void getPathName(std::string &psPath, unsigned int uiTile, unsigned int uiPoint);

	virtual ~rpcPathSelector();

protected:
	rpcPathSelector();

	static std::string sm_sPathNames[104];
	static rpcPathSelector* sm_pInstance;
};
