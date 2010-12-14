#include "Animation.h"
using namespace std;

Pather *aiPather;

Animation::Animation(void)
{
}
Animation::Animation(std::vector<std::vector<int>> tempMap, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCameraFPV, int difficulty, int SFR, int FFR, int SFD, int FFD, int DFD, int AIS)
{
	fogLevel = difficulty;
	mapSizeX = tempMap.size();
	mapSizeY = tempMap[0].size();

	aiPather = new Pather();
	aiPather->AIinit(tempMap, SFR, FFR, SFD, FFD, DFD, AIS);
	AnimationInit(mSceneMgr, mCameraFPV);
}
Animation::~Animation(void)
{
}
 
void Animation::AnimationInit(Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCameraFPV)
{
	// Enable shadows
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// Disable fog
	mSceneMgr->setFog(Ogre::FOG_NONE);

	// Enable Sky Dome
	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

    // Create robot entity
    ninjaEntity = mSceneMgr->createEntity("Ninja", "ninja.mesh");
    ninjaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NinjaNode");
    ninjaNode->attachObject(ninjaEntity);
	ninjaNode->setScale(0.09f, 0.09f, 0.09f);

	ninjaEntity->setCastShadows(true);
	mWalkSpeed = 5;
	mWalkList.push_back(Ogre::Vector3(20.0f,  0.0f, 20.0f));
	mDirection = Ogre::Vector3::ZERO;

	// Set idle animation
	mAnimationState = ninjaEntity->getAnimationState("Idle1");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);

	// Create arrow entity inc node
	arrowEntity = mSceneMgr->createEntity("Arrow", "cube.mesh");
	arrowNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ArrowNode");//,Ogre::Vector3(100.0f,30.0f,100.0f));
	arrowNode->attachObject(arrowEntity);
	arrowNode->setScale(0.002f,0.002f,0.01f);
	arrowEntity->setCastShadows(false);

	// Set variables
	robotAlive = true;
	robotDead = false;
	mRotating = false;
	animSpeedUp = 1;

	// Rand
	srand(time(NULL));

	// Generate random starting positions
	//mCameraFPV->setPosition(aiPather->randPlayerPos());
	//mCameraFPV->lookAt(aiPather->centerOfMap());
	//while (mCameraFPV->getPosition().positionCloses(ninjaNode->getPosition(),10))
	//{
		ninjaNode->setPosition(aiPather->randNinjaPos());
	//}

	// Create Grass
	createGrassMesh();
	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;

	Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 20000.0f, 20000.0f, 10, 10, true, 1, 50.0f, 50.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* planeEnt = mSceneMgr->createEntity("floor","plane");
	planeEnt->setMaterialName("Examples/GrassFloor");
	planeEnt->setCastShadows(false);
	grassNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("grassNode",Ogre::Vector3(0,-1,0));
	grassNode->attachObject(planeEnt);
	
	Ogre::Entity* grass = mSceneMgr->createEntity("grass", "GrassBladesMesh");
	Ogre::StaticGeometry* sg = mSceneMgr->createStaticGeometry("GrassArea");

	const int size = mapSizeX+600;
	const int amount = 6;

	sg->setRegionDimensions(Ogre::Vector3(size, size, size));
	sg->setOrigin(Ogre::Vector3(-size/2, 0, -size/2));

	for(int x = -size / 2; x < size /2; x += (size/amount))
	{
		for(int z = -size / 2; z < size / 2; z += (size/amount))
		{
			if (!aiPather->inMap(x,z))
			{
				Ogre::Real r = 50;//size / float(amount / 2);
				Ogre::Vector3 pos(x + Ogre::Math::RangeRandom(-r, r), 0, z + Ogre::Math::RangeRandom(-r, r));
				Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9f, 1.1f), 1);
				Ogre::Quaternion orientation;
			

			orientation.FromAngleAxis(Ogre::Degree(Ogre::Math::RangeRandom(0, 359)), Ogre::Vector3::UNIT_Y);

			sg->addEntity(grass, pos, orientation, scale);
			}
		}
		sg->build();
	}

}
bool Animation::UpdateAnimation(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCameraFPV)
{
	// Update
	bool status = updateNinja(evt, mSceneMgr, mCameraFPV);
	//updateFog(evt, mSceneMgr, mCameraFPV);
	updateArrow(evt, mSceneMgr, mCameraFPV);

	return status;
}
void Animation::createGrassMesh()
{
	//declare all of our grass variables
	const float width = 25.0f;
	const float height = 40.0f;

	Ogre::ManualObject grass("GrassObject");

	Ogre::Vector3 vec(width / 2, 0, 0);
	Ogre::Quaternion rot;
	rot.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);

	//start defining our manual object
	grass.begin("Examples/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	//define the 4 vertices of our quad and set to the texture coordinates
	for(int i = 0; i < 3; ++i)
	{
		grass.position(-vec.x, height, -vec.z);
		grass.textureCoord(0, 0);

		grass.position(vec.x, height, vec.z);
		grass.textureCoord(1, 0);

		grass.position(-vec.x, 0, -vec.z);
		grass.textureCoord(0, 1);

		grass.position(vec.x, 0, vec.z);
		grass.textureCoord(1, 1);

		int offset = i * 4;

		grass.triangle(offset, offset + 3, offset + 1);
		grass.triangle(offset, offset + 2, offset + 3);

		//rotate the next quad
		vec = rot * vec;
	}
	grass.end();

	//create an actual mesh out of this object
	grass.convertToMesh("GrassBladesMesh");
}

