#pragma once
#include <osg/AnimationPath>
#include <list>

typedef std::map<unsigned int, std::list<class osg::AnimationPath*>> rpcAnimationPointPaths;
typedef std::map<unsigned int, rpcAnimationPointPaths*> rpcTileAnimationPointPaths;

class rpcPathSelector
{
public:

	static rpcPathSelector* instance();

	void buildAnimationPaths();
	void addAnimationPath(std::string sPath);
	std::list<osg::AnimationPath*> getOrCreateAnimationPaths(const unsigned int uiTile, const unsigned int uiPoint);
	std::pair<unsigned, unsigned> retrieveIndexes(std::string sPath) const;
	osg::AnimationPath* getNewAnimationPath(unsigned int uiTile, unsigned int uiPoint);

	virtual ~rpcPathSelector();

protected:
	rpcPathSelector();
	rpcTileAnimationPointPaths m_mAnimationPaths;

	static rpcPathSelector* sm_pInstance;
};
