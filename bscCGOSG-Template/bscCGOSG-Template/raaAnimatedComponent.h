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

	osg::MatrixTransform* root();
	osg::Vec3f getDetectionPoint(osg::MatrixTransform* pRoot);
	void toggleDetectionBoxVisibility();
	void handleVehicleReactionToLight(bool bIsGlobalPause);
	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
	void setSpeed(double dSpeed);
	void setManualMultiplier(double dTimeMultiplier);

protected:
	static const osg::Vec3f csm_vfBack;
	static const osg::Vec3f csm_vfDetector_Position;

	osg::MatrixTransform* m_pRoot;
	bool m_bDetectorBoxVisible;
	osg::Switch* m_psDetectorSwitch;
	double m_dTimeMultiplier;
	double m_dSpeed;

	void initDetectionPoint();
	osg::Geode* initGeode();
	double calculateTimeOffset(double dAnimationTime, double dOriginalOffset, double dOriginalMultiplier,
		double dTotalMultiplier);

	void setDetectionBoxVisibility(const bool bIsVisible);
};

