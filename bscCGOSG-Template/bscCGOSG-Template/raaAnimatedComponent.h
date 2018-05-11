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
	void toggleDetectionBox();
	void toggleDetectionBoxVisibility();
	bool isDetectionBoxVisible();
	void toggleDetectionBoxes();

protected:
	osg::MatrixTransform* m_pRoot;
	osg::Vec3f m_vfFront;
	static const osg::Vec3f msc_vfDetector_Position;

	bool m_bDetectorBoxVisible;
	osg::Switch* m_psDetectorSwitch;
};

