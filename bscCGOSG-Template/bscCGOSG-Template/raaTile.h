#pragma once
#include <windows.h>
#include <osg/MatrixTransform>
#include <vector>


class raaAnimationPoints;

typedef std::list<class raaTile*> raaTileList;

//the basic road tile class. This is the root of all the tiles and should be extended and developed for generic tile functions
// you may also want to create additional specalisations for more variants of the road tiles.

class raaTile 
{
public:
	raaTile(unsigned int uiName, osg::Node* pModel, raaAnimationPoints* pAP);
	virtual ~raaTile();

	osg::Node* tile();
	osg::MatrixTransform* lowestTransform();

	void translate(float fTranslationX, float fTranslationY);
	void rotate(float fRotation);

	static void toggleAnimPoints();

	osg::Vec3f animPointPosition(unsigned int uiIndex);
	osg::Quat animPointRotation(unsigned int uiIndex);
	unsigned int numAnimPoints();

protected:
	osg::MatrixTransform*m_pTranslation;
	osg::MatrixTransform* m_pRotation;
	osg::Group* m_pLocalRoot;
	osg::Switch* m_pModelSwitch;
	osg::MatrixTransform*m_pOffset;
	osg::Switch* m_pAnimPointsSwitch;
	osg::Billboard* m_pAnimBillboard;
	osg::Group* m_pAnimGroup;
	unsigned int m_uiName;

	raaAnimationPoints *m_pAP;

	static bool sm_bAnimOn;
	static raaTileList sm_lTiles;
};

