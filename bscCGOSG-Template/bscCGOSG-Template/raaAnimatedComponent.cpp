#include "stdafx.h"

#include <windows.h>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "raaAnimatedComponent.h"
#include <osg/PolygonMode>
#include "raaTrafficLightUnit.h"
#include <valarray>

const osg::Vec3f raaAnimatedComponent::csm_vfDetector_Position = osg::Vec3f(0.0f, 140.0f, 20.0f);

const osg::Vec3f raaAnimatedComponent::csm_vfBack = osg::Vec3f(-40.0f, 0.0f, 20.0f);

// convert dimensions to consts
raaAnimatedComponent::raaAnimatedComponent(osg::AnimationPath *pAP): AnimationPathCallback(pAP), m_bDetectorBoxVisible(false), m_dTimeMultiplier(1.0), m_dSpeed(1.0)
{
	m_pLightDetected = 0;
	m_pRoot = new osg::MatrixTransform();
	m_pRoot->ref();
	// switch->transform->geode
	// bounding box for the geode not the matrix transform

	osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0.0f, 0.0f, 20.0f), 80.0f, 60.0f, 40.0f));

	osg::Geode* pGeode = initGeode();

	pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);
	pGeode->addDrawable(pSD);
	m_pRoot->addChild(pGeode);

	m_pDetectionBox = new rpcDetectionBox(csm_vfDetector_Position);
	m_psDetectorSwitch = new osg::Switch();
	initDetectionPoint();
	m_psDetectorSwitch->addChild(m_pDetectionBox->m_pRoot);
	m_pRoot->addChild(m_psDetectorSwitch);
	setDetectionBoxVisibility(m_bDetectorBoxVisible);

	m_pRoot->setUpdateCallback(this);
}

void raaAnimatedComponent::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	// Controllable global speed * car's speed in reaction to light colours
	double dTotalMultiplier = m_dTimeMultiplier * m_dSpeed;
	double dSimulationTime = _latestTime - _firstTime; // latest frame
	// Adjusting the multiplier will 
	setTimeOffset(calculateTimeOffset(dSimulationTime, _timeOffset, _timeMultiplier, dTotalMultiplier));
	setTimeMultiplier(dTotalMultiplier);
	AnimationPathCallback::operator()(node, nv);
}

/* Why does this not already exist??
 * ST = Simulation Time (latest - first), OC = Original Offset, OM = Original multiplier,
 * NO = New Offeset, NM = New Multiplier
 * (ST-OO)*OM=(ST-NO)*NM
 * ((ST-OO)*OM)/NM = ST-NO
 * NO = ST - (((ST-OO)*OM)/NM))
 */
double raaAnimatedComponent::calculateTimeOffset(double dSimulationTime, double dOriginalOffset, double dOriginalMultiplier, double dTotalMultiplier)
{
	return dSimulationTime - (dSimulationTime - dOriginalOffset) * dOriginalMultiplier / dTotalMultiplier;
}

void raaAnimatedComponent::setSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
}

void raaAnimatedComponent::setManualMultiplier(double dTimeMultiplier)
{
	m_dTimeMultiplier = dTimeMultiplier;
}

void raaAnimatedComponent::initDetectionPoint()
{
	if (!m_psDetectorSwitch) return;
	osg::MatrixTransform *pDetectionPointTransform = new osg::MatrixTransform();
	pDetectionPointTransform->setMatrix(osg::Matrix::translate(csm_vfBack));
	osg::Geode* pGeode = initGeode();
	osg::ShapeDrawable* pSPoint = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(0.0, 0.0, 0.0), 2.0f));

	pGeode->addDrawable(pSPoint);
	pDetectionPointTransform->addChild(pGeode);
	m_psDetectorSwitch->addChild(pDetectionPointTransform);
}

osg::Geode* raaAnimatedComponent::initGeode()
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

osg::Vec3f raaAnimatedComponent::getDetectionPoint(osg::MatrixTransform* pRoot)
{
	osg::Matrix m;
	if (pRoot) {
		m = osg::computeLocalToWorld(m_pRoot->getParentalNodePaths(pRoot)[0]);
		return csm_vfBack * m;
	}
	return csm_vfBack;
}

void raaAnimatedComponent::toggleDetectionBoxVisibility()
{
	m_bDetectorBoxVisible = !m_bDetectorBoxVisible;
	setDetectionBoxVisibility(m_bDetectorBoxVisible);
}

void raaAnimatedComponent::setDetectionBoxVisibility(const bool bIsVisible)
{
	if (m_psDetectorSwitch)
	{
		if (bIsVisible) m_psDetectorSwitch->setAllChildrenOn();
		else m_psDetectorSwitch->setAllChildrenOff();
	}
}

raaAnimatedComponent::~raaAnimatedComponent()
{
	m_pRoot->unref();
}

osg::MatrixTransform* raaAnimatedComponent::root()
{
	return m_pRoot;
}

void raaAnimatedComponent::handleVehicleReactionToLight(bool bIsGlobalPause)
{
	if (!m_pLightDetected) return;
	if (m_pLightDetected->m_eTrafficLightState == raaTrafficLightUnit::rpcTrafficLightState::STOP) {
		setPause(true);
		return;
	}

	switch (m_pLightDetected->m_eTrafficLightState)
	{
	case raaTrafficLightUnit::rpcTrafficLightState::SLOW:
		setSpeed(4.0);
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::READY:
		setSpeed(0.5);
		break;
	case raaTrafficLightUnit::rpcTrafficLightState::GO:
		setSpeed(1.0);
		break;
	default:
		break;
	}
	setPause(bIsGlobalPause);
}
