#include "stdafx.h"
#include <windows.h>

#include "rpcPathSelector.h"

#include <osg/Group>
#include "raaAnimationPathBuilder.h"

extern osg::Group *g_pRoot;

rpcPathSelector* rpcPathSelector::sm_pInstance;

rpcPathSelector::rpcPathSelector(): m_mAnimationPaths() {}

rpcPathSelector* rpcPathSelector::instance()
{
	if (!sm_pInstance) sm_pInstance = new rpcPathSelector();

	return sm_pInstance;
}

void rpcPathSelector::buildAnimationPaths()
{
	std::string sPathNames[104] = {
		"10_0_0", "10_0_1", "10_5_0", "10_5_1", "10_8_0", "10_8_1", "12_1", "12_2", "13_1", "13_2", "14_1", "15_0_0", "15_0_1", "15_5_0", "15_5_1", "15_8_0", "15_8_1",
		"16_1", "16_2", "17_0_0", "17_0_1", "17_0_2", "17_11_0", "17_11_1", "17_11_2", "17_5_0", "17_5_1", "17_5_2", "17_8_0", "17_8_1", "17_8_2", "18_1", "18_2",
		"19_0_0", "19_0_1", "19_5_0", "19_5_1", "19_8_0", "19_8_1", "1_1", "20_1", "20_2", "21_1", "21_2", "22_1", "22_2", "23_0_0", "23_0_1", "23_5_0", "23_5_1",
		"23_8_0", "23_8_1", "24_1", "24_2", "25_0_0", "25_0_1", "25_0_2", "25_11_0", "25_11_1", "25_11_2", "25_4_1", "25_5_0", "25_5_2", "25_8_0", "25_8_1", "25_8_2",
		"26_1", "26_2", "27_0_0", "27_0_1", "27_5_0", "27_5_1", "27_8_0", "27_8_1", "28_2", "29_1", "29_2", "2_0_0", "2_0_1", "2_5_0", "2_5_1", "2_8_0", "2_8_1", "30_2",
		"32_1", "33_0_0", "33_0_1", "33_5_0", "33_5_1", "33_8_0", "33_8_1", "34_2", "3_2", "5_1", "6_1", "6_2", "8_0_0", "8_0_1", "8_5_0", "8_5_1", "8_8_0", "8_8_1", "9_1", "9_2"
	};
	for (unsigned int i = 0; i < 104; ++i) addAnimationPath(sPathNames[i]);
}

void rpcPathSelector::addAnimationPath(const std::string sPath)
{
	osg::AnimationPath *pAP = new osg::AnimationPath();
	raaAnimationPathBuilder apBuilder(pAP, g_pRoot);
	std::pair<unsigned int, unsigned int> pIndexes = retrieveIndexes(sPath);
	apBuilder.load("../../data/" + sPath + ".txt"); // loading the animation path from file
	std::list<osg::AnimationPath*> lAnimationPathsForTileAndPoint = getOrCreateAnimationPaths(pIndexes.first, pIndexes.second);
	lAnimationPathsForTileAndPoint.push_back(pAP);
}

std::list<osg::AnimationPath*> rpcPathSelector::getOrCreateAnimationPaths(const unsigned uiTile, const unsigned uiPoint)
{
	if (m_mAnimationPaths.find(uiTile) == m_mAnimationPaths.end()) m_mAnimationPaths.insert(make_pair(uiTile, new rpcAnimationPointPaths()));
	rpcAnimationPointPaths *pointPaths = m_mAnimationPaths[uiTile];
	if (pointPaths->find(uiPoint) == pointPaths->end()) pointPaths->insert(make_pair(uiPoint, std::list<class osg::AnimationPath*>()));
	return (*pointPaths)[uiPoint];
}

std::pair<unsigned int, unsigned int> rpcPathSelector::retrieveIndexes(std::string sPath) const
{
	std::string delimiter = "_";
	unsigned int firstTokenEndIndex = sPath.find(delimiter);
	std::string sTile = sPath.substr(0, sPath.find(delimiter));
	std::string sRemainder = sPath.substr(firstTokenEndIndex + 1);
	std::string sPoint = sRemainder.substr(0, sRemainder.find(delimiter));
	int iTile = stoi(sTile);
	int iPoint = stoi(sPoint);
	printf("%d, %d\n", iTile, iPoint);
	return std::pair<unsigned int, unsigned int>(iTile, iPoint);
}

rpcPathSelector::~rpcPathSelector() {}