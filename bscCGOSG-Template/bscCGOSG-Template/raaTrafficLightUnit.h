#pragma once

#include <windows.h>
#include <osg/Node>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Geode>

#include <list>

typedef std::list<class raaTrafficLightUnit*>raaTrafficLights;

// a basic traffic light class. You will need to extend and develop this to operate the lights and develop junctions and controls for the animation
class raaTrafficLightUnit: public osg::Node
{
public:
	raaTrafficLightUnit();
	virtual ~raaTrafficLightUnit();

	enum rpcTrafficLightState
	{
		OFF, GO, STOP, SLOW, READY
	};

	raaTrafficLightUnit::rpcTrafficLightState eTrafficLightState;
	osg::Vec3f m_vfPosition;

	static void initAsset(std:: string sFile);
	static void finshAsset();

	osg::Node* node();
	void setTransform(float fX, float fY, float fRot, float fScale = raaTrafficLightUnit::csm_fDefaultScale);
	void setLightState(raaTrafficLightUnit::rpcTrafficLightState state = STOP);
	void toggleDetectionPointVisibility();

protected:
	static osg::Node* sm_pAsset;
	static osg::Material* sm_pRedOn;
	static osg::Material* sm_pRedOff;
	static osg::Material* sm_pAmberOn;
	static osg::Material* sm_pAmberOff;
	static osg::Material* sm_pGreenOn;
	static osg::Material* sm_pGreenOff;

	osg::MatrixTransform* m_pTranslation;
	osg::MatrixTransform* m_pScale;
	osg::MatrixTransform* m_pRotation;

	osg::Geode* m_pRed;
	osg::Geode* m_pGreen;
	osg::Geode* m_pAmber;

	static raaTrafficLights sm_lLights;

	const static float csm_fDefaultScale;

	bool m_bIsDetectionPointVisible;

	osg::Switch *m_pDetectionPointSwitch;

	void adjustLightColour();

	void setColour(osg::Material* pGreen, osg::Material* pAmber, osg::Material* pRed);

	static void materialBuilder(osg::Material* pOn, osg::Material* pOff, osg::Vec3f vMat);
};
