// Application
#include "keybuilder.h"
#include "controller.h"
#include "parametermgr.h"
#include "constants.h"
#include "helper.h"
#include "cryptomgr.h"

KeyBuilder *KeyBuilder::sInstance = nullptr;

//-------------------------------------------------------------------------------------------------

KeyBuilder::KeyBuilder(QObject *parent) : QObject(parent)
{
    m_pController = new Controller(this);
}

//-------------------------------------------------------------------------------------------------

KeyBuilder::~KeyBuilder()
{

}

//-------------------------------------------------------------------------------------------------

KeyBuilder *KeyBuilder::instance()
{
    if (sInstance == nullptr)
        sInstance = new KeyBuilder;
    return sInstance;
}

//-------------------------------------------------------------------------------------------------

bool KeyBuilder::startup()
{
    // Set controller on main window
    m_wMainWindow.setController(m_pController);

    // First installation? Hide tab widgets, deactivate debug mode
    if (m_pController->isFirstInstallation())
    {
        m_wMainWindow.setLicenseMode(true);
        m_wMainWindow.showApplicationBody(false);
        m_wMainWindow.showOpenSCADOutputLog(false);
        m_wMainWindow.setQuestion(m_pController->cryptoMgr()->question());
    }
    // Not first installation
    else
    {
        // Start controller
        if (m_pController->startup())
        {
            m_wMainWindow.setLicenseMode(false);
            m_wMainWindow.showApplicationBody(true);
            m_wMainWindow.buildMenus();
            m_wMainWindow.showOpenSCADOutputLog(m_pController->debugOn());
            m_wMainWindow.showDebugTab(m_pController->debugOn());
        }
        else
        {
            logError("COULD NOT STARTUP CONTROLLER. EXITING.");
            return false;
        }
    }

    // Show
    m_wMainWindow.showMaximized();

    return true;
}

//-------------------------------------------------------------------------------------------------

void KeyBuilder::shutdown()
{
    m_pController->shutdown();
}
