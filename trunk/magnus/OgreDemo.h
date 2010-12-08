#ifndef __OgreDemo_h_
#define __OgreDemo_h_
 
#include "BaseApplication.h"
#include <deque>
 
class OgreDemo : public BaseApplication
{
public:
    OgreDemo(void);
    virtual ~OgreDemo(void);

 
protected:
     virtual void createScene(void);
     virtual void createFrameListener(void);
     virtual bool nextLocation(void);
     virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
 
     Ogre::Real mDistance;                  // The distance the object has left to travel
     Ogre::Vector3 mDirection;              // The direction the object is moving	
	 Ogre::Vector3 mDestination;            // The destination the object is moving towards
	 Ogre::Real mWalkSpeed;                 // The speed at which the object is moving
	 std::deque<Ogre::Vector3> mWalkList;   // The list of points we are walking to

 
     Ogre::AnimationState *mAnimationState; // The current animation state of the object
 
     Ogre::Entity *mEntity;                 // The Entity we are animating
     Ogre::SceneNode *mNode;                // The SceneNode that the Entity is attached to

 
 
 
 
};
 
#endif // #ifndef __OgreDemo_h_