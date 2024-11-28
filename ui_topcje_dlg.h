/********************************************************************************
** Form generated from reading UI file 'topcje_dlg.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOPCJE_DLG_H
#define UI_TOPCJE_DLG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Topcje_dlg
{
public:
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab_General;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_8;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_7;
    QSlider *horizontalSlider_przezroczystosc;
    QLabel *label_transparency;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_6;
    QCheckBox *checkBox_na_wierzchu;
    QCheckBox *checkBox_blokuj_pozycje_na_ekranie;
    QCheckBox *checkBox_kilka_kopii;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *groupBox__uruchom_z_Linuxem;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_sciezka;
    QPushButton *pushButton_shutdown;
    QCheckBox *checkBox_uruchom_przy_logowaniu;
    QPushButton *pushButton_start_with_login;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboBox_language;
    QSpacerItem *verticalSpacer_3;
    QWidget *tab_Clock_face;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_15;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_11;
    QLabel *label_infor_mode;
    QHBoxLayout *horizontalLayout_6;
    QRadioButton *radioButton_factory_favourite;
    QSpacerItem *horizontalSpacer_3;
    QRadioButton *radioButton_previous;
    QLabel *label;
    QTableWidget *tableWidget_cyferblaty;
    QVBoxLayout *verticalLayout_13;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_14;
    QCheckBox *checkBox_full_date;
    QCheckBox *checkBox_display_date;
    QCheckBox *checkBox_dispaly_sekundnik;
    QCheckBox *checkBox_favourites;
    QGroupBox *groupBox_preferences;
    QVBoxLayout *verticalLayout_12;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_fav_info;
    QCheckBox *checkBox_remember__scale;
    QPushButton *pushButton_assign_preferred_hands;
    QPushButton *pushButton_return_favourite;
    QPushButton *pushButton_delete_preference;
    QLabel *label_inf_czy_fabryczne;
    QPushButton *pushButton_reset_to_factory;
    QSpacerItem *verticalSpacer_6;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox_zoom;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_zoom50;
    QPushButton *pushButton_zoom100;
    QPushButton *pushButton_zoom200;
    QSlider *horizontalSlider_zoom;
    QLabel *label_zoom;
    QSpacerItem *horizontalSpacer;
    QWidget *tab_hour;
    QVBoxLayout *verticalLayout_9;
    QGroupBox *groupBox_8;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *radioButton_hours_hand_bitmap_type;
    QRadioButton *radioButton_hours_hand_vector_type;
    QSpinBox *spinBox_nr_wsk_wektorowej_godz;
    QLabel *label_3;
    QTableWidget *tableWidget_wsk_godzinowe;
    QWidget *tab_minute;
    QVBoxLayout *verticalLayout_17;
    QGroupBox *groupBox_7;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_minutes_hand_bitmap_type;
    QRadioButton *radioButton_minutes_hand_vector_type;
    QSpinBox *spinBox_nr_wsk_wektorowej_min;
    QLabel *label_2;
    QTableWidget *tableWidget_wsk_minutowe;
    QWidget *tab_seconds;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_9;
    QHBoxLayout *horizontalLayout_5;
    QRadioButton *radioButton_sec_bitmap_type;
    QRadioButton *radioButton_sec_vector_type;
    QSpinBox *spinBox_nr_wsk_wektorowej_sek;
    QLabel *label_4;
    QTableWidget *tableWidget_wsk_sekundowe;
    QWidget *tab_zones;
    QVBoxLayout *verticalLayout_16;
    QSpacerItem *verticalSpacer_4;
    QCheckBox *checkBox_AM_PM;
    QLabel *label_godzina_w_strefie;
    QComboBox *comboBox_timezone;
    QLabel *label_6;
    QSpacerItem *verticalSpacer;
    QWidget *tab_about;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_5;

    void setupUi(QDialog *Topcje_dlg)
    {
        if (Topcje_dlg->objectName().isEmpty())
            Topcje_dlg->setObjectName(QString::fromUtf8("Topcje_dlg"));
        Topcje_dlg->resize(733, 719);
        Topcje_dlg->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        gridLayout = new QGridLayout(Topcje_dlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(Topcje_dlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Help|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);

        tabWidget = new QTabWidget(Topcje_dlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabPosition(QTabWidget::North);
        tab_General = new QWidget();
        tab_General->setObjectName(QString::fromUtf8("tab_General"));
        horizontalLayout = new QHBoxLayout(tab_General);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        groupBox = new QGroupBox(tab_General);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setCheckable(false);
        verticalLayout_7 = new QVBoxLayout(groupBox);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        horizontalSlider_przezroczystosc = new QSlider(groupBox);
        horizontalSlider_przezroczystosc->setObjectName(QString::fromUtf8("horizontalSlider_przezroczystosc"));
        horizontalSlider_przezroczystosc->setMaximum(255);
        horizontalSlider_przezroczystosc->setPageStep(50);
        horizontalSlider_przezroczystosc->setValue(255);
        horizontalSlider_przezroczystosc->setOrientation(Qt::Horizontal);
        horizontalSlider_przezroczystosc->setInvertedControls(true);
        horizontalSlider_przezroczystosc->setTickPosition(QSlider::TicksBelow);
        horizontalSlider_przezroczystosc->setTickInterval(10);

        verticalLayout_7->addWidget(horizontalSlider_przezroczystosc);

        label_transparency = new QLabel(groupBox);
        label_transparency->setObjectName(QString::fromUtf8("label_transparency"));
        label_transparency->setAlignment(Qt::AlignCenter);

        verticalLayout_7->addWidget(label_transparency);


        verticalLayout_8->addWidget(groupBox);

        groupBox_3 = new QGroupBox(tab_General);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_6 = new QVBoxLayout(groupBox_3);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        checkBox_na_wierzchu = new QCheckBox(groupBox_3);
        checkBox_na_wierzchu->setObjectName(QString::fromUtf8("checkBox_na_wierzchu"));

        verticalLayout_6->addWidget(checkBox_na_wierzchu);

        checkBox_blokuj_pozycje_na_ekranie = new QCheckBox(groupBox_3);
        checkBox_blokuj_pozycje_na_ekranie->setObjectName(QString::fromUtf8("checkBox_blokuj_pozycje_na_ekranie"));

        verticalLayout_6->addWidget(checkBox_blokuj_pozycje_na_ekranie);

        checkBox_kilka_kopii = new QCheckBox(groupBox_3);
        checkBox_kilka_kopii->setObjectName(QString::fromUtf8("checkBox_kilka_kopii"));

        verticalLayout_6->addWidget(checkBox_kilka_kopii);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_2);

        groupBox__uruchom_z_Linuxem = new QGroupBox(groupBox_3);
        groupBox__uruchom_z_Linuxem->setObjectName(QString::fromUtf8("groupBox__uruchom_z_Linuxem"));
        verticalLayout_3 = new QVBoxLayout(groupBox__uruchom_z_Linuxem);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton_sciezka = new QPushButton(groupBox__uruchom_z_Linuxem);
        pushButton_sciezka->setObjectName(QString::fromUtf8("pushButton_sciezka"));

        verticalLayout_3->addWidget(pushButton_sciezka);

        pushButton_shutdown = new QPushButton(groupBox__uruchom_z_Linuxem);
        pushButton_shutdown->setObjectName(QString::fromUtf8("pushButton_shutdown"));
        pushButton_shutdown->setEnabled(false);

        verticalLayout_3->addWidget(pushButton_shutdown);

        checkBox_uruchom_przy_logowaniu = new QCheckBox(groupBox__uruchom_z_Linuxem);
        checkBox_uruchom_przy_logowaniu->setObjectName(QString::fromUtf8("checkBox_uruchom_przy_logowaniu"));

        verticalLayout_3->addWidget(checkBox_uruchom_przy_logowaniu);

        pushButton_start_with_login = new QPushButton(groupBox__uruchom_z_Linuxem);
        pushButton_start_with_login->setObjectName(QString::fromUtf8("pushButton_start_with_login"));

        verticalLayout_3->addWidget(pushButton_start_with_login);


        verticalLayout_6->addWidget(groupBox__uruchom_z_Linuxem);


        verticalLayout_8->addWidget(groupBox_3);


        horizontalLayout->addLayout(verticalLayout_8);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_4 = new QGroupBox(tab_General);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_2 = new QVBoxLayout(groupBox_4);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        comboBox_language = new QComboBox(groupBox_4);
        comboBox_language->addItem(QString());
        comboBox_language->addItem(QString());
        comboBox_language->setObjectName(QString::fromUtf8("comboBox_language"));

        verticalLayout_2->addWidget(comboBox_language);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        verticalLayout_4->addWidget(groupBox_4);


        horizontalLayout->addLayout(verticalLayout_4);

        tabWidget->addTab(tab_General, QString());
        tab_Clock_face = new QWidget();
        tab_Clock_face->setObjectName(QString::fromUtf8("tab_Clock_face"));
        gridLayout_3 = new QGridLayout(tab_Clock_face);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        groupBox_6 = new QGroupBox(tab_Clock_face);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setMinimumSize(QSize(0, 60));
        gridLayout_2 = new QGridLayout(groupBox_6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        label_infor_mode = new QLabel(groupBox_6);
        label_infor_mode->setObjectName(QString::fromUtf8("label_infor_mode"));
        label_infor_mode->setTextFormat(Qt::RichText);

        verticalLayout_11->addWidget(label_infor_mode);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        radioButton_factory_favourite = new QRadioButton(groupBox_6);
        radioButton_factory_favourite->setObjectName(QString::fromUtf8("radioButton_factory_favourite"));

        horizontalLayout_6->addWidget(radioButton_factory_favourite);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        radioButton_previous = new QRadioButton(groupBox_6);
        radioButton_previous->setObjectName(QString::fromUtf8("radioButton_previous"));

        horizontalLayout_6->addWidget(radioButton_previous);


        verticalLayout_11->addLayout(horizontalLayout_6);


        gridLayout_2->addLayout(verticalLayout_11, 0, 0, 1, 1);


        verticalLayout_15->addWidget(groupBox_6);

        label = new QLabel(tab_Clock_face);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_15->addWidget(label);

        tableWidget_cyferblaty = new QTableWidget(tab_Clock_face);
        tableWidget_cyferblaty->setObjectName(QString::fromUtf8("tableWidget_cyferblaty"));
        tableWidget_cyferblaty->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout_15->addWidget(tableWidget_cyferblaty);


        gridLayout_3->addLayout(verticalLayout_15, 0, 0, 1, 1);

        verticalLayout_13 = new QVBoxLayout();
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        groupBox_2 = new QGroupBox(tab_Clock_face);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_14 = new QVBoxLayout(groupBox_2);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        checkBox_full_date = new QCheckBox(groupBox_2);
        checkBox_full_date->setObjectName(QString::fromUtf8("checkBox_full_date"));

        verticalLayout_14->addWidget(checkBox_full_date);

        checkBox_display_date = new QCheckBox(groupBox_2);
        checkBox_display_date->setObjectName(QString::fromUtf8("checkBox_display_date"));
        checkBox_display_date->setEnabled(false);

        verticalLayout_14->addWidget(checkBox_display_date);

        checkBox_dispaly_sekundnik = new QCheckBox(groupBox_2);
        checkBox_dispaly_sekundnik->setObjectName(QString::fromUtf8("checkBox_dispaly_sekundnik"));

        verticalLayout_14->addWidget(checkBox_dispaly_sekundnik);


        verticalLayout_13->addWidget(groupBox_2);

        checkBox_favourites = new QCheckBox(tab_Clock_face);
        checkBox_favourites->setObjectName(QString::fromUtf8("checkBox_favourites"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("pencil.png");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8(":/new/prefix1/content/pencil.png"), QSize(), QIcon::Normal, QIcon::On);
        }
        checkBox_favourites->setIcon(icon);

        verticalLayout_13->addWidget(checkBox_favourites);

        groupBox_preferences = new QGroupBox(tab_Clock_face);
        groupBox_preferences->setObjectName(QString::fromUtf8("groupBox_preferences"));
        groupBox_preferences->setCheckable(false);
        verticalLayout_12 = new QVBoxLayout(groupBox_preferences);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_12->addItem(verticalSpacer_5);

        label_fav_info = new QLabel(groupBox_preferences);
        label_fav_info->setObjectName(QString::fromUtf8("label_fav_info"));

        verticalLayout_12->addWidget(label_fav_info);

        checkBox_remember__scale = new QCheckBox(groupBox_preferences);
        checkBox_remember__scale->setObjectName(QString::fromUtf8("checkBox_remember__scale"));

        verticalLayout_12->addWidget(checkBox_remember__scale, 0, Qt::AlignRight);

        pushButton_assign_preferred_hands = new QPushButton(groupBox_preferences);
        pushButton_assign_preferred_hands->setObjectName(QString::fromUtf8("pushButton_assign_preferred_hands"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/content/pencil.png"), QSize(), QIcon::Normal, QIcon::On);
        pushButton_assign_preferred_hands->setIcon(icon1);
        pushButton_assign_preferred_hands->setIconSize(QSize(25, 25));

        verticalLayout_12->addWidget(pushButton_assign_preferred_hands);

        pushButton_return_favourite = new QPushButton(groupBox_preferences);
        pushButton_return_favourite->setObjectName(QString::fromUtf8("pushButton_return_favourite"));

        verticalLayout_12->addWidget(pushButton_return_favourite);

        pushButton_delete_preference = new QPushButton(groupBox_preferences);
        pushButton_delete_preference->setObjectName(QString::fromUtf8("pushButton_delete_preference"));

        verticalLayout_12->addWidget(pushButton_delete_preference);

        label_inf_czy_fabryczne = new QLabel(groupBox_preferences);
        label_inf_czy_fabryczne->setObjectName(QString::fromUtf8("label_inf_czy_fabryczne"));
        label_inf_czy_fabryczne->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 127);"));
        label_inf_czy_fabryczne->setTextFormat(Qt::RichText);

        verticalLayout_12->addWidget(label_inf_czy_fabryczne);

        pushButton_reset_to_factory = new QPushButton(groupBox_preferences);
        pushButton_reset_to_factory->setObjectName(QString::fromUtf8("pushButton_reset_to_factory"));

        verticalLayout_12->addWidget(pushButton_reset_to_factory);


        verticalLayout_13->addWidget(groupBox_preferences);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_13->addItem(verticalSpacer_6);

        groupBox_5 = new QGroupBox(tab_Clock_face);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMaximumSize(QSize(16777215, 5555));
        verticalLayout = new QVBoxLayout(groupBox_5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        checkBox_zoom = new QCheckBox(groupBox_5);
        checkBox_zoom->setObjectName(QString::fromUtf8("checkBox_zoom"));

        horizontalLayout_2->addWidget(checkBox_zoom);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_zoom50 = new QPushButton(groupBox_5);
        pushButton_zoom50->setObjectName(QString::fromUtf8("pushButton_zoom50"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_zoom50->sizePolicy().hasHeightForWidth());
        pushButton_zoom50->setSizePolicy(sizePolicy);
        pushButton_zoom50->setMinimumSize(QSize(50, 0));
        pushButton_zoom50->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_2->addWidget(pushButton_zoom50);

        pushButton_zoom100 = new QPushButton(groupBox_5);
        pushButton_zoom100->setObjectName(QString::fromUtf8("pushButton_zoom100"));
        pushButton_zoom100->setMinimumSize(QSize(50, 0));
        pushButton_zoom100->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_2->addWidget(pushButton_zoom100);

        pushButton_zoom200 = new QPushButton(groupBox_5);
        pushButton_zoom200->setObjectName(QString::fromUtf8("pushButton_zoom200"));
        pushButton_zoom200->setMinimumSize(QSize(50, 0));

        horizontalLayout_2->addWidget(pushButton_zoom200);

        horizontalLayout_2->setStretch(1, 5);
        horizontalLayout_2->setStretch(3, 1);
        horizontalLayout_2->setStretch(4, 1);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalSlider_zoom = new QSlider(groupBox_5);
        horizontalSlider_zoom->setObjectName(QString::fromUtf8("horizontalSlider_zoom"));
        horizontalSlider_zoom->setMaximum(300);
        horizontalSlider_zoom->setSingleStep(25);
        horizontalSlider_zoom->setPageStep(25);
        horizontalSlider_zoom->setSliderPosition(100);
        horizontalSlider_zoom->setOrientation(Qt::Horizontal);
        horizontalSlider_zoom->setTickPosition(QSlider::TicksBelow);
        horizontalSlider_zoom->setTickInterval(50);

        verticalLayout->addWidget(horizontalSlider_zoom);

        label_zoom = new QLabel(groupBox_5);
        label_zoom->setObjectName(QString::fromUtf8("label_zoom"));
        label_zoom->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_zoom);


        verticalLayout_13->addWidget(groupBox_5);


        gridLayout_3->addLayout(verticalLayout_13, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(318, 4, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 0, 1, 1);

        tabWidget->addTab(tab_Clock_face, QString());
        tab_hour = new QWidget();
        tab_hour->setObjectName(QString::fromUtf8("tab_hour"));
        verticalLayout_9 = new QVBoxLayout(tab_hour);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        groupBox_8 = new QGroupBox(tab_hour);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        horizontalLayout_4 = new QHBoxLayout(groupBox_8);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        radioButton_hours_hand_bitmap_type = new QRadioButton(groupBox_8);
        radioButton_hours_hand_bitmap_type->setObjectName(QString::fromUtf8("radioButton_hours_hand_bitmap_type"));

        horizontalLayout_4->addWidget(radioButton_hours_hand_bitmap_type);

        radioButton_hours_hand_vector_type = new QRadioButton(groupBox_8);
        radioButton_hours_hand_vector_type->setObjectName(QString::fromUtf8("radioButton_hours_hand_vector_type"));

        horizontalLayout_4->addWidget(radioButton_hours_hand_vector_type);

        spinBox_nr_wsk_wektorowej_godz = new QSpinBox(groupBox_8);
        spinBox_nr_wsk_wektorowej_godz->setObjectName(QString::fromUtf8("spinBox_nr_wsk_wektorowej_godz"));
        spinBox_nr_wsk_wektorowej_godz->setMaximum(2);
        spinBox_nr_wsk_wektorowej_godz->setValue(1);

        horizontalLayout_4->addWidget(spinBox_nr_wsk_wektorowej_godz);

        horizontalLayout_4->setStretch(0, 5);
        horizontalLayout_4->setStretch(1, 4);
        horizontalLayout_4->setStretch(2, 1);

        verticalLayout_9->addWidget(groupBox_8);

        label_3 = new QLabel(tab_hour);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_9->addWidget(label_3);

        tableWidget_wsk_godzinowe = new QTableWidget(tab_hour);
        tableWidget_wsk_godzinowe->setObjectName(QString::fromUtf8("tableWidget_wsk_godzinowe"));
        tableWidget_wsk_godzinowe->setIconSize(QSize(150, 150));
        tableWidget_wsk_godzinowe->horizontalHeader()->setVisible(false);

        verticalLayout_9->addWidget(tableWidget_wsk_godzinowe);

        tabWidget->addTab(tab_hour, QString());
        tab_minute = new QWidget();
        tab_minute->setObjectName(QString::fromUtf8("tab_minute"));
        verticalLayout_17 = new QVBoxLayout(tab_minute);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        groupBox_7 = new QGroupBox(tab_minute);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_7);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_minutes_hand_bitmap_type = new QRadioButton(groupBox_7);
        radioButton_minutes_hand_bitmap_type->setObjectName(QString::fromUtf8("radioButton_minutes_hand_bitmap_type"));

        horizontalLayout_3->addWidget(radioButton_minutes_hand_bitmap_type);

        radioButton_minutes_hand_vector_type = new QRadioButton(groupBox_7);
        radioButton_minutes_hand_vector_type->setObjectName(QString::fromUtf8("radioButton_minutes_hand_vector_type"));

        horizontalLayout_3->addWidget(radioButton_minutes_hand_vector_type);

        spinBox_nr_wsk_wektorowej_min = new QSpinBox(groupBox_7);
        spinBox_nr_wsk_wektorowej_min->setObjectName(QString::fromUtf8("spinBox_nr_wsk_wektorowej_min"));
        spinBox_nr_wsk_wektorowej_min->setMaximum(2);

        horizontalLayout_3->addWidget(spinBox_nr_wsk_wektorowej_min);


        verticalLayout_17->addWidget(groupBox_7);

        label_2 = new QLabel(tab_minute);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_17->addWidget(label_2);

        tableWidget_wsk_minutowe = new QTableWidget(tab_minute);
        tableWidget_wsk_minutowe->setObjectName(QString::fromUtf8("tableWidget_wsk_minutowe"));
        tableWidget_wsk_minutowe->setIconSize(QSize(150, 150));
        tableWidget_wsk_minutowe->horizontalHeader()->setVisible(false);

        verticalLayout_17->addWidget(tableWidget_wsk_minutowe);

        tabWidget->addTab(tab_minute, QString());
        tab_seconds = new QWidget();
        tab_seconds->setObjectName(QString::fromUtf8("tab_seconds"));
        verticalLayout_5 = new QVBoxLayout(tab_seconds);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        groupBox_9 = new QGroupBox(tab_seconds);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_9);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        radioButton_sec_bitmap_type = new QRadioButton(groupBox_9);
        radioButton_sec_bitmap_type->setObjectName(QString::fromUtf8("radioButton_sec_bitmap_type"));

        horizontalLayout_5->addWidget(radioButton_sec_bitmap_type);

        radioButton_sec_vector_type = new QRadioButton(groupBox_9);
        radioButton_sec_vector_type->setObjectName(QString::fromUtf8("radioButton_sec_vector_type"));

        horizontalLayout_5->addWidget(radioButton_sec_vector_type);

        spinBox_nr_wsk_wektorowej_sek = new QSpinBox(groupBox_9);
        spinBox_nr_wsk_wektorowej_sek->setObjectName(QString::fromUtf8("spinBox_nr_wsk_wektorowej_sek"));
        spinBox_nr_wsk_wektorowej_sek->setMaximum(2);

        horizontalLayout_5->addWidget(spinBox_nr_wsk_wektorowej_sek);


        verticalLayout_5->addWidget(groupBox_9);

        label_4 = new QLabel(tab_seconds);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        verticalLayout_5->addWidget(label_4);

        tableWidget_wsk_sekundowe = new QTableWidget(tab_seconds);
        tableWidget_wsk_sekundowe->setObjectName(QString::fromUtf8("tableWidget_wsk_sekundowe"));
        tableWidget_wsk_sekundowe->setIconSize(QSize(150, 150));
        tableWidget_wsk_sekundowe->horizontalHeader()->setVisible(false);

        verticalLayout_5->addWidget(tableWidget_wsk_sekundowe);

        tabWidget->addTab(tab_seconds, QString());
        tab_zones = new QWidget();
        tab_zones->setObjectName(QString::fromUtf8("tab_zones"));
        verticalLayout_16 = new QVBoxLayout(tab_zones);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_16->addItem(verticalSpacer_4);

        checkBox_AM_PM = new QCheckBox(tab_zones);
        checkBox_AM_PM->setObjectName(QString::fromUtf8("checkBox_AM_PM"));

        verticalLayout_16->addWidget(checkBox_AM_PM);

        label_godzina_w_strefie = new QLabel(tab_zones);
        label_godzina_w_strefie->setObjectName(QString::fromUtf8("label_godzina_w_strefie"));

        verticalLayout_16->addWidget(label_godzina_w_strefie);

        comboBox_timezone = new QComboBox(tab_zones);
        comboBox_timezone->setObjectName(QString::fromUtf8("comboBox_timezone"));
        comboBox_timezone->setMaxVisibleItems(40);

        verticalLayout_16->addWidget(comboBox_timezone);

        label_6 = new QLabel(tab_zones);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_16->addWidget(label_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_16->addItem(verticalSpacer);

        verticalLayout_16->setStretch(0, 1);
        verticalLayout_16->setStretch(2, 1);
        verticalLayout_16->setStretch(3, 1);
        verticalLayout_16->setStretch(5, 8);
        tabWidget->addTab(tab_zones, QString());
        tab_about = new QWidget();
        tab_about->setObjectName(QString::fromUtf8("tab_about"));
        verticalLayout_10 = new QVBoxLayout(tab_about);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        label_5 = new QLabel(tab_about);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_10->addWidget(label_5);

        tabWidget->addTab(tab_about, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);


        retranslateUi(Topcje_dlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), Topcje_dlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Topcje_dlg, SLOT(reject()));

        tabWidget->setCurrentIndex(1);
        comboBox_language->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Topcje_dlg);
    } // setupUi

    void retranslateUi(QDialog *Topcje_dlg)
    {
        Topcje_dlg->setWindowTitle(QCoreApplication::translate("Topcje_dlg", "Options", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Topcje_dlg", "Transparency of the clock  ", nullptr));
        label_transparency->setText(QCoreApplication::translate("Topcje_dlg", "255", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Topcje_dlg", "Options of the clock", nullptr));
        checkBox_na_wierzchu->setText(QCoreApplication::translate("Topcje_dlg", "Always on top", nullptr));
        checkBox_blokuj_pozycje_na_ekranie->setText(QCoreApplication::translate("Topcje_dlg", "Block position on the screen", nullptr));
        checkBox_kilka_kopii->setText(QCoreApplication::translate("Topcje_dlg", "Allow to run many copies of this program (in particular directory)", nullptr));
        groupBox__uruchom_z_Linuxem->setTitle(QCoreApplication::translate("Topcje_dlg", "Some other options for testing", nullptr));
        pushButton_sciezka->setText(QCoreApplication::translate("Topcje_dlg", "Test path and OS", nullptr));
        pushButton_shutdown->setText(QCoreApplication::translate("Topcje_dlg", "Shutdown now", nullptr));
        checkBox_uruchom_przy_logowaniu->setText(QCoreApplication::translate("Topcje_dlg", "Start during login (not working yet)", nullptr));
        pushButton_start_with_login->setText(QCoreApplication::translate("Topcje_dlg", "Start this program during loging", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Topcje_dlg", "Language", nullptr));
        comboBox_language->setItemText(0, QCoreApplication::translate("Topcje_dlg", "jeszcze nie dziala", nullptr));
        comboBox_language->setItemText(1, QCoreApplication::translate("Topcje_dlg", "nadal nie dzia\305\202a", nullptr));

        comboBox_language->setCurrentText(QCoreApplication::translate("Topcje_dlg", "jeszcze nie dziala", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_General), QCoreApplication::translate("Topcje_dlg", "General", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("Topcje_dlg", "Choose a mode for clock hands:", nullptr));
        label_infor_mode->setText(QCoreApplication::translate("Topcje_dlg", "<html><head/><body><p>While changing the clock face (by clicking on table below), <br/>such clock-hands will be used:</p></body></html>", nullptr));
        radioButton_factory_favourite->setText(QCoreApplication::translate("Topcje_dlg", "favourite or 'factory' hands", nullptr));
        radioButton_previous->setText(QCoreApplication::translate("Topcje_dlg", "recent  hands", nullptr));
        label->setText(QCoreApplication::translate("Topcje_dlg", "Click on a clock-face name to try", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("Topcje_dlg", "Show", nullptr));
        checkBox_full_date->setText(QCoreApplication::translate("Topcje_dlg", "full date", nullptr));
        checkBox_display_date->setText(QCoreApplication::translate("Topcje_dlg", "day of a month_long-short", nullptr));
        checkBox_dispaly_sekundnik->setText(QCoreApplication::translate("Topcje_dlg", "hand of seconds", nullptr));
        checkBox_favourites->setText(QCoreApplication::translate("Topcje_dlg", "Set preferred clock-hands for this clock face", nullptr));
        groupBox_preferences->setTitle(QCoreApplication::translate("Topcje_dlg", "Preferences about clock hands", nullptr));
        label_fav_info->setText(QCoreApplication::translate("Topcje_dlg", "TextLabel", nullptr));
        checkBox_remember__scale->setText(QCoreApplication::translate("Topcje_dlg", "Remember also a current scale (zoom) factor", nullptr));
        pushButton_assign_preferred_hands->setText(QCoreApplication::translate("Topcje_dlg", "Assign current hands as preferred ones", nullptr));
        pushButton_return_favourite->setText(QCoreApplication::translate("Topcje_dlg", "Restore preferred hands", nullptr));
        pushButton_delete_preference->setText(QCoreApplication::translate("Topcje_dlg", "Delete assignment", nullptr));
        label_inf_czy_fabryczne->setText(QCoreApplication::translate("Topcje_dlg", "<html><head/><body><p><span style=\" color:#001fea;\">You can return to the factory settings<br/></span></p></body></html>", nullptr));
        pushButton_reset_to_factory->setText(QCoreApplication::translate("Topcje_dlg", "Restore  factory hands", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("Topcje_dlg", "Resize clock", nullptr));
        checkBox_zoom->setText(QCoreApplication::translate("Topcje_dlg", "zoom enabled", nullptr));
        pushButton_zoom50->setText(QCoreApplication::translate("Topcje_dlg", "50%", nullptr));
        pushButton_zoom100->setText(QCoreApplication::translate("Topcje_dlg", "100%", nullptr));
        pushButton_zoom200->setText(QCoreApplication::translate("Topcje_dlg", "200%", nullptr));
        label_zoom->setText(QCoreApplication::translate("Topcje_dlg", "procent", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_Clock_face), QCoreApplication::translate("Topcje_dlg", "Clock face", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("Topcje_dlg", "Choose a prefered style    ", nullptr));
        radioButton_hours_hand_bitmap_type->setText(QCoreApplication::translate("Topcje_dlg", "bitmap-type hours' hand", nullptr));
        radioButton_hours_hand_vector_type->setText(QCoreApplication::translate("Topcje_dlg", "vector-type hours' hand", nullptr));
        label_3->setText(QCoreApplication::translate("Topcje_dlg", "Click on a clock's-hand to try", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_hour), QCoreApplication::translate("Topcje_dlg", "Hour hands", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("Topcje_dlg", "Choose a prefered style    ", nullptr));
        radioButton_minutes_hand_bitmap_type->setText(QCoreApplication::translate("Topcje_dlg", "bitmap-type minutes' hand", nullptr));
        radioButton_minutes_hand_vector_type->setText(QCoreApplication::translate("Topcje_dlg", "vector-type minutes' hand", nullptr));
        label_2->setText(QCoreApplication::translate("Topcje_dlg", "Click on a clock's-hand to try", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_minute), QCoreApplication::translate("Topcje_dlg", "Minutes hands", nullptr));
        groupBox_9->setTitle(QCoreApplication::translate("Topcje_dlg", "Choose a prefered style     ", nullptr));
        radioButton_sec_bitmap_type->setText(QCoreApplication::translate("Topcje_dlg", "bitmap-type seconds' hand", nullptr));
        radioButton_sec_vector_type->setText(QCoreApplication::translate("Topcje_dlg", "vector-type seconds' hand", nullptr));
        label_4->setText(QCoreApplication::translate("Topcje_dlg", "Click on a clock's-hand to try", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_seconds), QCoreApplication::translate("Topcje_dlg", "Seconds hands", nullptr));
        checkBox_AM_PM->setText(QCoreApplication::translate("Topcje_dlg", "Show AM or PM text   (useful for different  time zones)", nullptr));
        label_godzina_w_strefie->setText(QCoreApplication::translate("Topcje_dlg", "Choose a time zone from the list below", nullptr));
        label_6->setText(QCoreApplication::translate("Topcje_dlg", "<html><head/><body><p>Be warned, that in some countries may be currently a daylight-saving time (DST). <br/>The names of some cities shown by a world-clocks - are positioned accorting to non-DST.</p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_zones), QCoreApplication::translate("Topcje_dlg", "Time zones", nullptr));
        label_5->setText(QCoreApplication::translate("Topcje_dlg", "<html><head/><body><p align=\"center\"><span style=\" font-size:12pt;\">This freeware clock-reminder program <br/>is made specially for LINUX users <br/>by <br/>Jerzy Gr\304\231bosz <br/>and <br/>Krzysztof Gr\304\231bosz</span></p><p><span style=\" font-size:12pt;\"><br/></span></p><p><span style=\" font-size:12pt;\">We were inspired by a famous Windows program: ClocX<br/>written by Ing. Bohdan Rylko</span></p><p><span style=\" font-size:12pt;\"><br/>In this clock-reminder program you can choose some clock-faces designed by us, <br/>some other - which we found in internet public domain.<br/><br/>However, there are also some pictures of clocks made by world-famous producers. <br/></span><span style=\" font-size:12pt; color:#d89000;\">We dared to &quot;quote&quot; them in this freeware program, <br/>because we are really impressed by their excusive beauty.<br/>We would be very proud if you also admire their work so much, <br/>that some day you will buy a real clock of these producers.</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_about), QCoreApplication::translate("Topcje_dlg", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Topcje_dlg: public Ui_Topcje_dlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOPCJE_DLG_H
