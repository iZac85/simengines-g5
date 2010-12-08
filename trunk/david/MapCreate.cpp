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
	mRandGen->generateMap(dim_x,dim_y);

	Ogre::Entity* wallEnt[2];
	Ogre::Entity* floorEnt;
	Ogre::Entity* boxEnt[10];
	std::vector<Ogre::Entity*> wallEntDyn;
	Ogre::SceneNode* mFloorNode;

	Ogre::Plane plane;
	plane.normal = Ogre::Vector3::UNIT_Y;
	plane.d = 0;

	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, dim_x, dim_y, 1, 1, true, 1, 10.0f, 10.0f, Ogre::Vector3::UNIT_Z);
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
	
	floorEnt = mSceneMgr->createEntity("floor1", "floor");
	floorEnt->setMaterialName("FloorMat");
	floorEnt->setCastShadows(false);
	
	wallEnt[0] = mSceneMgr->createEntity("wall1", "cube.mesh");
	wallEnt[0]->setMaterialName("WallMat1");
	wallEnt[0]->setCastShadows(false);
	
	wallEnt[1] = mSceneMgr->createEntity("wall2", "cube.mesh");
	wallEnt[1]->setMaterialName("WallMat2");
	wallEnt[1]->setCastShadows(false);

	//wallEntDyn.push_back(mSceneMgr->createEntity("wall1", "cube.mesh"));
	//wallEntDyn[0]->setMaterialName("WallMat1");

	mFloorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("FloorNode", Ogre::Vector3(dim_x/2, 0.0f, dim_x/2));
	mFloorNode->attachObject(floorEnt);
	//mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(floorEnt);		//The Floor!
	Ogre::StaticGeometry* sg = mSceneMgr->createStaticGeometry("Walls");
	//const int MaxWalls = 10;
	//const int Dim = 50.0f;

	for(int i = 0; i < dim_x; i++)		//x
	{		
		for(int j = 0; j < dim_y; j++)	//y
		{
			if(mRandGen->arena.blackTile[i][j]==1)
			{
				Ogre::Vector3 pos(i, 1, j);
				Ogre::Vector3 scale(0.01, 0.03, 0.01); //y=height
				Ogre::Quaternion one, orientation;
			
				//if(Ogre::Math::RangeRandom(0, 1) > 0.5) one.FromAngleAxis(Ogre::Degree(0), Ogre::Vector3::UNIT_Y);
				//else one.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
				one.FromAngleAxis(Ogre::Degree(0), Ogre::Vector3::UNIT_Y);
				orientation =  one;
		
				//if (Ogre::Math::RangeRandom(0, 1) > 0.5) sg->addEntity(wallEnt[0], pos, orientation, scale);
				//else sg->addEntity(wallEnt[1], pos, orientation, scale);
				sg->addEntity(wallEnt[0], pos, orientation, scale);

				sg->build();
			}
		}
	}
}
//-------------------------------------------------------------------------------------
MapCreate::~MapCreate(void)
{
	delete mRandGen;
}
