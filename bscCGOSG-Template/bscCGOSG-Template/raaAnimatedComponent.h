#pragma once

#include <windows.h>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osg/Switch>

#include "rpcDetectionBox.h"
#include "raaTrafficLightUnit.h"
#include "rpcDetectable.h"

// a basic class as a start for the animated objects. This has a basic SG sub-tree containing a placeholder for the actual model you might use
// It is derived from the animation calbak to recieve a anmiation path
typedef std::list<class raaAnimatedComponent*> rpcVehicles;

class raaAnimatedComponent: public osg::AnimationPathCallback, public rpcDetectable
{
public:
	raaAnimatedComponent(osg::AnimationPath* pAP);

	rpcDetectionBox* m_pLightDetector;
	rpcDetectionBox* m_pVehicleDetector;
	raaTrafficLightUnit *m_pLightDetected;
	raaAnimatedComponent *m_pVehicleDetected;

	static void setManualMultiplier(float fTimeMultiplier);

	osg::MatrixTransform* root() const;
	void toggleDetectionBoxVisibility();
	void reactToLightInSights();
	void setSpeed(float fSpeed);
	void setFinalAnimationPathPoint(osg::AnimationPath *pAP);
	bool canSee(rpcDetectable* pDetectable, osg::Group* pRoot) const;
	void setMultiplier();
	void checkForNewPath();

	void operator()(osg::Node* node, osg::NodeVisitor* nv) override;
	osg::Vec3f getDetectionPointRelativeTo(osg::Node* pRoot) override;

	virtual ~raaAnimatedComponent();

protected:
	static const osg::Vec3f csm_vfBack;
	static const osg::Vec3f csm_vfLightDetectorPosition;
	static const osg::Vec3f csm_vfVehicleDetectorPosition;

	static float sm_fTimeMultiplier;

	osg::MatrixTransform* m_pRoot;
	bool m_bDetectorBoxVisible;
	osg::Switch* m_psDetectorSwitch;
	float m_fSpeed;
	unsigned int m_uiLastTileInAnimation;
	unsigned int m_uiLastAnimationPointInAnimation;

	static osg::Geode* makeBaseGeometry();
	static osg::Geode* makeGeode();
	double calculateTimeOffset(const double dTotalNewMultiplier) const;

	void initDetectionPoint() const;
	void setDetectionBoxVisibility(const bool bIsVisible) const;
	void checkForVehicles();
};

