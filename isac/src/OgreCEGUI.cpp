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

	//Load buttons
	quitBtn = (CEGUI::PushButton*)Wmgr->getWindow("OgreCEGUI/quitBtn");
	quitBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OgreCEGUI::quit, this));

	launchBtn = (CEGUI::PushButton*)Wmgr->getWindow("OgreCEGUI/launchBtn");
	launchBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&OgreCEGUI::launchDemo, this));

	//Load infobox
	infoBox = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/infoBox");

	//Load input fields for 3D settings
	nRoomsWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/NumOfRooms");
	arenaSizeWindow = (CEGUI::Window*)Wmgr->getWindow("OgreCEGUI/ArenaSize");
	nRooms = (CEGUI::Editbox*)Wmgr->getWindow("3DSettingsNumRooms");
	nRooms->subscribeEvent(CEGUI::Editbox::EventKeyDown, CEGUI::Event::Subscriber(&OgreCEGUI::nRoomsChanged, this));
	nRoomsSlider = (CEGUI::Slider*)Wmgr->getWindow("3DSettingsNumRoomsSlider");
	//nRoomsSlider->setRotation(float[90,0,0])
	nRoomsSlider->subscribeEvent(CEGUI::Slider::EventValueChanged, CEGUI::Event::Subscriber(&OgreCEGUI::nRoomsSliderChanged, this));
	arenaSizeX = (CEGUI::Editbox*)Wmgr->getWindow("3DSettingsASizeX");
	arenaSizeY = (CEGUI::Editbox*)Wmgr->getWindow("3DSettingsASizeY");

	//Set character limits for the input fields
	nRooms->setMaxTextLength(2);
	nRooms->setText("0");
	arenaSizeX->setMaxTextLength(3);
	arenaSizeX->setText("0");
	arenaSizeY->setMaxTextLength(3);
	arenaSizeY->setText("0");

	//Hide the fields for 3D settings as default
	nRoomsWindow->setVisible(false);
	arenaSizeWindow->setVisible(false);
	threeDSettingsActive = false;

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
	Ogre::String aiSettings;
	if (aiSettingsBtns[0] == 1) aiSettings = "Hard";//(aiSettingsOp1Btn->getText());
	else if (aiSettingsBtns[1] == 1) aiSettings = "Easy";//aiSettingsOp2Btn->getText();
	else aiSettings = "off";//aiSettingsOp3Btn->getText();

	Ogre::String physSettings;
	if (physSettingsBtns[0] == 1) physSettings = "Realistic";
	else if (physSettingsBtns[1] == 1) physSettings = "Arcade";
	else physSettings = "off";

	CEGUI::String threeDSettings;
	if (threeDSettingsActive == false) threeDSettings = "off";
	else
	{
		threeDSettings = "Number of rooms = " + nRooms->getText() + ", Arena size = " + arenaSizeX->getText() + " x " + arenaSizeY->getText();
	}
	infoBox->setText("Demo launch with AI: " + aiSettings + ", Physics: " + physSettings + ", 3D settings: " + threeDSettings);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::nRoomsChanged(const CEGUI::EventArgs &e)
{
	if (keyBuffer == 28)
	{
		float nRoomsFloat = 0;
		CEGUI::String nRoomsText = (nRooms->getText());
		OgreCEGUI::stringToFloat(nRoomsText, nRoomsFloat);
		nRoomsSlider->setCurrentValue(nRoomsFloat);
	}
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::nRoomsSliderChanged(const CEGUI::EventArgs &e)
{
	CEGUI::String nRoomsString;
	float numOfRooms = nRoomsSlider->getCurrentValue();
	OgreCEGUI::floatToString(numOfRooms,nRoomsString);
	nRooms->setText(nRoomsString);
	return true;
}
//-------------------------------------------------------------------------------------
bool OgreCEGUI::threeDSettingsOnBtnChanged(const CEGUI::EventArgs &e)
{
	if (threeDSettingsOnBtn->isSelected())
	{
		nRoomsWindow->setVisible(true);
		arenaSizeWindow->setVisible(true);
		threeDSettingsActive = true;
	}
	else
	{
		nRoomsWindow->setVisible(false);
		arenaSizeWindow->setVisible(false);
		threeDSettingsActive = false;
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
	int j = 1;
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
	std::string stringTens;
	std::string stringOnes;
	int tens = number/10;
	int ones = (number -= tens*10);

	if (tens == 0) stringTens = ' ';
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
	numberString = stringTens + stringOnes;
}
//-------------------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

//-------------------------------------------------------------------------------------
/*
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