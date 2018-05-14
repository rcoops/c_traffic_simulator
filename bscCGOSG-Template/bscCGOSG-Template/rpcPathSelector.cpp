#include "stdafx.h"
#include <windows.h>

#include "rpcPathSelector.h"

#include <osg/Group>
#include "raaAnimationPathBuilder.h"

extern osg::Group *g_pRoot;

rpcPathSelector* rpcPathSelector::sm_pInstance;
const unsigned int rpcPathSelector::csm_uiNumberOfPaths = 104;

std::string rpcPathSelector::sm_sPathNames[csm_uiNumberOfPaths] = {
	"10_0_0", "10_0_1", "10_5_0", "10_5_1", "10_8_0", "10_8_1", "12_1", "12_2", "13_1", "13_2", "14_1", "15_0_0", "15_0_1", "15_5_0", "15_5_1", "15_8_0", "15_8_1",
	"16_1", "16_2", "17_0_0", "17_0_1", "17_0_2", "17_11_0", "17_11_1", "17_11_2", "17_5_0", "17_5_1", "17_5_2", "17_8_0", "17_8_1", "17_8_2", "18_1", "18_2",
	"19_0_0", "19_0_1", "19_5_0", "19_5_1", "19_8_0", "19_8_1", "1_1", "20_1", "20_2", "21_1", "21_2", "22_1", "22_2", "23_0_0", "23_0_1", "23_5_0", "23_5_1",
	"23_8_0", "23_8_1", "24_1", "24_2", "25_0_0", "25_0_1", "25_0_2", "25_11_0", "25_11_1", "25_11_2", "25_4_1", "25_5_0", "25_5_2", "25_8_0", "25_8_1", "25_8_2",
	"26_1", "26_2", "27_0_0", "27_0_1", "27_5_0", "27_5_1", "27_8_0", "27_8_1", "28_2", "29_1", "29_2", "2_0_0", "2_0_1", "2_5_0", "2_5_1", "2_8_0", "2_8_1", "30_2",
	"32_1", "33_0_0", "33_0_1", "33_5_0", "33_5_1", "33_8_0", "33_8_1", "34_2", "3_2", "5_1", "6_1", "6_2", "8_0_0", "8_0_1", "8_5_0", "8_5_1", "8_8_0", "8_8_1", "9_1", "9_2"
};

rpcPathSelector::rpcPathSelector()
{
}

rpcPathSelector* rpcPathSelector::instance()
{
	if (!sm_pInstance) sm_pInstance = new rpcPathSelector();

	return sm_pInstance;
}

void rpcPathSelector::getPathName(std::string &spPath, unsigned int uiTile, unsigned int uiPoint)
{
	std::list<std::string> sValidPathNames = std::list<std::string>();
	for (int i = 0; i < csm_uiNumberOfPaths; i++)
	{
		std::pair<unsigned int, unsigned int> indexes = retrieveIndexes(sm_sPathNames[i]);
		if (indexes.first == uiTile && indexes.second == uiPoint) sValidPathNames.push_back(sm_sPathNames[i]);
	}

	unsigned int uiCount = sValidPathNames.size();
	unsigned int uiRandomIndex = rand() % uiCount;
	std::list<std::string>::iterator itPaths = sValidPathNames.begin();
	std::advance(itPaths, uiRandomIndex);
	spPath = *itPaths;
}

void rpcPathSelector::getRandomPath(std::string &spPath)
{
	unsigned int randomIndex = rand() % csm_uiNumberOfPaths;
	spPath = sm_sPathNames[randomIndex];
}

void rpcPathSelector::loadNewPoints(rpcContextAwareAnimationPath *pAP, float fStartingAnimationTime, unsigned int uiTile, unsigned int uiPoint)
{
	pAP->clear();
	raaAnimationPathBuilder apBuilder(pAP, g_pRoot, fStartingAnimationTime);
	std::string sPath;
	getPathName(sPath, uiTile, uiPoint); // load a potential path into a string from the points
	apBuilder.load("../../data/animationpaths/" + sPath + ".txt"); // loading the animation path from file
}

rpcContextAwareAnimationPath* rpcPathSelector::createRandomPath()
{
	rpcContextAwareAnimationPath *pAP = new rpcContextAwareAnimationPath();
	raaAnimationPathBuilder apBuilder(pAP, g_pRoot);
	std::string sPath;
	getRandomPath(sPath); // load a potential path into a string from the points
	apBuilder.load("../../data/animationpaths/" + sPath + ".txt"); // loading the animation path from file
	return pAP;
}

std::pair<unsigned int, unsigned int> rpcPathSelector::retrieveIndexes(std::string sPath) const
{
	std::string sDelimiter = "_"; // can't see a way round hardcoding this
	unsigned int uiFirstTokenEndIndex = sPath.find(sDelimiter);
	std::string sTile = sPath.substr(0, sPath.find(sDelimiter)); // Get our tile number as a string
	std::string sPathRemainder = sPath.substr(uiFirstTokenEndIndex + 1);
	std::string sPoint = sPathRemainder.substr(0, sPathRemainder.find(sDelimiter)); // Get our point number as a string

	return std::pair<unsigned int, unsigned int>(stoi(sTile), stoi(sPoint));
}

rpcPathSelector::~rpcPathSelector() {}
