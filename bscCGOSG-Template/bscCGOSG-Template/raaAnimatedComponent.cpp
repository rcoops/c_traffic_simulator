#include "stdafx.h"

#include <windows.h>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>

#include "raaAnimatedComponent.h"
#include <osg/PolygonMode>

const osg::Vec3f raaAnimatedComponent::msc_vfDetector_Position = osg::Vec3f(20.0f, 70.0f, 20.0f);

// convert dimensions to consts
raaAnimatedComponent::raaAnimatedComponent(osg::AnimationPath *pAP): osg::AnimationPathCallback(pAP), m_bDetectorBoxVisible(true)
{
	m_pRoot = new osg::MatrixTransform();
	m_pRoot->ref();

	m_vfFront = osg::Vec3f(40.0f, 0.0f, 20.0f);
	// switch->transform->geode
	// bounding box for the geode not the matrix transform
	osg::ShapeDrawable* pSPoint = new osg::ShapeDrawable(new osg::Sphere(m_vfFront, 2.0f));

	osg::Geode* pGeode = new osg::Geode();
	osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0.0f, 0.0f, 20.0f), 80.0f, 60.0f, 40.0f));

	osg::Material *pMat = new osg::Material();
	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(0.2f, 0.2f, 0.0f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.8f, 0.8f, 0.0f, 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
	pMat->setShininess(osg::Material::FRONT, 100.0f);

	pGeode->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pGeode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);
	pGeode->addDrawable(pSD);

	pGeode->addDrawable(pSPoint);
	m_pRoot->addChild(pGeode);
	m_pDetectionBox = new rpcDetectionBox(msc_vfDetector_Position);
	m_psDetectorSwitch = new osg::Switch();
	m_psDetectorSwitch->addChild(m_pDetectionBox->m_pRoot);
	toggleDetectionBox();
	m_pRoot->addChild(m_psDetectorSwitch);

	m_pRoot->setUpdateCallback(this);
}

osg::Vec3f raaAnimatedComponent::getDetectionPoint(osg::MatrixTransform* pRoot)
{
	osg::Matrix m;
	if (pRoot) {
		m = osg::computeLocalToWorld(m_pRoot->getParentalNodePaths(pRoot)[0]);
		return m_vfFront * m;
	}
	return m_vfFront;
}

void raaAnimatedComponent::toggleDetectionBox()
{
	osg::BoundingSphere sphere = m_pDetectionBox->m_pRoot->getBound();
	m_bDetectorBoxVisible = !m_bDetectorBoxVisible;
	if (m_psDetectorSwitch)
	{
		if (m_bDetectorBoxVisible) m_psDetectorSwitch->setAllChildrenOn();
		else m_psDetectorSwitch->setAllChildrenOff();
	}
	printf("");
}

void raaAnimatedComponent::toggleDetectionBoxVisibility()
{
	m_pDetectionBox->toggleVisibility();
}

bool raaAnimatedComponent::isDetectionBoxVisible()
{
	return m_bDetectorBoxVisible;
}

void raaAnimatedComponent::toggleDetectionBoxes()
{
	// TODO?
}

raaAnimatedComponent::~raaAnimatedComponent()
{
	m_pRoot->unref();
}

osg::MatrixTransform* raaAnimatedComponent::root()
{
	return m_pRoot;
}


