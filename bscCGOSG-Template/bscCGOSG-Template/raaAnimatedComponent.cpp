#include "stdafx.h"

#include <windows.h>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <valarray>
#include <osg/PolygonMode>
#include <osg/AnimationPath>
#include <osgDB/readFile>

#include "raaAnimatedComponent.h"
#include "raaTrafficLightUnit.h"
#include "raaJunctionController.h"
#include "rpcContextAwareAnimationPath.h"
#include "rpcPathSelector.h"
#include "rpcCarVeryon.h"
#include "rpcCarDelta.h"
#include "rpcDumpTruck.h"

extern osg::Group *g_pRoot;

const osg::Vec3f raaAnimatedComponent::csm_vfLightDetectorPosition = osg::Vec3f(0.0f, 140.0f, 20.0f);
const osg::Vec3f raaAnimatedComponent::csm_vfVehicleDetectorPosition = osg::Vec3f(120.0f, 0.0f, 20.0f);

const osg::Vec3f raaAnimatedComponent::csm_vfBack = osg::Vec3f(-40.0f, 0.0f, 20.0f);
float raaAnimatedComponent::sm_fTimeMultiplier = 1.0f;

raaAnimatedComponent::raaAnimatedComponent(rpcContextAwareAnimationPath *pAP): AnimationPathCallback(pAP), m_pAP(pAP), m_bDetectorBoxVisible(false), m_fSpeed(1.0f), m_fPrePauseSpeed(1.0f), m_uiLastTileInAnimation(pAP->m_uiEndTileIndex), m_uiLastAnimationPointInAnimation(pAP->m_uiEndPointIndex), m_bPaused(false)
{
	m_pLightDetected = nullptr;
	m_pRoot = new osg::MatrixTransform();
	m_pRoot->ref();
	// switch->transform->geode

	m_psDetectorSwitch = new osg::Switch();
	m_pRoot->addChild(m_psDetectorSwitch);
	initDetectionPoint();
	m_pLightDetector = new rpcDetectionBox(csm_vfLightDetectorPosition, osg::Vec3f(50.0f, 50.0f, 50.0f));
	m_pVehicleDetector = new rpcDetectionBox(csm_vfVehicleDetectorPosition);
	m_psDetectorSwitch->addChild(m_pLightDetector->root());
	m_psDetectorSwitch->addChild(m_pVehicleDetector->root());
	setDetectionBoxVisibility(m_bDetectorBoxVisible);
	loadNewPath();
	m_pRoot->setUpdateCallback(this);
}

void raaAnimatedComponent::loadNewPath()
{
	rpcPathSelector::instance()->loadNewPoints(m_pAP, getAnimationTime(), m_uiLastTileInAnimation, m_uiLastAnimationPointInAnimation);
	setFinalAnimationPathPoint();
}

void raaAnimatedComponent::setFinalAnimationPathPoint()
{
	if (m_pAP)
	{
		const std::pair<unsigned int, unsigned int> pPoints = m_pAP->getPoint();
		m_uiLastTileInAnimation = pPoints.first;
		m_uiLastAnimationPointInAnimation = pPoints.second;
	}
}

void raaAnimatedComponent::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	checkForNewPath();
	setMultiplier();
	checkForVehicles();
	AnimationPathCallback::operator()(node, nv);
}

void raaAnimatedComponent::setMultiplier()
{
	// Controllable global speed * car's speed in reaction to light colours
	const double dTotalMultiplier = sm_fTimeMultiplier * m_fSpeed; // latest frame
															 // Adjusting the multiplier will move the animation speed so need to get an offset
	setTimeOffset(calculateTimeOffset(dTotalMultiplier));
	setTimeMultiplier(dTotalMultiplier);
}

void raaAnimatedComponent::checkForNewPath()
{
	if (m_pAP && m_pAP->isEndOfAnimation(getAnimationTime()))
	{
		loadNewPath();
	}
}

void raaAnimatedComponent::setPause(const bool bPause)
{
	if (bPause && !m_bPaused)
	{
		m_fPrePauseSpeed = m_fSpeed;
		m_bPaused = true;
		m_fSpeed = 0.00000001f;
	}
	else if (!bPause && m_bPaused)
	{
		m_bPaused = false;
		setSpeed(m_fPrePauseSpeed);
	}
	//if (bPause && !m_bPaused) {
	//	double animationTime = getAnimationTime();
	//	m_dAnimationTimeBeforePause = getAnimationTime();
	//	m_bPaused = true;
	//}
	//else if (!bPause && m_bPaused) {
	//	double animationTime = getAnimationTime();
	//	double offset = getTimeOffset();
	//	setTimeOffset(getTimeOffset() + getAnimationTime() - m_dAnimationTimeBeforePause);
	//	m_bPaused = false;
	//}
	//AnimationPathCallback::setPause(bPause);
}

osg::Vec3f raaAnimatedComponent::getDetectionPointRelativeTo(osg::Node *pRoot)
{
	if (pRoot) return csm_vfBack * computeLocalToWorld(m_pRoot->getParentalNodePaths(pRoot)[0]);
	return csm_vfBack; // can't really happen - this wont exist if tree root doesn't
}

/* Why is this not a function of the callback??
 * Animation Time = (latest time - first time) * timemultiplier, so we need to rejig the offset:
 * ST = Simulation Time (latest - first), OC = Original Offset, OM = Original multiplier,
 * NO = New Offeset, NM = New Multiplier
 * (ST-OO)*OM=(ST-NO)*NM
 * ((ST-OO)*OM)/NM = ST-NO
 * NO = ST - (((ST-OO)*OM)/NM))
 */
