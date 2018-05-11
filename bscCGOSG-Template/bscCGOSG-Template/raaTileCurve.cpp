#include "stdafx.h"
#include "raaRoadSet.h"
#include "raaTileCurve.h"


raaTileCurve::raaTileCurve(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP): raaTile(uiName, pNode, pAP)
{
	osg::Matrixf m;
	m=m.translate(raaRoadSet::sm_fTileUnit, 0.0, 0.0);
	m_pOffset->setMatrix(m);
}


raaTileCurve::~raaTileCurve()
{
}
