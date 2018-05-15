#include "stdafx.h"
#include <windows.h>
#include <osgDB\readFile>
#include "rpcCarVeryon.h"

const float rpcCarVeryon::csm_fSlowMultiplier = 0.75f;
const float rpcCarVeryon::csm_fFastMultiplier = 6.0f;
const float rpcCarVeryon::csm_fCruisingMultiplier = 2.0f;

osg::Node* rpcCarVeryon::sm_pGeometry = nullptr;

rpcCarVeryon::rpcCarVeryon(rpcContextAwareAnimationPath* pAP) : raaAnimatedComponent(pAP)
{
	rpcCarVeryon::buildGeometry();
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

void rpcCarVeryon::buildGeometry()
{
	m_pTransform = new osg::MatrixTransform();
	m_pTransform->ref();
	osg::Matrixf mT, mR, mS;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 30.0f));
	mR.makeRotate(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 0.0f, 1.0f));
	mS.makeScale(osg::Vec3f(10.0f, 10.0f, 10.0f));
	m_pTransform->setMatrix(mS * mR * mT);
	m_pTransform->addChild(static_cast<osg::Node*>(sm_pGeometry->clone(osg::CopyOp::DEEP_COPY_NODES)));
	m_pRoot->addChild(m_pTransform); // add to root
}

void rpcCarVeryon::initAsset(const std::string sPath)
{
	if (!sm_pGeometry)
	{
		sm_pGeometry = osgDB::readNodeFile(sPath);
		sm_pGeometry->ref();
	}
}

rpcCarVeryon::~rpcCarVeryon()
{
	m_pTransform->unref();
}