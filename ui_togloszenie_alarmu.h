/********************************************************************************
** Form generated from reading UI file 'togloszenie_alarmu.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOGLOSZENIE_ALARMU_H
#define UI_TOGLOSZENIE_ALARMU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Togloszenie_alarmu
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton_close;

    void setupUi(QDialog *Togloszenie_alarmu)
    {
        if (Togloszenie_alarmu->objectName().isEmpty())
            Togloszenie_alarmu->setObjectName("Togloszenie_alarmu");
        Togloszenie_alarmu->resize(400, 300);
        Togloszenie_alarmu->setModal(false);
        verticalLayout = new QVBoxLayout(Togloszenie_alarmu);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(Togloszenie_alarmu);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        plainTextEdit = new QPlainTextEdit(Togloszenie_alarmu);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        verticalLayout->addWidget(plainTextEdit);

        pushButton_close = new QPushButton(Togloszenie_alarmu);
        pushButton_close->setObjectName("pushButton_close");

        verticalLayout->addWidget(pushButton_close);


        retranslateUi(Togloszenie_alarmu);

        QMetaObject::connectSlotsByName(Togloszenie_alarmu);
    } // setupUi

    void retranslateUi(QDialog *Togloszenie_alarmu)
    {
        Togloszenie_alarmu->setWindowTitle(QCoreApplication::translate("Togloszenie_alarmu", " Alarm!", nullptr));
        label->setText(QCoreApplication::translate("Togloszenie_alarmu", "<html><head/><body><p><span style=\" font-size:16pt; font-weight:600; color:#ff0000;\">Zegar Alarm</span></p></body></html>", nullptr));
        pushButton_close->setText(QCoreApplication::translate("Togloszenie_alarmu", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Togloszenie_alarmu: public Ui_Togloszenie_alarmu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOGLOSZENIE_ALARMU_H
