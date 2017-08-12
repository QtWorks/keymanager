// Qt
#include <QDoubleValidator>
#include <QDebug>

// Application
#include "widgetfactory.h"
#include "constants.h"
#include "lineeditwidget.h"
#include "filepickerwidget.h"
#include "genericparametertable.h"
#include "exclusivechoicewidget.h"
#include "doubletripletwidget.h"
#include "dxforstlfilepicker.h"
#include "yesnowidget.h"
#include "controller.h"
#include "parametermgr.h"

//-------------------------------------------------------------------------------------------------

WidgetFactory::WidgetFactory(QObject *parent) : QObject(parent),
    m_pController(nullptr)
{

}

//-------------------------------------------------------------------------------------------------

WidgetFactory::~WidgetFactory()
{

}

//-------------------------------------------------------------------------------------------------

void WidgetFactory::setController(Controller *pController)
{
    m_pController = pController;
}

//-------------------------------------------------------------------------------------------------

BaseWidget *WidgetFactory::getWidgetByVariableName(const QString &sParameterVariable) const
{
    return m_hWidgetHash[sParameterVariable];
}

//-------------------------------------------------------------------------------------------------

BaseWidget *WidgetFactory::buildWidget(const CXMLNode &xParameter, QWidget *pParentWidget)
{
    Parameter *pParameter = nullptr;
    BaseWidget *pWidget = nullptr;
    QString sParameterVariable = xParameter.attributes()[PROPERTY_VARIABLE].simplified();
    QString sParameterUI = xParameter.attributes()[PROPERTY_UI].simplified();
    if (sParameterUI == WIDGET_GENERIC_PARAMETER_TABLE)
    {
        QString sColumnLabels = xParameter.attributes()[PROPERTY_COLUMN_LABELS].simplified();
        QString sColumnVariables = xParameter.attributes()[PROPERTY_COLUMN_VARIABLES].simplified();
        QString sActionSetNumberOfPins = xParameter.attributes()[ACTION_SET_NUMBER_OF_ROWS];
        if (!sColumnLabels.isEmpty() && !sColumnVariables.isEmpty())
        {
            QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
            int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
            QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE];
            QString sVariableMethod = xParameter.attributes()[PROPERTY_VARIABLE_METHOD];
            QString sDefaultValue = xParameter.attributes()[PROPERTY_DEFAULT].simplified();
            GenericParameterTable *pGenericParameterTable = new GenericParameterTable(m_pController, sColumnLabels.split(","), sColumnVariables.split(","), sDefaultValue, sTargetRow, nRows, sTargetVariable, sVariableMethod, sActionSetNumberOfPins, pParentWidget);
            pWidget = pGenericParameterTable;

            QHash<QString, Position> hParameterVariableHash = pGenericParameterTable->parameterVariableHashTable();
            for (QHash<QString, Position>::iterator it=hParameterVariableHash.begin(); it!=hParameterVariableHash.end(); ++it)
                m_hWidgetHash[it.key()] = pWidget;
        }
    }
    else
    {
        pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
        if (pParameter != nullptr)
        {
            QString sParameterUI = xParameter.attributes()[PROPERTY_UI].simplified();
            if (sParameterUI == WIDGET_LINE_EDIT)
            {
                LineEditWidget *pLineEdit = new LineEditWidget(m_pController, pParameter->name(), pParameter->defaultValue(), pParameter->autoScript(), pParameter->enabledCondtion(), pParentWidget);
                if (pParameter->type() == PROPERTY_DOUBLE)
                {
                    QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, this);
                    pLineEdit->setValidator(pValidator);
                }
                pWidget = pLineEdit;
            }
            else
            if (sParameterUI == WIDGET_FILE_PICKER)
            {
                QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION];
                FilePickerWidget *pFilePickerWidget = new FilePickerWidget(m_pController, pParameter->name(), sFileExtension, pParameter->defaultValue(), pParameter->autoScript(), pParameter->enabledCondtion(), pParentWidget);
                pWidget = pFilePickerWidget;
            }
            else
            if (sParameterUI == WIDGET_DXF_OR_STL_FILE_PICKER)
            {
                QString sParameterSTLVariable = xParameter.attributes()[PROPERTY_STL_VARIABLE].simplified();
                QString sParameterDXFVariable = xParameter.attributes()[PROPERTY_DXF_VARIABLE].simplified();
                DXForSTLFilePicker *pFilePickerWidget = new DXForSTLFilePicker(m_pController, pParameter->defaultValue(), sParameterSTLVariable, sParameterDXFVariable, pParameter->autoScript(), pParameter->enabledCondtion(), pParentWidget);
                pWidget = pFilePickerWidget;
            }
            else
            if (sParameterUI == WIDGET_EXCLUSIVE_CHOICE)
            {
                QString sLabels = xParameter.attributes()[PROPERTY_LABELS].simplified();
                QString sValues = xParameter.attributes()[PROPERTY_VALUES].simplified();

                if (!sLabels.isEmpty() && !sValues.isEmpty())
                {
                    ExclusiveChoiceWidget *pExclusiveChoiceWidet = new ExclusiveChoiceWidget(m_pController, sLabels.split(","), sValues.split(","), pParameter->name(), pParameter->defaultValue(), pParameter->autoScript(), pParameter->enabledCondtion(), pParentWidget);
                    pWidget = pExclusiveChoiceWidet;
                }
            }
            else
            if (sParameterUI == WIDGET_DOUBLE_TRIPLET)
            {
                DoubleTripletWidget *pTriplet = new DoubleTripletWidget(m_pController, pParameter->name(), pParameter->defaultValue(), pParameter->autoScript(), pParameter->enabledCondtion(), pParentWidget);
                pWidget = pTriplet;
            }
            else
            if (sParameterUI == WIDGET_YES_NO)
            {
                YesNoWidget *pYesNoWidget = new YesNoWidget(m_pController, pParameter->name(), pParameter->defaultValue(), pParameter->autoScript(), pParameter->enabledCondtion(), pParentWidget);
                pWidget = pYesNoWidget;
            }
        }
    }

    if (pWidget != nullptr)
    {
        pWidget->setParameterVariable(sParameterVariable);
        m_hWidgetHash[sParameterVariable] = pWidget;

        if (pParameter != nullptr)
        {
            QHash<QString, Parameter *> hWatchedParameters;

            // Retrieve autoscript
            QString sAutoScript = pParameter->autoScript();
            if (!sAutoScript.isEmpty())
            {
                QVector<QString> vVariableNames = ParameterMgr::extractVariableNames(sAutoScript);
                foreach (QString sParameterVariable, vVariableNames)
                {
                    Parameter *pWatchedParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
                    if (pWatchedParameter != nullptr)
                        hWatchedParameters[sParameterVariable] = pWatchedParameter;
                }
            }

            // Retrieve enabled condition
            QString sEnabledCondition = pParameter->enabledCondtion();
            if (!sEnabledCondition.isEmpty())
            {
                QVector<QString> vVariableNames = ParameterMgr::extractVariableNames(sEnabledCondition);
                foreach (QString sParameterVariable, vVariableNames)
                {
                    Parameter *pWatchedParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
                    if (pWatchedParameter != nullptr)
                        hWatchedParameters[sParameterVariable] = pWatchedParameter;
                }
            }

            if (!hWatchedParameters.isEmpty())
                pWidget->setWatchedParameters(hWatchedParameters);
        }

        pWidget->applyDefaultValue();
        pWidget->onEvaluateEnabledCondition();
    }

    return pWidget;
}
