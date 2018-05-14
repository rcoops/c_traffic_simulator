#include "stdafx.h"
#include <windows.h>

#include "raaRoadSet.h"
#include "raaTile.h"
#include <osgDB/ReadFile>
#include "raaAnimationPathBuilder.h"

/*
 * PLAN FOR DYNAMIC PATHS
 * ----------------------
 * DONE extend animation path to hold a tile number and point number for the last point added
 * DONE extend animated component to hold those vals and populate them when handed the (extended) AP
 * DONE add a setter for the same
 * DONE update path selector to return *single* path for given tile/point number (if multiple pick one randomly)
 * on callback, if current time >= path getLastTime() - alternatively query the roadset for the tile number and point number and animPointPosition (see addControlPoint)
 *		interrogate path selector using stored tile number and point number for point
 *		add path to animated component
 *		go!
 */
const float raaAnimationPathBuilder::csm_fFrameRate = 100.0f;

raaAnimationPathBuilder::raaAnimationPathBuilder(rpcContextAwareAnimationPath *pAP, osg::Node *pRoot, float fAnimationStartTime): m_pAP(pAP), m_pRoot(pRoot),
	m_vfPreviousGlobalTranslation(0.0f, 0.0f, 0.0f), m_fAnimationTime(fAnimationStartTime), m_bIsBeginningOfAnimation(0)
{
}

raaAnimationPathBuilder::~raaAnimationPathBuilder()
{
}

void raaAnimationPathBuilder::load(std::string sFile)
{
	std::ifstream file(sFile);

	unsigned int uiTile, uiAP;

	while(!file.eof())
	{
		file >> uiTile >> uiAP;
		m_lTiles.push_back(uiTile);
		m_lPoints.push_back(uiAP);
	}

	file.close();
	addControlPoints();
}

void raaAnimationPathBuilder::save(std::string sFile)
{
	std::ofstream file(sFile);

	unsigned int uiTile, uiAP;

	uIntList::iterator itTile = m_lTiles.begin();
	uIntList::iterator itAP = m_lPoints.begin();

	for (; itTile != m_lTiles.end() && itAP != m_lPoints.end(); itTile++, itAP++)
	{
		file << (*itTile) << " " << (*itAP) << std::endl;
	}
	file.close();
}

// osg docs for animation path & animation path callback

unsigned int raaAnimationPathBuilder::popReference(uIntList &lReferences)
{
	unsigned int uiReference = lReferences.front();
	lReferences.pop_front();
	return uiReference;
}

void raaAnimationPathBuilder::addControlPoints()
{
	unsigned int uiCurrentTile, uiCurrentPoint;

	uIntList::iterator uiiTile = m_lTiles.begin();
	uIntList::iterator uiiPoint = m_lPoints.begin();
	for (; uiiTile != m_lTiles.end(); uiiTile++, uiiPoint++)
	{
		m_bIsBeginningOfAnimation = uiiTile == m_lTiles.begin();
		// get road set singleton, get tile, get animation point
		addControlPoint(*uiiTile, *uiiPoint);
	}
}

void raaAnimationPathBuilder::addControlPoint(unsigned int uiCurrentTile, unsigned int uiCurrentPoint)
{
	raaRoadSet *pRoadSet = raaRoadSet::instance();
//	printf("Tile number: %d, Point number: %d\n", uiCurrentTile, uiCurrentPoint);
	raaTile *pTile = pRoadSet->tile(uiCurrentTile);

	// transform is easy - multiply matrix by vector
	osg::Matrixf mfGlobalTransform = computeLocalToWorld(pTile->lowestTransform()->getParentalNodePaths()[0], true);

	osg::Vec3f vfGlobalTranslation = pTile->animPointPosition(uiCurrentPoint) * mfGlobalTransform;
	// rotation is harder - local quarternian to matrix, multiply global by local matrix, resulting matrix to quarternian (or convert the global matrix to quarternian
	osg::Quat qGlobalRotation = mfGlobalTransform.getRotate() * pTile->animPointRotation(uiCurrentPoint);

	if (!m_bIsBeginningOfAnimation)
	{
		//float fDistance = (vfGlobalTranslation - m_vfPreviousGlobalTranslation).length();
		//m_fAnimationTime += fDistance / csm_fFrameRate;
		m_fAnimationTime = calculateTimeChange(m_fAnimationTime, vfGlobalTranslation, m_vfPreviousGlobalTranslation);
	}
	m_vfPreviousGlobalTranslation = vfGlobalTranslation;
	addControlPointToPath(m_fAnimationTime, vfGlobalTranslation, qGlobalRotation, uiCurrentTile, uiCurrentPoint);
}

float raaAnimationPathBuilder::calculateTimeChange(float fOriginalTime, osg::Vec3f vfCurrentGlobalTranslation, osg::Vec3f vfGlobalPerviousTranslation)
{
	float fDistance = (vfCurrentGlobalTranslation - m_vfPreviousGlobalTranslation).length();
	fOriginalTime += fDistance / csm_fFrameRate;
	return fOriginalTime;
}

void raaAnimationPathBuilder::addControlPointToPath(float fAnimationTime, osg::Vec3f &avfGlobalTranlation, osg::Quat &aqGlobalRotation,
	unsigned int uiCurrentTile, unsigned int uiCurrentPoint)
{
	m_pAP->insertPoint(m_fAnimationTime, osg::AnimationPath::ControlPoint(avfGlobalTranlation, aqGlobalRotation), uiCurrentTile, uiCurrentPoint);
}
