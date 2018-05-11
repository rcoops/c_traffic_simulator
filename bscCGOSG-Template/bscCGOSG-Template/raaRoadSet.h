#pragma once
#include <windows.h>
#include <string>
#include <osg/Node>


// a singleton class to facilitate the building of the road model

class raaAnimationPoints;
class raaTile;

typedef std::map<unsigned int, raaTile*> raaTiles;

class raaRoadSet
{
public:
	static float sm_fTileUnit;
	static void start(std::string sAssetFile, std::string sMapFile);
	static void stop();
	static raaRoadSet* instance();

	osg::Node* model();
	osg::Group* sg();

	raaTile* addStraight(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation);
	raaTile* addTJunction(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation);
	raaTile* addCurve(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation);
	raaTile* addXJunction(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation);


	raaTile* tile(unsigned int uiIndex);

protected:
	raaRoadSet(std::string sAssetFile);
	virtual ~raaRoadSet();

	osg::Node *m_pModel;
	osg::Group *m_pSG;
	osg::Node *m_pStraight;
	osg::Node *m_pCurve;
	osg::Node *m_pTJunction;
	osg::Node *m_pXJunction;

	raaTiles m_mTiles;

private:
	static unsigned int sm_uiInstance;
	static raaRoadSet* sm_pInstance;

	raaAnimationPoints *m_pStraightAP;
	raaAnimationPoints *m_pCurveAP;
	raaAnimationPoints *m_pTJunctionAP;
	raaAnimationPoints *m_pXJunctionAP;

	void buildAnimationPoints();
};

