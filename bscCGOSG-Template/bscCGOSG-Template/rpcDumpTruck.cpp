#include "stdafx.h"
#include <windows.h>
#include "rpcDumpTruck.h"
#include <osgDB/ReadFile>

const float rpcDumpTruck::csm_fSlowMultiplier = 0.25f;
const float rpcDumpTruck::csm_fFastMultiplier = 2.0f;
const float rpcDumpTruck::csm_fCruisingMultiplier = 0.75f;

rpcDumpTruck::rpcDumpTruck(rpcContextAwareAnimationPath* pAP): raaAnimatedComponent(pAP)
{
	m_pRoot->addChild(rpcDumpTruck::makeBaseGeometry());
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

osg::Node* rpcDumpTruck::makeBaseGeometry()
{
	m_pGeometry = osgDB::readNodeFile("../../Data/dumptruck.osgt");
	m_pGeometry->ref();
	//raaPrinter printer;
	//printer.apply(*m_pGeometry);
	osg::MatrixTransform *pMatrixTransform = new osg::MatrixTransform();
	osg::Matrixf mT, mS;
	mT.makeTranslate(osg::Vec3f(0.0f, 0.0f, 60.0f));
	mS.makeScale(osg::Vec3f(10.0f, 10.0f, 10.0f));
	pMatrixTransform->setMatrix(mS * mT);
	pMatrixTransform->addChild(m_pGeometry);

	return pMatrixTransform;
}

rpcDumpTruck::~rpcDumpTruck()
{
	m_pGeometry->unref();
}
