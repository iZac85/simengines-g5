#include "OgreDemo.h"
#include "AI_Extension.h"


Pather *aiPather;

//-------------------------------------------------------------------------------------
OgreDemo::OgreDemo(void)
{
	aiPather = new Pather();
	aiPather->AIinit();
}
//-------------------------------------------------------------------------------------
OgreDemo::~OgreDemo(void)
{
}
 
//-------------------------------------------------------------------------------------
void OgreDemo::createScene(void)
{
    // Set the default lighting.
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    // Create the entity
    mEntity = mSceneMgr->createEntity("Robot", "robot.mesh");
 
    // Create the scene node
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode", Ogre::Vector3(100.0f, 0.0f, 100.0f));
    mNode->attachObject(mEntity);
	mNode->setScale(0.1f, 0.1f, 0.1f);
	//mNode->pitch(Ogre::Degree(90));

	// Set starting position
	mWalkList.push_back(Ogre::Vector3(100.0f,  0.0f, 100.0f));

    // Create objects so we can see movement
    Ogre::Entity *ent;
    Ogre::SceneNode *node;
 
    ent = mSceneMgr->createEntity("Knot1", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot1Node", Ogre::Vector3(0.0f, 0.0f,  100.0f));
    node->attachObject(ent);
    node->setScale(0.005f, 0.1f, 2.0f);
 
    ent = mSceneMgr->createEntity("Knot2", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot2Node", Ogre::Vector3(200.0f, 0.0f,  100.0f));
    node->attachObject(ent);
    node->setScale(0.005f, 0.1f, 2.0f);
 
    ent = mSceneMgr->createEntity("Knot3", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot3Node", Ogre::Vector3(100.0f, 0.0f, 0.0f));
    node->attachObject(ent);
    node->setScale(2.0f, 0.1f, 0.005f);

	ent = mSceneMgr->createEntity("Knot4", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot4Node", Ogre::Vector3(100.0f, 0.0f, 200.0f));
    node->attachObject(ent);
    node->setScale(2.0f, 0.1f, 0.005f);

	ent = mSceneMgr->createEntity("Knot5", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot5Node", Ogre::Vector3(50.0f, 0.0f, 50.0f));
    node->attachObject(ent);
    node->setScale(0.5f, 0.1f, 0.5f);

	ent = mSceneMgr->createEntity("Knot6", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot6Node", Ogre::Vector3(150.0f, 0.0f, 150.0f));
    node->attachObject(ent);
    node->setScale(0.5f, 0.1f, 0.5f);

	ent = mSceneMgr->createEntity("Knot7", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot7Node", Ogre::Vector3(50.0f, 0.0f, 150.0f));
    node->attachObject(ent);
    node->setScale(0.5f, 0.1f, 0.5f);

	ent = mSceneMgr->createEntity("Knot8", "cube.mesh");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Knot8Node", Ogre::Vector3(150.0f, 0.0f, 50.0f));
    node->attachObject(ent);
    node->setScale(0.5f, 0.1f, 0.5f);

	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;
 
	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 3000.0f, 3000.0f, 10, 10, true, 1, 100.0f, 100.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* planeEnt = mSceneMgr->createEntity("plane", "floor");
	planeEnt->setMaterialName("Examples/GrassFloor");
	planeEnt->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(planeEnt);

	// Set the camera to look at our handiwork
    mCamera->setPosition(100.0f, 5.0f, 100.0f);
	mCamera->lookAt(50,0,50);
}
void OgreDemo::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	mCamera->setPosition(Ogre::Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Ogre::Vector3(0,0,-300));
	mCamera->setNearClipDistance(0.1);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller
}
void OgreDemo::createFrameListener(void){
    BaseApplication::createFrameListener();
    // Set idle animation
    mAnimationState = mEntity->getAnimationState("Idle");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);
    // Set default values for variables
    mWalkSpeed = 10;//100.0f;
//	mWalkSpeed = setSpeed();
    mDirection = Ogre::Vector3::ZERO;
}   
bool OgreDemo::nextLocation(void){

	// PathPlanning and Avoidance
	Ogre::Vector3 rp = mNode->getPosition();	// RobotPosition
	Ogre::Vector3 pp = mCamera->getPosition();  // PlayerPosition
	mCamera->setPosition(pp.x,5.0,pp.z);
	mCameraMan->setTopSpeed(12);

	if (mDestination == rp && mWalkList.empty())
		mWalkList.push_back(aiPather->AIframe(rp.x,rp.z,pp.x, pp.z));
	mDestination = mWalkList.front();  // this gets the front of the deque
	mWalkList.pop_front();             // this removes the front of the deque
	mDirection = mDestination - mNode->getPosition();
	mDistance = mDirection.normalise();
    updateCamera();
	return true;

	
}
void OgreDemo::updateCamera()
{
}
bool OgreDemo::frameRenderingQueued(const Ogre::FrameEvent &evt){
    	if (mDirection == Ogre::Vector3::ZERO) {
		if (nextLocation()) {
			// Set walking animation
			mAnimationState = mEntity->getAnimationState("Walk");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);				
		}//if
	}else{
		Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
		mDistance -= move;
		if (mDistance <= 0.0f){                 
			mNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;				
			// Set animation based on if the robot has another point to walk to. 
			if (!nextLocation()){
				// Set Idle animation                     
				mAnimationState = mEntity->getAnimationState("Idle");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
			}else{
				// Rotation Code will go here later
				Ogre::Vector3 src = mNode->getOrientation() * Ogre::Vector3::UNIT_X;
				if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) {
					mNode->yaw(Ogre::Degree(180));						
				}else{
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					mNode->rotate(quat);
				} // else
			}//else
		}else{
			mNode->translate(mDirection * move);
		} // else
	} // if
	mAnimationState->addTime(evt.timeSinceLastFrame);
	return BaseApplication::frameRenderingQueued(evt);
}

/*
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
        OgreDemo app;
 
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
*/