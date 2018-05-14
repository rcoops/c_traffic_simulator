#include "stdafx.h"
#include <windows.h>
#include <osgDB\readFile>
#include "rpcCarVeryon.h"

rpcCarVeryon::rpcCarVeryon(rpcContextAwareAnimationPath* pAP) : raaAnimatedComponent(pAP)
{
	m_pRoot->addChild(makeBaseGeometry());
}

osg::Node* rpcCarVeryon::makeBaseGeometry()
{
	osg::Node *pNode = osgDB::readNodeFile("../../Data/car-veyron.OSGB");
	osg::MatrixTransform *pMatrixTransform = new osg::MatrixTransform();
	osg::Matrixf mT, mR, mS;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 30.0f));
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f));
	mS.makeScale(osg::Vec3f(10.0f, 10.0f, 10.0f));
	pMatrixTransform->setMatrix(mS * mR * mT);
	pMatrixTransform->addChild(pNode);
	return pMatrixTransform;
}

rpcCarVeryon::~rpcCarVeryon()
{

}