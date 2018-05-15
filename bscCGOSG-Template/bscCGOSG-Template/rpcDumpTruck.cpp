#include "stdafx.h"
#include <windows.h>
#include "rpcDumpTruck.h"
#include <osgDB/ReadFile>

const float rpcDumpTruck::csm_fSlowMultiplier = 0.25f;
const float rpcDumpTruck::csm_fFastMultiplier = 2.0f;
const float rpcDumpTruck::csm_fCruisingMultiplier = 0.75f;

osg::Node* rpcDumpTruck::sm_pGeometry = nullptr;

rpcDumpTruck::rpcDumpTruck(rpcContextAwareAnimationPath* pAP): raaAnimatedComponent(pAP)
{
	rpcDumpTruck::buildGeometry();
	if (m_pTransform) initDetectionPoint(osg::Vec3f(-m_pTransform->getBound().radius(), 0.0f, 20.0f));
}

void rpcDumpTruck::goFast()
{
	setSpeed(csm_fFastMultiplier);
}

void rpcDumpTruck::goSlow()
{
	setSpeed(csm_fSlowMultiplier);
}

void rpcDumpTruck::goCruising()
{
	setSpeed(csm_fCruisingMultiplier);
}

void rpcDumpTruck::buildGeometry()
{
	m_pTransform = new osg::MatrixTransform();
	m_pTransform->ref();
	osg::Matrixf mT, mS;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 60.0f));
	mS.makeScale(osg::Vec3f(10.0f, 10.0f, 10.0f));
	m_pTransform->setMatrix(mS * mT);
	m_pTransform->addChild(static_cast<osg::Node*>(sm_pGeometry->clone(osg::CopyOp::DEEP_COPY_NODES)));
	m_pRoot->addChild(m_pTransform);
}

void rpcDumpTruck::initAsset(const std::string sPath)
{
	if (!sm_pGeometry)
	{
		sm_pGeometry = osgDB::readNodeFile(sPath);
		sm_pGeometry->ref();
	}
}

rpcDumpTruck::~rpcDumpTruck()
{
	m_pTransform->unref();
}
