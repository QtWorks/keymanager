// Application
#include "keybuilder.h"
#include "controller.h"
#include "parametermgr.h"
#include "constants.h"

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

bool KeyBuilder::startup()
{
    // Start controller
    if (!m_pController->startup())
        return false;

    // Show UI
    m_wMainWindow.setController(m_pController);
    m_wMainWindow.showMaximized();

    return true;
}

//-------------------------------------------------------------------------------------------------

void KeyBuilder::shutdown()
{
    m_pController->shutdown();
}
