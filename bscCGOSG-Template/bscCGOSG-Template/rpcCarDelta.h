#pragma once
#include "stdafx.h"
#include <windows.h>
#include "raaAnimatedComponent.h"

class rpcCarDelta : public raaAnimatedComponent
{
public:
	rpcCarDelta(rpcContextAwareAnimationPath* pAP);

	static void initAsset(const std::string sPath);

	virtual ~rpcCarDelta();
protected:
	const static float csm_fSlowMultiplier;
	const static float csm_fFastMultiplier;
	const static float csm_fCruisingMultiplier;
	const static unsigned int csm_uiNumberOfBodyParts;
	const static unsigned int csm_uiNumberOfWindowParts;
	const static unsigned int csm_uiNumberOfMetallicParts;

	static osg::Node* sm_pGeometry;
	static osg::Material *sm_pWindowMat;
	static osg::Material *sm_pMetallicMat;

	osg::MatrixTransform* m_pRotate;
	osg::Material *m_pBodyMat;

	void goFast() override;
	void goSlow() override;
	void goCruising() override;
	void buildGeometry() override;
	void applyMaterials();

	void paintBody(osg::Geode* pGeode);
	osg::Geode* getGeode(const std::string psNodeName) const;

	static float generateRandomColourValue();
	static void getOrCreateMaterial(osg::Material **pMat, osg::Vec3f vMat);
	static void buildNewMaterial(osg::Material** pMat, osg::Vec3f vAmb, osg::Vec3f vDiff, osg::Vec3f vSpec, float fShininess);
	static void fullMaterialBuilder(osg::Material* pMat, osg::Vec3f vAmb, osg::Vec3f vDiff, osg::Vec3f vSpec, float fShininess);
	static void paintWindow(osg::Geode* pGeode);
	static void paintIndicators(osg::Geode* pFrontRightIndicator, osg::Geode* pFrontLeftIndicator);
	static void paintHeadLights(osg::Geode* pFrontRightInner, osg::Geode* pFrontLeftInner, osg::Geode* pFrontRightOuter, osg::Geode* pFrontLeftOuter);
	static void paintTailLights(osg::Geode* pBackLights);
	static void buildLightMaterial(osg::Material* PMat, osg::Vec3f vMat);
	static void paintMetallicPart(osg::Geode* pGeode);
	static void materialBuilder(osg::Material* pMat, osg::Vec3f vMat);
};