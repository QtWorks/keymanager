// Qt
#include <QDoubleValidator>

// Application
#include "widgetfactory.h"
#include "constants.h"
#include "lineeditwidget.h"
#include "filepickerwidget.h"
#include "genericparametertable.h"
#include "exclusivechoicewidget.h"
#include "doubletripletwidget.h"
#include "dxforstlfilepicker.h"
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

BaseWidget *WidgetFactory::buildWidget(const CXMLNode &xParameter, QWidget *pParentWidget)
{
    BaseWidget *pWidget = nullptr;
    QString sParameterVariable = xParameter.attributes()[PROPERTY_VARIABLE].simplified();
    Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);

    if (pParameter != nullptr)
    {
        QString sParameterUI = xParameter.attributes()[PROPERTY_UI].simplified();
        if (sParameterUI == WIDGET_LINE_EDIT)
        {
            LineEditWidget *pLineEdit = new LineEditWidget(pParameter->name(), pParameter->defaultValue(), pParameter->autoScript(), pParentWidget);
            if (pParameter->type() == PROPERTY_DOUBLE)
            {
                QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, this);
                pLineEdit->setValidator(pValidator);
            }

            QString sAutoScript = pParameter->autoScript();
            if (!sAutoScript.isEmpty())
            {
                QVector<QString> vVariableNames = ParameterMgr::extractVariableNames(sAutoScript);
                QHash<QString, Parameter *> hParameters;
                foreach (QString sParameterVariableName, vVariableNames)
                {
                    Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariableName);
                    if (pParameter != nullptr)
                        hParameters[sParameterVariableName] = pParameter;
                }
                if (!hParameters.isEmpty() && (hParameters.size() == vVariableNames.size()))
                    pLineEdit->setWatchedParameters(hParameters);
            }
            pWidget = pLineEdit;
        }
        else
        if (sParameterUI == WIDGET_FILE_PICKER)
        {
            QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION];
            FilePickerWidget *pFilePickerWidget = new FilePickerWidget(pParameter->name(), sFileExtension, pParameter->defaultValue(), pParentWidget);
            pWidget = pFilePickerWidget;
        }
        else
        if (sParameterUI == WIDGET_DXF_OR_STL_FILE_PICKER)
        {
            QString sParameterSTLVariable = xParameter.attributes()[PROPERTY_STL_VARIABLE].simplified();
            QString sParameterDXFVariable = xParameter.attributes()[PROPERTY_DXF_VARIABLE].simplified();
            DXForSTLFilePicker *pFilePickerWidget = new DXForSTLFilePicker(pParameter->defaultValue(), sParameterSTLVariable, sParameterDXFVariable, pParentWidget);
            pWidget = pFilePickerWidget;
        }
        else
        if (sParameterUI == WIDGET_EXCLUSIVE_CHOICE)
        {
            QString sLabels = xParameter.attributes()[PROPERTY_LABELS].simplified();
            QString sValues = xParameter.attributes()[PROPERTY_VALUES].simplified();

            if (!sLabels.isEmpty() && !sValues.isEmpty())
            {
                ExclusiveChoiceWidget *pExclusiveChoiceWidet = new ExclusiveChoiceWidget(sLabels.split(","), sValues.split(","), pParameter->name(), pParameter->defaultValue(), pParentWidget);
                pWidget = pExclusiveChoiceWidet;
            }
        }
        else
        if (sParameterUI == WIDGET_DOUBLE_TRIPLET)
        {
            DoubleTripletWidget *pTriplet = new DoubleTripletWidget(pParameter->name(), pParameter->defaultValue(), pParentWidget);
            pWidget = pTriplet;
        }
        else
        if (sParameterUI == WIDGET_GENERIC_PARAMETER_TABLE)
        {
            QString sColumnLabels = xParameter.attributes()[PROPERTY_COLUMN_LABELS].simplified();
            QString sColumnVariables = xParameter.attributes()[PROPERTY_COLUMN_VARIABLES].simplified();

            if (!sColumnLabels.isEmpty() && !sColumnVariables.isEmpty())
            {
                QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
                int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
                QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE];
                QString sVariableMethod = xParameter.attributes()[PROPERTY_VARIABLE_METHOD];
                QStringList lDefaultValues;
                QString sDefaultValue = pParameter->defaultValue();
                if (!sDefaultValue.isEmpty() && sDefaultValue.contains(","))
                    lDefaultValues = sDefaultValue.split(",");
                GenericParameterTable *pGenericParameterTable = new GenericParameterTable(sColumnLabels.split(","), sColumnVariables.split(","), lDefaultValues, sTargetRow, nRows, sTargetVariable, sVariableMethod, pParentWidget);
                pWidget = pGenericParameterTable;
            }
        }
    }

    if (pWidget != nullptr)
    {
        pWidget->setController(m_pController);
        pWidget->setParameterVariable(sParameterVariable);
        pWidget->applyDefaultValue();
    }

    return pWidget;
}
