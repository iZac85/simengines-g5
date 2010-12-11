#ifndef __Animation_h_
#define __Animation_h_
 
#include "BaseApplication.h"
#include <deque>
#include <vector>

class Animation
{
public:
	Animation(void);
    Animation(std::vector<std::vector<int>>, Ogre::SceneManager*, Ogre::Camera*,int);
    ~Animation(void);

	void AnimationInit(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera);
	void UpdateAnimation(const Ogre::FrameEvent &evt,Ogre::SceneManager*, Ogre::Camera*);

protected:

     bool NextLocation(Ogre::Camera*);
 
     
	 Ogre::Real mDistance;                  // The distance the object has left to travel
     Ogre::Vector3 mDirection;              // The direction the object is moving	
	 Ogre::Vector3 mDestination;            // The destination the object is moving towards
	 Ogre::Real mWalkSpeed;                 // The speed at which the object is moving
	 std::deque<Ogre::Vector3> mWalkList;   // The list of points we are walking to

 
     Ogre::AnimationState *mAnimationState; // The current animation state of the object
 
     Ogre::Entity *mEntity;                 // The Entity we are animating
     Ogre::SceneNode *mNode;                // The SceneNode that the Entity is attached to
	 Ogre::Entity *ent2;
	 Ogre::SceneNode *node2;
	 bool robotAlive;
	 bool robotDead;
	 int animSpeedUp;
	 float deathAnimationTime;

	 Ogre::Quaternion mOrientSrc;               // Initial orientation
	 Ogre::Quaternion mOrientDest;              // Destination orientation
	 Ogre::Real mRotProgress;                   // How far we've interpolated
	 Ogre::Real mRotFactor;                     // Interpolation step
	 bool mRotating;
	
	 int fogLevel;
 
 
 
};
 
#endif // #ifndef __Animation_h_