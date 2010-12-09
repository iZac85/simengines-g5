/*
-----------------------------------------------------------------------------
Filename:    g5main.cpp
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
#include "g5main.h"
//-------------------------------------------------------------------------------------
g5main::g5main(void)
	: mKeysReleased(false)
{
	mCEGUI = new OgreCEGUI();
}
//-------------------------------------------------------------------------------------
g5main::~g5main(void)
{
	delete mCEGUI;
}
//-------------------------------------------------------------------------------------
void g5main::createScene(void)
{
	mCEGUI->createScene();
	//mMapCreate = new MapCreate(mSceneMgr);// fel st�lle
}
//-------------------------------------------------------------------------------------
void g5main::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
//-------------------------------------------------------------------------------------
void g5main::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);
	//mCEGUI->createFrameListener();
}
//-------------------------------------------------------------------------------------
bool g5main::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(!mKeysReleased)
	{
		if(mWindow->isClosed()) return false;

		if(mCEGUI->mShutDown) return false;

		if(mCEGUI->mLaunch) launch();
		//Need to capture/update each device
		mKeyboard->capture();
		mMouse->capture();

		return true;
		//mCEGUI->frameRenderingQueued(evt);
		//return true;
	}
	// AI
	//if (mCEGUI->extensionSettings.aiSettings == 1)
		//mAnimation->UpdateAnimation(evt, mSceneMgr, mCamera);

	mBulletWorld->mWorld->stepSimulation(evt.timeSinceLastFrame);

	return BaseApplication::frameRenderingQueued(evt);
}
//-------------------------------------------------------------------------------------
bool g5main::keyPressed( const OIS::KeyEvent &arg )
{
	if(!mKeysReleased)
	{
		mCEGUI->keyPressed(arg);
		if (arg.key == OIS::KC_ESCAPE)
		{
			mCEGUI->mShutDown = true;
		}
		return true;
	}
	return BaseApplication::keyPressed(arg);
}
//-------------------------------------------------------------------------------------
bool g5main::keyReleased( const OIS::KeyEvent &arg )
{
	if(!mKeysReleased)
	{
		mCEGUI->keyReleased(arg);
		return true;
	}
	if (arg.key == OIS::KC_B){//&& mTimeUntilNextToggle <=0){

		// starting position of the box
		Ogre::Vector3 position = (mCamera->getDerivedPosition() + mCamera->getDerivedDirection().normalisedCopy() * 10);
		Ogre::Vector3 speed = mCamera->getDerivedDirection().normalisedCopy() * 9.0f;


		OBBox *box = new OBBox(mSceneMgr, mBulletWorld->mWorld, position, speed, mBulletWorld->mNumEntitiesInstanced, "cube.mesh");

		mBulletWorld->mNumEntitiesInstanced++;				
		//mTimeUntilNextToggle = 0.5;

		// push the created objects to the dequese
		mBulletWorld->mShapes.push_back(box->sceneBoxShape);
		mBulletWorld->mBodies.push_back(box->defaultBody);				
		//mTimeUntilNextToggle = 0.5;

		return true;
	}
	return BaseApplication::keyReleased(arg);
}
//-------------------------------------------------------------------------------------
bool g5main::mouseMoved( const OIS::MouseEvent &arg )
{
	if(!mKeysReleased)
	{
		mCEGUI->mouseMoved(arg);
		return true;
	}
	//return true;
	return BaseApplication::mouseMoved(arg);
}
//-------------------------------------------------------------------------------------
bool g5main::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!mKeysReleased)
	{
		mCEGUI->mousePressed(arg,id);
		return true;
	}
	//return true;
	return BaseApplication::mousePressed(arg,id);
}
//-------------------------------------------------------------------------------------
bool g5main::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if(!mKeysReleased)
	{
		mCEGUI->mouseReleased(arg,id);
		return true;
	}
	//return true;
	return BaseApplication::mouseReleased(arg,id);
}
//-------------------------------------------------------------------------------------
bool g5main::launch()
{
	/*Ogre::Entity* ogreHead1 = mSceneMgr->createEntity("Head1","ogrehead.mesh");

	Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(ogreHead1);

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);
	*/

	mCEGUI->mLaunch = false;
	mKeysReleased = true;

	BaseApplication::createFrameListener();
	// Set up camera
	mCamera->setPosition(30, 50, 200);
	mCamera->lookAt(25, 0, 25);
	mMapCreate = new MapCreate(mSceneMgr, mCEGUI->extensionSettings.threeDSettingsArenaSizeX, mCEGUI->extensionSettings.threeDSettingsArenaSizeY, 12, 12, mCEGUI->extensionSettings.threeDsettingsMaxRoomSize, mCEGUI->extensionSettings.threeDsettingsDoorCnt, mCEGUI->extensionSettings.threeDsettingsFurnitureEn);


	// AI
	//if (mCEGUI->extensionSettings.aiSettings == 1)
		//mAnimation = new Animation(mMapCreate->map, mSceneMgr, mCamera);

	//Create the Physics world
	mBulletWorld = new BulletInitWorld(mSceneMgr,
									   mMapCreate->returnFloorNode(),
									   mMapCreate->returnWallNodeVec(),
									   mCEGUI->extensionSettings.threeDSettingsArenaSizeX,
								       mCEGUI->extensionSettings.threeDSettingsArenaSizeY,
									   mCEGUI->extensionSettings.threeDSettingsArenaSizeX);

	delete mMapCreate;
	return true;
}
//-------------------------------------------------------------------------------------
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        g5main app;
		//OgreBulletProg app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif