#include "Animation.h"
using namespace std;

Pather *aiPather;

Animation::Animation(void)
{
}
Animation::Animation(std::vector<std::vector<int>> tempMap, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera, int SFR, int FFR, int SFD, int FFD, int DFD, float AIS)
{
	mapSizeX = tempMap.size();
	mapSizeY = tempMap[0].size();

	aiPather = new Pather();
	aiPather->AIinit(tempMap, SFR, FFR, SFD, FFD, DFD, AIS);
	AnimationInit(mSceneMgr, mCamera);
}
Animation::~Animation(void)
{
}
 
void Animation::AnimationInit(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{

    // Create robot entity
    ninjaEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
    ninjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
    ninjaNode->attachObject(ninjaEntity);
	ninjaNode->setScale(0.09f, 0.09f, 0.09f);
	ninjaEntity->setCastShadows(true);
	mWalkSpeed = 5;
	mDirection = Ogre::Vector3::ZERO;

	// Set idle animation
	mAnimationState = ninjaEntity->getAnimationState("Idle1");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);

	// Create arrow entity inc node
	arrowEntity = mSceneMgr->createEntity("Arrow", "arrow.mesh");
	arrowNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ArrowNode");//,Ogre::Vector3(100.0f,30.0f,100.0f));
	arrowEntity->setMaterialName("Arrow");
	arrowNode->attachObject(arrowEntity);
	arrowNode->setScale(0.02f,0.02f,0.02f);//(0.007f,0.007f,0.007f);
	arrowEntity->setCastShadows(false);

	// Set variables
	robotAlive = true;
	robotDead = false;
	mRotating = false;

	// Rand
	srand(time(NULL));

	// Generate random starting positions
	//mCamera->setPosition(aiPather->randPlayerPos());
	//mCamera->lookAt(aiPather->centerOfMap());
	ninjaNode->setPosition(Ogre::Vector3(0,0,0));
	while(ninjaNode->getPosition().x < 15 && ninjaNode->getPosition().z < 15)
	{
		ninjaNode->setPosition(aiPather->randNinjaPos());
	}
	mWalkList.push_back(ninjaNode->getPosition());

}
bool Animation::UpdateAnimation(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
	// Update Ninja
	bool status = updateNinja(evt, mSceneMgr, mCamera);

	// Only show Arrow if player is on ground
	Ogre::Vector3 tempYpos = mCamera->getDerivedPosition();
	if(tempYpos.y < 15)
	{
		// Update Arrow
		updateArrow(evt, mSceneMgr, mCamera);
		arrowNode->setVisible(true);
	}
	else
		// Hide Arrow
		arrowNode->setVisible(false);

	return status;
}

void Animation::updateArrow(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
	cameraPos = mCamera->getDerivedPosition();
	cameraDir = mCamera->getDerivedDirection();
	Ogre::Vector3 cameraUp = mCamera->getDerivedUp();
	Ogre::Vector3 cameraRight = mCamera->getDerivedRight();
	Ogre::Vector3 cameraOriUp = mCamera->getDerivedOrientation()*cameraUp;
	Ogre::Vector3 cameraOriRight = mCamera->getDerivedOrientation()*cameraRight;

	ninjaPos = ninjaNode->getPosition();
	ninjaPos.y = ninjaPos.y+20;
	arrowPos = arrowNode->getPosition();
	arrowOri = arrowNode->getOrientation() * Ogre::Vector3::UNIT_Y;
	arrowDir = ninjaPos - arrowPos;
	arrowQuat = arrowOri.getRotationTo(arrowDir);
	arrowNode->rotate(arrowQuat);

	
	
	arrowNode->setPosition(cameraPos+cameraDir+cameraUp*0.3+cameraRight*0.4);
	//arrowNode->setPosition(cameraPos+cameraDir*10+cameraUp*Ogre::Viewport->getActualHeight()/0.9+cameraRight*Ogre::Viewport->getActualWidth()/0.9);

}
bool Animation::updateNinja(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCamera)
{
	Ogre::Vector3 deathDist = ninjaNode->getPosition() - mCamera->getDerivedPosition();
	if(deathDist < Ogre::Vector3(6,25,6) && deathDist > Ogre::Vector3(-6,-25,-6))	// RobotPosition
		robotAlive = false;

	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (NextLocation(mCamera) && robotAlive) 
		{
			// Set walking animation
			mAnimationState = ninjaEntity->getAnimationState("Walk");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
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
			mWalkSpeed = 1;
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
					mRotFactor = 1.0f / 10.0f;
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



	mAnimationState->addTime(evt.timeSinceLastFrame*mWalkSpeed*0.15);		//*2 to speed up animation

	if (robotDead)
		return false;
	else
		return true;
}
bool Animation::NextLocation(Ogre::Camera* mCamera){

	// PathPlanning and Avoidance
	Ogre::Vector3 rp = ninjaNode->getPosition();	// RobotPosition
	Ogre::Vector3 pp = mCamera->getDerivedPosition();  // PlayerPosition

	if (mDestination == rp && mWalkList.empty())
		mWalkList.push_back(aiPather->AIframe(rp.x,rp.z,pp.x, pp.z));
	mDestination = mWalkList.front();  // this gets the front of the deque
	mWalkList.pop_front();             // this removes the front of the deque
	mDirection = mDestination - ninjaNode->getPosition();
	mDistance = mDirection.normalise();
	mWalkSpeed = (int)aiPather->ninjaSpeed;
	return true;
}