// bscCGOSG-Template.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <iostream>
//#include <string>
//#include <conio.h>
#include <windows.h>
#include "raaAnimationPathBuilder.h"
//#include <gl/gl.h>
//#include <mmsystem.h>
//#include <osgDB/WriteFile>
//#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
//#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
//#include <osgGA/GUIEventHandler>
#include <osg/Drawable>
//#include <osg/ShapeDrawable>
//#include <osgUtil/UpdateVisitor>
#include <osgViewer/ViewerEventHandlers>
//#include <osg/Material>
#include <osg/AnimationPath>
#include "raaAnimatedComponent.h"
#include "raaPrinter.h"
#include "raaRoadSet.h"
#include "raaTrafficLightUnit.h"
#include "raaJunctionController.h"
#include "raaInputController.h"


//#include <osgDB/ReadFile>


osg::Node *g_pModel = 0; // holds the tile model
osg::Group *g_pRoot = 0; // root of the sg

raaAnimatedComponent* g_pAnim = 0;

unsigned int g_uiCPs = 0;
double g_dLastTime = 0.0;
osg::Vec3f g_vPastPos;

// define a SPEED rather than calculating for in between each set of two points

int main(int argc, char** argv)
{
	osgViewer::Viewer viewer;

	// the root of the scene - use for rendering
	g_pRoot = new osg::Group();
	g_pRoot->ref();

	// parse command line options
	std::string sTrafficLightAsset = "../../Data/raaTrafficLight.OSGB";
	std::string sRoadMap = "../../Data/roads2.txt";
	std::string sRoadAsset = "../../Data/RoadSet.OSGB";
	std::string sAnimPath = "../../Data/animPath.txt";

	for (int i = 0; i < argc; i++)
	{
		if (std::string(argv[i]) == "-ta") sTrafficLightAsset = (argv[++i]);
		else if (std::string(argv[i]) == "-rm") sRoadMap = (argv[++i]);
		else if (std::string(argv[i]) == "-ra") sRoadAsset = (argv[++i]);
		else if (std::string(argv[i]) == "-ap") sAnimPath = (argv[++i]);
	}

	// loads the tile assets for laying out the tiles. 
	raaTrafficLightUnit::initAsset(sTrafficLightAsset);
	
	// loads the road description and populates it
	raaRoadSet::start(sRoadAsset, sRoadMap);

	/* Example of manually network - use instead of file loading if you wish
	raaRoadSet::instance()->addCurve("c", 1.0f, 0.0f, 0.0f);
	raaRoadSet::instance()->addStraight("s", 0.0f, 1.0f, 0.0f);
	raaRoadSet::instance()->addXJunction("x", 0.0f, -1.0f, 0.0f);
	raaRoadSet::instance()->addTJunction("t", -1.0f, 0.0f, 0.0f);
	*/

	g_pRoot->addChild(raaRoadSet::instance()->sg()); // adds the road description to the SG

	// building an animation path
	osg::AnimationPath *pAP = new osg::AnimationPath();
	raaAnimationPathBuilder apBuilder(pAP, g_pRoot);

	/* Example of manually adding control points - use instead of file loading if you wish
	apBuilder.addControlPoint(1, 2);
	apBuilder.addControlPoint(1, 0);
	apBuilder.addControlPoint(2, 2);
	....
	apBuilder.addControlPoint(0, 10);
*/

	//	apBuilder.save("../../data/animPointsOut.txt");
	apBuilder.load(sAnimPath); // loading the animation path from file

	// create an animated component and add to the scene with the animation path included
	g_pAnim = new raaAnimatedComponent(pAP);
	g_pRoot->addChild(g_pAnim->root());

	// add the vehcile to the junction controller for traffic light control
	raaJunctionController::addVehicle(g_pAnim);

	// setup stuff
	osg::GraphicsContext::Traits *pTraits = new osg::GraphicsContext::Traits();
	pTraits->x = 20;
	pTraits->y = 20;
	pTraits->width = 600;
	pTraits->height = 480;
	pTraits->windowDecoration = true;
	pTraits->doubleBuffer = true;
	pTraits->sharedContext = 0;

	osg::GraphicsContext *pGC = osg::GraphicsContext::createGraphicsContext(pTraits);
	osgGA::KeySwitchMatrixManipulator* pKeyswitchManipulator = new osgGA::KeySwitchMatrixManipulator();
	pKeyswitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
	pKeyswitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator());
	pKeyswitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator());
	viewer.setCameraManipulator(pKeyswitchManipulator);
	osg::Camera *pCamera = viewer.getCamera();
	pCamera->setGraphicsContext(pGC);
	pCamera->setViewport(new osg::Viewport(0, 0, pTraits->width, pTraits->height));

	// add own event handler - thish currently switches on an off the animation points
	viewer.addEventHandler(new raaInputController());

	// add the state manipulator
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

	// add the thread model handler
	viewer.addEventHandler(new osgViewer::ThreadingHandler);

	// add the window size toggle handler
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// add the stats handler
	viewer.addEventHandler(new osgViewer::StatsHandler);

	// add the record camera path handler
	viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);

	// add the LOD Scale handler
	viewer.addEventHandler(new osgViewer::LODScaleHandler);

	// add the screen capture handler
	viewer.addEventHandler(new osgViewer::ScreenCaptureHandler);

	// set the scene to render
	viewer.setSceneData(g_pRoot);

	viewer.realize();

	return viewer.run();
}


