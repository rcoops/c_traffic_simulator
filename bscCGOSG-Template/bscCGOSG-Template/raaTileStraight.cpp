#include "stdafx.h"
#include "raaRoadSet.h"
#include "raaTileStraight.h"


raaTileStraight::raaTileStraight(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP): raaTile(uiName, pNode, pAP)
{
	osg::Matrixf m;
	m.makeTranslate(-raaRoadSet::sm_fTileUnit, 0.0, 0.0);
	m_pOffset->setMatrix(m);

}

raaTileStraight::~raaTileStraight()
{
}
 