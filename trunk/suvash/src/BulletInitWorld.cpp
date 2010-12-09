#include "BulletInitWorld.h"


BulletInitWorld::BulletInitWorld(Ogre::SceneManager* mSceneMgr, 
								 Ogre::SceneNode* ogreFloorNode,
								 Ogre::Real dimx,
								 Ogre::Real dimy,
								 Ogre::Real dimz )
{
	this->ogreSceneMgr = mSceneMgr;
	this->bulletWorldX = dimx;
	this->bulletWorldY = dimy;
	this->bulletWorldZ = dimz;

	this->initWorld();
	this->debugBoxBullet(true);

	this->addFloor(ogreFloorNode);
}


BulletInitWorld::~BulletInitWorld(void)
{

}

void BulletInitWorld::initWorld()
{
	// Initialize a box count variable
	mNumEntitiesInstanced = 0;

	//Define some bulletworld bounds for now
	Ogre::Vector3 minBound = Ogre::Vector3(-0.5*bulletWorldX, -0.5*bulletWorldY, -0.5*bulletWorldZ);
	Ogre::Vector3 maxBound = Ogre::Vector3( 0.5*bulletWorldX,  0.5*bulletWorldY,  0.5*bulletWorldZ);


	//Start Bullet
	gravityVector = Ogre::Vector3(0,-9.81,0);
	bounds = Ogre::AxisAlignedBox(minBound, maxBound);
	mWorld = new OgreBulletDynamics::DynamicsWorld(ogreSceneMgr, bounds, gravityVector);

}

void BulletInitWorld::debugBoxBullet(bool debugFlag)
{
	if(debugFlag)
	{
		// add Debug info display tool
		debugDrawer = new OgreBulletCollisions::DebugDrawer();
		debugDrawer->setDrawWireframe(true);	// we want to see the Bullet containers

		mWorld->setDebugDrawer(debugDrawer);
		mWorld->setShowDebugShapes(true);		// enable it if you want to see the Bullet containers

		//Create a node and attach it to the Ogre Scene
		Ogre::SceneNode *node = ogreSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
		node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));
	}
}

void BulletInitWorld::addFloor(Ogre::SceneNode* ogreFloorNode)
{
	Ogre::AxisAlignedBox floorBoundingB = ogreFloorNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 floor1SizeB = floorBoundingB.getSize();
	//floorSizeB /= 2.0f;
	//floorSizeB *= 0.95f;
	
	Ogre::Vector3 floorSizeB(25, 2, 25);

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

void BulletInitWorld::run()
{
}