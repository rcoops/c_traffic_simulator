#include "stdafx.h"

#include <windows.h>
#include <osg/Switch>
#include <osgText/Text>
#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <sstream>

#include "raaAnimationPoints.h"
#include "raaRoadSet.h"
#include "raaTile.h"

raaTileList raaTile::sm_lTiles;
bool raaTile::sm_bAnimOn = false;

raaTile::raaTile(unsigned int uiName, osg::Node* pModel, raaAnimationPoints* pAP): m_pAP(pAP)
{
	sm_lTiles.push_back(this);
	m_pTranslation = new osg::MatrixTransform();
	m_pRotation=new osg::MatrixTransform();
	m_pLocalRoot=new osg::Group();
	m_pModelSwitch = new osg::Switch();
	m_pOffset = new osg::MatrixTransform();
	m_pAnimPointsSwitch=new osg::Switch();

	m_pTranslation->ref();

	m_pTranslation->addChild(m_pRotation);
	m_pRotation->addChild(m_pLocalRoot);
	m_pLocalRoot->addChild(m_pModelSwitch);
	m_pLocalRoot->addChild(m_pAnimPointsSwitch);
	m_pModelSwitch->setAllChildrenOn();
	m_pModelSwitch->addChild(m_pOffset);
	m_pAnimPointsSwitch->setAllChildrenOff();
	m_pOffset->addChild(pModel);

	m_uiName = uiName;

	m_pAnimBillboard = new osg::Billboard();
	osgText::Text* pText = new osgText::Text();
	std::stringstream ss;
	ss << uiName;
	pText->setText(ss.str());
	pText->setColor(osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
	pText->setAlignment(osgText::Text::AlignmentType::CENTER_CENTER);
	m_pAnimBillboard->addDrawable(pText);
	m_pAnimBillboard->setMode(osg::Billboard::POINT_ROT_EYE);
	m_pAnimBillboard->setPosition(0, osg::Vec3f(0.0f, 0.0f, 0.1f*raaRoadSet::sm_fTileUnit));
	m_pAnimBillboard->setNormal(osg::Vec3f(0.0f, 0.0f, 1.0f));
	m_pAnimPointsSwitch->addChild(m_pAnimBillboard);
	m_pAnimGroup = new osg::Group();
	m_pAnimPointsSwitch->addChild(m_pAnimGroup);

	if (sm_bAnimOn) m_pAnimPointsSwitch->setAllChildrenOn();
	else m_pAnimPointsSwitch->setAllChildrenOff();

	m_pAnimPointsSwitch->addChild(m_pAP->sg());
}


raaTile::~raaTile()
{
	if (m_pTranslation) m_pTranslation->unref();
	sm_lTiles.remove(this);
}

void raaTile::translate(float fTranslationX, float fTranslationY)
{
	osg::Matrixf m = m_pTranslation->getMatrix();
	m=m.translate(fTranslationX*raaRoadSet::sm_fTileUnit, fTranslationY*raaRoadSet::sm_fTileUnit, 0.0f);
	m_pTranslation->setMatrix(m);
}

void raaTile::rotate(float fRotation)
{
	osg::Matrixf m = m_pRotation->getMatrix();
	m=m.rotate(osg::DegreesToRadians(fRotation), osg::Vec3f(0.0f, 0.0f, 1.0f));
	m_pRotation->setMatrix(m);
}

osg::Vec3f raaTile::animPointPosition(unsigned uiIndex)
{
	if (uiIndex < numAnimPoints()) return m_pAP->pos(uiIndex);
	return osg::Vec3f(0.0f, 0.0f, 0.0f);
}

osg::Quat raaTile::animPointRotation(unsigned uiIndex)
{
	if (uiIndex < numAnimPoints()) return m_pAP->rot(uiIndex);
	return osg::Quat(0.0f, osg::Vec3f(0.0f, 0.0f, 1.0f));

}

unsigned raaTile::numAnimPoints()
{
	if (m_pAP) return m_pAP->numPoints();
	return 0;
}

void raaTile::toggleAnimPoints()
{
	sm_bAnimOn = !sm_bAnimOn;
	for (raaTileList::iterator it = sm_lTiles.begin(); it != sm_lTiles.end(); it++)
		if (sm_bAnimOn) (*it)->m_pAnimPointsSwitch->setAllChildrenOn();
		else (*it)->m_pAnimPointsSwitch->setAllChildrenOff();
}

osg::Node* raaTile::tile()
{
	return m_pTranslation;
}

osg::MatrixTransform* raaTile::lowestTransform()
{
	return m_pRotation;
}
