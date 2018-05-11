#include "stdafx.h"
#include "raaRoadSet.h"
#include "raaTileTJunction.h"


raaTileTJunction::raaTileTJunction(unsigned int uiName, osg::Node* pNode, raaAnimationPoints* pAP): raaTile(uiName, pNode, pAP)
{
	osg::Matrixf m;
	m.makeTranslate(-2.0f*raaRoadSet::sm_fTileUnit, 0.0, 0.0);
	m_pOffset->setMatrix(m);

}


raaTileTJunction::~raaTileTJunction()
{
}
