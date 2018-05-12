#include "stdafx.h"
#include "rpcDetectionBox.h"

#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include "raaTrafficLightUnit.h"

rpcDetectionBox::rpcDetectionBox(): m_pRoot(0)
{
}

rpcDetectionBox::rpcDetectionBox(osg::Vec3f vfPosition): m_bVisible(true)
{
	m_pRoot = new osg::MatrixTransform();
	m_pRoot->ref();
	m_pRoot->setMatrix(osg::Matrix::translate(vfPosition[0], vfPosition[1], vfPosition[2]));

	m_pScale = new osg::MatrixTransform();
	m_pScale->setMatrix(osg::Matrix::scale(30.0f, 30.0f, 30.0f));
	m_pScale->ref();
	m_pRoot->addChild(m_pScale);
	osg::Geode* pGeode = makeGeometry();
	m_pSwitch = new osg::Switch();
	m_pSwitch->ref();
	m_pSwitch->addChild(pGeode);
	m_pScale->addChild(m_pSwitch);
}

osg::Geode* rpcDetectionBox::makeGeometry()
{
	osg::Geode* pGeode = new osg::Geode();
	osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Sphere(m_pScale->getBound().center(), m_pScale->getBound().radius()));

	osg::Material *pMat = makeMaterial();

	pGeode->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pGeode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);
	pGeode->addDrawable(pSD);
	return pGeode;
}

void rpcDetectionBox::toggleVisibility()
{
	m_bVisible = !m_bVisible;
	if (m_pSwitch) {
		if (m_bVisible) m_pSwitch->setAllChildrenOn();
		else m_pSwitch->setAllChildrenOff();
	}
}

osg::Material* rpcDetectionBox::makeMaterial()
{
	osg::Material *pMat = new osg::Material();
	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(0.2f, 0.2f, 0.0f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.0f, 0.8f, 0.0f, 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
	pMat->setShininess(osg::Material::FRONT, 100.0f);
	return pMat;
}

rpcDetectionBox::~rpcDetectionBox()
{
	m_pRoot->unref();
	m_pSwitch->unref();
}
