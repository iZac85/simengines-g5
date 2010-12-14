#ifndef __OBPlayer_h_
#define __OBPlayer_h_
 
#include "Ogre.h"
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet


class OBPlayer
{

public:

	Ogre::SceneNode *node;

	Ogre::Vector3 sizeB;

	OgreBulletCollisions::BoxCollisionShape* sceneBoxShape;
	OgreBulletDynamics::RigidBody *defaultBody;

	OBPlayer(Ogre::SceneManager* mSceneMgr, 
				  OgreBulletDynamics::DynamicsWorld *mWorld,
				  Ogre::Vector3 position,
				  Ogre::Vector3 speed,
				  int instanceNumber,
				  Ogre::String meshName);
	

	~OBPlayer(void);

private:

};


#endif // #ifndef __OBPlayer_h_