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

	rpcTrafficLightState m_eTrafficLightState;
	rpcTrafficLightState m_eLastState;
	bool m_bIsManual;
	const static osg::Vec3f csm_vfPosition;
	osg::MatrixTransform* m_pRotation;

	static void initAsset(std:: string sFile);
	static void finshAsset();

	osg::Node* node();
	void setTransform(float fX, float fY, float fRot, float fScale = csm_fDefaultScale);
	void setLightState(const rpcTrafficLightState eNewState = STOP);
	void setManualState(const rpcTrafficLightState eNewState);
	void turnOffManualState();
	void setRed();
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

	osg::Geode* m_pRed;
	osg::Geode* m_pGreen;
	osg::Geode* m_pAmber;

	const static float csm_fDefaultScale;

	bool m_bIsDetectionPointVisible;

	osg::Switch *m_pDetectionPointSwitch;

	void adjustLightColour();

	void setColour(osg::Material* pGreen, osg::Material* pAmber, osg::Material* pRed);
	void setDetectionPointVisibility(const bool bIsVisible);

	static void materialBuilder(osg::Material* pOn, osg::Material* pOff, osg::Vec3f vMat);
};
