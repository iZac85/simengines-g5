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

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// OIS::KeyListener
	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

	// OIS::MouseListener
	virtual bool mouseMoved( const OIS::MouseEvent &arg );
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//-----------VARIABLES------------//
	struct settings
	{
		int aiSettings;
		int physSettings;
		bool threeDSettingsActive;
		int threeDsettingsDoorCnt;
		bool threeDsettingsFurnitureEn;
		int threeDsettingsMaxRoomSize;
		int threeDsettingsMinRoomSize;
		int threeDSettingsArenaSizeX;
		int threeDSettingsArenaSizeY;
	};

	settings extensionSettings;

protected:

	//-----------BUTTON FUNCTIONS------------//

	bool quit(const CEGUI::EventArgs &e);
	bool launchDemo(const CEGUI::EventArgs &e);

	bool aiSettingsOp1BtnChanged(const CEGUI::EventArgs &e);
	bool aiSettingsOp2BtnChanged(const CEGUI::EventArgs &e);
	bool aiSettingsOp3BtnChanged(const CEGUI::EventArgs &e);

	bool physSettingsOp1BtnChanged(const CEGUI::EventArgs &e);
	bool physSettingsOp2BtnChanged(const CEGUI::EventArgs &e);
	bool physSettingsOp3BtnChanged(const CEGUI::EventArgs &e);

	bool threeDSettingsOnBtnChanged(const CEGUI::EventArgs &e);
	bool threeDSettingsOffBtnChanged(const CEGUI::EventArgs &e);

	bool arenaSizeXsliderChanged(const CEGUI::EventArgs &e);
	bool arenaSizeYsliderChanged(const CEGUI::EventArgs &e);

	bool arenaSizeXChanged(const CEGUI::EventArgs &e);
	bool arenaSizeYChanged(const CEGUI::EventArgs &e);

	bool doorsCntChanged(const CEGUI::EventArgs &e);
	bool roomSizeMaxChanged(const CEGUI::EventArgs &e);
	bool roomSizeMinChanged(const CEGUI::EventArgs &e);
	bool furnitureEnableChanged(const CEGUI::EventArgs &e);

	//-----------FUNCTIONS------------//

	void floatToString(float&, CEGUI::String&);
	float calcSliderValue(bool, float);
	void updateRoomSize(int, int);

	//-----------VARIABLES------------//

	char keyBuffer;
	bool aiSettingsBtns[3];
	bool physSettingsBtns[3];

	//-----------CONSTANTS------------//

	static const int ARENA_SIZE_MAX_VALUE = 999;
	static const int ARENA_SIZE_MIN_VALUE = 30;

	static const int DOOR_CNT_MAX = 4;
	static const int DOOR_CNT_MIN = 2;

	static const int MAX_ROOM_SIZE = 100; //% OLD VALUE = 998001;
	static const int MIN_ROOM_SIZE = 1; //% OLD VALUE = 16;

	//-----------CEGUI POINTERS------------//

	CEGUI::OgreRenderer* mRenderer;
	CEGUI::WindowManager *Wmgr;

	//-----------CEGUI OBJECTS------------//

	CEGUI::Window* infoBox;
	CEGUI::Window* rootWindow;
	CEGUI::Window* arenaSizeWindow;
	CEGUI::Window* roomSizeWindow;
	CEGUI::Window* doorCntWindow;
	CEGUI::Window* furnitureWindow;

	CEGUI::Checkbox* furnitureEnable;

	CEGUI::Spinner* doorsCnt;
	CEGUI::Spinner* roomSizeMax;
	CEGUI::Spinner* roomSizeMin;
	CEGUI::Spinner* arenaSizeX;
	CEGUI::Spinner* arenaSizeY;

	CEGUI::Slider* arenaSizeXslider;
	CEGUI::Slider* arenaSizeYslider;

	CEGUI::PushButton* quitBtn;
	CEGUI::PushButton* launchBtn;

	CEGUI::RadioButton* aiSettingsOp1Btn;
	CEGUI::RadioButton* aiSettingsOp2Btn;
	CEGUI::RadioButton* aiSettingsOp3Btn;
	CEGUI::RadioButton* physSettingsOp1Btn;
	CEGUI::RadioButton* physSettingsOp2Btn;
	CEGUI::RadioButton* physSettingsOp3Btn;
	CEGUI::RadioButton* threeDSettingsOnBtn;
	CEGUI::RadioButton* threeDSettingsOffBtn;

};

#endif // #ifndef __OgreCEGUI_h_
