/*
-----------------------------------------------------------------------------
Filename:    OgreCEGUI.h
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
#ifndef __OgreCEGUI_h_
#define __OgreCEGUI_h_

#include "BaseApplication.h"
#include "CEGUIOgreRenderer.h"
#include "CEGUI.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

class OgreCEGUI : public BaseApplication
{
public:
    OgreCEGUI(void);
    ~OgreCEGUI(void);

	virtual void createScene(void);

	virtual void createFrameListener(void);

	//Functions for loading the menus
	void ShowIngameMenu(bool);
	void ShowMainMenu(void);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg, bool );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//-----------VARIABLES------------//
	struct settings
	{
		bool aiSettingsOn;
		bool aiSettingsCustomOn;
		int aiSettingsSFRVal;	// Slow Flee Radius
		int aiSettingsFFRVal;	// Fast Flee Radius
		int aiSettingsSFDVal;	// Slow Flee Distance
		int aiSettingsFFDVal;	// Fast Flee Distance
		int aiSettingsDFDVal;	// Don't Flee Distance
		float aiSettingsAISVal;	// AI Speed (%, range 50% to 400%)
		bool physSettingsOn;
		bool threeDSettingsActive;
		int threeDsettingsDoorCnt;
		bool threeDsettingsFurnitureEn;
		int threeDsettingsMaxRoomSize;
		int threeDSettingsArenaSizeX;
		int threeDSettingsArenaSizeY;
	};
	settings extensionSettings;

	bool showMainMenu;
	bool ingameMenuVisible;

protected:

	//-----------EVENT SUBSCRIBERS------------//

	bool quit(const CEGUI::EventArgs &e);
	bool launchDemo(const CEGUI::EventArgs &e);

	bool inGameQuitBtnClicked(const CEGUI::EventArgs &e);
	bool inGameReturnBtnClicked(const CEGUI::EventArgs &e);
	bool inGameCloseBtnClicked(const CEGUI::EventArgs &e);

	bool aiSettingsOnBtnChanged(const CEGUI::EventArgs &e);
	bool aiSettingsOffBtnChanged(const CEGUI::EventArgs &e);

	bool aiSettingsCustomOnChanged(const CEGUI::EventArgs &e);

	bool aiSettingsSFRChanged(const CEGUI::EventArgs &e);
	bool aiSettingsFFRChanged(const CEGUI::EventArgs &e);
	bool aiSettingsSFDChanged(const CEGUI::EventArgs &e);
	bool aiSettingsFFDChanged(const CEGUI::EventArgs &e);
	bool aiSettingsDFDChanged(const CEGUI::EventArgs &e);
	bool aiSettingsAISChanged(const CEGUI::EventArgs &e);
	
	bool physSettingsOnBtnChanged(const CEGUI::EventArgs &e);
	bool physSettingsOffChanged(const CEGUI::EventArgs &e);

	bool arenaSizeXsliderChanged(const CEGUI::EventArgs &e);
	bool arenaSizeYsliderChanged(const CEGUI::EventArgs &e);

	bool arenaSizeXChanged(const CEGUI::EventArgs &e);
	bool arenaSizeYChanged(const CEGUI::EventArgs &e);

	bool doorsCntChanged(const CEGUI::EventArgs &e);
	bool roomSizeMaxChanged(const CEGUI::EventArgs &e);
	bool furnitureEnableChanged(const CEGUI::EventArgs &e);
	bool threeDSettingsEnableChanged(const CEGUI::EventArgs &e);

	//-----------FUNCTIONS------------//

	void IntToString(int&, CEGUI::String&);
	float calcSliderValue(bool, float);
	void updateRoomSize(int);
	int checkMinPercentRoomSize(int, int);

	//-----------VARIABLES------------//

	char keyBuffer;
	bool aiSettingsBtns[2];
	bool physSettingsBtns[2];
	bool ingameMenuAlreadyLoaded;

//--------------------------CONSTANTS-----------------------------//

	static const int ARENA_SIZE_MAX_VALUE = 500;
	static const int ARENA_SIZE_MIN_VALUE = 3;

	static const int DOOR_CNT_MAX = 4;
	static const int DOOR_CNT_MIN = 1;

	static const int MAX_ROOM_SIZE = 100; //%, OLD VALUE = 998001;
	static const int MIN_ROOM_SIZE = 1; //meters, OLD VALUE = 16;

	static const int MIN_ROOM_WIDTH = 14;

	static const int AI_SFR_MIN = 35;	// Slow Flee Radius
	static const int AI_SFR_MAX = 100;
	static const int AI_SFR_CURR = 50;

	static const int AI_FFR_MIN = 5;	// Fast Flee Radius
	static const int AI_FFR_MAX = 30;
	static const int AI_FFR_CURR = 10;

	static const int AI_SFD_MIN = 5;	// Slow Flee Distance
	static const int AI_SFD_MAX = 30;
	static const int AI_SFD_CURR = 20;

	static const int AI_FFD_MIN = 1;	// Fast Flee Distance
	static const int AI_FFD_MAX = 10;
	static const int AI_FFD_CURR = 3;

	static const int AI_DFD_MIN = 50;	// Don't Flee Distance
	static const int AI_DFD_MAX = 200;
	static const int AI_DFD_CURR = 100;

	static const int AI_AIS_MIN = 50;	// AI Speed (%, range 50% to 400%)
	static const int AI_AIS_MAX = 400;
	static const int AI_AIS_CURR = 100;

//------------------------CEGUI POINTERS--------------------------//

	CEGUI::OgreRenderer* mRenderer;
	CEGUI::WindowManager *Wmgr;

//------------------------CEGUI OBJECTS---------------------------//

//////////////MAIN MENU OBJECTS/////////////////

	CEGUI::Window *mainMenuRoot;	
	CEGUI::Window* mainMenuRootWindow;

	CEGUI::Window* infoBox;

	CEGUI::Window* arenaSizeWindow;
	CEGUI::Window* roomSizeMaxWindow;
	CEGUI::Window* doorCntWindow;
	CEGUI::Window* furnitureWindow;

	CEGUI::Window* aiSettingsCustomWindow;
	CEGUI::Window* aiSettingsSFRWindow;
	CEGUI::Window* aiSettingsFFRWindow;
	CEGUI::Window* aiSettingsSFDWindow;
	CEGUI::Window* aiSettingsFFDWindow;
	CEGUI::Window* aiSettingsDFDWindow;
	CEGUI::Window* aiSettingsAISWindow;

	CEGUI::PushButton* quitBtn;
	CEGUI::PushButton* launchBtn;

	//Input fields for the 3D settings
	CEGUI::Checkbox* furnitureEnable;
	CEGUI::Checkbox* threeDSettingsEnable;

	CEGUI::Spinner* doorsCnt;
	CEGUI::Spinner* roomSizeMax;
	CEGUI::Spinner* arenaSizeX;
	CEGUI::Spinner* arenaSizeY;

	CEGUI::Slider* arenaSizeXslider;
	CEGUI::Slider* arenaSizeYslider;

	//Input for Ai settings
	CEGUI::Checkbox* aiSettingsCustomOn;

	CEGUI::RadioButton* aiSettingsOnBtn;
	CEGUI::RadioButton* aiSettingsOffBtn;

	CEGUI::Spinner* aiSettingsSFR;
	CEGUI::Spinner* aiSettingsFFR;
	CEGUI::Spinner* aiSettingsSFD;
	CEGUI::Spinner* aiSettingsFFD;
	CEGUI::Spinner* aiSettingsDFD;
	CEGUI::Spinner* aiSettingsAIS;

	CEGUI::RadioButton* physSettingsOnBtn;
	CEGUI::RadioButton* physSettingsOffBtn;

/////////////INGAME MENU OBJECTS//////////////

	CEGUI::Window *ingameMenuRoot;

	CEGUI::Window* ingameMenuRootWindow;
	CEGUI::Window* ingameMenuWindow;
	CEGUI::Window* GameFinishedWindow;

	//Buttons for the in game Menu
	CEGUI::PushButton* inGameQuitBtn;
	CEGUI::PushButton* inGameReturnBtn;
	CEGUI::PushButton* inGameCloseBtn;

	//Buttons for the GameFinished Menu
	CEGUI::PushButton* gameFinishedQuitBtn;
	CEGUI::PushButton* gameFinishedReturnBtn;

	CEGUI::Texture* mainMenuImageTex;
	CEGUI::Imageset* mainMenuImageSet;
	CEGUI::DefaultWindow* mainMenuImageWindow;
};

#endif // #ifndef __OgreCEGUI_h_
