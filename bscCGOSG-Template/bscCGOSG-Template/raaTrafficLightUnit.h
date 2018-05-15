#pragma once

#include <windows.h>
#include <osg/Node>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Geode>

#include <list>
#include "rpcDetectable.h"

typedef std::list<class raaTrafficLightUnit*>raaTrafficLights;

// a basic traffic light class. You will need to extend and develop this to operate the lights and develop junctions and controls for the animation
class raaTrafficLightUnit: public osg::Node, public rpcDetectable
{
public:
	raaTrafficLightUnit();

	enum rpcTrafficLightState
	{
		off, go, stop, slow, ready
	};

	const static osg::Vec3f csm_vfDetectionPoint;

	osg::MatrixTransform* m_pRotation;

	rpcTrafficLightState m_eTrafficLightState;
	rpcTrafficLightState m_eLastState;
	bool m_bIsManual;

	static void initAsset(std:: string sFile);
	static void finshAsset();

	Node* node() const;
	void setTransform(float fX, float fY, float fRot, float fScale = csm_fDefaultScale) const;
	void setLightState(const rpcTrafficLightState eNewState = stop);
	void setManualState(const rpcTrafficLightState eNewState);
	void turnOffManualState();;
	void toggleDetectionPointVisibility();
	osg::Vec3f getDetectionPointRelativeTo(Node* pRoot) override;
	virtual ~raaTrafficLightUnit();

protected:
	const static float csm_fDefaultScale;

	static Node* sm_pAsset;
	static osg::Material* sm_pRedOn;
	static osg::Material* sm_pRedOff;
	static osg::Material* sm_pAmberOn;
	static osg::Material* sm_pAmberOff;
	static osg::Material* sm_pGreenOn;
	static osg::Material* sm_pGreenOff;

	osg::MatrixTransform* m_pTranslation;
	osg::MatrixTransform* m_pScale;
	osg::Switch *m_pDetectionPointSwitch;
	osg::Geode* m_pRed;
	osg::Geode* m_pGreen;
	osg::Geode* m_pAmber;

	bool m_bIsDetectionPointVisible;

	void adjustLightColour() const;
	void initDetectionPoint();
	void initLightNodes();
	void setColour(osg::Material* pGreen, osg::Material* pAmber, osg::Material* pRed) const;
	void setDetectionPointVisibility(const bool bIsVisible) const;

	static void materialBuilder(osg::Material* pOn, osg::Material* pOff, osg::Vec3f vMat);
};
