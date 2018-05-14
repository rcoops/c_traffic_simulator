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
	virtual osg::Node* makeBaseGeometry() override;
};