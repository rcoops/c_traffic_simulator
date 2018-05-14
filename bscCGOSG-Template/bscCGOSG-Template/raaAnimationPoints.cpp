#include "stdafx.h"

#include <windows.h>
#include <sstream>
#include <gl/gl.h>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Billboard>
#include <osg/Material>
#include <osgText/Text>

#include "raaAnimationPoints.h"

osg::Geode *raaAnimationPoints::m_pAnimGeode=0;

raaAnimationPoints::raaAnimationPoints()
{
	if (!m_pAnimGeode)
	{
		m_pAnimGeode = new osg::Geode();
		osg::Cone *pCone = new osg::Cone(osg::Vec3f(-10.0f, 0.0f, 0.0f), 7.0f, 40.0f);
		pCone->setRotation(osg::Quat(osg::DegreesToRadians(90.0f), osg::Vec3f(0.0f, 1.0f, 0.0f)));

		osg::ShapeDrawable *pSD = new osg::ShapeDrawable(pCone);

		osg::Material *pMat = new osg::Material();
		pMat->setAmbient(osg::Material::FRONT, osg::Vec4f(0.2f, 0.0f, 0.0f, 1.0f));
		pMat->setDiffuse(osg::Material::FRONT, osg::Vec4f(0.8f, 0.1f, 0.1f, 1.0f));
		pMat->setSpecular(osg::Material::FRONT, osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
		pMat->setShininess(osg::Material::FRONT, 180.0f);
		m_pAnimGeode->addDrawable(pSD);

		m_pAnimGeode->getOrCreateStateSet()->setAttributeAndModes(pMat, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
		pSD->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
	}

	m_pSG = new osg::Group();
	m_pSG->ref();

	m_pBillboard = new osg::Billboard();
	m_pSG->addChild(m_pBillboard);
	m_pBillboard->setMode(osg::Billboard::POINT_ROT_EYE);
	m_pBillboard->setNormal(osg::Vec3f(0.0f, 0.0f, 1.0f));
}

osg::Group* raaAnimationPoints::sg()
{
	return m_pSG;
}

unsigned raaAnimationPoints::addPoint(osg::Vec3f vPos, osg::Quat qRot)
{
	m_vPoints.push_back(vPos);
	m_vRots.push_back(qRot);
	unsigned int uiIndex = m_vPoints.size() - 1;

	osg::MatrixTransform *pMT = new osg::MatrixTransform();
	pMT->setMatrix(osg::Matrixf::rotate(qRot)*osg::Matrixf::translate(vPos));
	pMT->addChild(m_pAnimGeode);
	m_pSG->addChild(pMT);
	
	osgText::Text *pText = new osgText::Text();
	pText->setCharacterSize(10.0f);
	std::stringstream ss;
	ss << uiIndex;

	pText->setText(ss.str());
	pText->setAlignment(osgText::Text::AlignmentType::CENTER_CENTER);

	m_pBillboard->addDrawable(pText);
	m_pBillboard->setPosition(uiIndex, vPos+osg::Vec3f(0.0f,0.0f, 20.0f));

	return uiIndex;
}

osg::Vec3f raaAnimationPoints::pos(unsigned uiIndex)
{
	if (uiIndex < m_vPoints.size()) return m_vPoints[uiIndex];
	return osg::Vec3f(0.0f, 0.0f, 0.0f);
}

osg::Quat raaAnimationPoints::rot(unsigned uiIndex)
{
	if (uiIndex < m_vRots.size()) return m_vRots[uiIndex];
	return osg::Quat(0.0f, osg::Vec3f(1.0f, 0.0f, 0.0f));
}

unsigned raaAnimationPoints::numPoints()
{
	return m_vPoints.size();
}

raaAnimationPoints::~raaAnimationPoints()
{
	m_pSG->unref();
}
