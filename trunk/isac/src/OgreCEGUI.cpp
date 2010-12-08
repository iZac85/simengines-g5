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
bool OgreCEGUI::keyPressed( const OIS::KeyEvent &arg )
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	keyBuffer = arg.key;
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

	//------------------------LOAD CEGUI RESOURCES---------------------//
	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	
	//------------------------LOAD THE CEGUI LAYOUT---------------------//
	Wmgr = CEGUI::WindowManager::getSingletonPtr();
	try
	{
		CEGUI::Window *guiRoot = Wmgr->getSingleton().loadWindowLayout("OgreCEGUI.layout");
		CEGUI::System::getSingleton().setGUISheet(guiRoot);
	}
	catch(CEGUI::Exception &e)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, e.getMessage().c_str(), "Error Loading Layout!");
	}

	//------------------------LOAD GUI OBJECTS AND CONNECT EVENTS---------------------//

	//Load root window
	rootWindow = (CEGUI::Window*)Wmgr->getWindow("Root");

	//Load buttons
	quitBtn = (CEGUI::PushButton*)Wmgr->getWindow("OgreCEGUI/quitBtn");
	quitBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OgreCEGUI::quit, this));

	launchBtn = (CEGUI::PushButton*)Wmgr->getWindow("OgreCEGUI/launchBtn");
	launchBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OgreCEGUI::launchDemo, this));

	//Load infobox
	infoBox = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/infoBox");

	//Load text fields for 3D settings
	arenaSizeWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/ArenaSize");
	roomSizeWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/RoomSize");
	doorCntWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/DoorCnt");
	furnitureWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/FurnitureEn");

	//Load the sliders that affect the arena size settings and connect them to an event subscribing function each.
	arenaSizeXslider = (CEGUI::Slider*)Wmgr->getWindow("3DSettingsArenaSizeXSlider");
	arenaSizeXslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::arenaSizeXsliderChanged, this));
	arenaSizeXslider->setRotation(CEGUI::Vector3(0,0,90));
	arenaSizeYslider = (CEGUI::Slider*)Wmgr->getWindow("3DSettingsArenaSizeYSlider");
	arenaSizeYslider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::arenaSizeYsliderChanged, this));
	arenaSizeYslider->setRotation(CEGUI::Vector3(0,0,90));

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
	roomSizeMin = (CEGUI::Spinner*)Wmgr->getWindow("3DSettingsRoomSizeMin");
	roomSizeMin->subscribeEvent(CEGUI::Spinner::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::roomSizeMinChanged, this));
	furnitureEnable = (CEGUI::Checkbox*)Wmgr->getWindow("3DSettingsFurnitureEn");
	furnitureEnable->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::furnitureEnableChanged, this));

	//----Set init values and value limits for the input fields----//

	//Input field for setting the maximum room size (in % of the arena size), limited to between 1-100.
	roomSizeMax->setMaximumValue(MAX_ROOM_SIZE);
	roomSizeMax->setMinimumValue(MIN_ROOM_SIZE);
	roomSizeMax->setCurrentValue(MIN_ROOM_SIZE);
	extensionSettings.threeDsettingsMaxRoomSize = MIN_ROOM_SIZE;

	//Input field for setting the minimum room size (in % of the arena size), limited to between 1-100.
	roomSizeMin->setMaximumValue(MAX_ROOM_SIZE);
	roomSizeMin->setMinimumValue(MIN_ROOM_SIZE);
	roomSizeMin->setCurrentValue(MIN_ROOM_SIZE);
	extensionSettings.threeDsettingsMaxRoomSize = MIN_ROOM_SIZE;

	//Input field for setting the number of doors per room limited to between DOOR_CNT_MIN = 2 and DOOR_CNT_MAX = 4
	doorsCnt->setMaximumValue(DOOR_CNT_MAX);
	doorsCnt->setMinimumValue(DOOR_CNT_MIN);
	doorsCnt->setCurrentValue(DOOR_CNT_MIN);
	extensionSettings.threeDsettingsDoorCnt = DOOR_CNT_MIN;

	//Input fields for setting the arena size as x and y, ARENA_SIZE_MIN_VALUE = 30, ARENA_SIZE_MAX_VALUE = 999
	arenaSizeX->setMaximumValue(ARENA_SIZE_MAX_VALUE);
	arenaSizeX->setMinimumValue(ARENA_SIZE_MIN_VALUE);
	arenaSizeX->setCurrentValue(ARENA_SIZE_MIN_VALUE);
	arenaSizeY->setMaximumValue(ARENA_SIZE_MAX_VALUE);
	arenaSizeY->setMinimumValue(ARENA_SIZE_MIN_VALUE);
	arenaSizeY->setCurrentValue(ARENA_SIZE_MIN_VALUE);

	//Checkbox for furniture should initially be set to false
	furnitureEnable->setSelected(false);
	extensionSettings.threeDsettingsFurnitureEn = false;

	//Hide the fields for 3D settings as default
	arenaSizeWindow->setVisible(false);
	roomSizeWindow->setVisible(false);
	doorCntWindow->setVisible(false);
	furnitureWindow->setVisible(false);
	extensionSettings.threeDSettingsActive = false;

	//Load the radiobuttons for 3D settings and connect the events of each button changing to their corresponding functions (threeDSettingsXBtnChanged), X=On,Off
	threeDSettingsOnBtn = (CEGUI::RadioButton*)Wmgr->getWindow("3DSettingsOn");
	threeDSettingsOnBtn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::threeDSettingsOnBtnChanged, this));
	threeDSettingsOffBtn = (CEGUI::RadioButton*)Wmgr->getWindow("3DSettingsOff");
	threeDSettingsOffBtn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::threeDSettingsOffBtnChanged, this));

	//Set the "off" value as default for 3D settings
	threeDSettingsOffBtn -> setSelected(true); 

	//Load the radiobuttons for AI settings and connect the events of each button changing to their corresponding functions (aiSettingsOpXBtnChanged), X=1,2,3
	aiSettingsOp1Btn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOp1");
	aiSettingsOp1Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOp1BtnChanged, this));
	aiSettingsOp2Btn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOp2");
	aiSettingsOp2Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOp2BtnChanged, this));
	aiSettingsOp3Btn = (CEGUI::RadioButton*)Wmgr->getWindow("AISettingsOp3");
	aiSettingsOp3Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::aiSettingsOp3BtnChanged, this));

	//Set the "off" value as default for AI settings
	aiSettingsOp3Btn -> setSelected(true);

	//Load the radiobuttons for Physics settings and connect the events of each button changing to their corresponding functions (physSettingsOpXBtnChanged), X=1,2,3
	physSettingsOp1Btn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOp1");
	physSettingsOp1Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOp1BtnChanged, this));
	physSettingsOp2Btn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOp2");
	physSettingsOp2Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOp2BtnChanged, this));
	physSettingsOp3Btn = (CEGUI::RadioButton*)Wmgr->getWindow("PhysSettingsOp3");
	physSettingsOp3Btn -> subscribeEvent(CEGUI::RadioButton::EventSelectStateChanged, CEGUI::Event::Subscriber(&OgreCEGUI::physSettingsOp3BtnChanged, this));

	//Set the "off" value as default for Physics settings
	physSettingsOp3Btn -> setSelected(true);
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
	rootWindow->destroy();
	 //Ogre::String aiSettingsString;
	 //if (aiSettingsBtns[0] == 1) 
	 //{
		// aiSettingsString = "Hard";//(aiSettingsOp1Btn->getText());
		// extensionSettings.aiSettings = 2;
	 //}
	 //else if (aiSettingsBtns[1] == 1)
	 //{
		// aiSettingsString = "Easy";//aiSettingsOp2Btn->getText();
		// extensionSettings.aiSettings = 1;
	 //}
	 //else 
	 //{
		// aiSettingsString = "off";//aiSettingsOp3Btn->getText();
		// extensionSettings.aiSettings = 0;
	 //}

	 //Ogre::String physSettingsString;
	 //if (physSettingsBtns[0] == 1) 
	 //{
		// physSettingsString = "Realistic";
		// extensionSettings.physSettings = 2;
	 //}
	 //else if (physSettingsBtns[1] == 1) 
	 //{
		// physSettingsString = "Arcade";
		// extensionSettings.physSettings = 1;
	 //}
	 //else 
	 //{
		// physSettingsString = "off";
		// extensionSettings.physSettings = 0;
	 //}

	 //CEGUI::String threeDSettings;
	 //if(extensionSettings.threeDSettingsActive == false) threeDSettings = "off";
	 //else
	 //{
		// CEGUI::String furnitureEn;
		// if (extensionSettings.threeDsettingsFurnitureEn == true) furnitureEn = "Activate";
		// else furnitureEn = "Inactivate";

		// CEGUI::String doorCntString;
		// float doorCntFloat = float(extensionSettings.threeDsettingsDoorCnt);
		// floatToString(doorCntFloat, doorCntString);

		// CEGUI::String maxRoomSizeString;
		// float maxRoomSizeFloat = float(extensionSettings.threeDsettingsMaxRoomSize);
		// floatToString(maxRoomSizeFloat, maxRoomSizeString);

		// CEGUI::String minRoomSizeString;
		// float minRoomSizeFloat = float(extensionSettings.threeDsettingsMinRoomSize);
		// floatToString(minRoomSizeFloat, minRoomSizeString);

		// CEGUI::String arenaSizeXString;
		// float arenaSizeXfloat = float(extensionSettings.threeDSettingsArenaSizeX);
		// floatToString(arenaSizeXfloat, arenaSizeXString);

		// CEGUI::String arenaSizeYString;
		// float arenaSizeYfloat = float(extensionSettings.threeDSettingsArenaSizeY);
		// floatToString(arenaSizeYfloat, arenaSizeYString);

		// threeDSettings = "Arena size = " + arenaSizeXString + " x " + arenaSizeYString + ", Room size: MIN = " + minRoomSizeString + ", MAX = " + maxRoomSizeString + "DoorsCnt = " + doorCntString + ", Furniture = " + furnitureEn;
	 //}
	 //infoBox->setText("Demo launch with AI: " + aiSettingsString + ", Physics: " + physSettingsString + ", 3D settings: " + threeDSettings);
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
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::roomSizeMaxChanged(const CEGUI::EventArgs &e)
{
	int currentMinValue = roomSizeMin->getCurrentValue();
	int currentMaxValue = roomSizeMax->getCurrentValue();
	if (currentMaxValue < currentMinValue) currentMaxValue = currentMinValue;
	roomSizeMax->setCurrentValue(currentMaxValue);
	int arenaArea = extensionSettings.threeDSettingsArenaSizeX*extensionSettings.threeDSettingsArenaSizeY;
	extensionSettings.threeDsettingsMaxRoomSize = (float(currentMaxValue)/100) * arenaArea;
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::roomSizeMinChanged(const CEGUI::EventArgs &e)
{
	int currentMinValue = roomSizeMin->getCurrentValue();
	int currentMaxValue = roomSizeMax->getCurrentValue();
	if (currentMinValue > currentMaxValue) currentMinValue = currentMaxValue;
	roomSizeMin->setCurrentValue(currentMinValue);
	int arenaArea = extensionSettings.threeDSettingsArenaSizeX*extensionSettings.threeDSettingsArenaSizeY;
	extensionSettings.threeDsettingsMinRoomSize = (float(currentMinValue)/100) * arenaArea;
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeXChanged(const CEGUI::EventArgs &e)
{
	float currentSpinnerValue = (arenaSizeX->getCurrentValue());
	float newSpinnerValue = calcSliderValue(false, currentSpinnerValue);
	arenaSizeXslider->setCurrentValue(newSpinnerValue);
	extensionSettings.threeDSettingsArenaSizeX = int(currentSpinnerValue);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::arenaSizeYChanged(const CEGUI::EventArgs &e)
{
	float currentSpinnerValue = (arenaSizeY->getCurrentValue());
	float newSpinnerValue = calcSliderValue(false, currentSpinnerValue);
	arenaSizeYslider->setCurrentValue(newSpinnerValue);
	extensionSettings.threeDSettingsArenaSizeY = int(currentSpinnerValue);
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
float OgreCEGUI::calcSliderValue(bool sliderToSpinner, float currentValue)
{
	float newValue;
	if (sliderToSpinner)
	{
		return newValue = ARENA_SIZE_MIN_VALUE + ((currentValue) * (ARENA_SIZE_MAX_VALUE - ARENA_SIZE_MIN_VALUE));
	}
	else
	{
		// Interpolate from arena size (between 55 and 999) to slider value (between 0 and 1) ((x - x1)(y2 - y1)) / (x2 - x1)
		return newValue  = (currentValue - ARENA_SIZE_MIN_VALUE)/(ARENA_SIZE_MAX_VALUE - ARENA_SIZE_MIN_VALUE); 
	}
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::threeDSettingsOnBtnChanged(const CEGUI::EventArgs &e)
{
	if (threeDSettingsOnBtn->isSelected())
	{
		arenaSizeWindow->setVisible(true);
		extensionSettings.threeDSettingsActive = true;
		roomSizeWindow->setVisible(true);
		doorCntWindow->setVisible(true);
		furnitureWindow->setVisible(true);
	}
	else
	{
		arenaSizeWindow->setVisible(false);
		extensionSettings.threeDSettingsActive = false;
		roomSizeWindow->setVisible(false);
		doorCntWindow->setVisible(false);
		furnitureWindow->setVisible(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::threeDSettingsOffBtnChanged(const CEGUI::EventArgs &e)
{
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::aiSettingsOp1BtnChanged(const CEGUI::EventArgs &e)
{
	if (aiSettingsOp1Btn->isSelected())
	{
		aiSettingsBtns[0] = true;
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
	}
	else
	{
		physSettingsBtns[2] = false;
	}
	return true;
}
//-------------------------------------------------------------------------------------
void OgreCEGUI::stringToFloat(CEGUI::String &numberString, float &numberFloat)
{
	int j = 2;
	float number = 0;
	int powOf = 1;
	//int j = CEGUI::String.length(numberString);
	for (int i = 0 ; i <= j ; i++)
	{
		if (numberString[i] == '0') number = 0;
		else if (numberString[i] == '1') number = 1;
		else if (numberString[i] == '2') number = 2;
		else if (numberString[i] == '3') number = 3;
		else if (numberString[i] == '4') number = 4;
		else if (numberString[i] == '5') number = 5;
		else if (numberString[i] == '6') number = 6;
		else if (numberString[i] == '7') number = 7;
		else if (numberString[i] == '8') number = 8;
		else if (numberString[i] == '9') number = 9;
		else 
		{
			number = 0;
			numberFloat = numberFloat/10;
		}
		for (int k = 0;k <= j-i; k++)
		{
			if (k == 0) powOf = 1;
			else powOf = powOf*10;
		}
		numberFloat = numberFloat + number*powOf;
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