#include "OgreBulletProg.h"
#include "OBBox.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>



//-------------------------------------------------------------------------------------
OgreBulletProg::OgreBulletProg(void) : mRoot(0), mPluginsCfg(Ogre::StringUtil::BLANK)
{

}
//-------------------------------------------------------------------------------------
OgreBulletProg::~OgreBulletProg(void)
{
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}
//-------------------------------------------------------------------------------------
void OgreBulletProg::loadResources(void)
{
#ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif

    // construct Ogre::Root
    mRoot = new Ogre::Root(mPluginsCfg);

	// setup resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
 
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
bool OgreBulletProg::configureDialogue(void)
{
	// configure
	// Show the configuration dialog and initialise the system
	if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
	{
		return false;
	}

	mWindow = mRoot->initialise(true, "Cool OgreBullet Project");

}
 //-------------------------------------------------------------------------------------
void OgreBulletProg::initManagers(void)
{
	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

}
//-------------------------------------------------------------------------------------
void OgreBulletProg::setupCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
 
	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,18,70));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

	mRotX = 0; mRotY = 0; mTranslateVector = Ogre::Vector3(0,0,0);
}
//-------------------------------------------------------------------------------------
void OgreBulletProg::setupViewport(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
 
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
    Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

}
//-------------------------------------------------------------------------------------
void OgreBulletProg::setupLights(void)
{
	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
 
	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);
}
//-------------------------------------------------------------------------------------
void OgreBulletProg::initBullet(void)
{
	// Initialise a box count variable
	mNumEntitiesInstanced = 0;
	
	//Define some bulletworld bounds for now
	Ogre::Vector3 minBound = Ogre::Vector3(-10000,-10000,-10000);
	Ogre::Vector3 maxBound = Ogre::Vector3( 10000, 10000, 10000);

	//Start Bullet
	gravityVector = Ogre::Vector3(0,-9.81,0);
	bounds = Ogre::AxisAlignedBox(minBound, maxBound);
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
}
//-------------------------------------------------------------------------------------
void OgreBulletProg::debugBoxBullet(bool debugFlag)
{
	if(debugFlag)
	{
		// add Debug info display tool
 		debugDrawer = new OgreBulletCollisions::DebugDrawer();
 		debugDrawer->setDrawWireframe(true);	// we want to see the Bullet containers

		mWorld->setDebugDrawer(debugDrawer);
 		mWorld->setShowDebugShapes(true);		// enable it if you want to see the Bullet containers

		//Create a node and attach it to the Ogre Scene
		Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
 		node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));
	}
}
//-------------------------------------------------------------------------------------
void OgreBulletProg::addFloor(void)
{
	// Define a floor plane mesh
 	Ogre::Entity *ent;
    Ogre::Plane p;
    p.normal = Ogre::Vector3(0,1,0);
	p.d = 0;
    
	Ogre::MeshManager::getSingleton().createPlane("FloorPlane", 
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
                                            p, 200000, 200000, 20, 20, true, 1, 9000, 9000, 
                                            Ogre::Vector3::UNIT_Z);
    // Create an entity (the floor)
	ent = mSceneMgr->createEntity("floor", "FloorPlane");
 	ent->setMaterialName("Examples/BumpyMetal");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

	// add collision detection to it
 	OgreBulletCollisions::CollisionShape *Shape;
 	Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,1,0), 0); // (normal vector, distance)

	// a body is needed for the shape
 	OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody("BasePlane", mWorld);
 	defaultPlaneBody->setStaticShape(Shape, 0.1, 0.8);// (shape, restitution, friction)

	// push the created objects to the deques
 	mShapes.push_back(Shape);
 	mBodies.push_back(defaultPlaneBody);

}
//-------------------------------------------------------------------------------------
void OgreBulletProg::initOIS(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
 
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
	mInputManager = OIS::InputManager::createInputSystem( pl );
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));

	mTimeUntilNextToggle = 0;
}
//-------------------------------------------------------------------------------------
void OgreBulletProg::renderAndListen(void)
{
	//Set initial mouse clipping size
	windowResized(mWindow);
 
	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
	
	mRoot->addFrameListener(this);
	mRoot->startRendering();

}
//-------------------------------------------------------------------------------------
void OgreBulletProg::updateCamera(void)
{
	// Make all the changes to the camera
	mCamera->moveRelative(mTranslateVector);

}
//-------------------------------------------------------------------------------------
bool OgreBulletProg::go(void)
{
	//Load the resources
	this->loadResources();

	//Manage the initial configure dialogue
	this->configureDialogue();

	//Initialise Managers/Mipmaps/Resources
	this->initManagers();

	//Setup the cameras
	this->setupCamera();

	//Setup the viewport
	this->setupViewport();

	//Setup basic lighting
	this->setupLights();

	//Initialise BulletWorld
	this->initBullet();

	//Debug bullet AABBs
	this->debugBoxBullet(true);

	//Add a Floor to the scene (physical and bullet)
	this->addFloor();

	//Initialise OIS for keyb/mouse input
	this->initOIS();

	//Set initial mouseclipsize, register self listener and render then listen
	this->renderAndListen();
    
	return true;
}

