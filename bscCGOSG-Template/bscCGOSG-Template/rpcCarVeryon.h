#pragma once
#include "stdafx.h"
#include <windows.h>
#include "raaAnimatedComponent.h"

class rpcCarVeryon : public raaAnimatedComponent
{
public:
	rpcCarVeryon(rpcContextAwareAnimationPath* pAP);

	static void initAsset(const std::string sPath);

	virtual ~rpcCarVeryon();
protected:
	const static float csm_fSlowMultiplier;
	const static float csm_fFastMultiplier;
	const static float csm_fCruisingMultiplier;

	static osg::Node *sm_pGeometry;

	osg::MatrixTransform* m_pTransform;

	void goFast() override;
	void goSlow() override;
	void goCruising() override;

	void buildGeometry() override;
};