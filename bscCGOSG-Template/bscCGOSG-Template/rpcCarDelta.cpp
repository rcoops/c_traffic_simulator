#include "stdafx.h"
#include <windows.h>
#include <osgDB\readFile>

#include "rpcCarDelta.h"

rpcCarDelta::rpcCarDelta(rpcContextAwareAnimationPath* pAP) : raaAnimatedComponent(pAP)
{
	m_pRoot->addChild(makeBaseGeometry());
}

osg::Node* rpcCarDelta::makeBaseGeometry()
{
	osg::Geode* pGeode = makeGeode();
	osg::Node *pNode = osgDB::readNodeFile("../../Data/car-delta.OSGB");
	osg::MatrixTransform *pTranslate = new osg::MatrixTransform();
	osg::MatrixTransform *pRotate = new osg::MatrixTransform();
	osg::MatrixTransform *pScale = new osg::MatrixTransform();
	osg::Matrixf matrix = osg::Matrixf();
	osg::Matrixf mT, mR, mS;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 30.0f));
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f));
	mS.makeScale(osg::Vec3f(10.0f, 10.0f, 10.0f));
	pTranslate->setMatrix(mT);
	pTranslate->addChild(pRotate);
	pRotate->setMatrix(mR);
	pRotate->addChild(pScale);
	pScale->setMatrix(mS);
	pScale->addChild(pNode);
	//osg::MatrixTransform *matT = new osg::MatrixTransform();
	//matT->setMatrix(osg::Matrix::scale(osg::Vec3f(50.0f, 50.0f, 50.0f)));
	//osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Box(osg::Vec3f(0.0f, 0.0f, 20.0f), 80.0f, 60.0f, 40.0f));
	//pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::Material::ON | osg::Material::OVERRIDE);

	//pGeode->addDrawable(pSD);
	//pGeode->addChild(pNode);
	return pTranslate;
}

rpcCarDelta::~rpcCarDelta() {}