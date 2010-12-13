#ifndef __BulletInitWorld_h_
#define __BulletInitWorld_h_

#include <Ogre.h>

#include <OgreBulletDynamicsRigidBody.h>				 // for OgreBullet
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h> // for static planes
#include <Shapes/OgreBulletCollisionsBoxShape.h>// for Boxes


class BulletInitWorld
{
public:

	//
	Ogre::SceneManager* ogreSceneMgr;

	//OgreBullet Stuff
	std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
	std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;

	OgreBulletDynamics::DynamicsWorld *mWorld;	// OgreBullet World
	OgreBulletCollisions::DebugDrawer *debugDrawer;
	Ogre::Vector3 gravityVector;
	Ogre::AxisAlignedBox bounds;

	int mNumEntitiesInstanced;

	Ogre::Real bulletWorldX;
	Ogre::Real bulletWorldY;
	Ogre::Real bulletWorldZ;

	//Initialize the world
	void initWorld();
	void debugBoxBullet(bool);
	void addFloor(Ogre::SceneNode*);
	void addWalls(std::vector<Ogre::SceneNode*>);
	
	void run();

	//Add dynamics object
	//void addDynObj

	BulletInitWorld(Ogre::SceneManager*,
					Ogre::SceneNode*,
					std::vector<Ogre::SceneNode*>,
					Ogre::Real, 
					Ogre::Real, 
					Ogre::Real);

	~BulletInitWorld(void);
};

#endif // #ifndef __BulletInitWorld_h_