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
MapCreate::MapCreate(Ogre::Root* mRoot, Ogre::SceneManager* mSceneMgr, int dim_x, int dim_y, int room_min_x, int room_min_y, int room_max_area, int door_cnt, bool furniture_enable, int desk_cnt, int chair_cnt, int shelf_cnt, int painting_cnt)
{	
	mRandGen = new RandGen();
	// Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
	mRandGen->generateMap(dim_x,dim_y,room_min_x,room_min_y,room_max_area,door_cnt,furniture_enable,desk_cnt,chair_cnt,shelf_cnt,painting_cnt);
	//mRandGen->generateMap(27,27,14,14,196,1,furniture_enable,desk_cnt,chair_cnt,shelf_cnt,painting_cnt);
	//mRandGen->generateMap(100,100,14,14,600,2,furniture_enable,desk_cnt,chair_cnt,shelf_cnt,painting_cnt);
	Ogre::Entity* mfloorEnt;
	Ogre::Entity* mTmpEnt;
	std::vector<Ogre::Entity*> mWallEnt;
	Ogre::SceneNode* mTmpNode;

	//std::vector<Ogre::TextureUnitState*> mWallTexture;
	//Ogre::TextureUnitState* mTmpTex;

	//std::vector<Ogre::MaterialPtr> mWallMtl;
	//Ogre::MaterialPtr mTmpMtl;

	//Materials
	Ogre::MaterialPtr mat1 = Ogre::MaterialManager::getSingleton().create("FloorMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture1 = mat1->getTechnique(0)->getPass(0)->createTextureUnitState("MRAMOR6X6.jpg");

	Ogre::MaterialPtr mat2 = Ogre::MaterialManager::getSingleton().create("WallMat1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture2 = mat2->getTechnique(0)->getPass(0)->createTextureUnitState("RustedMetal.jpg");

	Ogre::MaterialPtr mat3 = Ogre::MaterialManager::getSingleton().create("WallMat2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::TextureUnitState* tuisTexture3 = mat3->getTechnique(0)->getPass(0)->createTextureUnitState("KAMEN320x240.jpg");
	
	mfloorEnt = mSceneMgr->createEntity("floor1", "cube.mesh");
	mfloorEnt->setMaterialName("FloorMat");

	mFloorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("FloorNode", Ogre::Vector3(dim_x/2, -0.5, dim_y/2));
	mFloorNode->attachObject(mfloorEnt);
	mFloorNode->setScale(0.01 * dim_x, 0.01, 0.01 * dim_y);
	
	for(int i = 0; i < mRandGen->arena.wall_cnt; i++)
	{
		mTmpEnt = mSceneMgr->createEntity("cube.mesh");
		int r = mRandGen->randInt(0,1);
		switch (r)
		{
			case 0: 
				mTmpEnt->setMaterialName("WallMat1");
				break;
			case 1: 
				mTmpEnt->setMaterialName("WallMat2");
				break;
		}
		mWallEnt.push_back(mTmpEnt);

		mTmpNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mTmpNode->attachObject(mWallEnt[i]);
		mTmpNode->setScale(0.01 * (mRandGen->arena.wall[i].dim.x) ,0.2, 0.01 * (mRandGen->arena.wall[i].dim.y));
		mTmpNode->setPosition(mRandGen->arena.wall[i].pos3D.x,10,mRandGen->arena.wall[i].pos3D.y);
		mWallNode.push_back(mTmpNode);
		mRoot->renderOneFrame();
	}
	map = mRandGen->arena.blackTile;
}
//-------------------------------------------------------------------------------------
MapCreate::~MapCreate(void)
{
	delete mRandGen;
}

Ogre::SceneNode* MapCreate::returnFloorNode()
{
	return mFloorNode;
}

std::vector<Ogre::SceneNode*> MapCreate::returnWallNodeVec()
{
	return mWallNode;
}