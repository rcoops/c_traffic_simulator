#include "stdafx.h"

#include <windows.h>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>


#include "raaTile.h"

#include "raaTileCurve.h"
#include "raaTileStraight.h"
#include "raaTileTJunction.h"
#include "raaTileXJunction.h"
#include "raaPrinter.h"
#include "raaRoadSet.h"
#include "raaAnimationPoints.h"
#include "raaFinder.h"

unsigned int raaRoadSet::sm_uiInstance=0;
raaRoadSet* raaRoadSet::sm_pInstance=0;
float raaRoadSet::sm_fTileUnit = 472.0f;

void raaRoadSet::start(std::string sAssetFile, std::string sMapFile)
{
	if (!sm_uiInstance++ && !sm_pInstance) new raaRoadSet(sAssetFile);


	std::ifstream file(sMapFile);

	while(!file.eof())
	{
		unsigned int uiName;
		char cType;
		float fTX, fTY, fR;

		file >> cType >> uiName >> fTX>>fTY>>fR;

		switch(cType)
		{
		case 's':
			instance()->addStraight(uiName, fTX, fTY, fR);
			break;
		case 'c':
			instance()->addCurve(uiName, fTX, fTY, fR);
			break;
		case 't':
			instance()->addTJunction(uiName, fTX, fTY, fR);
			break;
		case 'x':
			instance()->addXJunction(uiName, fTX, fTY, fR);
			break;
		}
	}

	file.close();
}

void raaRoadSet::stop()
{
	if (!--sm_uiInstance && sm_pInstance) delete sm_pInstance;
}

raaRoadSet* raaRoadSet::instance()
{
	return sm_pInstance;
}

raaTile* raaRoadSet::addStraight(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation)
{
	raaTile *pTile = new raaTileStraight(uiName, m_pStraight, m_pStraightAP);
	m_mTiles[uiName] = pTile;
	pTile->translate(fTranslationX, fTranslationY);
	pTile->rotate(fRotation);
	m_pSG->addChild(pTile->tile());
	return pTile;
}

raaTile* raaRoadSet::addTJunction(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation)
{
	raaTile *pTile = new raaTileTJunction(uiName, m_pTJunction, m_pTJunctionAP);
	m_mTiles[uiName] = pTile;
	pTile->translate(fTranslationX, fTranslationY);
	pTile->rotate(fRotation);
	m_pSG->addChild(pTile->tile());
	return pTile;
}

raaTile* raaRoadSet::addCurve(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation)
{
	raaTile *pTile = new raaTileCurve(uiName, m_pCurve, m_pCurveAP);
	m_mTiles[uiName] = pTile;
	pTile->translate(fTranslationX, fTranslationY);
	pTile->rotate(fRotation);
	m_pSG->addChild(pTile->tile());
	return pTile;
}

raaTile* raaRoadSet::addXJunction(unsigned int  uiName, float fTranslationX, float fTranslationY, float fRotation)
{
	raaTile *pTile = new raaTileXJunction(uiName, m_pXJunction, m_pXJunctionAP);
	m_mTiles[uiName] = pTile;
	pTile->translate(fTranslationX, fTranslationY);
	pTile->rotate(fRotation);
	m_pSG->addChild(pTile->tile());
	return pTile;
}

raaTile* raaRoadSet::tile(unsigned uiIndex)
{
	if (m_mTiles.find(uiIndex) != m_mTiles.end()) return m_mTiles[uiIndex];
	return 0;
}

osg::Node* raaRoadSet::model()
{
	return m_pModel;
}

osg::Group* raaRoadSet::sg()
{
	return m_pSG;
}

raaRoadSet::raaRoadSet(std::string sFile)
{
	if (!sm_pInstance) sm_pInstance = this;
	buildAnimationPoints();

	m_pSG = new osg::Group();
	m_pSG->ref();

	m_pModel = osgDB::readNodeFile("../../Data/RoadSet.OSGB");

	raaPrinter printer;
	printer.apply(*m_pModel);

	raaFinder<osg::MatrixTransform> straightFinder("2Lanes");
	straightFinder.apply(*m_pModel);
	m_pStraight = straightFinder.node();

	raaFinder<osg::MatrixTransform> curveFinder("2LanesTurn");
	curveFinder.apply(*m_pModel);
	m_pCurve = curveFinder.node();

	raaFinder<osg::MatrixTransform> tJunctionFinder("2LanesTIntersection");
	tJunctionFinder.apply(*m_pModel);
	m_pTJunction = tJunctionFinder.node();

	raaFinder<osg::MatrixTransform> xJunctionFinder("2LanesXIntersection");
	xJunctionFinder.apply(*m_pModel);
	m_pXJunction = xJunctionFinder.node();


	if (m_pModel) m_pModel->ref();
}

