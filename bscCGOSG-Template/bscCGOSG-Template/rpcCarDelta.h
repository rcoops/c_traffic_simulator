#pragma once
#include "stdafx.h"
#include <windows.h>
#include "raaAnimatedComponent.h"

class rpcCarDelta : public raaAnimatedComponent
{
public:
	rpcCarDelta(rpcContextAwareAnimationPath* pAP);

	virtual ~rpcCarDelta();
protected:
	const static float csm_fSlowMultiplier;
	const static float csm_fFastMultiplier;
	const static float csm_fCruisingMultiplier;
	const static unsigned int csm_uiNumberOfBodyParts;
	const static unsigned int csm_uiNumberOfWindowParts;
	const static unsigned int csm_uiNumberOfMetallicParts;

	
	static osg::Material *sm_pWindowMat;
	static osg::Material *sm_pMetallicMat;

	osg::Node* m_pGeometry;
	osg::Material *m_pBodyMat;

	void goFast() override;
	void goSlow() override;
	void goCruising() override;

	osg::Node* makeBaseGeometry() override;
	void paintBody(osg::Geode* pGeode);
	static float generateRandomColourValue();
	void getOrCreateMaterial(osg::Material **pMat, osg::Vec3f vMat);
	void buildNewMaterial(osg::Material** pMat, osg::Vec3f vAmb, osg::Vec3f vDiff, osg::Vec3f vSpec, float fShininess);
	void fullMaterialBuilder(osg::Material* pMat, osg::Vec3f vAmb, osg::Vec3f vDiff, osg::Vec3f vSpec, float fShininess);
	osg::Geode* getGeode(const std::string psNodeName) const;
	void paintWindow(osg::Geode* pGeode);
	void paintIndicators(osg::Geode* pFrontRightIndicator, osg::Geode* pFrontLeftIndicator);
	void paintHeadLights(osg::Geode* pFrontRightInner, osg::Geode* pFrontLeftInner, osg::Geode* pFrontRightOuter, osg::Geode* pFrontLeftOuter);
	void paintTailLights(osg::Geode* pBackLights);
	void buildLightMaterial(osg::Material* PMat, osg::Vec3f vMat);
	void paintTest(osg::Geode* pGeode);
	void paintMetallicPart(osg::Geode* pGeode);
	void materialBuilder(osg::Material* pMat, osg::Vec3f vMat);
};