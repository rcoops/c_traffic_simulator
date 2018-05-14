#pragma once
#include "stdafx.h"
#include <windows.h>
#include "raaAnimatedComponent.h"

class rpcCarVeryon : public raaAnimatedComponent
{
public:
	rpcCarVeryon(rpcContextAwareAnimationPath* pAP);

	virtual ~rpcCarVeryon();
protected:
	const static float csm_fSlowMultiplier;
	const static float csm_fFastMultiplier;
	const static float csm_fCruisingMultiplier;

	osg::Node *m_pGeometry;

	void goFast() override;
	void goSlow() override;
	void goCruising() override;

	osg::Node* makeBaseGeometry() override;
};