raaRoadSet::~raaRoadSet()
{
	if (sm_pInstance == this) sm_pInstance = 0;
	
	m_pSG->unref();

}

void raaRoadSet::buildAnimationPoints()
{
	m_pStraightAP = new raaAnimationPoints();
	m_pStraightAP->addPoint(osg::Vec3f(0.45f*sm_fTileUnit, 0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pStraightAP->addPoint(osg::Vec3f(0.45f*sm_fTileUnit, -0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pStraightAP->addPoint(osg::Vec3f(-0.45f*sm_fTileUnit, 0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pStraightAP->addPoint(osg::Vec3f(-0.45f*sm_fTileUnit, -0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));

	m_pCurveAP = new raaAnimationPoints();
	m_pCurveAP->addPoint(osg::Vec3f(0.12f*sm_fTileUnit, 0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pCurveAP->addPoint(osg::Vec3f(0.24f*sm_fTileUnit, 0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pCurveAP->addPoint(osg::Vec3f(0.45f*sm_fTileUnit, 0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pCurveAP->addPoint(osg::Vec3f(0.45f*sm_fTileUnit, -0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pCurveAP->addPoint(osg::Vec3f(0.06f*sm_fTileUnit, 0.06f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pCurveAP->addPoint(osg::Vec3f(-0.12f*sm_fTileUnit, 0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));

	m_pTJunctionAP = new raaAnimationPoints();
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.45f*sm_fTileUnit, 0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.24f*sm_fTileUnit, 0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.12f*sm_fTileUnit, 0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(0.02f*sm_fTileUnit, 0.02f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(0.12f*sm_fTileUnit, -0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(0.12f*sm_fTileUnit, 0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.02f*sm_fTileUnit, 0.02f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.45f*sm_fTileUnit, -0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.12f*sm_fTileUnit, -0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));
	m_pTJunctionAP->addPoint(osg::Vec3f(-0.24f*sm_fTileUnit, -0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f)));

	m_pXJunctionAP = new raaAnimationPoints();
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.45f*sm_fTileUnit, 0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //0
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.24f*sm_fTileUnit, 0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //1
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.12f*sm_fTileUnit, 0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //2
	m_pXJunctionAP->addPoint(osg::Vec3f(0.02f*sm_fTileUnit, 0.02f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //3
	m_pXJunctionAP->addPoint(osg::Vec3f(0.12f*sm_fTileUnit, -0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //4
	m_pXJunctionAP->addPoint(osg::Vec3f(0.12f*sm_fTileUnit, 0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //5
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.02f*sm_fTileUnit, 0.02f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //6
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.45f*sm_fTileUnit, -0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //7
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.12f*sm_fTileUnit, -0.45f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //8
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.24f*sm_fTileUnit, -0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //9
	m_pXJunctionAP->addPoint(osg::Vec3f(0.45f*sm_fTileUnit, 0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(0.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //10
	m_pXJunctionAP->addPoint(osg::Vec3f(0.45f*sm_fTileUnit, -0.12f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(180.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //11
	m_pXJunctionAP->addPoint(osg::Vec3f(0.24f*sm_fTileUnit, -0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //12
	m_pXJunctionAP->addPoint(osg::Vec3f(-0.02f*sm_fTileUnit, -0.02f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(135.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //13
	m_pXJunctionAP->addPoint(osg::Vec3f(0.24f*sm_fTileUnit, 0.24f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(-45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //14
	m_pXJunctionAP->addPoint(osg::Vec3f(0.02f*sm_fTileUnit, -0.02f*sm_fTileUnit, 0.0f), osg::Quat(osg::DegreesToRadians(45.0f), osg::Vec3f(0.0f, 0.0f, 1.0f))); //15

}
