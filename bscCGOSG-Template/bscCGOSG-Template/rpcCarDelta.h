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
	osg::Material *m_pBodyMat;
	osg::Node* m_pGeometry;
	const static std::list<std::string> lsBodyNames;

	virtual osg::Node* makeBaseGeometry() override;
	void paintGeode(osg::Material* pMat, osg::Geode* pGeode);
	osg::Geode* getGeode(std::string psNodeName);
	void materialBuilder(osg::Material* pMat, osg::Vec3f vMat);
};