//Adjust mouse clipping area
void OgreBulletProg::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
//Unattach OIS before window shutdown (very important under Linux)
void OgreBulletProg::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
 
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

bool OgreBulletProg::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;
	
   //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

	if( !mMouse->buffered() || !mKeyboard->buffered() )
	{
		// one of the input modes is immediate, so setup what is needed for immediate movement
		if (mTimeUntilNextToggle >= 0)
			mTimeUntilNextToggle -= evt.timeSinceLastFrame;

		this->updateCamera();

		mTranslateVector = Ogre::Vector3::ZERO;

	}

	Ogre::Real moveScale = 1;
	Ogre::Degree mRotScale = Ogre::Degree(1);

	if(mKeyboard->isKeyDown(OIS::KC_A))
		mTranslateVector.x = -moveScale;	// Move camera left

	if(mKeyboard->isKeyDown(OIS::KC_D))
		mTranslateVector.x = moveScale;	// Move camera RIGHT

	if(mKeyboard->isKeyDown(OIS::KC_W))
		mTranslateVector.z = -moveScale;	// Move camera forward

	if(mKeyboard->isKeyDown(OIS::KC_S))
		mTranslateVector.z = moveScale;	// Move camera backward

	if(mKeyboard->isKeyDown(OIS::KC_UP))
		mTranslateVector.y = moveScale;	// Move camera up

	if(mKeyboard->isKeyDown(OIS::KC_DOWN))
		mTranslateVector.y = -moveScale;	// Move camera down

	if(mKeyboard->isKeyDown(OIS::KC_RIGHT))
	mCamera->yaw(-mRotScale);

	if(mKeyboard->isKeyDown(OIS::KC_LEFT))
		mCamera->yaw(mRotScale);

 
    if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)){
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Pressed Esc ***");
        return false;
	}

	if(mKeyboard->isKeyDown(OIS::KC_B)&& mTimeUntilNextToggle <=0){
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Pressed B ***");
		
		// starting position of the box
 		Ogre::Vector3 position = (mCamera->getDerivedPosition() + mCamera->getDerivedDirection().normalisedCopy() * 10);
		Ogre::Vector3 speed = mCamera->getDerivedDirection().normalisedCopy() * 9.0f;
 		

		OBBox *box = new OBBox(mSceneMgr, mWorld, position, speed, mNumEntitiesInstanced, "cube.mesh");

        mNumEntitiesInstanced++;				
		mTimeUntilNextToggle = 0.5;

 		// push the created objects to the dequese
 		mShapes.push_back(box->sceneBoxShape);
 		mBodies.push_back(box->defaultBody);				
 		//mTimeUntilNextToggle = 0.5;
		
		return true;
	}
	
	//Step the simulation
	mWorld->stepSimulation(evt.timeSinceLastFrame);

    return true;
}
 
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
        OgreBulletProg app;
 
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