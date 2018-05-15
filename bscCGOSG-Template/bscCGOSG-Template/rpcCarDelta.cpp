#include "stdafx.h"
#include <windows.h>
#include <osgDB\readFile>
#include <osg\BlendFunc>
#include "raaPrinter.h"
#include "raaFinder.h"
#include "rpcCarDelta.h"

const float rpcCarDelta::csm_fSlowMultiplier = 0.5f;
const float rpcCarDelta::csm_fFastMultiplier = 4.0f;
const float rpcCarDelta::csm_fCruisingMultiplier = 1.0f;
// admit this is just to avoid scrolling
const unsigned int rpcCarDelta::csm_uiNumberOfBodyParts = 6;
const unsigned int rpcCarDelta::csm_uiNumberOfWindowParts = 1;
const unsigned int rpcCarDelta::csm_uiNumberOfMetallicParts = 12;

osg::Material* rpcCarDelta::sm_pMetallicMat = nullptr;
osg::Material* rpcCarDelta::sm_pWindowMat = nullptr;

static std::string asBodyNames[6] = { "HDM_01_07_trunk_spoiler-GEODE", "HDM_01_07_top-GEODE", "HDM_01_07_front_fenders-GEODE", "HDM_01_07_hood-GEODE", "HDM_01_07_doors-GEODE", "HDM_01_07_trunk_lid-GEODE" };
static std::string asWindowNames[1] = { "HDM_01_07_side_windows-GEODE" };
static std::string asMetallicNames[12] = { "HDM_01_07_front_bumper_reflectors-GEODE", "HDM_01_07_bottom_sill-GEODE", "HDM_01_07_grill-GEODE", "HDM_01_07_muffer-GEODE", "HDM_01_07_rim-GEODE", "HDM_01_07_rim01-GEODE", "HDM_01_07_rim02-GEODE", "HDM_01_07_rim03-GEODE", "HDM_01_07_rim_bolts-GEODE", "HDM_01_07_rim_bolts01-GEODE", "HDM_01_07_rim_bolts02-GEODE", "HDM_01_07_rim_bolts03-GEODE" };

rpcCarDelta::rpcCarDelta(rpcContextAwareAnimationPath* pAP) : raaAnimatedComponent(pAP)
{
	m_pRoot->addChild(rpcCarDelta::makeBaseGeometry());
}

void rpcCarDelta::goFast()
{
	setSpeed(csm_fFastMultiplier);
}

void rpcCarDelta::goSlow()
{
	setSpeed(csm_fSlowMultiplier);
}

void rpcCarDelta::goCruising()
{
	setSpeed(csm_fCruisingMultiplier);
}

osg::Node* rpcCarDelta::makeBaseGeometry()
{
	m_pGeometry = osgDB::readNodeFile("../../Data/car-delta.OSGB");
	m_pGeometry->ref();
//	raaPrinter printer;
//	printer.apply(*m_pGeometry);
	osg::MatrixTransform *pMatrixTransform = new osg::MatrixTransform();
	osg::Matrixf mR;
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f));
	pMatrixTransform->setMatrix(mR);
	pMatrixTransform->addChild(m_pGeometry);
	
	for (int i = 0; i < csm_uiNumberOfBodyParts; ++i) paintBody(getGeode(asBodyNames[i]));
	for (int i = 0; i < csm_uiNumberOfMetallicParts; ++i) paintMetallicPart(getGeode(asMetallicNames[i]));
	for (int i = 0; i < csm_uiNumberOfWindowParts; ++i) paintWindow(getGeode(asWindowNames[i]));
	paintTailLights(getGeode("HDM_01_07_taillights-GEODE"));
	paintHeadLights(getGeode("HDM_01_07_Object02-GEODE"), getGeode("HDM_01_07_Object03-GEODE"), getGeode("HDM_01_07_Object01-GEODE"), getGeode("HDM_01_07_Object04-GEODE"));
	paintIndicators(getGeode("HDM_01_07_Object05-GEODE"), getGeode("HDM_01_07_Object08-GEODE"));
	paintTailLights(getGeode("HDM_01_07_Object07-GEODE"));

	return pMatrixTransform;
}
osg::Geode* rpcCarDelta::getGeode(const std::string psNodeName) const
{
	raaFinder<osg::Geode> finder(psNodeName, m_pGeometry);
	return finder.node();
}
// http://devernay.free.fr/cours/opengl/materials.html

void rpcCarDelta::paintWindow(osg::Geode* pGeode)
{
	if (!pGeode) return;
	pGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	getOrCreateMaterial(&sm_pWindowMat, osg::Vec3f(0.1f, 0.1f, 0.1f));
	pGeode->getStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	sm_pWindowMat->setTransparency(osg::Material::FRONT, 0.9f);
	pGeode->getStateSet()->setAttributeAndModes(sm_pWindowMat, osg::StateAttribute::OVERRIDE);
}

