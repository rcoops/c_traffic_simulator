#pragma once

#include <windows.h>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osg/Switch>

#include "rpcDetectionBox.h"

// a basic class as a start for the animated objects. This has a basic SG sub-tree containing a placeholder for the actual model you might use
// It is derived from the animation calbak to recieve a anmiation path
typedef std::list<class raaAnimatedComponent*> rpcVehicles;

class raaAnimatedComponent: public osg::AnimationPathCallback
{
public:
	raaAnimatedComponent(osg::AnimationPath* pAP);
	virtual ~raaAnimatedComponent();

	rpcDetectionBox* m_pDetectionBox;

	osg::MatrixTransform* root();
	osg::Vec3f getDetectionPoint(osg::MatrixTransform* pRoot);
	void toggleDetectionBoxVisibility();

protected:
	osg::MatrixTransform* m_pRoot;
	static const osg::Vec3f csm_vfBack;
	void initDetectionPoint();
	osg::Geode* initGeode();
	static const osg::Vec3f csm_vfDetector_Position;

	bool m_bDetectorBoxVisible;
	osg::Switch* m_psDetectorSwitch;

	void setDetectionBoxVisibility(const bool bIsVisible);
};