void Animation::updateFog(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCameraFPV)
{
	//Enable Fog
	if(robotAlive){
		Ogre::Vector3 pp = mCameraFPV->getPosition();
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
void Animation::updateArrow(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCameraFPV)
{
	cameraPos = mCameraFPV->getPosition();
	cameraDir = mCameraFPV->getDirection();
	Ogre::Vector3 cameraUp = mCameraFPV->getUp();
	Ogre::Vector3 cameraRight = mCameraFPV->getRight();
	Ogre::Vector3 cameraOriUp = mCameraFPV->getOrientation()*cameraUp;
	Ogre::Vector3 cameraOriRight = mCameraFPV->getOrientation()*cameraRight;

	ninjaPos = ninjaNode->getPosition();
	arrowPos = arrowNode->getPosition();
	if (arrowPos.z < ninjaPos.z)
		arrowOri = arrowNode->getOrientation() * Ogre::Vector3::UNIT_Z;
	else
		arrowOri = arrowNode->getOrientation() * -Ogre::Vector3::UNIT_Z;

	ninjaPos.y = ninjaPos.y+20;
	arrowDir = ninjaPos - arrowPos;
	arrowQuat = arrowOri.getRotationTo(arrowDir);
	arrowNode->rotate(arrowQuat);

	
	
	arrowNode->setPosition(cameraPos+cameraDir*10+cameraUp*3.5+cameraRight*3.5);
	//arrowNode->setPosition(cameraPos+cameraDir*10+cameraUp*Ogre::Viewport->getActualHeight()/0.9+cameraRight*Ogre::Viewport->getActualWidth()/0.9);

}
bool Animation::updateNinja(const Ogre::FrameEvent &evt, Ogre::SceneManager* mSceneMgr, Ogre::Camera* mCameraFPV)
{
	Ogre::Vector3 deathDist = ninjaNode->getPosition() - mCameraFPV->getPosition();
	if(deathDist < Ogre::Vector3(10,40,10) && deathDist > Ogre::Vector3(-10,-40,-10))	// RobotPosition
		robotAlive = false;

	if (mDirection == Ogre::Vector3::ZERO)
	{
		if (NextLocation(mCameraFPV) && robotAlive) 
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
			if (!NextLocation(mCameraFPV))
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
					mRotFactor = 1.0f / 5.0f;
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

	if (robotDead)
		return false;
	else
		return true;
}
bool Animation::NextLocation(Ogre::Camera* mCameraFPV){

	// PathPlanning and Avoidance
	Ogre::Vector3 rp = ninjaNode->getPosition();	// RobotPosition
	Ogre::Vector3 pp = mCameraFPV->getPosition();  // PlayerPosition

	if (mDestination == rp && mWalkList.empty())
		mWalkList.push_back(aiPather->AIframe(rp.x,rp.z,pp.x, pp.z));
	mDestination = mWalkList.front();  // this gets the front of the deque
	mWalkList.pop_front();             // this removes the front of the deque
	mDirection = mDestination - ninjaNode->getPosition();
	mDistance = mDirection.normalise();
	mWalkSpeed = aiPather->ninjaSpeed;
	return true;
}