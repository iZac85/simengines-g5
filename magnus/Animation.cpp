#include "Animation.h"
#include "AI_Extension.h"

using namespace std;

Pather *aiPather;

//-------------------------------------------------------------------------------------

Animation::Animation(void)
{
}
Animation::Animation(std::vector<std::vector<int>> tempMap, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, int difficulty)
{
	fogLevel = difficulty;
	mapSizeX = tempMap.size();
	mapSizeY = tempMap[0].size();

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
	// Enable shadows
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    // Create robot entity
    ninjaEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
    ninjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode", Ogre::Vector3(10.0f, 0.0f, 10.0f));
    ninjaNode->attachObject(ninjaEntity);
	ninjaNode->setScale(0.09f, 0.09f, 0.09f);

	ninjaEntity->setCastShadows(true);
	mWalkSpeed = 15;
	mWalkList.push_back(Ogre::Vector3(20.0f,  0.0f, 20.0f));
	mDirection = Ogre::Vector3::ZERO;

	// Create arrow entity inc node
	arrowEntity = mSceneMgr->createEntity("Arrow", "knot.mesh");
	arrowEntity->setCastShadows(false);
	arrowNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ArrowNode",Ogre::Vector3(0.0f,0.0f,0.0f));
	arrowNode->attachObject(arrowEntity);
	arrowNode->setScale(0.01f,0.01f,0.01f);



	// Set variables
	robotAlive = true;
	robotDead = false;
	mRotating = false;


}
bool Animation::NextLocation(Ogre::Camera* mCamera){

	// PathPlanning and Avoidance
	Ogre::Vector3 rp = ninjaNode->getPosition();	// RobotPosition
	Ogre::Vector3 pp = mCamera->getPosition();  // PlayerPosition

	if (mDestination == rp && mWalkList.empty())
		mWalkList.push_back(aiPather->AIframe(rp.x,rp.z,pp.x, pp.z));
	mDestination = mWalkList.front();  // this gets the front of the deque
	mWalkList.pop_front();             // this removes the front of the deque
	mDirection = mDestination - ninjaNode->getPosition();
	mDistance = mDirection.normalise();
	return true;
}
void Animation::UpdateAnimation(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
	Ogre::Vector3 deathDist = ninjaNode->getPosition() - mCamera->getPosition();
	if(deathDist < Ogre::Vector3(10,40,10) && deathDist > Ogre::Vector3(-10,-40,-10))	// RobotPosition
		robotAlive = false;

    if (mDirection == Ogre::Vector3::ZERO) 
	{
		if (NextLocation(mCamera) && robotAlive) 
		{
			// Set walking animation
			mAnimationState = ninjaEntity->getAnimationState("Walk");
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
		mAnimationState = ninjaEntity->getAnimationState("Death2");
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(true);
		robotDead = true;
		animSpeedUp = 1;
		mSceneMgr->setFog(Ogre::FOG_LINEAR,Ogre::ColourValue (0.4,0,0,0.8), 0.001 ,1, max(mapSizeX,mapSizeY)/8);
		}
	}
	else if (robotAlive)
	{
		Ogre::Real move = mWalkSpeed * evt.timeSinceLastFrame;
		mDistance -= move;
		if (mDistance <= 0.0f)
		{                 
			ninjaNode->setPosition(mDestination);
			mDirection = Ogre::Vector3::ZERO;				
			// Set animation based on if the robot has another point to walk to. 
			if (!NextLocation(mCamera))
			{
				// Set Idle animation                     
				mAnimationState = ninjaEntity->getAnimationState("Idle1");
				mAnimationState->setLoop(true);
				mAnimationState->setEnabled(true);
			}
			else
			{
				// Rotation Code will go here later
				Ogre::Vector3 src = ninjaNode->getOrientation() * -Ogre::Vector3::UNIT_Z;
				if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
				{
					ninjaNode->yaw(Ogre::Degree(180));	
				}
				else if (mRotating == false)
				{
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					//mNode->rotate(quat);
					mRotating = true;
					mRotFactor = 1.0f / 50.0f;
					mOrientSrc = ninjaNode->getOrientation();
					mOrientDest = quat * mOrientSrc;           // We want dest orientation, not a relative rotation (quat)
					mRotProgress = 0;
				} // else
			}//else
		}
		else
		{
			ninjaNode->translate(mDirection * move);
		} // else
	} // if
	if(mRotating)                                // Process timed rotation
	{
		mRotProgress += mRotFactor;
		if(mRotProgress>1)
		{
			mRotating = false;
		}
		else
		{
			Ogre::Quaternion delta = Ogre::Quaternion::Slerp(mRotProgress, mOrientSrc, mOrientDest, true);
			ninjaNode->setOrientation(delta);
		}
	}  // if mRotating

	mAnimationState->addTime(evt.timeSinceLastFrame*animSpeedUp);		//*2 to speed up animation


	//Enable Fog
	if(robotAlive){
		Ogre::Vector3 pp = mCamera->getPosition();
		if (pp.y<80)
		{
			if (fogLevel == 1)
				mSceneMgr->setFog(Ogre::FOG_LINEAR,Ogre::ColourValue (0.1,0.1,0.1,0.3), 0.001 ,10, max(mapSizeX,mapSizeY));
			else if (fogLevel == 2)
				mSceneMgr->setFog(Ogre::FOG_LINEAR,Ogre::ColourValue (0.1,0.1,0.1,0.8), 0.001 ,1, max(mapSizeX,mapSizeY)/4);
		}
		else
			mSceneMgr->setFog(Ogre::FOG_NONE);
	}

}