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
 * DONE on callback, if current time >= path getLastTime() - alternatively query the roadset for the tile number and point number and animPointPosition (see addControlPoint)
 *		interrogate path selector using stored tile number and point number for point
 *		add path to animated component
 *		go!
 */
const float raaAnimationPathBuilder::csm_fFrameRate = 200.0f;

raaAnimationPathBuilder::raaAnimationPathBuilder(rpcContextAwareAnimationPath *pAP, osg::Node *pRoot, const float fAnimationStartTime): m_pAP(pAP), m_pRoot(pRoot),
	m_vfPreviousGlobalTranslation(0.0f, 0.0f, 0.0f), m_fAnimationTime(fAnimationStartTime), m_bIsBeginningOfAnimation(false)
{
}

raaAnimationPathBuilder::~raaAnimationPathBuilder()
{
}

void raaAnimationPathBuilder::load(const std::string sFile)
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

void raaAnimationPathBuilder::save(const std::string sFile)
{
	std::ofstream file(sFile);

	uIntList::iterator itTile = m_lTiles.begin();
	uIntList::iterator itAP = m_lPoints.begin();

	for (; itTile != m_lTiles.end() && itAP != m_lPoints.end(); ++itTile, ++itAP)
	{
		file << (*itTile) << " " << (*itAP) << std::endl;
	}
	file.close();
}

// osg docs for animation path & animation path callback

void raaAnimationPathBuilder::addControlPoints()
{
	uIntList::iterator uiiTile = m_lTiles.begin();
	uIntList::iterator uiiPoint = m_lPoints.begin();
	for (; uiiTile != m_lTiles.end(); uiiTile++, uiiPoint++)
	{
		m_bIsBeginningOfAnimation = uiiTile == m_lTiles.begin(); // set this so we can avoid calc for first point later on
		addControlPoint(*uiiTile, *uiiPoint);
	}
}

void raaAnimationPathBuilder::addControlPoint(const unsigned int uiCurrentTile, const unsigned int uiCurrentPoint)
{
	raaRoadSet *pRoadSet = raaRoadSet::instance(); // get roadset
//	printf("Tile number: %d, Point number: %d\n", uiCurrentTile, uiCurrentPoint);
	raaTile *pTile = pRoadSet->tile(uiCurrentTile);

	// transform is easy - multiply matrix by vector
	// get global transform
	osg::Matrixf mfGlobalTransform = computeLocalToWorld(pTile->lowestTransform()->getParentalNodePaths()[0], true);
	const osg::Vec3f vfGlobalTranslation = pTile->animPointPosition(uiCurrentPoint) * mfGlobalTransform;
	// rotation is harder - local quarternian to matrix, multiply global by local matrix, resulting matrix to quarternian (or convert the global matrix to quarternian)
	// get global rotation (convert global matrix to quat and multiply by tile rotation)
	const osg::Quat qGlobalRotation = mfGlobalTransform.getRotate() * pTile->animPointRotation(uiCurrentPoint);

	if (!m_bIsBeginningOfAnimation) m_fAnimationTime = calculateNewAnimationTime(m_fAnimationTime, vfGlobalTranslation);
	m_vfPreviousGlobalTranslation = vfGlobalTranslation; // save the position for next time calc
	// create new control point with our calculated animation time, translation & rotation (and assign tile & point numbers)
	m_pAP->insertPoint(m_fAnimationTime, osg::AnimationPath::ControlPoint(vfGlobalTranslation, qGlobalRotation), uiCurrentTile, uiCurrentPoint);
}

float raaAnimationPathBuilder::calculateNewAnimationTime(float fOriginalTime, const osg::Vec3f vfCurrentGlobalTranslation) const
{
	const float fDistance = (vfCurrentGlobalTranslation - m_vfPreviousGlobalTranslation).length();
	fOriginalTime += fDistance / csm_fFrameRate;
	return fOriginalTime;
}
