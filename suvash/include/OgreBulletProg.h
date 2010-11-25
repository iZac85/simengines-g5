#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreWindowEventUtilities.h>

#include <OgreBulletDynamicsRigidBody.h>				 // for OgreBullet
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h> // for static planes
#include <Shapes/OgreBulletCollisionsBoxShape.h>		 // for Boxes


class OgreBulletProg : public Ogre::WindowEventListener, public Ogre::FrameListener
{
public:
    OgreBulletProg(void);
    virtual ~OgreBulletProg(void);
    bool go(void);
private:
	// Member Functions
	void loadResources(void);
	bool configureDialogue(void);
	void initManagers(void);
	void setupCamera(void);
	void setupViewport(void);
	void setupLights(void);

	void initBullet(void);
	void debugBoxBullet(bool);
	void addFloor(void);

	void initOIS(void);

	void renderAndListen(void);

	//Variables --------------
    Ogre::Root* mRoot;
    Ogre::String mPluginsCfg;
	Ogre::String mResourcesCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;

	// OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;

	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	//OgreBullet Stuff
	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

	OgreBulletDynamics::DynamicsWorld *mWorld;	// OgreBullet World
	OgreBulletCollisions::DebugDrawer *debugDrawer;
	Ogre::Vector3 gravityVector;
	Ogre::AxisAlignedBox bounds;

	int mNumEntitiesInstanced;

	//OIS Stuff
	Ogre::Real mTimeUntilNextToggle;

};