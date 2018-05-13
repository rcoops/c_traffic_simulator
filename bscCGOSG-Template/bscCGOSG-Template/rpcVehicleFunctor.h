#pragma once

#include "raaAnimatedComponent.h"
class rpcVehicleFunctor
{
public:
	rpcVehicleFunctor() {}

	virtual void operator()(raaAnimatedComponent *pVehicle) = 0;

	virtual ~rpcVehicleFunctor() {}
	
};

class rpcVehicleDetectionBoxVisibilityToggleFunctor : public rpcVehicleFunctor
{
public:
	rpcVehicleDetectionBoxVisibilityToggleFunctor() {}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		pVehicle->toggleDetectionBoxVisibility();
	}

	virtual ~rpcVehicleDetectionBoxVisibilityToggleFunctor() {}
};

class rpcPauseVehicles : public rpcVehicleFunctor
{
public:
	rpcPauseVehicles(bool bIsPause)
	{
		m_bIsPause = bIsPause;
	}

	void operator()(raaAnimatedComponent *pVehicle) override
	{
		pVehicle->setPause(m_bIsPause);
	}

	virtual ~rpcPauseVehicles() {};
protected:
	bool m_bIsPause;
};
