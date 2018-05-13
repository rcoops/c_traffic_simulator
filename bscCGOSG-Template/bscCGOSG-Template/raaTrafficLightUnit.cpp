#include "stdafx.h"
#include <windows.H>
#include <osgDB/ReadFile>
#include "raaTrafficLightUnit.h"
#include "raaFinder.h"
#include "raaPrinter.h"
#include <osg/PolygonMode>
#include <osg/ShapeDrawable>
#include <osg/Switch>

osg::Node* raaTrafficLightUnit::sm_pAsset = nullptr;
osg::Material* raaTrafficLightUnit::sm_pRedOn=nullptr;
osg::Material* raaTrafficLightUnit::sm_pRedOff=nullptr;
osg::Material* raaTrafficLightUnit::sm_pAmberOn=nullptr;
osg::Material* raaTrafficLightUnit::sm_pAmberOff=nullptr;
osg::Material* raaTrafficLightUnit::sm_pGreenOn=nullptr;
osg::Material* raaTrafficLightUnit::sm_pGreenOff=nullptr;

const float raaTrafficLightUnit::csm_fDefaultScale = 0.03f;
const osg::Vec3f raaTrafficLightUnit::csm_vfDetectionPoint = osg::Vec3f(0.0f, -120.0f, 40.0f);

raaTrafficLightUnit::raaTrafficLightUnit(): m_bIsDetectionPointVisible(false)
{
	m_pTranslation = new osg::MatrixTransform();
	m_pTranslation->ref();
	m_pRotation = new osg::MatrixTransform();
	m_pScale = new osg::MatrixTransform();
	m_pTranslation->addChild(m_pRotation);
	m_pRotation->addChild(m_pScale);

	m_pDetectionPointSwitch = new osg::Switch();
	osg::Geode* pGeode = new osg::Geode();
	osg::ShapeDrawable* pSPoint = new osg::ShapeDrawable(new osg::Sphere(csm_vfDetectionPoint, 10.0f));

	m_pRotation->addChild(m_pDetectionPointSwitch); // Don't want to scale the point
	m_pDetectionPointSwitch->addChild(pGeode);
	pGeode->addDrawable(pSPoint);

	m_pScale->addChild(static_cast<Node*>(sm_pAsset->clone(osg::CopyOp::DEEP_COPY_NODES)));

	raaFinder<osg::Geode> redFinder("trafficLight::RedLamp-GEODE", m_pRotation);
	raaFinder<osg::Geode> greenFinder("trafficLight::GreenLamp-GEODE", m_pRotation);
	raaFinder<osg::Geode> amberFinder("trafficLight::AmberLamp-GEODE", m_pRotation);

	m_pRed = redFinder.node();
	m_pGreen = greenFinder.node();
	m_pAmber = amberFinder.node();

	setLightState();
	setDetectionPointVisibility(m_bIsDetectionPointVisible);
}

void raaTrafficLightUnit::toggleDetectionPointVisibility()
{
	m_bIsDetectionPointVisible = !m_bIsDetectionPointVisible;
	setDetectionPointVisibility(m_bIsDetectionPointVisible);
}

osg::Vec3f raaTrafficLightUnit::getDetectionPointRelativeTo(Node *pRoot)
{
	if (pRoot) {
		return csm_vfDetectionPoint * computeLocalToWorld(m_pRotation->getParentalNodePaths(pRoot)[0]);
	}
	return csm_vfDetectionPoint;
}

void raaTrafficLightUnit::setDetectionPointVisibility(const bool bIsVisible) const
{
	if (m_pDetectionPointSwitch)
	{
		if (bIsVisible) m_pDetectionPointSwitch->setAllChildrenOn();
		else m_pDetectionPointSwitch->setAllChildrenOff();
	}
}

void raaTrafficLightUnit::setLightState(const rpcTrafficLightState eNewState)
{
	m_eTrafficLightState = eNewState;
	adjustLightColour();
}

void raaTrafficLightUnit::setManualState(const rpcTrafficLightState eNewState)
{
	m_bIsManual = true;
	m_eLastState = m_eTrafficLightState;
	setLightState(eNewState);
}

void raaTrafficLightUnit::turnOffManualState()
{
	m_bIsManual = false;
	setLightState(m_eLastState);
}

