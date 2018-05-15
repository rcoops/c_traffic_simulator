#include "stdafx.h"
#include <windows.h>
#include <osgDB\readFile>
#include "rpcCarVeryon.h"

const float rpcCarVeryon::csm_fSlowMultiplier = 0.75f;
const float rpcCarVeryon::csm_fFastMultiplier = 6.0f;
const float rpcCarVeryon::csm_fCruisingMultiplier = 2.0f;

rpcCarVeryon::rpcCarVeryon(rpcContextAwareAnimationPath* pAP) : raaAnimatedComponent(pAP)
{
	m_pRoot->addChild(rpcCarVeryon::makeBaseGeometry());
}

void rpcCarVeryon::goFast()
{
	setSpeed(csm_fFastMultiplier);
}

void rpcCarVeryon::goSlow()
{
	setSpeed(csm_fSlowMultiplier);
}

void rpcCarVeryon::goCruising()
{
	setSpeed(csm_fCruisingMultiplier);
}

osg::Node* rpcCarVeryon::makeBaseGeometry()
{
	m_pGeometry = osgDB::readNodeFile("../../Data/car-veyron.OSGB");
	m_pGeometry->ref();
	osg::MatrixTransform *pMatrixTransform = new osg::MatrixTransform();
	osg::Matrixf mT, mR, mS;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 30.0f));
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f));
	mS.makeScale(osg::Vec3f(10.0f, 10.0f, 10.0f));
	pMatrixTransform->setMatrix(mS * mR * mT);
	pMatrixTransform->addChild(m_pGeometry);
	return pMatrixTransform;
}

rpcCarVeryon::~rpcCarVeryon()
{
	m_pGeometry->unref();
}