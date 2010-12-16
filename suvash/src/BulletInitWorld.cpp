#include "BulletInitWorld.h"


BulletInitWorld::BulletInitWorld(Ogre::SceneManager* mSceneMgr, 
								 Ogre::SceneNode* ogreFloorNode,
								 std::vector<Ogre::SceneNode*> ogreWallNodeVec,
								 std::vector<Ogre::SceneNode*> ogreBoxNodeVec,
								 Ogre::Real dimx,
								 Ogre::Real dimy,
								 Ogre::Real dimz,
								 bool enableGravity)
{
	this->ogreSceneMgr = mSceneMgr;
	this->bulletWorldX = dimx;
	this->bulletWorldY = dimy;
	this->bulletWorldZ = dimz;
	this->zeroGravityisOn = !enableGravity;

	this->initWorld();
	this->debugBoxBullet(false);

	this->addFloor(ogreFloorNode);
	this->addWalls(ogreWallNodeVec);
	this->addBoxes(ogreBoxNodeVec);

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
	if(zeroGravityisOn)
	{
		gravityVector = Ogre::Vector3(0,0,0);
	}
	else
	{
		gravityVector = Ogre::Vector3(0,-20.81,0);
	}
	
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
		mWorld->setShowDebugShapes(true);		// enable it if you want to see the Bullet containers

		//Create a node and attach it to the Ogre Scene
		Ogre::SceneNode *node = ogreSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
		node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));
	}
}

void BulletInitWorld::addFloor(Ogre::SceneNode* ogreFloorNode)
{
	//ogreFloorNode->showBoundingBox(true);
	
	Ogre::AxisAlignedBox floorBoundingB = ogreFloorNode->getAttachedObject(0)->getBoundingBox();
	Ogre::Vector3 floorSizeB = floorBoundingB.getSize();
	// Don't ask me why i do the following
	floorSizeB /= 2.0f;
	floorSizeB *= 0.98f;

	floorSizeB.x *= ogreFloorNode->getScale().x;
	floorSizeB.y *= ogreFloorNode->getScale().y;
	floorSizeB.z *= ogreFloorNode->getScale().z;
	
	//ogreFloorNode->showBoundingBox(true);
	
	// add collision detection to it
	OgreBulletCollisions::CollisionShape *floorShape = new OgreBulletCollisions::BoxCollisionShape(floorSizeB);
	//Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,1,0), 0); // (normal vector, distance)

	// a body is needed for the shape
	OgreBulletDynamics::RigidBody *defaultFloorBody = new OgreBulletDynamics::RigidBody("BasePlane", mWorld);
	defaultFloorBody->setStaticShape(floorShape, 0.0, 0.8,ogreFloorNode->getPosition());// (shape, restitution, friction)

	//defaultFloorBody->showDebugShape(true);

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

		//ogreWallNodeVec[i]->showBoundingBox(true);

		OgreBulletCollisions::BoxCollisionShape *wallBoxShape = new OgreBulletCollisions::BoxCollisionShape(wallSizeB);
		
		OgreBulletDynamics::RigidBody *defaultWallBody = new OgreBulletDynamics::RigidBody(
															"wallRigid" + Ogre::StringConverter::toString(i), 
															mWorld);
		defaultWallBody->setStaticShape(wallBoxShape, 0.1, 0.8, ogreWallNodeVec[i]->getPosition());

		mShapes.push_back(wallBoxShape);
		 mBodies.push_back(defaultWallBody);
	}
}

void BulletInitWorld::addBoxes(std::vector<Ogre::SceneNode*> ogreBoxNodeVec)
{
	for (int i=0; i < (int)ogreBoxNodeVec.size(); i++)
	{

		Ogre::AxisAlignedBox boxBoundingB = ogreBoxNodeVec[i]->getAttachedObject(0)->getBoundingBox();
		Ogre::Vector3 boxSizeB = boxBoundingB.getSize();
		boxSizeB /= 2.0f;
		boxSizeB *= 0.98f;

		boxSizeB.x *= ogreBoxNodeVec[i]->getScale().x;
		boxSizeB.y *= ogreBoxNodeVec[i]->getScale().y;
		boxSizeB.z *= ogreBoxNodeVec[i]->getScale().z;

		OgreBulletCollisions::BoxCollisionShape *boxBoxShape = new OgreBulletCollisions::BoxCollisionShape(boxSizeB);

		OgreBulletDynamics::RigidBody *defaultBoxBody = new OgreBulletDynamics::RigidBody(
			"boxRigid" + Ogre::StringConverter::toString(i), 
			mWorld);

		Ogre::Vector3 posBox = Ogre::Vector3(ogreBoxNodeVec[i]->_getDerivedPosition());
		defaultBoxBody->setShape(ogreBoxNodeVec[i],boxBoxShape,0.0,0.8,0.001f,posBox);

		mShapes.push_back(boxBoxShape);
		mBodies.push_back(defaultBoxBody);
	}
}

void BulletInitWorld::run()
{
}