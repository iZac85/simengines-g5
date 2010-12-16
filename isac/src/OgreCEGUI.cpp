/*
-----------------------------------------------------------------------------
Filename:    OgreCEGUI.cpp
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
#include "OgreCEGUI.h"
//-------------------------------------------------------------------------------------
OgreCEGUI::OgreCEGUI(void)
	: showMainMenu(false),
	ingameMenuAlreadyLoaded(false)
{
}
//-------------------------------------------------------------------------------------
OgreCEGUI::~OgreCEGUI(void)
{
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
 
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
    mInputManager = OIS::InputManager::createInputSystem( pl );
 
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));
 
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
 
    //Set initial mouse clipping size
    windowResized(mWindow);
 
    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
 
    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed()) return false;
 
    if(mShutDown) return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
 
    return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::keyPressed( const OIS::KeyEvent &arg, bool gameActive )
{
	if (arg.key == OIS::KC_ESCAPE && ingameMenuVisible && gameActive)
	{
		ingameMenuRootWindow->setVisible(false);
		ingameMenuVisible = false;
		CEGUI::MouseCursor::getSingleton().hide();
		return true;
	}
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(arg.key);
	sys.injectChar(arg.text);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::keyReleased( const OIS::KeyEvent &arg )
{
    CEGUI::System::getSingleton().injectKeyUp(arg.key);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::mouseMoved( const OIS::MouseEvent &arg )
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);

	// Scroll wheel.
	if (arg.state.Z.rel)
		sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	return true;
}
//-------------------------------------------------------------------------------------
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
	return true;
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::createScene(void)
{
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	Wmgr = CEGUI::WindowManager::getSingletonPtr();

	//------------------------LOAD CEGUI RESOURCES---------------------//
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");
	CEGUI::SchemeManager::getSingleton().create("AquaLookSkin.scheme");

	CEGUI::System::getSingleton().setDefaultMouseCursor("AquaLook", "MouseArrow");

	/////////////////////////INGAME-MENU//////////////////////////

	//-------LOAD THE LAYOUT OF THE MENU---------//
	try
	{
		ingameMenuRoot = Wmgr->getSingleton().loadWindowLayout("IngameMenu.layout");
	}
	catch(CEGUI::Exception &e)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getMessage().c_str(), "Error Loading Layout!");
	}

	//----LOAD MENU OBJECTS AND CONNECT EVENTS----//

	//Load in game root window
	ingameMenuRootWindow = (CEGUI::Window*)Wmgr->getWindow("IngameMenuRoot");
	ingameMenuWindow = (CEGUI::Window*)Wmgr->getWindow("IngameMenu");

	//Load buttons on the in game menu
	inGameQuitBtn = (CEGUI::PushButton*)Wmgr->getWindow("InGameQuitBtn");
	inGameQuitBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreCEGUI::inGameQuitBtnClicked, this));
	inGameReturnBtn = (CEGUI::PushButton*)Wmgr->getWindow("InGameReturnBtn");
	inGameReturnBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreCEGUI::inGameReturnBtnClicked, this));
	inGameCloseBtn = (CEGUI::PushButton*)Wmgr->getWindow("InGameCloseBtn");
	inGameCloseBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreCEGUI::inGameCloseBtnClicked, this));

	//////////////////////GAME-FINISHED-MENU///////////////////////

	//Load game finished root window
	GameFinishedWindow = (CEGUI::Window*)Wmgr->getWindow("GameFinished");

	//Load game finished buttons
	gameFinishedQuitBtn = (CEGUI::PushButton*)Wmgr->getWindow("GameFinishedQuitBtn");
	gameFinishedQuitBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreCEGUI::inGameQuitBtnClicked, this));
	gameFinishedReturnBtn = (CEGUI::PushButton*)Wmgr->getWindow("GameFinishedReturnBtn");
	gameFinishedReturnBtn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreCEGUI::inGameReturnBtnClicked, this));

	GameFinishedWindow->setVisible(false);

	///////////////////////////MAIN-MENU///////////////////////////

	//-------LOAD THE LAYOUT OF THE MENU---------//
	try
	{
		mainMenuRoot = Wmgr->getSingleton().loadWindowLayout("OgreCEGUI.layout");
		CEGUI::System::getSingleton().setGUISheet(mainMenuRoot);
	}
	catch(CEGUI::Exception &e)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getMessage().c_str(), "Error Loading Layout!");
	}

	//----LOAD MENU OBJECTS AND CONNECT EVENTS----//

	//Load main menu root window
	mainMenuRootWindow = (CEGUI::Window*)Wmgr->getWindow("Root");

	//Load main menu buttons
	quitBtn = (CEGUI::PushButton*)Wmgr->getWindow("OgreCEGUI/quitBtn");
	quitBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OgreCEGUI::quit, this));

	launchBtn = (CEGUI::PushButton*)Wmgr->getWindow("OgreCEGUI/launchBtn");
	launchBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OgreCEGUI::launchDemo, this));

	//Load infobox
	infoBox = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/infoBox");

	//Load text fields for 3D settings
	arenaSizeWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/ArenaSize");
	roomSizeMaxWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/RoomSizeMax");
	doorCntWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/DoorCnt");
	furnitureWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/FurnitureEn");

	//Load the sliders that affect the arena size settings and connect them to an event subscribing function each.
	arenaSizeXslider = (CEGUI::Slider*)Wmgr->getWindow("3DSettingsArenaSizeXSlider");
	arenaSizeXslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::arenaSizeXsliderChanged, this));
	
	arenaSizeYslider = (CEGUI::Slider*)Wmgr->getWindow("3DSettingsArenaSizeYSlider");
	arenaSizeYslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::arenaSizeYsliderChanged, this));

	//Load the edit boxes that affect the arena size settings and connect them to an event subscribing function each.
	arenaSizeX = (CEGUI::Spinner*)Wmgr->getWindow("3DSettingsASizeX");
	arenaSizeX->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::arenaSizeXChanged, this));
	
	arenaSizeY = (CEGUI::Spinner*)Wmgr->getWindow("3DSettingsASizeY");
	arenaSizeY->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::arenaSizeYChanged, this));

	//Load additional input fields for 3D settings
	doorsCnt = (CEGUI::Spinner*)Wmgr->getWindow("3DSettingsDoorCnt");
	doorsCnt->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::doorsCntChanged, this));
	
	roomSizeMax = (CEGUI::Spinner*)Wmgr->getWindow("3DSettingsRoomSizeMax");
	roomSizeMax->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::roomSizeMaxChanged, this));

	furnitureEnable = (CEGUI::Checkbox*)Wmgr->getWindow("3DSettingsFurnitureEn");
	furnitureEnable->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::furnitureEnableChanged, this));
	
	threeDSettingsEnable = (CEGUI::Checkbox*)Wmgr->getWindow("3DSettingsEnable");
	threeDSettingsEnable->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::threeDSettingsEnableChanged, this));

	//----Set init values and value limits for the input fields----//
	//Input fields for setting the arena size as x and y, ARENA_SIZE_MIN_VALUE = 30, ARENA_SIZE_MAX_VALUE = 999
	arenaSizeX->setMaximumValue(ARENA_SIZE_MAX_VALUE);
	arenaSizeX->setMinimumValue(ARENA_SIZE_MIN_VALUE);
	arenaSizeX->setCurrentValue(ARENA_SIZE_MIN_VALUE * 5);
	extensionSettings.threeDSettingsArenaSizeX = (arenaSizeX->getCurrentValue()) * 10;
	arenaSizeY->setMaximumValue(ARENA_SIZE_MAX_VALUE);
	arenaSizeY->setMinimumValue(ARENA_SIZE_MIN_VALUE);
	arenaSizeY->setCurrentValue(ARENA_SIZE_MIN_VALUE * 5);
	extensionSettings.threeDSettingsArenaSizeY = (arenaSizeY->getCurrentValue())  * 10;

	//Input field for setting the maximum room size (in % of the arena size), limited to between 1-100.
	roomSizeMax->setMaximumValue(MAX_ROOM_SIZE);
	roomSizeMax->setMinimumValue(MIN_ROOM_SIZE);
	roomSizeMax->setCurrentValue(MAX_ROOM_SIZE/4);

	//Store the init values in extensionSettings 
	int arenaArea = extensionSettings.threeDSettingsArenaSizeX*extensionSettings.threeDSettingsArenaSizeY;
	int currRoomSizeMax = roomSizeMax->getCurrentValue();
	extensionSettings.threeDsettingsMaxRoomSize = (float(currRoomSizeMax)/100) * (arenaArea); //Convert arena area to dm

	//Input field for setting the number of doors per room limited to between DOOR_CNT_MIN = 2 and DOOR_CNT_MAX = 4
	doorsCnt->setMaximumValue(DOOR_CNT_MAX);
	doorsCnt->setMinimumValue(DOOR_CNT_MIN);
	doorsCnt->setCurrentValue(DOOR_CNT_MIN);
	extensionSettings.threeDsettingsDoorCnt = DOOR_CNT_MIN;

	//Checkbox for furniture should initially be set to false
	furnitureEnable->setSelected(false);
	extensionSettings.threeDsettingsFurnitureEn = false;

	//Hide the fields for 3D settings as default
	arenaSizeWindow->setVisible(false);
	roomSizeMaxWindow->setVisible(false);
	doorCntWindow->setVisible(false);
	furnitureWindow->setVisible(false);
	extensionSettings.threeDSettingsActive = false;

	//Set the "off" value as default for custom 3D settings
	threeDSettingsEnable->setSelected(false); 

	//Load the radiobuttons for AI settings and connect the events of each button changing to their corresponding functions (aiSettingsOpXBtnChanged), X=1,2,3
	aiSettingsOnBtn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOn");
	aiSettingsOnBtn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOnBtnChanged, this));
	aiSettingsOffBtn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOff");
	aiSettingsOffBtn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOffBtnChanged, this));
	
	//Load text fields for AI settings
	aiSettingsCustomWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsCustom");
	aiSettingsSFRWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsSFRWindow");
	aiSettingsFFRWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsFFRWindow");
	aiSettingsSFDWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsSFDWindow");
	aiSettingsFFDWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsFFDWindow");
	aiSettingsDFDWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsDFDWindow");
	aiSettingsAISWindow = (CEGUI::Window*)Wmgr->getWindow("AISettingsAISWindow");

	//Hide the fields for 3D settings as default
	aiSettingsCustomWindow->setVisible(false);
	aiSettingsSFRWindow->setVisible(false);
	aiSettingsFFRWindow->setVisible(false);
	aiSettingsSFDWindow->setVisible(false);
	aiSettingsFFDWindow->setVisible(false);
	aiSettingsDFDWindow->setVisible(false);
	aiSettingsAISWindow->setVisible(false);

	//Load the input fields for AI settings
	aiSettingsCustomOn = (CEGUI::Checkbox*)Wmgr->getWindow("AISettingsCustomOn");
	aiSettingsCustomOn->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsCustomOnChanged, this));
	aiSettingsSFR = (CEGUI::Spinner*)Wmgr->getWindow("AISettingsSFR");
	aiSettingsSFR-> subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsSFRChanged, this));
	aiSettingsFFR = (CEGUI::Spinner*)Wmgr->getWindow("AISettingsFFR");
	aiSettingsFFR-> subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsFFRChanged, this));
	aiSettingsSFD = (CEGUI::Spinner*)Wmgr->getWindow("AISettingsSFD");
	aiSettingsSFD-> subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsSFDChanged, this));
	aiSettingsFFD = (CEGUI::Spinner*)Wmgr->getWindow("AISettingsFFD");
	aiSettingsFFD-> subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsFFDChanged, this));
	aiSettingsDFD = (CEGUI::Spinner*)Wmgr->getWindow("AISettingsDFD");
	aiSettingsDFD-> subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsDFDChanged, this));
	aiSettingsAIS = (CEGUI::Spinner*)Wmgr->getWindow("AISettingsAIS");
	aiSettingsAIS-> subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsAISChanged, this));

	//Set limits and default values for the input fields for AI settings
	aiSettingsSFR->setMinimumValue(AI_SFR_MIN);	// Slow Flee Radius
	aiSettingsSFR->setMaximumValue(AI_SFR_MAX);
	aiSettingsSFR->setCurrentValue(AI_SFR_CURR);
	extensionSettings.aiSettingsSFRVal = AI_SFR_CURR;

	aiSettingsFFR->setMinimumValue(AI_FFR_MIN);	// Fast Flee Radius
	aiSettingsFFR->setMaximumValue(AI_FFR_MAX);
	aiSettingsFFR->setCurrentValue(AI_FFR_CURR);
	extensionSettings.aiSettingsFFRVal = AI_FFR_CURR;

	aiSettingsSFD->setMinimumValue(AI_SFD_MIN);	// Slow Flee Distance
	aiSettingsSFD->setMaximumValue(AI_SFD_MAX);
	aiSettingsSFD->setCurrentValue(AI_SFD_CURR);
	extensionSettings.aiSettingsSFDVal = AI_SFD_CURR;

	aiSettingsFFD->setMinimumValue(AI_FFD_MIN);	// Fast Flee Distance
	aiSettingsFFD->setMaximumValue(AI_FFD_MAX);
	aiSettingsFFD->setCurrentValue(AI_FFD_CURR);
	extensionSettings.aiSettingsFFDVal = AI_FFD_CURR;

	aiSettingsDFD->setMinimumValue(AI_DFD_MIN);	// Don't Flee Distance
	aiSettingsDFD->setMaximumValue(AI_DFD_MAX);
	aiSettingsDFD->setCurrentValue(AI_DFD_CURR);
	extensionSettings.aiSettingsDFDVal = AI_DFD_CURR;

	aiSettingsAIS->setMinimumValue(AI_AIS_MIN);	// AI Speed (%, range 50% to 400%)
	aiSettingsAIS->setMaximumValue(AI_AIS_MAX);
	aiSettingsAIS->setCurrentValue(AI_AIS_CURR);
	extensionSettings.aiSettingsAISVal = float((aiSettingsAIS->getCurrentValue())/100);

	//Set the "Off" value as default for AI settings
	aiSettingsOffBtn -> setSelected(true);
	extensionSettings.aiSettingsOn = false;

	//Load the radiobuttons for Physics settings and connect the events of each button changing to their corresponding functions (physSettingsOpXBtnChanged), X=1,2,3
	physSettingsOnBtn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOn");
	physSettingsOnBtn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOnBtnChanged, this));
	physSettingsOffBtn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOff");
	physSettingsOffBtn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOffChanged, this));
	
	//Set the "Off" value as default for Physics settings
	physSettingsOffBtn -> setSelected(true);
	extensionSettings.physSettingsOn = false;

	//Load input fields for physics settings
	physSettingsGravityWindow = (CEGUI::Window*)Wmgr->getWindow("PhysSettingsGravWindow");
	physSettingsGravityWindow->setVisible(false);

	physSettingsGravityEn = (CEGUI::Checkbox*)Wmgr->getWindow("PhysSettingsGravEn");
	physSettingsGravityEn->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsGravityEnChanged, this));

	//Set Gravity ON as default
	physSettingsGravityEn->setSelected(true);
	extensionSettings.physSettingsGravEn = true;
	}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::quit(const CEGUI::EventArgs &e)
{
	mShutDown = true;
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::launchDemo(const CEGUI::EventArgs &e)
{
	mLaunch = true;
	mainMenuRootWindow->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	return true;
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::ShowMainMenu(void)
{
	CEGUI::System::getSingleton().setGUISheet(mainMenuRoot);
	mainMenuRootWindow->setVisible(true);
	CEGUI::MouseCursor::getSingleton().show();
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::ShowIngameMenu(bool gameActive)
{
	if (gameActive)
	{
		if (ingameMenuAlreadyLoaded) 
		{
			ingameMenuRootWindow->setVisible(true);
			ingameMenuWindow->setVisible(true);
			GameFinishedWindow->setVisible(false);
		}
		else 
		{
			CEGUI::System::getSingleton().setGUISheet(ingameMenuRoot);
			ingameMenuRootWindow->setVisible(true);
			ingameMenuWindow->setVisible(true);
			GameFinishedWindow->setVisible(false);
			ingameMenuAlreadyLoaded = true;
		}
	}
	else
	{
		if (ingameMenuAlreadyLoaded)
		{
			ingameMenuRootWindow->setVisible(true);
			ingameMenuWindow->setVisible(false);
			GameFinishedWindow->setVisible(true);
		}
		else
		{
			CEGUI::System::getSingleton().setGUISheet(ingameMenuRoot);
			ingameMenuRootWindow->setVisible(true);
			ingameMenuWindow->setVisible(false);
			GameFinishedWindow->setVisible(true);
		}
	}
	ingameMenuVisible = true;
	CEGUI::MouseCursor::getSingleton().show();
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::inGameQuitBtnClicked(const CEGUI::EventArgs &e)
{
	mShutDown = true;
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::inGameReturnBtnClicked(const CEGUI::EventArgs &e)
{
	ingameMenuRootWindow->setVisible(false);
	GameFinishedWindow->setVisible(false);
	ingameMenuAlreadyLoaded = false;
	ingameMenuVisible = false;
	showMainMenu = true;
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::inGameCloseBtnClicked(const CEGUI::EventArgs &e)
{
	ingameMenuRootWindow->setVisible(false);
	ingameMenuVisible = false;
	CEGUI::MouseCursor::getSingleton().hide();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::doorsCntChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.threeDsettingsDoorCnt = doorsCnt->getCurrentValue();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::furnitureEnableChanged(const CEGUI::EventArgs &e)
{
	if (furnitureEnable->isSelected()) extensionSettings.threeDsettingsFurnitureEn = true;
	else extensionSettings.threeDsettingsFurnitureEn = false;
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::roomSizeMaxChanged(const CEGUI::EventArgs &e)
{
	int currentMaxValue = roomSizeMax->getCurrentValue();
	int lowestMaxRoomSize = checkMinPercentRoomSize((arenaSizeX->getCurrentValue()), (arenaSizeY->getCurrentValue()));
	if (currentMaxValue < lowestMaxRoomSize) 
	{
		roomSizeMax->setCurrentValue(lowestMaxRoomSize);
		currentMaxValue = lowestMaxRoomSize;
	}
	updateRoomSize(currentMaxValue);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeXChanged(const CEGUI::EventArgs &e)
{
	int currentMaxRoomValue = roomSizeMax->getCurrentValue();
	float currentSpinnerValue = (arenaSizeX->getCurrentValue());
	float arenaSizeYCurrVal = (arenaSizeY->getCurrentValue());
	float newSpinnerValue = calcSliderValue(false, currentSpinnerValue);
	arenaSizeXslider->setCurrentValue(newSpinnerValue);
	extensionSettings.threeDSettingsArenaSizeX = int(currentSpinnerValue * 10);
	int lowestMaxRoomSize = checkMinPercentRoomSize(currentSpinnerValue, arenaSizeYCurrVal);
	if (currentMaxRoomValue < lowestMaxRoomSize) roomSizeMax->setCurrentValue(lowestMaxRoomSize);
	updateRoomSize(roomSizeMax->getCurrentValue());
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeYChanged(const CEGUI::EventArgs &e)
{
	int currentMaxRoomValue = roomSizeMax->getCurrentValue();
	float currentSpinnerValue = (arenaSizeY->getCurrentValue());
	float arenaSizeXCurrVal = (arenaSizeX->getCurrentValue());
	float newSpinnerValue = calcSliderValue(false, currentSpinnerValue);
	arenaSizeYslider->setCurrentValue(newSpinnerValue);
	extensionSettings.threeDSettingsArenaSizeY = int(currentSpinnerValue * 10);
	int lowestMaxRoomSize = checkMinPercentRoomSize(arenaSizeXCurrVal, currentSpinnerValue);
	if (currentMaxRoomValue < lowestMaxRoomSize) roomSizeMax->setCurrentValue(lowestMaxRoomSize);
	updateRoomSize(roomSizeMax->getCurrentValue());
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeXsliderChanged(const CEGUI::EventArgs &e)
{
	float currentSliderValue = (arenaSizeXslider->getCurrentValue());
	float newSliderValue = calcSliderValue(true, currentSliderValue);
	arenaSizeX->setCurrentValue(newSliderValue);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeYsliderChanged(const CEGUI::EventArgs &e)
{
	float currentSliderValue = (arenaSizeYslider->getCurrentValue());
	double newSliderValue = calcSliderValue(true, currentSliderValue);
	arenaSizeY->setCurrentValue(newSliderValue);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::threeDSettingsEnableChanged(const CEGUI::EventArgs &e)
{
	if (threeDSettingsEnable->isSelected())
	{
		arenaSizeWindow->setVisible(true);
		extensionSettings.threeDSettingsActive = true;
		roomSizeMaxWindow->setVisible(true);
		doorCntWindow->setVisible(true);
		furnitureWindow->setVisible(true);
	}
	else
	{
		arenaSizeWindow->setVisible(false);
		extensionSettings.threeDSettingsActive = false;
		roomSizeMaxWindow->setVisible(false);
		doorCntWindow->setVisible(false);
		furnitureWindow->setVisible(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsOnBtnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsOnBtn->isSelected())
	{
		aiSettingsCustomWindow->setVisible(true);
		aiSettingsBtns[0] = true;
		extensionSettings.aiSettingsOn = true;
	}
	else
	{
		aiSettingsCustomWindow->setVisible(false);
		aiSettingsBtns[0] = false;
		extensionSettings.aiSettingsOn = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsOffBtnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsOffBtn->isSelected())
	{
		aiSettingsBtns[1] = true;
		extensionSettings.aiSettingsOn = false;
		aiSettingsSFRWindow->setVisible(false);
		aiSettingsFFRWindow->setVisible(false);
		aiSettingsSFDWindow->setVisible(false);
		aiSettingsFFDWindow->setVisible(false);
		aiSettingsDFDWindow->setVisible(false);
		aiSettingsAISWindow->setVisible(false);
		aiSettingsCustomOn->setSelected(false);
		extensionSettings.aiSettingsCustomOn = false;
	}
	else
	{
		aiSettingsBtns[1] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsCustomOnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsCustomOn->isSelected())
	{
		aiSettingsSFRWindow->setVisible(true);
		aiSettingsFFRWindow->setVisible(true);
		aiSettingsSFDWindow->setVisible(true);
		aiSettingsFFDWindow->setVisible(true);
		aiSettingsDFDWindow->setVisible(true);
		aiSettingsAISWindow->setVisible(true);
		extensionSettings.aiSettingsCustomOn = true;
	}
	else
	{
		aiSettingsSFRWindow->setVisible(false);
		aiSettingsFFRWindow->setVisible(false);
		aiSettingsSFDWindow->setVisible(false);
		aiSettingsFFDWindow->setVisible(false);
		aiSettingsDFDWindow->setVisible(false);
		aiSettingsAISWindow->setVisible(false);
		extensionSettings.aiSettingsCustomOn = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::physSettingsOnBtnChanged(const CEGUI::EventArgs &e)
{
	if (physSettingsOnBtn->isSelected())
	{
		physSettingsBtns[0] = true;
		extensionSettings.physSettingsOn = true;
		physSettingsGravityWindow->setVisible(true);
	}
	else
	{
		physSettingsBtns[0] = false;
		physSettingsGravityWindow->setVisible(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::physSettingsOffChanged(const CEGUI::EventArgs &e)
{
	if (physSettingsOffBtn->isSelected())
	{
		physSettingsBtns[1] = false;
		extensionSettings.physSettingsOn = false;
	}
	else
	{
		physSettingsBtns[1] = true;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::physSettingsGravityEnChanged(const CEGUI::EventArgs &e)
{
	if (physSettingsGravityEn->isSelected())
	{
		extensionSettings.physSettingsGravEn = true;
	}
	else
	{
		extensionSettings.physSettingsGravEn = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsSFRChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.aiSettingsSFRVal = aiSettingsSFR->getCurrentValue();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsFFRChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.aiSettingsFFRVal = aiSettingsFFR->getCurrentValue();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsSFDChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.aiSettingsSFDVal = aiSettingsSFD->getCurrentValue();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsFFDChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.aiSettingsFFDVal = aiSettingsFFD->getCurrentValue();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsDFDChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.aiSettingsDFDVal = aiSettingsDFD->getCurrentValue();
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsAISChanged(const CEGUI::EventArgs &e)
{
	extensionSettings.aiSettingsAISVal = float((aiSettingsAIS->getCurrentValue())/100);
	return true;
}
//-------------------------------------------------------------------------------------
float OgreCEGUI::calcSliderValue(bool sliderToSpinner, float currentValue)
{
	float newValue;
	if (sliderToSpinner)
	{
		return newValue = ARENA_SIZE_MIN_VALUE + ((currentValue) * (ARENA_SIZE_MAX_VALUE - ARENA_SIZE_MIN_VALUE));
	}
	else
	{
		// Interpolate from arena size (between 30 and 999) to slider value (between 0 and 1) ((x - x1)(y2 - y1)) / (x2 - x1)
		return newValue  = (currentValue - ARENA_SIZE_MIN_VALUE)/(ARENA_SIZE_MAX_VALUE - ARENA_SIZE_MIN_VALUE); 
	}
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::updateRoomSize(int currentVal)
{
	int arenaSizeXval = extensionSettings.threeDSettingsArenaSizeX;
	int arenaSizeYval = extensionSettings.threeDSettingsArenaSizeY;
	int arenaArea = arenaSizeXval * arenaSizeYval;
	extensionSettings.threeDsettingsMaxRoomSize = (float(currentVal)/100) * (arenaArea);
}
//-------------------------------------------------------------------------------------
int OgreCEGUI::checkMinPercentRoomSize(int aSizeX, int aSizeY)
{
	int arenaArea = aSizeX * aSizeY * 100;
	for (int i = 1; i <= 100; i++)
	{
		if ((float (i)/100) * (arenaArea) >= 200) return i;
	}
	return 100;
}