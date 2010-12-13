/*
-----------------------------------------------------------------------------
Filename:    g5main.h
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (August 2010)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#ifndef __g5main_h_
#define __g5main_h_

#include "OgreCEGUI.h"
#include "OgreBulletProg.h"
#include "AI_Extension.h"
#include "MapCreate.h"
#include "BulletInitWorld.h"
#include "OBBox.h"
#include "OBPlayer.h"
//#include "BaseApplication.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


class g5main : public BaseApplication
{
public:
    g5main(void);
    virtual ~g5main(void);

protected:

	OgreCEGUI *mCEGUI;
	MapCreate *mMapCreate;
	Animation *mAnimation;

	Ogre::Camera *mCameraFPV;
	OgreBites::SdkCameraMan *mCameraFPVMan;


	void createScene(void);
	void createCamera(void);
	void clearScene(void);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual void createFrameListener(void);

	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//Ogre Bullet stuff
	BulletInitWorld *mBulletWorld;
	OBPlayer *player;
	bool cameraFPVinUse;


	bool launch();
	bool mGuiActive;
	bool mainMenuActive;
	bool gameFinished;
};

#endif // #ifndef __g5main_h_
