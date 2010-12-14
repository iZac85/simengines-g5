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
	//CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
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

	//load in a file, replace with your own file
	//mainMenuImageTex = mRenderer->createTexture("logo.png", "Imagesets");

	////create a new imageset with an image that spans the entire texture
	//mainMenuImageSet = & CEGUI::ImagesetManager::getSingleton().createFromImageFile( "logo", "logo.png", "Imagesets" );

	//mainMenuImageSet = CEGUI::ImagesetManager::getSingleton().create((CEGUI::utf8*)"MainMenuImageSet",mainMenuImageTex);
	//mainMenuImageSet.defineImage("BaseImage", CEGUI::Point(0.0f, 0.0f), CEGUI::Size(guiTex.getSize().d_width, guiTex.getSize().d_height), CEGUI::Point(0.0f, 0.0f));

	//mainMenuImageWindow = (CEGUI::DefaultWindow*)Wmgr->getWindow("OgreCEGUI/Image");
	//mainMenuImageWindow->setProperty("Image", "BaseImage");

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

	//Input field for setting the minimum room size (in % of the arena size), limited to between 1-100.
	//roomSizeMin->setMaximumValue(ARENA_SIZE_MAX_VALUE);
	//roomSizeMin->setMinimumValue(MIN_ROOM_SIZE);
	//roomSizeMin->setCurrentValue(MIN_ROOM_SIZE);

	//Store the init values in extensionSettings 
	int arenaArea = extensionSettings.threeDSettingsArenaSizeX*extensionSettings.threeDSettingsArenaSizeY;
	int currRoomSizeMax = roomSizeMax->getCurrentValue();
	extensionSettings.threeDsettingsMaxRoomSize = (float(currRoomSizeMax)/100) * (arenaArea); //Convert arena area to dm
	//extensionSettings.threeDsettingsMinRoomSize = MIN_ROOM_WIDTH;

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
	//roomSizeMinWindow->setVisible(false);
	doorCntWindow->setVisible(false);
	furnitureWindow->setVisible(false);
	extensionSettings.threeDSettingsActive = false;

	//Set the "off" value as default for 3D settings
	threeDSettingsEnable->setSelected(false); 

	//Load the radiobuttons for AI settings and connect the events of each button changing to their corresponding functions (aiSettingsOpXBtnChanged), X=1,2,3
	aiSettingsOp1Btn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOp1");
	aiSettingsOp1Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOp1BtnChanged, this));
	aiSettingsOp2Btn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOp2");
	aiSettingsOp2Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOp2BtnChanged, this));
	aiSettingsOp3Btn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOp3");
	aiSettingsOp3Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOp3BtnChanged, this));

	//Set the "Easy" value as default for AI settings
	aiSettingsOp2Btn -> setSelected(true);
	extensionSettings.aiSettings = 1;

	//Load the radiobuttons for Physics settings and connect the events of each button changing to their corresponding functions (physSettingsOpXBtnChanged), X=1,2,3
	physSettingsOp1Btn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOp1");
	physSettingsOp1Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOp1BtnChanged, this));
	physSettingsOp2Btn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOp2");
	physSettingsOp2Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOp2BtnChanged, this));
	physSettingsOp3Btn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOp3");
	physSettingsOp3Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOp3BtnChanged, this));

	//Set the "Arcade" value as default for Physics settings
	physSettingsOp2Btn -> setSelected(true);
	extensionSettings.physSettings = 1;
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
	//Wmgr->destroyWindow(mainMenuRootWindow);
	mainMenuRootWindow->setVisible(false);
	CEGUI::MouseCursor::getSingleton().hide();
	/*CEGUI::String aiSettingsString;
	if (aiSettingsBtns[0] == 1) 
	{
	aiSettingsString = "Hard";
	extensionSettings.aiSettings = 2;
	}
	else if (aiSettingsBtns[1] == 1)
	{
	aiSettingsString = "Easy";
	extensionSettings.aiSettings = 1;
	}
	else 
	{
	aiSettingsString = "off";
	extensionSettings.aiSettings = 0;
	}

	CEGUI::String physSettingsString;
	if (physSettingsBtns[0] == 1) 
	{
	physSettingsString = "Realistic";
	extensionSettings.physSettings = 2;
	}
	else if (physSettingsBtns[1] == 1) 
	{
	physSettingsString = "Arcade";
	extensionSettings.physSettings = 1;
	}
	else 
	{
	physSettingsString = "off";
	extensionSettings.physSettings = 0;
	}

	CEGUI::String threeDSettings;
	if(extensionSettings.threeDSettingsActive == false) threeDSettings = "off";
	else
	{
	CEGUI::String furnitureEn;
	if (extensionSettings.threeDsettingsFurnitureEn == true) furnitureEn = "Activate";
	else furnitureEn = "Inactivate";

	CEGUI::String doorCntString;
	float doorCntFloat = float(extensionSettings.threeDsettingsDoorCnt);
	floatToString(doorCntFloat, doorCntString);

	CEGUI::String maxRoomSizeString;
	float maxRoomSizeFloat = float(extensionSettings.threeDsettingsMaxRoomSize);
	floatToString(maxRoomSizeFloat, maxRoomSizeString);

	CEGUI::String minRoomSizeString;
	float minRoomSizeFloat = float(extensionSettings.threeDsettingsMinRoomSize);
	floatToString(minRoomSizeFloat, minRoomSizeString);

	CEGUI::String arenaSizeXString;
	float arenaSizeXfloat = float(extensionSettings.threeDSettingsArenaSizeX);
	floatToString(arenaSizeXfloat, arenaSizeXString);

	CEGUI::String arenaSizeYString;
	float arenaSizeYfloat = float(extensionSettings.threeDSettingsArenaSizeY);
	floatToString(arenaSizeYfloat, arenaSizeYString);

	threeDSettings = "Arena size = " + arenaSizeXString + " x " + arenaSizeYString + ", Room size: MIN = " + minRoomSizeString + ", MAX = " + maxRoomSizeString + ", DoorsCnt = " + doorCntString + ", Furniture = " + furnitureEn;
	}
	infoBox->setText("Demo launch with AI: " + aiSettingsString + ", Physics: " + physSettingsString + ", 3D settings: " + threeDSettings);*/
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
//bool OgreCEGUI::roomSizeMinChanged(const CEGUI::EventArgs &e)
//{
//	int currentMinValue = roomSizeMin->getCurrentValue();
//	int arenaSizeXval = arenaSizeX->getCurrentValue();
//	int arenaSizeYval = arenaSizeY->getCurrentValue();
//	if (currentMinValue > arenaSizeXval || currentMinValue > arenaSizeYval)
//	{
//		if (arenaSizeXval < arenaSizeYval) currentMinValue = arenaSizeXval;
//		else currentMinValue = arenaSizeYval;
//		roomSizeMin->setCurrentValue(currentMinValue);
//	}
//	updateRoomSize(0, currentMinValue);
//	return true;
//}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeXChanged(const CEGUI::EventArgs &e)
{
	//int currentMinRoomValue = roomSizeMin->getCurrentValue();
	int currentMaxRoomValue = roomSizeMax->getCurrentValue();
	float currentSpinnerValue = (arenaSizeX->getCurrentValue());
	float arenaSizeYCurrVal = (arenaSizeY->getCurrentValue());
	float newSpinnerValue = calcSliderValue(false, currentSpinnerValue);
	arenaSizeXslider->setCurrentValue(newSpinnerValue);
	extensionSettings.threeDSettingsArenaSizeX = int(currentSpinnerValue * 10);
	/*if (currentMinRoomValue > currentSpinnerValue)
	{
	currentMinRoomValue = currentSpinnerValue;
	roomSizeMin->setCurrentValue(currentMinRoomValue);
	}*/
	int lowestMaxRoomSize = checkMinPercentRoomSize(currentSpinnerValue, arenaSizeYCurrVal);
	if (currentMaxRoomValue < lowestMaxRoomSize) roomSizeMax->setCurrentValue(lowestMaxRoomSize);
	//updateRoomSize(0, roomSizeMin->getCurrentValue());
	updateRoomSize(roomSizeMax->getCurrentValue());
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeYChanged(const CEGUI::EventArgs &e)
{
	//int currentMinRoomValue = roomSizeMin->getCurrentValue();
	int currentMaxRoomValue = roomSizeMax->getCurrentValue();
	float currentSpinnerValue = (arenaSizeY->getCurrentValue());
	float arenaSizeXCurrVal = (arenaSizeX->getCurrentValue());
	float newSpinnerValue = calcSliderValue(false, currentSpinnerValue);
	arenaSizeYslider->setCurrentValue(newSpinnerValue);
	extensionSettings.threeDSettingsArenaSizeY = int(currentSpinnerValue * 10);
	/*if (currentMinRoomValue > currentSpinnerValue)
	{
	currentMinRoomValue = currentSpinnerValue;
	roomSizeMin->setCurrentValue(currentMinRoomValue);
	}*/
	int lowestMaxRoomSize = checkMinPercentRoomSize(arenaSizeXCurrVal, currentSpinnerValue);
	if (currentMaxRoomValue < lowestMaxRoomSize) roomSizeMax->setCurrentValue(lowestMaxRoomSize);
	//updateRoomSize(0, roomSizeMin->getCurrentValue());
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
		//roomSizeMinWindow->setVisible(true);
		doorCntWindow->setVisible(true);
		furnitureWindow->setVisible(true);
	}
	else
	{
		arenaSizeWindow->setVisible(false);
		extensionSettings.threeDSettingsActive = false;
		roomSizeMaxWindow->setVisible(false);
		//roomSizeMinWindow->setVisible(false);
		doorCntWindow->setVisible(false);
		furnitureWindow->setVisible(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsOp1BtnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsOp1Btn->isSelected())
	{
		aiSettingsBtns[0] = true;
		extensionSettings.aiSettings = 2;
	}
	else
	{
		aiSettingsBtns[0] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsOp2BtnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsOp2Btn->isSelected())
	{
		aiSettingsBtns[1] = true;
		extensionSettings.aiSettings = 1;
	}
	else
	{
		aiSettingsBtns[1] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsOp3BtnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsOp3Btn->isSelected())
	{
		aiSettingsBtns[2] = true;
		extensionSettings.aiSettings = 0;
	}
	else
	{
		aiSettingsBtns[2] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::physSettingsOp1BtnChanged(const CEGUI::EventArgs &e)
{
	if (physSettingsOp1Btn->isSelected())
	{
		physSettingsBtns[0] = true;
		extensionSettings.physSettings = 2;
	}
	else
	{
		physSettingsBtns[0] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::physSettingsOp2BtnChanged(const CEGUI::EventArgs &e)
{
	if (physSettingsOp2Btn->isSelected())
	{
		physSettingsBtns[1] = true;
		extensionSettings.physSettings = 1;
	}
	else
	{
		physSettingsBtns[1] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::physSettingsOp3BtnChanged(const CEGUI::EventArgs &e)
{
	if (physSettingsOp3Btn->isSelected())
	{
		physSettingsBtns[2] = true;
		extensionSettings.physSettings = 0;
	}
	else
	{
		physSettingsBtns[2] = false;
	}
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
void OgreCEGUI::floatToString(float &numberFloat, CEGUI::String &numberString)
{
	int number = int(numberFloat);
	std::string stringHundredThousands;
	std::string stringTenThousands;
	std::string stringThousands;
	std::string stringHundreds;
	std::string stringTens;
	std::string stringOnes;
	int hundredThousands = number/100000;
	int tenThousands = number/10000 - hundredThousands*10;
	int thousands = number/1000 - tenThousands*10 - hundredThousands*100;
	int hundreds = number/100 - thousands*10 - tenThousands*100 - hundredThousands*1000;
	int tens = number/10 - hundreds*10 - thousands*100 - tenThousands*1000 - hundredThousands*10000;
	int ones = (number -= (tens*10 + hundreds*100 + thousands*1000 + tenThousands*10000 + hundredThousands*100000));

	if (hundredThousands == 0) stringHundredThousands = ' ';
	else if (hundredThousands == 1) stringHundredThousands = '1';
	else if (hundredThousands == 2) stringHundredThousands = '2';
	else if (hundredThousands == 3) stringHundredThousands = '3';
	else if (hundredThousands == 4) stringHundredThousands = '4';
	else if (hundredThousands == 5) stringHundredThousands = '5';
	else if (hundredThousands == 6) stringHundredThousands = '6';
	else if (hundredThousands == 7) stringHundredThousands = '7';
	else if (hundredThousands == 8) stringHundredThousands = '8';
	else stringHundredThousands = '9';

	if (tenThousands == 0 && hundredThousands == 0) stringTenThousands = ' ';
	else if (tenThousands == 0) stringTenThousands = '0';
	else if (tenThousands == 1) stringTenThousands = '1';
	else if (tenThousands == 2) stringTenThousands = '2';
	else if (tenThousands == 3) stringTenThousands = '3';
	else if (tenThousands == 4) stringTenThousands = '4';
	else if (tenThousands == 5) stringTenThousands = '5';
	else if (tenThousands == 6) stringTenThousands = '6';
	else if (tenThousands == 7) stringTenThousands = '7';
	else if (tenThousands == 8) stringTenThousands = '8';
	else stringTenThousands = '9';

	if (thousands == 0 && tenThousands == 0 && hundredThousands == 0) stringThousands = ' ';
	else if (thousands == 0) stringThousands = '0';
	else if (thousands == 1) stringThousands = '1';
	else if (thousands == 2) stringThousands = '2';
	else if (thousands == 3) stringThousands = '3';
	else if (thousands == 4) stringThousands = '4';
	else if (thousands == 5) stringThousands = '5';
	else if (thousands == 6) stringThousands = '6';
	else if (thousands == 7) stringThousands = '7';
	else if (thousands == 8) stringThousands = '8';
	else stringThousands = '9';
	
	if (hundreds == 0 && thousands == 0 && tenThousands == 0 && hundredThousands == 0) stringHundreds = ' ';
	else if (hundreds == 0) stringHundreds = '0';
	else if (hundreds == 1) stringHundreds = '1';
	else if (hundreds == 2) stringHundreds = '2';
	else if (hundreds == 3) stringHundreds = '3';
	else if (hundreds == 4) stringHundreds = '4';
	else if (hundreds == 5) stringHundreds = '5';
	else if (hundreds == 6) stringHundreds = '6';
	else if (hundreds == 7) stringHundreds = '7';
	else if (hundreds == 8) stringHundreds = '8';
	else stringHundreds = '9';

	if (tens == 0 && hundreds == 0 && thousands == 0 && tenThousands == 0 && hundredThousands == 0) stringTens = ' ';
	else if (tens == 0) stringTens = '0';
	else if (tens == 1) stringTens = '1';
	else if (tens == 2) stringTens = '2';
	else if (tens == 3) stringTens = '3';
	else if (tens == 4) stringTens = '4';
	else if (tens == 5) stringTens = '5';
	else if (tens == 6) stringTens = '6';
	else if (tens == 7) stringTens = '7';
	else if (tens == 8) stringTens = '8';
	else stringTens = '9';
	
	if (ones == 0) stringOnes = '0';
	else if (ones == 1) stringOnes = '1';
	else if (ones == 2) stringOnes = '2';
	else if (ones == 3) stringOnes = '3';
	else if (ones == 4) stringOnes = '4';
	else if (ones == 5) stringOnes = '5';
	else if (ones == 6) stringOnes = '6';
	else if (ones == 7) stringOnes = '7';
	else if (ones == 8) stringOnes = '8';
	else stringOnes = '9';

	     if (hundredThousands == 0 && tenThousands == 0 && thousands == 0 && hundreds == 0 && tens == 0) numberString = stringOnes;
	else if (hundredThousands == 0 && tenThousands == 0 && thousands == 0 && hundreds == 0) numberString = stringTens + stringOnes;
	else if (hundredThousands == 0 && tenThousands == 0 && thousands == 0) numberString = stringHundreds + stringTens + stringOnes;
	else if (hundredThousands == 0 && tenThousands == 0) numberString = stringThousands + stringHundreds + stringTens + stringOnes;
	else if (hundredThousands == 0) numberString = stringTenThousands + stringThousands + stringHundreds + stringTens + stringOnes;
	else   numberString = stringHundredThousands + stringTenThousands + stringThousands + stringHundreds + stringTens + stringOnes;
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::updateRoomSize(int currentVal)
{
	int arenaSizeXval = extensionSettings.threeDSettingsArenaSizeX;
	int arenaSizeYval = extensionSettings.threeDSettingsArenaSizeY;
	int arenaArea = arenaSizeXval * arenaSizeYval;
	extensionSettings.threeDsettingsMaxRoomSize = (float(currentVal)/100) * (arenaArea);
	//if (max_min == 1) 
	//else if (max_min == 0)
	//{
	//	if (currentVal == 1) currentVal = MIN_ROOM_WIDTH;
	//	currentVal = currentVal*10;	//Convert the meters into decimeters
	//	extensionSettings.threeDsettingsMinRoomSize = currentVal;
	//}
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
//-------------------------------------------------------------------------------------
/*
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
//-------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        OgreCEGUI app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
*/