void raaTrafficLightUnit::adjustLightColour() const
{
	switch (m_eTrafficLightState) {
	case go:
		setColour(sm_pGreenOn, sm_pAmberOff, sm_pRedOff);
		break;
	case stop:
		setColour(sm_pGreenOff, sm_pAmberOff, sm_pRedOn);
		break;
	case slow:
		setColour(sm_pGreenOff, sm_pAmberOn, sm_pRedOn);
		break;
	case ready:
		setColour(sm_pGreenOff, sm_pAmberOn, sm_pRedOff);
		break;
	default:
		setColour(sm_pGreenOff, sm_pAmberOff, sm_pRedOff);
	}
}

void raaTrafficLightUnit::setColour(osg::Material* pGreen, osg::Material* pAmber, osg::Material* pRed) const
{
	m_pGreen->getOrCreateStateSet()->setAttributeAndModes(pGreen, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pRed->getOrCreateStateSet()->setAttributeAndModes(pAmber, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	m_pAmber->getOrCreateStateSet()->setAttributeAndModes(pRed, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void raaTrafficLightUnit::initAsset(const std::string sFile)
{
	if(!sm_pAsset)
	{
		sm_pAsset = osgDB::readNodeFile(sFile);
		sm_pAsset->ref();

		sm_pRedOn = new osg::Material();
		sm_pRedOn->ref();
		sm_pRedOff = new osg::Material();
		sm_pRedOff->ref();
		materialBuilder(sm_pRedOn, sm_pRedOff, osg::Vec3f(1.0f, 0.0f, 0.0f));

		sm_pGreenOn = new osg::Material();
		sm_pGreenOn->ref();
		sm_pGreenOff = new osg::Material();
		sm_pGreenOff->ref();
		materialBuilder(sm_pGreenOn, sm_pGreenOff, osg::Vec3f(0.0f, 1.0f, 0.0f));

		sm_pAmberOn = new osg::Material();
		sm_pAmberOn->ref();
		sm_pAmberOff = new osg::Material();
		sm_pAmberOff->ref();
		materialBuilder(sm_pAmberOn, sm_pAmberOff, osg::Vec3f(1.0f, 0.5f, 0.0f));
	}
}

void raaTrafficLightUnit::finshAsset()
{
	if(sm_pAsset)
	{
		sm_pAsset->unref();
		sm_pAsset = nullptr;

		sm_pRedOn->unref();
		sm_pRedOff->unref();
		sm_pGreenOn->unref();
		sm_pGreenOff->unref();
		sm_pAmberOn->unref();
		sm_pAmberOff->unref();
	}
}


void raaTrafficLightUnit::setTransform(const float fX, const float fY, const float fRot, const float fScale) const
{
	if(m_pTranslation)
	{
		osg::Matrixf mT, mR, mS;

		mT.makeTranslate(osg::Vec3f(fX, fY, 0.0f));
		mR.makeRotate(osg::DegreesToRadians(fRot), osg::Vec3f(0.0f, 0.0f, 1.0f));
		mS.makeScale(osg::Vec3f(fScale, fScale, fScale));

		m_pTranslation->setMatrix(mT);
		m_pRotation->setMatrix(mR);
		m_pScale->setMatrix(mS);
	}
}

osg::Node* raaTrafficLightUnit::node() const
{
	return m_pTranslation;
}

void raaTrafficLightUnit::materialBuilder(osg::Material* pOn, osg::Material* pOff, osg::Vec3f vMat)
{
	pOn->setAmbient(osg::Material::FRONT, osg::Vec4f(vMat[0] * 0.2f, vMat[1] * 0.2f, vMat[2] * 0.2f, 1.0f));
	pOn->setDiffuse(osg::Material::FRONT, osg::Vec4f(vMat[0], vMat[1], vMat[2], 1.0f));
	pOn->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	pOn->setShininess(osg::Material::FRONT, 200.0f);

	pOff->setAmbient(osg::Material::FRONT, osg::Vec4f(vMat[0] * 0.1f, vMat[1] * 0.1f, vMat[2] * 0.1f, 1.0f));
	pOff->setDiffuse(osg::Material::FRONT, osg::Vec4f(vMat[0]*0.3f, vMat[1]*0.3f, vMat[2]*0.3f, 1.0f));
	pOff->setSpecular(osg::Material::FRONT, osg::Vec4f(0.3f, 0.3f, 0.3f, 1.0f));
	pOff->setShininess(osg::Material::FRONT, 10.0f);
}

raaTrafficLightUnit::~raaTrafficLightUnit()
{
	if (m_pTranslation) m_pTranslation->unref();
}
