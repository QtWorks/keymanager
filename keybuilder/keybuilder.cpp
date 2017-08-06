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
    m_wMainWindow.show();

    // This forces blocks to update their enabled state
    m_pController->parameterMgr()->setParameterValue(VARIABLE_TYPE_OF_KEY, "regular");

    return true;
}

//-------------------------------------------------------------------------------------------------

void KeyBuilder::shutdown()
{
    m_pController->shutdown();
}
