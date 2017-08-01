// Application
#include "keybuilder.h"
#include "controller.h"

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
    m_wMainWindow.show();
    return true;
}

//-------------------------------------------------------------------------------------------------

void KeyBuilder::shutdown()
{
    m_pController->shutdown();
}
