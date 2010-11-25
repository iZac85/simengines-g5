#ifndef __OBBox_h_
#define __OBBox_h_
 
#include "Ogre.h"
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"		 // for Boxes
#include "OgreBulletDynamicsRigidBody.h"				 // for OgreBullet


class OBBox
{

public:

	OgreBulletCollisions::BoxCollisionShape* sceneBoxShape;
	OgreBulletDynamics::RigidBody *defaultBody;

	OBBox(Ogre::SceneManager* mSceneMgr, 
				  OgreBulletDynamics::DynamicsWorld *mWorld,
				  Ogre::Vector3 position,
				  Ogre::Vector3 speed,
				  int instanceNumber,
				  Ogre::String meshName);
	

	~OBBox(void);

private:

};


#endif // #ifndef __OBBox_h_