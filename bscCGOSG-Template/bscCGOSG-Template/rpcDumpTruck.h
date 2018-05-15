#pragma once
#include "raaAnimatedComponent.h"

class rpcDumpTruck : public raaAnimatedComponent
{
public:
	rpcDumpTruck(rpcContextAwareAnimationPath* pAP);

	static void initAsset(const std::string sPath);

	virtual ~rpcDumpTruck();
protected:
	const static float csm_fSlowMultiplier;
	const static float csm_fFastMultiplier;
	const static float csm_fCruisingMultiplier;

	static osg::Node* sm_pGeometry;

	osg::MatrixTransform* m_pTransform;
	
	void goFast() override;
	void goSlow() override;
	void goCruising() override;

	void buildGeometry() override;
};