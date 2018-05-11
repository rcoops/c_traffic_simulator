#pragma once

#include <windows.h>
#include <osg/Vec3f>
#include <osg/Quat>
#include <osg/Group>
#include <vector>

typedef std::vector<osg::Vec3f>raaPoints;
typedef std::vector<osg::Quat>raaRots;

// class used to draw the animation points and their numbers with the tile number

class raaAnimationPoints
{
public:
	raaAnimationPoints();
	virtual ~raaAnimationPoints();

	unsigned int addPoint(osg::Vec3f vPos, osg::Quat qRot);

	osg::Vec3f pos(unsigned int uiIndex);
	osg::Quat rot(unsigned int uiIndex);
	unsigned int numPoints();
	osg::Group* sg();

protected:
	raaPoints m_vPoints;
	raaRots m_vRots;
	osg::Group *m_pSG;
	osg::Billboard *m_pBillboard;

	static osg::Geode *m_pAnimGeode;
	

};

