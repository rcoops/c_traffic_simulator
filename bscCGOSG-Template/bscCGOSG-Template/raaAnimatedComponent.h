#pragma once

#include <windows.h>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osg/Switch>

#include "rpcDetectionBox.h"
#include "raaTrafficLightUnit.h"

// a basic class as a start for the animated objects. This has a basic SG sub-tree containing a placeholder for the actual model you might use
// It is derived from the animation calbak to recieve a anmiation path
typedef std::list<class raaAnimatedComponent*> rpcVehicles;

class raaAnimatedComponent: public osg::AnimationPathCallback
{
public:
	raaAnimatedComponent(osg::AnimationPath* pAP);
	virtual ~raaAnimatedComponent();

	rpcDetectionBox* m_pDetectionBox;
	raaTrafficLightUnit *m_pLightDetected;

	osg::MatrixTransform* root() const;
	osg::Vec3f getDetectionPoint(osg::MatrixTransform* pRoot) const;
	void toggleDetectionBoxVisibility();
	void handleVehicleReactionToLight(const bool bIsGlobalPause);
	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
	void setSpeed(float fSpeed);
	void setManualMultiplier(float fTimeMultiplier);
	bool canSee(osg::Vec3f pvfGlobalCoordinates, osg::Group *pRoot) const;

protected:
	static const osg::Vec3f csm_vfBack;
	static const osg::Vec3f csm_vfDetector_Position;

	osg::MatrixTransform* m_pRoot;
	bool m_bDetectorBoxVisible;
	osg::Switch* m_psDetectorSwitch;
	float m_fTimeMultiplier;
	float m_fSpeed;

	void initDetectionPoint() const;
	static osg::Geode* initGeode();
	static double calculateTimeOffset(double dAnimationTime, double dOriginalOffset, double dOriginalMultiplier,
		double dTotalMultiplier);

	void setDetectionBoxVisibility(const bool bIsVisible) const;
};

