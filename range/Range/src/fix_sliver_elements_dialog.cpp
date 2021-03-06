/*********************************************************************
 *  AUTHOR: Tomas Soltys                                             *
 *  FILE:   fix_sliver_elements_dialog.cpp                           *
 *  GROUP:  Range                                                    *
 *  TYPE:   source file (*.cpp)                                      *
 *  DATE:   1-st December 2016                                       *
 *                                                                   *
 *  DESCRIPTION: Fix sliver elements dialog class definition         *
 *********************************************************************/

#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "fix_sliver_elements_dialog.h"
#include "session.h"
#include "model_action.h"
#include "job_manager.h"

FixSliverElementsDialog::FixSliverElementsDialog(uint modelID, QWidget *parent)
    : QDialog(parent)
    , modelID(modelID)
{
    QIcon cancelIcon(":/icons/file/pixmaps/range-cancel.svg");
    QIcon okIcon(":/icons/file/pixmaps/range-ok.svg");

    this->setWindowTitle(tr("Fix sliver elements"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout (mainLayout);

    QLabel *label = new QLabel(tr("Fix sliver elements for model") + " <b>" + Session::getInstance().getModel(this->modelID).getName() + "</b>");
    mainLayout->addWidget(label);

    QHBoxLayout *toleranceLayout = new QHBoxLayout;
    mainLayout->addLayout(toleranceLayout);

    QLabel *toleranceLabel = new QLabel(tr("Edge length ratio:"));
    toleranceLayout->addWidget(toleranceLabel);

    this->edgeRatioEdit = new ValueLineEdit(1.0,1e10);
    this->edgeRatioEdit->setValue(Model::SliverElementEdgeRatio);
    toleranceLayout->addWidget(this->edgeRatioEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);

    QPushButton *cancelButton = new QPushButton(cancelIcon, tr("Cancel"));
    buttonsLayout->addWidget(cancelButton);

    QPushButton *okButton = new QPushButton(okIcon, tr("Ok"));
    okButton->setDefault(true);
    buttonsLayout->addWidget(okButton);

    QObject::connect(cancelButton,&QPushButton::clicked,this,&FixSliverElementsDialog::reject);
    QObject::connect(okButton,&QPushButton::clicked,this,&FixSliverElementsDialog::accept);
}

int FixSliverElementsDialog::exec(void)
{
    int retVal = QDialog::exec();

    if (retVal == QDialog::Accepted)
    {
        ModelActionInput modelActionInput(this->modelID);
        modelActionInput.setFixSliverElements(this->edgeRatioEdit->getValue());

        ModelAction *modelAction = new ModelAction;
        modelAction->setAutoDelete(true);
        modelAction->addAction(modelActionInput);
        JobManager::getInstance().submit(modelAction);
    }

    return retVal;
}
