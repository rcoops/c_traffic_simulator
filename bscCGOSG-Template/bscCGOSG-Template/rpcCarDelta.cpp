#include "stdafx.h"
#include <windows.h>
#include <osgDB\readFile>
#include "raaPrinter.h"
#include "raaFinder.h"
#include "rpcCarDelta.h"

std::string asBodyNames[6] = { "HDM_01_07_top-GEODE", "HDM_01_07_hood-GEODE", "HDM_01_07_front_fenders-GEODE", "HDM_01_07_doors-GEODE", "HDM_01_07_trunk_spoiler-GEODE", "HDM_01_07_bottom_sill-GEODE" };

rpcCarDelta::rpcCarDelta(rpcContextAwareAnimationPath* pAP) : raaAnimatedComponent(pAP)
{
	m_pRoot->addChild(makeBaseGeometry());
}

osg::Node* rpcCarDelta::makeBaseGeometry()
{
	m_pGeometry = osgDB::readNodeFile("../../Data/car-delta.OSGB");
	m_pGeometry->ref();
	//raaPrinter printer;
	//printer.apply(*m_pGeometry);
	osg::MatrixTransform *pMatrixTransform = new osg::MatrixTransform();
	osg::Matrixf mT, mR;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 0.0f));
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f));
	pMatrixTransform->setMatrix(mT * mR);
	pMatrixTransform->addChild(m_pGeometry);
	
	m_pBodyMat = new osg::Material();
	m_pBodyMat->ref();
	materialBuilder(m_pBodyMat, osg::Vec3f(0.0f, 0.8f, 0.5f));
	for (int i = 0; i < 6; ++i) paintGeode(m_pBodyMat, getGeode(asBodyNames[i]));

	return pMatrixTransform;
}
osg::Geode* rpcCarDelta::getGeode(std::string psNodeName)
{
	raaFinder<osg::Geode> finder(psNodeName, m_pGeometry);
	return finder.node();
}

void rpcCarDelta::paintGeode(osg::Material* pMat, osg::Geode* pGeode)
{
	pGeode->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

void rpcCarDelta::materialBuilder(osg::Material* pMat, osg::Vec3f vMat)
{
	pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(vMat[0] * 0.2f, vMat[1] * 0.2f, vMat[2] * 0.2f, 1.0f));
	pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(vMat[0], vMat[1], vMat[2], 1.0f));
	pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
	pMat->setShininess(osg::Material::FRONT, 128.0f);
}

rpcCarDelta::~rpcCarDelta() {
	m_pBodyMat->unref();
	m_pGeometry->unref();
}