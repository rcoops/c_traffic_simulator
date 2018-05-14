#include "stdafx.h"

#include <windows.h>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "raaAnimatedComponent.h"
#include <osg/PolygonMode>
#include "raaTrafficLightUnit.h"
#include <valarray>
#include "raaJunctionController.h"
#include "rpcContextAwareAnimationPath.h"

extern osg::Group *g_pRoot;

const osg::Vec3f raaAnimatedComponent::csm_vfLightDetectorPosition = osg::Vec3f(0.0f, 140.0f, 20.0f);
const osg::Vec3f raaAnimatedComponent::csm_vfVehicleDetectorPosition = osg::Vec3f(80.0f, 0.0f, 20.0f);

const osg::Vec3f raaAnimatedComponent::csm_vfBack = osg::Vec3f(-40.0f, 0.0f, 20.0f);
float raaAnimatedComponent::sm_fTimeMultiplier = 1.0f;

raaAnimatedComponent::raaAnimatedComponent(osg::AnimationPath *pAP): AnimationPathCallback(pAP), m_bDetectorBoxVisible(false), m_fSpeed(1.0f)
{
	m_pLightDetected = nullptr;
	m_pRoot = new osg::MatrixTransform();
	m_pRoot->ref();
	// switch->transform->geode

	m_pRoot->addChild(makeBaseGeometry());

	m_psDetectorSwitch = new osg::Switch();
	m_pRoot->addChild(m_psDetectorSwitch);
	initDetectionPoint();
	m_pLightDetector = new rpcDetectionBox(csm_vfLightDetectorPosition, osg::Vec3f(50.0f, 50.0f, 50.0f));
	m_pVehicleDetector = new rpcDetectionBox(csm_vfVehicleDetectorPosition);
	m_psDetectorSwitch->addChild(m_pLightDetector->root());
	m_psDetectorSwitch->addChild(m_pVehicleDetector->root());
	setDetectionBoxVisibility(m_bDetectorBoxVisible);
	setFinalAnimationPathPoint(pAP);
	m_pRoot->setUpdateCallback(this);
}

void raaAnimatedComponent::setFinalAnimationPathPoint(osg::AnimationPath *pAP)
{
	rpcContextAwareAnimationPath *pPath = dynamic_cast<rpcContextAwareAnimationPath*>(pAP);
	if (pPath)
	{
		std::pair<unsigned int, unsigned int> pPoints = pPath->getPoint();
		m_uiLastTileInAnimation = pPoints.first;
		m_uiLastTileInAnimation = pPoints.second;
	}
}

void raaAnimatedComponent::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	// Controllable global speed * car's speed in reaction to light colours
	const double dTotalMultiplier = sm_fTimeMultiplier * m_fSpeed;
	const double dSimulationTime = _latestTime - _firstTime; // latest frame
	// Adjusting the multiplier will 
	setTimeOffset(calculateTimeOffset(dSimulationTime, _timeOffset, _timeMultiplier, dTotalMultiplier));
	setTimeMultiplier(dTotalMultiplier);
	AnimationPathCallback::operator()(node, nv);
	checkForVehicles();
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
double raaAnimatedComponent::calculateTimeOffset(const double dSimulationTime, const double dOriginalOffset, const double dOriginalMultiplier, const double dTotalMultiplier)
{
	return dSimulationTime - (dSimulationTime - dOriginalOffset) * dOriginalMultiplier / dTotalMultiplier;
}

void raaAnimatedComponent::setSpeed(const float fSpeed)
{
	m_fSpeed = fSpeed;
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

osg::Geode* raaAnimatedComponent::makeBaseGeometry()
{
	osg::Geode* pGeode = makeGeode();
	osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0.0f, 0.0f, 20.0f), 80.0f, 60.0f, 40.0f));
	pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);

	pGeode->addDrawable(pSD);
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
		if (canSee((*itVehicle), g_pRoot)) m_pVehicleDetected = *itVehicle; 
	}
	if (m_pVehicleDetected)
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
		setSpeed(4.0f); // GET THROUGH THE LIGHT QUICK!!!
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::ready:
		setSpeed(0.5f); // speeding up
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::go:
		setSpeed(1.0f); // standard
		break;
	default:
		break;
	}
	// Haven't taken a red action so just check it here if it's red, pause, if not take global
	setPause(m_pLightDetected->m_eTrafficLightState == raaTrafficLightUnit::rpcTrafficLightState::stop || rpcCollidables::instance()->m_bIsGlobalPause);
}

raaAnimatedComponent::~raaAnimatedComponent()
{
	m_pRoot->unref();
}
