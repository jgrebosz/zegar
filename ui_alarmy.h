/********************************************************************************
** Form generated from reading UI file 'alarmy.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALARMY_H
#define UI_ALARMY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Alarmy
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_nowy;
    QPushButton *pushButton_edytuj;
    QPushButton *pushButton_usun_rzad;
    QPushButton *pushButton_test;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;
    QPushButton *pushButton_OK;

    void setupUi(QDialog *Alarmy)
    {
        if (Alarmy->objectName().isEmpty())
            Alarmy->setObjectName("Alarmy");
        Alarmy->resize(620, 465);
        verticalLayout_3 = new QVBoxLayout(Alarmy);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        tableWidget = new QTableWidget(Alarmy);
        if (tableWidget->columnCount() < 15)
            tableWidget->setColumnCount(15);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setMinimumSize(QSize(300, 0));
        QFont font;
        font.setPointSize(8);
        tableWidget->setFont(font);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setDragEnabled(true);
        tableWidget->setDragDropMode(QAbstractItemView::NoDragDrop);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget->setShowGrid(false);
        tableWidget->setGridStyle(Qt::DashLine);
        tableWidget->setSortingEnabled(false);
        tableWidget->setColumnCount(15);
        tableWidget->verticalHeader()->setProperty("showSortIndicator", QVariant(false));

        verticalLayout_2->addWidget(tableWidget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        verticalLayout_2->addItem(horizontalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        pushButton_nowy = new QPushButton(Alarmy);
        pushButton_nowy->setObjectName("pushButton_nowy");

        verticalLayout->addWidget(pushButton_nowy);

        pushButton_edytuj = new QPushButton(Alarmy);
        pushButton_edytuj->setObjectName("pushButton_edytuj");

        verticalLayout->addWidget(pushButton_edytuj);

        pushButton_usun_rzad = new QPushButton(Alarmy);
        pushButton_usun_rzad->setObjectName("pushButton_usun_rzad");

        verticalLayout->addWidget(pushButton_usun_rzad);

        pushButton_test = new QPushButton(Alarmy);
        pushButton_test->setObjectName("pushButton_test");

        verticalLayout->addWidget(pushButton_test);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(Alarmy);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);

        pushButton_OK = new QPushButton(Alarmy);
        pushButton_OK->setObjectName("pushButton_OK");

        verticalLayout->addWidget(pushButton_OK);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout);


        retranslateUi(Alarmy);

        QMetaObject::connectSlotsByName(Alarmy);
    } // setupUi

    void retranslateUi(QDialog *Alarmy)
    {
        Alarmy->setWindowTitle(QCoreApplication::translate("Alarmy", "User defined alarms", nullptr));
        pushButton_nowy->setText(QCoreApplication::translate("Alarmy", "New alarm", nullptr));
        pushButton_edytuj->setText(QCoreApplication::translate("Alarmy", "Edit selected", nullptr));
        pushButton_usun_rzad->setText(QCoreApplication::translate("Alarmy", "Remove selected", nullptr));
        pushButton_test->setText(QCoreApplication::translate("Alarmy", " Test now", nullptr));
        pushButton->setText(QCoreApplication::translate("Alarmy", "Refresh", nullptr));
        pushButton_OK->setText(QCoreApplication::translate("Alarmy", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Alarmy: public Ui_Alarmy {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALARMY_H