double raaAnimatedComponent::calculateTimeOffset(const double dTotalNewMultiplier) const
{
	const double dSimulationTime = _latestTime - _firstTime;
	return dSimulationTime - (dSimulationTime - _timeOffset) * _timeMultiplier / dTotalNewMultiplier;
}

void raaAnimatedComponent::setSpeed(const float fSpeed)
{
	m_fSpeed = m_fPrePauseSpeed = fSpeed;
}

void raaAnimatedComponent::setManualMultiplier(const float fTimeMultiplier)
{
	sm_fTimeMultiplier = fTimeMultiplier;
}

bool raaAnimatedComponent::canSee(rpcDetectable *pDetectable, osg::Group *pRoot) const
{
	const osg::Vec3f vfGlobalCoordinates = pDetectable->getDetectionPointRelativeTo(pRoot);
	if (dynamic_cast<raaTrafficLightUnit*>(pDetectable)) // is it a light?
	{
		return m_pLightDetector->contains(vfGlobalCoordinates, pRoot); // cool, check the light detector
	}
	return m_pVehicleDetector->contains(vfGlobalCoordinates, pRoot); // no? must be a vehicle
}

void raaAnimatedComponent::initDetectionPoint() const
{
	if (!m_psDetectorSwitch) return;
	osg::MatrixTransform *pDetectionPointTransform = new osg::MatrixTransform();
	pDetectionPointTransform->setMatrix(osg::Matrix::translate(csm_vfBack));
	osg::Geode* pGeode = makeGeode();
	osg::ShapeDrawable* pSPoint = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(0.0, 0.0, 0.0), 2.0f));
	pSPoint->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);
	pGeode->addDrawable(pSPoint);
	pDetectionPointTransform->addChild(pGeode);
	m_psDetectorSwitch->addChild(pDetectionPointTransform);
}
osg::Geode* raaAnimatedComponent::makeGeode()
{
	osg::Geode* pGeode = new osg::Geode();
	osg::Material *pMat = new osg::Material();

	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(0.2f, 0.2f, 0.0f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.8f, 0.8f, 0.0f, 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
	pMat->setShininess(osg::Material::FRONT, 100.0f);

	pGeode->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pGeode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	return pGeode;
}

void raaAnimatedComponent::toggleDetectionBoxVisibility()
{
	m_bDetectorBoxVisible = !m_bDetectorBoxVisible;
	setDetectionBoxVisibility(m_bDetectorBoxVisible);
}

void raaAnimatedComponent::setDetectionBoxVisibility(const bool bIsVisible) const
{
	if (m_psDetectorSwitch)
	{
		if (bIsVisible) m_psDetectorSwitch->setAllChildrenOn();
		else m_psDetectorSwitch->setAllChildrenOff();
	}
}

void raaAnimatedComponent::checkForVehicles()
{
	if (!g_pRoot) return;
	raaVehicles::iterator itVehicle = rpcCollidables::sm_lVehicles.begin();
	// All the vehicles
	for (; itVehicle != rpcCollidables::sm_lVehicles.end() && !m_pVehicleDetected; ++itVehicle)
	{
		if (*itVehicle == this)  return; // No point checking detection on self
		bool detected = canSee((*itVehicle), g_pRoot);
		if (detected) m_pVehicleDetected = *itVehicle; 
	}
	if (m_pVehicleDetected) // Car in my radar
	{
		// still in sight?
		const bool bCanStillSeeVehicle = canSee(m_pVehicleDetected, g_pRoot);
		// if it is, set pause true, if not, take global val
		setPause(bCanStillSeeVehicle || rpcCollidables::instance()->m_bIsGlobalPause);
		if (!bCanStillSeeVehicle) m_pVehicleDetected = nullptr; // discard reference if out of sight
	}
}

osg::MatrixTransform* raaAnimatedComponent::root() const
{
	return m_pRoot;
}

void raaAnimatedComponent::reactToLightInSights()
{
		if (!m_pLightDetected) return;
		switch (m_pLightDetected->m_eTrafficLightState)
		{
		case raaTrafficLightUnit::rpcTrafficLightState::slow:
			goFast();// GET THROUGH THE LIGHT QUICK!!!
			break;
		case raaTrafficLightUnit::rpcTrafficLightState::ready:
			goSlow();
			break;
		case raaTrafficLightUnit::rpcTrafficLightState::go:
			goCruising();
			break;
		default:
			break;
		}
		setPause(m_pLightDetected->m_eTrafficLightState == raaTrafficLightUnit::rpcTrafficLightState::stop || rpcCollidables::instance()->m_bIsGlobalPause);
}

raaAnimatedComponent* raaAnimatedComponent::vehicleFactory(const vehicleType eVehicleType, rpcContextAwareAnimationPath *pAP)
{
	switch (eVehicleType)
	{
	case delta:
		return new rpcCarDelta(pAP);
	case truck:
		return new rpcDumpTruck(pAP);
	default:
		return new rpcCarVeryon(pAP);
	}
}

raaAnimatedComponent* raaAnimatedComponent::buildRandomVehicle(rpcContextAwareAnimationPath *pAP)
{
	return vehicleFactory(getRandomType(), pAP);
}

raaAnimatedComponent::vehicleType raaAnimatedComponent::getRandomType()
{
	unsigned int uiRandom = rand() % numberOfTypes;
	return static_cast<vehicleType>(uiRandom);
}

raaAnimatedComponent::~raaAnimatedComponent()
{
	m_pRoot->unref();
}
