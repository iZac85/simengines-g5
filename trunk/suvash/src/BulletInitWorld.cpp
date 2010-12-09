#include "BulletInitWorld.h"


BulletInitWorld::BulletInitWorld(Ogre::SceneManager* mSceneMgr, 
								 Ogre::SceneNode* ogreFloorNode,
								 std::vector<Ogre::SceneNode*> ogreWallNodeVec,
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
	this->addWalls(ogreWallNodeVec);
}


BulletInitWorld::~BulletInitWorld(void)
{

}

void BulletInitWorld::initWorld()
{
	// Initialize a box count variable
	mNumEntitiesInstanced = 0;

	//Define some bulletworld bounds for now
	Ogre::Vector3 minBound = Ogre::Vector3(0, 0, 0);
	Ogre::Vector3 maxBound = Ogre::Vector3( bulletWorldX, bulletWorldY, bulletWorldZ);


	//Start Bullet
	gravityVector = Ogre::Vector3(0,-9.81,0);
	bounds = Ogre::AxisAlignedBox(minBound, maxBound);
	mWorld = new OgreBulletDynamics::DynamicsWorld(ogreSceneMgr, bounds, gravityVector);
	//mWorld->

}

void BulletInitWorld::debugBoxBullet(bool debugFlag)
{
	if(debugFlag)
	{
		// add Debug info display tool
		debugDrawer = new OgreBulletCollisions::DebugDrawer();
		debugDrawer->setDrawWireframe(true);	// we want to see the Bullet containers

		mWorld->setDebugDrawer(debugDrawer);
		mWorld->setShowDebugShapes(false);		// enable it if you want to see the Bullet containers

		//Create a node and attach it to the Ogre Scene
		Ogre::SceneNode *node = ogreSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
		node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));
	}
}

void BulletInitWorld::addFloor(Ogre::SceneNode* ogreFloorNode)
{
	Ogre::AxisAlignedBox floorBoundingB = ogreFloorNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 floorSizeB = floorBoundingB.getSize();
	// Don't ask me why i do the following
	floorSizeB /= 2.0f;
	floorSizeB *= 0.98f;

	floorSizeB.x *= ogreFloorNode->getScale().x;
	floorSizeB.y *= ogreFloorNode->getScale().y;
	floorSizeB.z *= ogreFloorNode->getScale().z;
	
	ogreFloorNode->showBoundingBox(true);
	
	// add collision detection to it
	OgreBulletCollisions::CollisionShape *floorShape = new OgreBulletCollisions::BoxCollisionShape(floorSizeB);
	//Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,1,0), 0); // (normal vector, distance)

	// a body is needed for the shape
	OgreBulletDynamics::RigidBody *defaultFloorBody = new OgreBulletDynamics::RigidBody("BasePlane", mWorld);
	defaultFloorBody->setStaticShape(floorShape, 0.1, 0.8,ogreFloorNode->getPosition());// (shape, restitution, friction)

	// push the created objects to the deques
	mShapes.push_back(floorShape);
	mBodies.push_back(defaultFloorBody);

}

void BulletInitWorld::addWalls(std::vector<Ogre::SceneNode*> ogreWallNodeVec)
{
	for (int i=0; i < (int)ogreWallNodeVec.size(); i++)
	{
		Ogre::AxisAlignedBox wallBoundingB = ogreWallNodeVec[i]->getAttachedObject(0)->getBoundingBox();
		Ogre::Vector3 wallSizeB = wallBoundingB.getSize();
		wallSizeB /= 2.0f;
		wallSizeB *= 0.98f;

		wallSizeB.x *= ogreWallNodeVec[i]->getScale().x;
		wallSizeB.y *= ogreWallNodeVec[i]->getScale().y;
		wallSizeB.z *= ogreWallNodeVec[i]->getScale().z;

		ogreWallNodeVec[i]->showBoundingBox(true);

		OgreBulletCollisions::BoxCollisionShape *wallBoxShape = new OgreBulletCollisions::BoxCollisionShape(wallSizeB);
		
		OgreBulletDynamics::RigidBody *defaultWallBody = new OgreBulletDynamics::RigidBody(
															"wallRigid" + Ogre::StringConverter::toString(i), 
															mWorld);
		defaultWallBody->setStaticShape(wallBoxShape, 0.1, 0.8, ogreWallNodeVec[i]->getPosition());

		mShapes.push_back(wallBoxShape);
		 mBodies.push_back(defaultWallBody);
	}
}

void BulletInitWorld::run()
{
}