void rpcCarDelta::paintIndicators(osg::Geode* pFrontRightIndicator, osg::Geode* pFrontLeftIndicator)
{
	if (pFrontRightIndicator || pFrontLeftIndicator)
	{
		osg::Material *pMat = new osg::Material();
		buildLightMaterial(pMat, osg::Vec3f(1.0f, 0.5f, 0.0f));
		if (pFrontRightIndicator) pFrontRightIndicator->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
		if (pFrontLeftIndicator) pFrontLeftIndicator->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
	}
}

void rpcCarDelta::paintHeadLights(osg::Geode* pFrontRightInner, osg::Geode* pFrontLeftInner, osg::Geode* pFrontRightOuter, osg::Geode* pFrontLeftOuter)
{
	if (pFrontRightInner || pFrontLeftInner || pFrontRightOuter || pFrontLeftOuter)
	{
		osg::Material *pMat = new osg::Material();
		buildLightMaterial(pMat, osg::Vec3f(1.0f, 1.0f, 1.0f));
		if (pFrontRightInner) pFrontRightInner->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
		if (pFrontLeftInner) pFrontLeftInner->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
		if (pFrontRightOuter) pFrontRightOuter->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
		if (pFrontLeftOuter) pFrontLeftOuter->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
	}
}

void rpcCarDelta::paintTailLights(osg::Geode* pBackLights)
{
	if (pBackLights)
	{
		osg::Material *pMat = new osg::Material();
		buildLightMaterial(pMat, osg::Vec3f(1.0f, 0.0f, 0.0f));
		pBackLights->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::OVERRIDE);
	}
}

void rpcCarDelta::buildLightMaterial(osg::Material *PMat, osg::Vec3f vMat)
{
	PMat->setAmbient(osg::Material::FRONT, osg::Vec4f(vMat[0] * 0.1f, vMat[1] * 0.1f, vMat[2] * 0.1f, 1.0f));
	PMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(vMat[0] * 0.3f, vMat[1] * 0.3f, vMat[2] * 0.3f, 1.0f));
	PMat->setSpecular(osg::Material::FRONT, osg::Vec4f(0.3f, 0.3f, 0.3f, 1.0f));
	PMat->setShininess(osg::Material::FRONT, 10.0f);
}

void rpcCarDelta::paintMetallicPart(osg::Geode* pGeode)
{
	if (!pGeode) return;
	buildNewMaterial(&sm_pMetallicMat, osg::Vec3f(0.19225f, 0.19225f, 0.19225f),
		osg::Vec3f(0.50754f, 0.50754f, 0.50754f), osg::Vec3f(0.508273f, 0.508273f, 0.508273f), 128.0f);
	pGeode->getOrCreateStateSet()->setAttributeAndModes(sm_pMetallicMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void rpcCarDelta::paintBody(osg::Geode* pGeode)
{
	if (!pGeode) return;
	const float fRed = generateRandomColourValue();
	const float fGreen = generateRandomColourValue();
	const float fBlue = generateRandomColourValue();
	getOrCreateMaterial(&m_pBodyMat, osg::Vec3f(fRed, fGreen, fBlue));
	pGeode->getOrCreateStateSet()->setAttributeAndModes(m_pBodyMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

float rpcCarDelta::generateRandomColourValue()
{
	return (rand() % 10) / 10.0f;
}

void rpcCarDelta::getOrCreateMaterial(osg::Material **pMat, osg::Vec3f vMat)
{
	if (!*pMat)
	{
		*pMat = new osg::Material();
		(*pMat)->ref();
		materialBuilder(*pMat, vMat);
	}
}

void rpcCarDelta::buildNewMaterial(osg::Material **pMat, const osg::Vec3f vAmb, const osg::Vec3f vDiff, const osg::Vec3f vSpec, const float fShininess)
{
	if (!*pMat)
	{
		*pMat = new osg::Material();
		(*pMat)->ref();
		fullMaterialBuilder(*pMat, vAmb, vDiff, vSpec, fShininess);
	}
}

void rpcCarDelta::fullMaterialBuilder(osg::Material* pMat, osg::Vec3f vAmb, osg::Vec3f vDiff, osg::Vec3f vSpec, const float fShininess)
{
	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(vAmb[0], vAmb[1], vAmb[2], 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(vDiff[0], vDiff[1], vDiff[2], 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(vSpec[0], vSpec[1], vSpec[2], 1.0f));
	pMat->setShininess(osg::Material::FRONT, fShininess);
}

void rpcCarDelta::materialBuilder(osg::Material* pMat, osg::Vec3f vMat)
{
	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(vMat[0] * 0.2f, vMat[1] * 0.2f, vMat[2] * 0.2f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(vMat[0], vMat[1], vMat[2], 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	pMat->setShininess(osg::Material::FRONT, 128.0f);
}

rpcCarDelta::~rpcCarDelta()
{
	m_pBodyMat->unref();
	m_pGeometry->unref();
}
