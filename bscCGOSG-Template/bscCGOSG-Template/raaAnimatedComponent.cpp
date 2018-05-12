#include "stdafx.h"

#include <windows.h>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "raaAnimatedComponent.h"
#include <osg/PolygonMode>

const osg::Vec3f raaAnimatedComponent::csm_vfDetector_Position = osg::Vec3f(20.0f, 70.0f, 20.0f);

const osg::Vec3f raaAnimatedComponent::csm_vfBack = osg::Vec3f(-40.0f, 0.0f, 20.0f);

void raaAnimatedComponent::initDetectionPoints()
{
	if (!m_psDetectorSwitch) return;
	osg::Geode* pGeode = initGeode();
	osg::ShapeDrawable* pSPoint = new osg::ShapeDrawable(new osg::Sphere(csm_vfBack, 2.0f));
	osg::ShapeDrawable* pSPoint2 = new osg::ShapeDrawable(new osg::Sphere(csm_vfDetector_Position, 2.0f));

	pGeode->addDrawable(pSPoint);
	pGeode->addDrawable(pSPoint2);
	m_psDetectorSwitch->addChild(pGeode);
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

// convert dimensions to consts
raaAnimatedComponent::raaAnimatedComponent(osg::AnimationPath *pAP): osg::AnimationPathCallback(pAP), m_bDetectorBoxVisible(false)
{
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
	initDetectionPoints();
	m_psDetectorSwitch->addChild(m_pDetectionBox->m_pRoot);
	m_pRoot->addChild(m_psDetectorSwitch);
	setDetectionBoxVisibility(m_bDetectorBoxVisible);

	m_pRoot->setUpdateCallback(this);
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


