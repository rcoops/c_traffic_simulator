#include "stdafx.h"
#include <windows.h>

#include "rpcPathSelector.h"

#include <osg/Group>
#include "raaAnimationPathBuilder.h"
#include "rpcContextAwareAnimationPath.h"

extern osg::Group *g_pRoot;

rpcPathSelector* rpcPathSelector::sm_pInstance;

rpcPathSelector::rpcPathSelector()
{
	m_mAnimationPaths = new rpcTileAnimationPointPaths();
}

rpcPathSelector* rpcPathSelector::instance()
{
	if (!sm_pInstance) sm_pInstance = new rpcPathSelector();

	return sm_pInstance;
}

void rpcPathSelector::buildAnimationPaths()
{
	// This is mucky but querying for file names in a directory in windows = painful
	std::string sPathNames[104] = {
		"10_0_0", "10_0_1", "10_5_0", "10_5_1", "10_8_0", "10_8_1", "12_1", "12_2", "13_1", "13_2", "14_1", "15_0_0", "15_0_1", "15_5_0", "15_5_1", "15_8_0", "15_8_1",
		"16_1", "16_2", "17_0_0", "17_0_1", "17_0_2", "17_11_0", "17_11_1", "17_11_2", "17_5_0", "17_5_1", "17_5_2", "17_8_0", "17_8_1", "17_8_2", "18_1", "18_2",
		"19_0_0", "19_0_1", "19_5_0", "19_5_1", "19_8_0", "19_8_1", "1_1", "20_1", "20_2", "21_1", "21_2", "22_1", "22_2", "23_0_0", "23_0_1", "23_5_0", "23_5_1",
		"23_8_0", "23_8_1", "24_1", "24_2", "25_0_0", "25_0_1", "25_0_2", "25_11_0", "25_11_1", "25_11_2", "25_4_1", "25_5_0", "25_5_2", "25_8_0", "25_8_1", "25_8_2",
		"26_1", "26_2", "27_0_0", "27_0_1", "27_5_0", "27_5_1", "27_8_0", "27_8_1", "28_2", "29_1", "29_2", "2_0_0", "2_0_1", "2_5_0", "2_5_1", "2_8_0", "2_8_1", "30_2",
		"32_1", "33_0_0", "33_0_1", "33_5_0", "33_5_1", "33_8_0", "33_8_1", "34_2", "3_2", "5_1", "6_1", "6_2", "8_0_0", "8_0_1", "8_5_0", "8_5_1", "8_8_0", "8_8_1", "9_1", "9_2"
	};
	for (unsigned int i = 0; i < 104; ++i) addAnimationPath(sPathNames[i]);
	printf("");
}

void rpcPathSelector::addAnimationPath(const std::string sPath)
{
	osg::AnimationPath *pAP = new rpcContextAwareAnimationPath();
	raaAnimationPathBuilder apBuilder(pAP, g_pRoot);
	const std::pair<unsigned int, unsigned int> pruiTileAndPointIndex = retrieveIndexes(sPath); // parse the file name for tile and point number
	apBuilder.load("../../data/animationpaths/" + sPath + ".txt"); // loading the animation path from file
	// Add the path on to the list retrieved or created at the indexes
	std::list<osg::AnimationPath*> *plPaths = getOrCreateAnimationPaths(pruiTileAndPointIndex.first, pruiTileAndPointIndex.second);
	plPaths->push_back(pAP);
	printf("");
}

// Wont exist a lot of the time but as we're storing multiple paths in some points this is needed
std::list<osg::AnimationPath*>* rpcPathSelector::getOrCreateAnimationPaths(const unsigned int uiTile, const unsigned int uiPoint)
{
	// if there's no map on the tile, make one and get the reference
	if (m_mAnimationPaths->find(uiTile) == m_mAnimationPaths->end())
		m_mAnimationPaths->insert(make_pair(uiTile, new rpcAnimationPointPaths()));
	rpcAnimationPointPaths *pointPaths = (*m_mAnimationPaths)[uiTile];
	// if there's no map in the point make one
	if (pointPaths->find(uiPoint) == pointPaths->end())
		pointPaths->insert(make_pair(uiPoint, std::list<class osg::AnimationPath*>()));
	return &((*pointPaths)[uiPoint]); // return the reference to the (potentially newly created) list
}

std::pair<unsigned int, unsigned int> rpcPathSelector::retrieveIndexes(std::string sPath) const
{
	std::string sDelimiter = "_"; // can't see a way round hardcoding this
	unsigned int uiFirstTokenEndIndex = sPath.find(sDelimiter);
	std::string sTile = sPath.substr(0, sPath.find(sDelimiter)); // Get our tile number as a string
	std::string sPathRemainder = sPath.substr(uiFirstTokenEndIndex + 1);
	std::string sPoint = sPathRemainder.substr(0, sPathRemainder.find(sDelimiter)); // Get our point number as a string
	// convert to ints
	unsigned int iTile = stoi(sTile);
	unsigned int iPoint = stoi(sPoint);
//	printf("%d, %d\n", iTile, iPoint);
	return std::pair<unsigned int, unsigned int>(iTile, iPoint);
}

osg::AnimationPath* rpcPathSelector::getNewAnimationPath(unsigned int uiTile, unsigned int uiPoint)
{
	std::list<osg::AnimationPath*> *plPaths = getOrCreateAnimationPaths(uiTile, uiPoint);
	unsigned int count = plPaths->size();
	unsigned int randomIndex = rand() % count;
	std::list<osg::AnimationPath*>::iterator itPaths = plPaths->begin();
	std::advance(itPaths, randomIndex);
	return (*itPaths);
}

rpcPathSelector::~rpcPathSelector() {}
