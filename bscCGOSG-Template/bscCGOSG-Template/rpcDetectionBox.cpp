#include "stdafx.h"
#include "rpcDetectionBox.h"

#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include "raaTrafficLightUnit.h"

const osg::Vec3f rpcDetectionBox::vfDefaultSize = osg::Vec3f(30.0f, 30.0f, 30.0f);

rpcDetectionBox::rpcDetectionBox(osg::Vec3f vfPosition, osg::Vec3f vfSize)
{
	// move & size it (need the translate separate for calc of box
	m_pTranslate = new osg::MatrixTransform();
	m_pTranslate->ref();
	m_pTranslate->setMatrix(osg::Matrix::translate(vfPosition[0], vfPosition[1], vfPosition[2]));
	m_pScale = new osg::MatrixTransform();
	m_pScale->setMatrix(osg::Matrix::scale(vfSize[0], vfSize[1], vfSize[2]));
	m_pScale->ref();
	m_pTranslate->addChild(m_pScale);
	m_pScale->addChild(makeGeometry()); // load in wireframe
}

osg::Geode* rpcDetectionBox::makeGeometry() const
{
	osg::Geode* pGeode = new osg::Geode();
	// set the drawables shape to a sphere with bound centre and co-ords
	osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Sphere(m_pScale->getBound().center(), m_pScale->getBound().radius()));

	pGeode->getOrCreateStateSet()->setAttributeAndModes(makeMaterial(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pGeode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	pGeode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);

	pGeode->addDrawable(pSD);
	return pGeode;
}

bool rpcDetectionBox::contains(const osg::Vec3f pvfGlobalCoordinates, osg::Group *pRoot) const
{
	return m_pScale->computeBound().contains(pvfGlobalCoordinates * computeWorldToLocal(m_pTranslate->getParentalNodePaths(pRoot)[0]));
}

osg::Material* rpcDetectionBox::makeMaterial()
{
	osg::Material *pMat = new osg::Material();
	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(0.2f, 0.2f, 0.0f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.8f, 0.8f, 0.0f, 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 0.0f, 1.0f));
	pMat->setShininess(osg::Material::FRONT, 100.0f);
	return pMat;
}

osg::MatrixTransform* rpcDetectionBox::node() const
{
	return m_pTranslate;
}

rpcDetectionBox::~rpcDetectionBox()
{
	m_pTranslate->unref();
	m_pScale->unref();
}
