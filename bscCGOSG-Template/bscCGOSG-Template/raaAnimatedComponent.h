#pragma once

#include <windows.h>
#include <osg/MatrixTransform>
#include <osg/Switch>

#include "rpcDetectionBox.h"
#include "raaTrafficLightUnit.h"
#include "rpcDetectable.h"
#include "rpcContextAwareAnimationPath.h"

// a basic class as a start for the animated objects. This has a basic SG sub-tree containing a placeholder for the actual model you might use
// It is derived from the animation calbak to recieve a anmiation path
typedef std::list<class raaAnimatedComponent*> rpcVehicles;

class raaAnimatedComponent: public osg::AnimationPathCallback, public rpcDetectable
{
public:
	raaAnimatedComponent(rpcContextAwareAnimationPath* pAP);

	rpcDetectionBox* m_pLightDetector;
	rpcDetectionBox* m_pVehicleDetector;
	raaTrafficLightUnit *m_pLightDetected;
	raaAnimatedComponent *m_pVehicleDetected;

	static void setManualMultiplier(float fTimeMultiplier);

	osg::MatrixTransform* root() const;
	void toggleDetectionBoxVisibility();
	void reactToLightInSights();
	void setSpeed(float fSpeed);
	void setFinalAnimationPathPoint();
	void loadNewPath();
	void setMultiplier();
	void setPause(const bool bPause);
	bool canSee(rpcDetectable* pDetectable) const;

	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
	osg::Vec3f getDetectionPointRelativeTo(osg::Node* pRoot) override;

	virtual ~raaAnimatedComponent();

protected:
	static const osg::Vec3f csm_vfLightDetectorPosition;
	static const osg::Vec3f csm_vfVehicleDetectorPosition;

	static osg::Vec3f sm_vfBackDefault;
	static float sm_fTimeMultiplier;

	osg::MatrixTransform* m_pRoot;
	osg::Switch* m_psDetectorSwitch;
	osg::Vec3f *m_vfDetectionPoint;
	bool m_bDetectorBoxVisible;
	bool m_bPaused;
	float m_fSpeed;
	float m_fPrePauseSpeed;
	double m_dAnimationTimeBeforePause;
	unsigned int m_uiLastTileInAnimation;
	unsigned int m_uiLastAnimationPointInAnimation;
	rpcContextAwareAnimationPath *m_pAP;

	static osg::Geode* makeGeode();

	double calculateTimeOffset(const double dTotalNewMultiplier) const;
	void initDetectionPoint(const osg::Vec3f vPos = sm_vfBackDefault) const;
	void setDetectionBoxVisibility(const bool bIsVisible) const;
	void checkForVehicles();

	virtual void goFast() = 0;
	virtual void goSlow() = 0;
	virtual void goCruising() = 0;
	virtual void buildGeometry() = 0;
};

