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
#include "intvalidator.h"
#include "doublevalidator.h"

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
    BaseWidget *pWidget = nullptr;
    QString sParameterVariable = xParameter.attributes()[PROPERTY_VARIABLE].simplified();
    Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
    QString sParameterName = (pParameter != nullptr) ? pParameter->name() : xParameter.attributes()[PROPERTY_NAME].simplified();
    QString sParameterType = (pParameter != nullptr) ? pParameter->type() : xParameter.attributes()[PROPERTY_TYPE].simplified();
    QString sParameterUI = xParameter.attributes()[PROPERTY_UI].simplified();
    QString sDefaultValue = (pParameter != nullptr) ? pParameter->defaultValue() : xParameter.attributes()[PROPERTY_DEFAULT].simplified();
    QString sAutoScript = (pParameter != nullptr) ? pParameter->autoScript() : xParameter.attributes()[PROPERTY_AUTO].simplified();
    sAutoScript.replace(" ", "");
    QString sEnabledCondition = (pParameter != nullptr) ? pParameter->enabledCondtion() : xParameter.attributes()[PROPERTY_ENABLED].simplified();
    sEnabledCondition.replace(" ", "");

    if (sParameterUI == WIDGET_GENERIC_PARAMETER_TABLE)
    {
        QString sColumnLabels = xParameter.attributes()[PROPERTY_COLUMN_LABELS].simplified();
        QString sColumnVariables = xParameter.attributes()[PROPERTY_COLUMN_VARIABLES].simplified();
        QString sActionSetNumberOfPins = xParameter.attributes()[ACTION_SET_NUMBER_OF_ROWS].simplified();
        if (!sColumnLabels.isEmpty() && !sColumnVariables.isEmpty())
        {
            QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
            int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
            QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE].simplified();
            QString sVariableMethod = xParameter.attributes()[PROPERTY_VARIABLE_METHOD].simplified();
            sAutoScript = xParameter.attributes()[PROPERTY_AUTO].simplified();
            GenericParameterTable *pGenericParameterTable = new GenericParameterTable(m_pController, sColumnLabels.split(","), sColumnVariables.split(","), sDefaultValue, sTargetRow, nRows, sTargetVariable, sVariableMethod, sActionSetNumberOfPins, sAutoScript, pParentWidget);
            pWidget = pGenericParameterTable;

            QHash<QString, Position> hParameterVariableHash = pGenericParameterTable->parameterVariableHashTable();
            for (QHash<QString, Position>::iterator it=hParameterVariableHash.begin(); it!=hParameterVariableHash.end(); ++it)
                m_hWidgetHash[it.key()] = pWidget;
        }
    }
    else
    if (sParameterUI == WIDGET_FILE_PICKER)
    {
        QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION].simplified();
        FilePickerWidget *pFilePickerWidget = new FilePickerWidget(m_pController, sParameterName, sFileExtension, sDefaultValue, sAutoScript, sEnabledCondition, pParentWidget);
        pWidget = pFilePickerWidget;
    }
    if (sParameterUI == WIDGET_LINE_EDIT)
    {
        QString sMinValue = xParameter.attributes()[PROPERTY_MIN_VALUE].simplified();
        QString sMaxValue = xParameter.attributes()[PROPERTY_MAX_VALUE].simplified();
        LineEditWidget *pLineEdit = new LineEditWidget(m_pController, sParameterName, sDefaultValue, sAutoScript, sEnabledCondition, pParentWidget);
        if (sParameterType == PROPERTY_INT)
        {
            int iMin = 0;
            int iMax = 100;
            if (!sMinValue.isEmpty() && !sMaxValue.isEmpty())
            {
                bool bOKMin = true;
                bool bOKMax = true;
                int iTestMin = sMinValue.toInt(&bOKMin);
                int iTestMax = sMaxValue.toInt(&bOKMax);
                if (bOKMin && bOKMax)
                {
                    iMin = qMin(iTestMin, iTestMax);
                    iMax = qMax(iTestMin, iTestMax);
                }
            }
            IntValidator *pValidator = new IntValidator(iMin, iMax, this);
            pLineEdit->setValidator(pValidator);
        }
        else
        if (sParameterType == PROPERTY_DOUBLE)
        {
            double dMin = -1000.;
            double dMax = 1000.;
            if (!sMinValue.isEmpty() && !sMaxValue.isEmpty())
            {
                bool bOKMin = true;
                bool bOKMax = true;
                double dTestMin = sMinValue.toDouble(&bOKMin);
                double dTestMax = sMaxValue.toDouble(&bOKMax);
                if (bOKMin && bOKMax)
                {
                    dMin = qMin(dTestMin, dTestMax);
                    dMax = qMax(dTestMin, dTestMax);
                }
            }
            DoubleValidator *pValidator = new DoubleValidator(dMin, dMax, 3, this);
            pLineEdit->setValidator(pValidator);
        }
        pWidget = pLineEdit;
    }
    else
    if (sParameterUI == WIDGET_DXF_OR_STL_FILE_PICKER)
    {
        QString sParameterSTLVariable = xParameter.attributes()[PROPERTY_STL_VARIABLE].simplified();
        QString sParameterDXFVariable = xParameter.attributes()[PROPERTY_DXF_VARIABLE].simplified();
        DXForSTLFilePicker *pFilePickerWidget = new DXForSTLFilePicker(m_pController, sDefaultValue, sParameterSTLVariable, sParameterDXFVariable, sAutoScript, sEnabledCondition, pParentWidget);
        pWidget = pFilePickerWidget;
    }
    else
    if (sParameterUI == WIDGET_EXCLUSIVE_CHOICE)
    {
        QString sLabels = xParameter.attributes()[PROPERTY_LABELS].simplified();
        QString sValues = xParameter.attributes()[PROPERTY_VALUES].simplified();

        if (!sLabels.isEmpty() && !sValues.isEmpty())
        {
            ExclusiveChoiceWidget *pExclusiveChoiceWidet = new ExclusiveChoiceWidget(m_pController, sLabels.split(","), sValues.split(","), sParameterName, sDefaultValue, sAutoScript, sEnabledCondition, pParentWidget);
            pWidget = pExclusiveChoiceWidet;
        }
    }
    else
    if (sParameterUI == WIDGET_DOUBLE_TRIPLET)
    {
        DoubleTripletWidget *pTriplet = new DoubleTripletWidget(m_pController, sParameterName, sDefaultValue, sAutoScript, sEnabledCondition, pParentWidget);
        pWidget = pTriplet;
    }
    else
    if (sParameterUI == WIDGET_YES_NO)
    {
        YesNoWidget *pYesNoWidget = new YesNoWidget(m_pController, sParameterName, sDefaultValue, sAutoScript, sEnabledCondition, pParentWidget);
        pWidget = pYesNoWidget;
    }

    if (pWidget != nullptr)
    {
        DXForSTLFilePicker *pDXFOrSTLFilePicker = dynamic_cast<DXForSTLFilePicker *>(pWidget);
        if (pDXFOrSTLFilePicker != nullptr)
        {
            m_hWidgetHash[pDXFOrSTLFilePicker->dxfVariable()] = pWidget;
            m_hWidgetHash[pDXFOrSTLFilePicker->stlVariable()] = pWidget;
        }
        else
        {
            pWidget->setParameterVariable(sParameterVariable);
            m_hWidgetHash[sParameterVariable] = pWidget;
        }

        QHash<QString, Parameter *> hWatchedParameters;

        // Check auto script
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
        if (sParameterUI != WIDGET_GENERIC_PARAMETER_TABLE)
            pWidget->applyDefaultValue();
        pWidget->onEvaluateEnabledCondition();
    }

    return pWidget;
}
