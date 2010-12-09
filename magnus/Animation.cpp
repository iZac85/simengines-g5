#include "Animation.h"
#include "AI_Extension.h"

using namespace std;

Pather *aiPather;

//-------------------------------------------------------------------------------------

Animation::Animation(void)
{
}
Animation::Animation(std::vector<std::vector<int>> tempMap, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
	aiPather = new Pather();
	aiPather->AIinit(tempMap);
	AnimationInit(mSceneMgr, mCamera);
}
//-------------------------------------------------------------------------------------
Animation::~Animation(void)
{
}
 
//-------------------------------------------------------------------------------------
void Animation::AnimationInit(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
    // Create the entity
    mEntity = mSceneMgr->createEntity("Robot", "ninja.mesh");
	robotAlive = true;
	robotDead = false;
 
    // Create the scene node
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode", Ogre::Vector3(10.0f, 0.0f, 10.0f));
    mNode->attachObject(mEntity);
	mNode->setScale(0.03f, 0.03f, 0.03f);
	mWalkSpeed = 10;
	mWalkList.push_back(Ogre::Vector3(20.0f,  0.0f, 20.0f));
	mDirection = Ogre::Vector3::ZERO;
}
bool Animation::NextLocation(Ogre::Camera* mCamera){

	// PathPlanning and Avoidance
	Ogre::Vector3 rp = mNode->getPosition();	// RobotPosition
	Ogre::Vector3 pp = mCamera->getPosition();  // PlayerPosition

	if (mDestination == rp && mWalkList.empty())
		mWalkList.push_back(aiPather->AIframe(rp.x,rp.z,pp.x, pp.z));
	mDestination = mWalkList.front();  // this gets the front of the deque
	mWalkList.pop_front();             // this removes the front of the deque
	mDirection = mDestination - mNode->getPosition();
	mDistance = mDirection.normalise();
	return true;
}
void Animation::UpdateAnimation(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
	Ogre::Vector3 deathDist = mNode->getPosition() - mCamera->getPosition();
	if(deathDist < Ogre::Vector3(5,5,5) && deathDist > Ogre::Vector3(-5,-5,-5))	// RobotPosition
		robotAlive = false;

    if (mDirection == Ogre::Vector3::ZERO) 
	{
		if (NextLocation(mCamera) && robotAlive) 
		{
			// Set walking animation
			mAnimationState = mEntity->getAnimationState("Walk");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
			animSpeedUp = 2;
		}//if
	}
	else if (!robotAlive)
	{
		if (!robotDead)
		{
		// Set Idle animation                     
		mAnimationState = mEntity->getAnimationState("Death2");
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(true);
		robotDead = true;
		animSpeedUp = 1;
		//deathAnimationTime = time(NULL)+10;
		}
	}
	else if (robotAlive)
	{
		Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
		mDistance -= move;
		if (mDistance <= 0.0f)
		{                 
			mNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;				
			// Set animation based on if the robot has another point to walk to. 
			if (!NextLocation(mCamera))
			{
				// Set Idle animation                     
				mAnimationState = mEntity->getAnimationState("Idle1");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
			}
			else
			{
				// Rotation Code will go here later
				Ogre::Vector3 src = mNode->getOrientation() * -Ogre::Vector3::UNIT_Z;
				if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
				{
					mNode->yaw(Ogre::Degree(180));						
				}
				else
				{
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					mNode->rotate(quat);
				} // else
			}//else
		}
		else
		{
			mNode->translate(mDirection * move);
		} // else
	} // if
	mAnimationState->addTime(evt.timeSinceLastFrame*animSpeedUp);		//*2 to speed up animation
}