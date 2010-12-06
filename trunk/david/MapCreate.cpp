/*
-----------------------------------------------------------------------------
Filename:    OgreApp3.cpp
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (August 2010)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/

#include "MapCreate.h"

//-------------------------------------------------------------------------------------
MapCreate::MapCreate(Ogre::SceneManager* mSceneMgr, int dim_x, int dim_y, int room_min_x, int room_min_y, int room_max_area, int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt)
{	
	mRandGen = new RandGen();
	// Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
	//mRandGen->generateMap();

	Ogre::Entity* boxEnt[3];
	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;

	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 450.0f, 450.0f, 1, 1, true, 1, 10.0f, 10.0f, Ogre::Vector3::UNIT_Z);
	//Ogre::MeshManager::getSingleton().createPresfabCube();
	//Materials
	Ogre::MaterialPtr mat1 = Ogre::MaterialManager::getSingleton().create("FloorMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture1 = mat1->getTechnique(0)->getPass(0)->createTextureUnitState("MRAMOR6X6.jpg");
	//mat1->setCullingMode(Ogre::CullingMode::CULL_NONE);

	Ogre::MaterialPtr mat2 = Ogre::MaterialManager::getSingleton().create("WallMat1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture2 = mat2->getTechnique(0)->getPass(0)->createTextureUnitState("RustedMetal.jpg");
	//mat2->setCullingMode(Ogre::CullingMode::CULL_NONE);

	Ogre::MaterialPtr mat3 = Ogre::MaterialManager::getSingleton().create("WallMat2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture3 = mat3->getTechnique(0)->getPass(0)->createTextureUnitState("KAMEN320x240.jpg");
	//mat3->setCullingMode(Ogre::CullingMode::CULL_NONE);
	
	boxEnt[0] = mSceneMgr->createEntity("plane1", "floor");
	boxEnt[0]->setMaterialName("FloorMat");
	boxEnt[0]->setCastShadows(false);
	
	boxEnt[1] = mSceneMgr->createEntity("cube1", "cube.mesh");
	boxEnt[1]->setMaterialName("WallMat1");
	boxEnt[1]->setCastShadows(false);
	
	boxEnt[2] = mSceneMgr->createEntity("cube2", "cube.mesh");
	boxEnt[2]->setMaterialName("WallMat2");
	boxEnt[2]->setCastShadows(false);

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(boxEnt[0]);		//The Floor!
	Ogre::StaticGeometry* sg = mSceneMgr->createStaticGeometry("Walls");
	const int MaxWalls = 100;
	const int Dim = 50.0f;
	for(int i = 0; i < MaxWalls; i++)
	{
		Ogre::Real r = 450/2;
		Ogre::Vector3 pos(Ogre::Math::RangeRandom(-r, r), 20, Ogre::Math::RangeRandom(-r, r));
		Ogre::Vector3 scale(0.1, 0.5, Ogre::Math::RangeRandom(0.2f, 0.5f));
		Ogre::Quaternion one, two, three, orientation;
			
		if(Ogre::Math::RangeRandom(0, 1) > 0.5) one.FromAngleAxis(Ogre::Degree(0), Ogre::Vector3::UNIT_Y);
		else one.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
		
		two.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_X);
		three.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_Z);
		orientation =  one;// * three * two;
		
		if (Ogre::Math::RangeRandom(0, 1) > 0.5) sg->addEntity(boxEnt[1], pos, orientation, scale);
		else sg->addEntity(boxEnt[2], pos, orientation, scale);

		sg->build();
	}
}
//-------------------------------------------------------------------------------------
MapCreate::~MapCreate(void)
{
	delete mRandGen;
}