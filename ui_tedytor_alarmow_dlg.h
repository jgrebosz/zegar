/********************************************************************************
** Form generated from reading UI file 'tedytor_alarmow_dlg.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEDYTOR_ALARMOW_DLG_H
#define UI_TEDYTOR_ALARMOW_DLG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Tedytor_alarmow_dlg
{
public:
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_9;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_nazwa;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_Repeat;
    QComboBox *comboBox_powtarzanie;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_data;
    QHBoxLayout *horizontalLayout;
    QDateEdit *dateEdit_daty;
    QToolButton *toolButton_kalendarz;
    QHBoxLayout *horizontalLayout__godzina;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_3;
    QTimeEdit *timeEdit_godzina;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout;
    QLabel *label_interwal;
    QSpinBox *spinBox_interwal;
    QVBoxLayout *verticalLayout_10;
    QCalendarWidget *calendarWidget_kalendarz;
    QPushButton *pushButton_today;
    QFrame *frame_dni_tygodnia;
    QVBoxLayout *verticalLayout_13;
    QVBoxLayout *verticalLayout_12;
    QCheckBox *checkBox_pn;
    QCheckBox *checkBox_wt;
    QCheckBox *checkBox_sr;
    QCheckBox *checkBox_cz;
    QCheckBox *checkBox_pt;
    QCheckBox *checkBox_so;
    QCheckBox *checkBox_nd;
    QFrame *line;
    QFrame *frame_week_even_odd;
    QVBoxLayout *verticalLayout_14;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_8;
    QRadioButton *radioButton_even;
    QRadioButton *radioButton_odd;
    QLabel *label__current_week_nr;
    QLabel *label__info;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_8;
    QCheckBox *checkBox_pokaz_okno;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_5;
    QPlainTextEdit *plainTextEdit_dodatkowy_opis;
    QCheckBox *checkBox_odtworz_dzwiek;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_plik_muz;
    QToolButton *toolButton_wybor_pliku_muz;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_4;
    QSpinBox *spinBox__sekund_dzwieku;
    QCheckBox *checkBox_loop_muzyki;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *checkBox_flag_uruchom_program;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *lineEdit_nazwa_programu;
    QToolButton *toolButton_nazwy_programu;
    QCheckBox *checkBox_wylacz_komputer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Tedytor_alarmow_dlg)
    {
        if (Tedytor_alarmow_dlg->objectName().isEmpty())
            Tedytor_alarmow_dlg->setObjectName(QString::fromUtf8("Tedytor_alarmow_dlg"));
        Tedytor_alarmow_dlg->resize(1147, 767);
        Tedytor_alarmow_dlg->setMaximumSize(QSize(16777215, 902));
        Tedytor_alarmow_dlg->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        verticalLayout_11 = new QVBoxLayout(Tedytor_alarmow_dlg);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(Tedytor_alarmow_dlg);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_2->addWidget(label);

        lineEdit_nazwa = new QLineEdit(Tedytor_alarmow_dlg);
        lineEdit_nazwa->setObjectName(QString::fromUtf8("lineEdit_nazwa"));

        verticalLayout_2->addWidget(lineEdit_nazwa);


        horizontalLayout_6->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);


        verticalLayout_9->addLayout(horizontalLayout_6);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_Repeat = new QLabel(Tedytor_alarmow_dlg);
        label_Repeat->setObjectName(QString::fromUtf8("label_Repeat"));
        label_Repeat->setTextFormat(Qt::RichText);

        verticalLayout_3->addWidget(label_Repeat);

        comboBox_powtarzanie = new QComboBox(Tedytor_alarmow_dlg);
        comboBox_powtarzanie->setObjectName(QString::fromUtf8("comboBox_powtarzanie"));

        verticalLayout_3->addWidget(comboBox_powtarzanie);


        horizontalLayout_7->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_data = new QLabel(Tedytor_alarmow_dlg);
        label_data->setObjectName(QString::fromUtf8("label_data"));

        verticalLayout_4->addWidget(label_data);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        dateEdit_daty = new QDateEdit(Tedytor_alarmow_dlg);
        dateEdit_daty->setObjectName(QString::fromUtf8("dateEdit_daty"));

        horizontalLayout->addWidget(dateEdit_daty);

        toolButton_kalendarz = new QToolButton(Tedytor_alarmow_dlg);
        toolButton_kalendarz->setObjectName(QString::fromUtf8("toolButton_kalendarz"));

        horizontalLayout->addWidget(toolButton_kalendarz);


        verticalLayout_4->addLayout(horizontalLayout);


        horizontalLayout_7->addLayout(verticalLayout_4);


        verticalLayout_9->addLayout(horizontalLayout_7);

        horizontalLayout__godzina = new QHBoxLayout();
        horizontalLayout__godzina->setObjectName(QString::fromUtf8("horizontalLayout__godzina"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_3 = new QLabel(Tedytor_alarmow_dlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_5->addWidget(label_3);

        timeEdit_godzina = new QTimeEdit(Tedytor_alarmow_dlg);
        timeEdit_godzina->setObjectName(QString::fromUtf8("timeEdit_godzina"));
        timeEdit_godzina->setInputMethodHints(Qt::ImhPreferNumbers|Qt::ImhTime);

        verticalLayout_5->addWidget(timeEdit_godzina);


        horizontalLayout__godzina->addLayout(verticalLayout_5);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout__godzina->addItem(horizontalSpacer_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_interwal = new QLabel(Tedytor_alarmow_dlg);
        label_interwal->setObjectName(QString::fromUtf8("label_interwal"));

        verticalLayout->addWidget(label_interwal);

        spinBox_interwal = new QSpinBox(Tedytor_alarmow_dlg);
        spinBox_interwal->setObjectName(QString::fromUtf8("spinBox_interwal"));
        spinBox_interwal->setMinimum(1);
        spinBox_interwal->setMaximum(1440);

        verticalLayout->addWidget(spinBox_interwal);


        horizontalLayout__godzina->addLayout(verticalLayout);


        verticalLayout_9->addLayout(horizontalLayout__godzina);

        verticalLayout_9->setStretch(0, 5);
        verticalLayout_9->setStretch(1, 1);
        verticalLayout_9->setStretch(2, 5);
        verticalLayout_9->setStretch(3, 5);

        horizontalLayout_9->addLayout(verticalLayout_9);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        calendarWidget_kalendarz = new QCalendarWidget(Tedytor_alarmow_dlg);
        calendarWidget_kalendarz->setObjectName(QString::fromUtf8("calendarWidget_kalendarz"));

        verticalLayout_10->addWidget(calendarWidget_kalendarz);

        pushButton_today = new QPushButton(Tedytor_alarmow_dlg);
        pushButton_today->setObjectName(QString::fromUtf8("pushButton_today"));

        verticalLayout_10->addWidget(pushButton_today);


        horizontalLayout_9->addLayout(verticalLayout_10);

        frame_dni_tygodnia = new QFrame(Tedytor_alarmow_dlg);
        frame_dni_tygodnia->setObjectName(QString::fromUtf8("frame_dni_tygodnia"));
        frame_dni_tygodnia->setMinimumSize(QSize(102, 0));
        frame_dni_tygodnia->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        frame_dni_tygodnia->setFrameShape(QFrame::StyledPanel);
        frame_dni_tygodnia->setFrameShadow(QFrame::Raised);
        verticalLayout_13 = new QVBoxLayout(frame_dni_tygodnia);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        checkBox_pn = new QCheckBox(frame_dni_tygodnia);
        checkBox_pn->setObjectName(QString::fromUtf8("checkBox_pn"));
        checkBox_pn->setChecked(false);

        verticalLayout_12->addWidget(checkBox_pn);

        checkBox_wt = new QCheckBox(frame_dni_tygodnia);
        checkBox_wt->setObjectName(QString::fromUtf8("checkBox_wt"));

        verticalLayout_12->addWidget(checkBox_wt);

        checkBox_sr = new QCheckBox(frame_dni_tygodnia);
        checkBox_sr->setObjectName(QString::fromUtf8("checkBox_sr"));

        verticalLayout_12->addWidget(checkBox_sr);

        checkBox_cz = new QCheckBox(frame_dni_tygodnia);
        checkBox_cz->setObjectName(QString::fromUtf8("checkBox_cz"));

        verticalLayout_12->addWidget(checkBox_cz);

        checkBox_pt = new QCheckBox(frame_dni_tygodnia);
        checkBox_pt->setObjectName(QString::fromUtf8("checkBox_pt"));

        verticalLayout_12->addWidget(checkBox_pt);

        checkBox_so = new QCheckBox(frame_dni_tygodnia);
        checkBox_so->setObjectName(QString::fromUtf8("checkBox_so"));

        verticalLayout_12->addWidget(checkBox_so);

        checkBox_nd = new QCheckBox(frame_dni_tygodnia);
        checkBox_nd->setObjectName(QString::fromUtf8("checkBox_nd"));

        verticalLayout_12->addWidget(checkBox_nd);


        verticalLayout_13->addLayout(verticalLayout_12);

        line = new QFrame(frame_dni_tygodnia);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_13->addWidget(line);

        frame_week_even_odd = new QFrame(frame_dni_tygodnia);
        frame_week_even_odd->setObjectName(QString::fromUtf8("frame_week_even_odd"));
        frame_week_even_odd->setFrameShape(QFrame::StyledPanel);
        frame_week_even_odd->setFrameShadow(QFrame::Raised);
        verticalLayout_14 = new QVBoxLayout(frame_week_even_odd);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        label_2 = new QLabel(frame_week_even_odd);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_14->addWidget(label_2);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        radioButton_even = new QRadioButton(frame_week_even_odd);
        radioButton_even->setObjectName(QString::fromUtf8("radioButton_even"));

        horizontalLayout_8->addWidget(radioButton_even);

        radioButton_odd = new QRadioButton(frame_week_even_odd);
        radioButton_odd->setObjectName(QString::fromUtf8("radioButton_odd"));

        horizontalLayout_8->addWidget(radioButton_odd);


        verticalLayout_14->addLayout(horizontalLayout_8);

        label__current_week_nr = new QLabel(frame_week_even_odd);
        label__current_week_nr->setObjectName(QString::fromUtf8("label__current_week_nr"));
        label__current_week_nr->setScaledContents(false);
        label__current_week_nr->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout_14->addWidget(label__current_week_nr);


        verticalLayout_13->addWidget(frame_week_even_odd);

        verticalLayout_13->setStretch(0, 10);
        verticalLayout_13->setStretch(1, 1);
        verticalLayout_13->setStretch(2, 3);

        horizontalLayout_9->addWidget(frame_dni_tygodnia);


        verticalLayout_11->addLayout(horizontalLayout_9);

        label__info = new QLabel(Tedytor_alarmow_dlg);
        label__info->setObjectName(QString::fromUtf8("label__info"));

        verticalLayout_11->addWidget(label__info);

        groupBox = new QGroupBox(Tedytor_alarmow_dlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_8 = new QVBoxLayout(groupBox);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        checkBox_pokaz_okno = new QCheckBox(groupBox);
        checkBox_pokaz_okno->setObjectName(QString::fromUtf8("checkBox_pokaz_okno"));

        verticalLayout_8->addWidget(checkBox_pokaz_okno);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setTextFormat(Qt::RichText);
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_6->addWidget(label_5);

        plainTextEdit_dodatkowy_opis = new QPlainTextEdit(groupBox);
        plainTextEdit_dodatkowy_opis->setObjectName(QString::fromUtf8("plainTextEdit_dodatkowy_opis"));
        plainTextEdit_dodatkowy_opis->setMaximumSize(QSize(16777215, 100));

        verticalLayout_6->addWidget(plainTextEdit_dodatkowy_opis);


        horizontalLayout_2->addLayout(verticalLayout_6);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 5);

        verticalLayout_8->addLayout(horizontalLayout_2);

        checkBox_odtworz_dzwiek = new QCheckBox(groupBox);
        checkBox_odtworz_dzwiek->setObjectName(QString::fromUtf8("checkBox_odtworz_dzwiek"));

        verticalLayout_8->addWidget(checkBox_odtworz_dzwiek);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        lineEdit_plik_muz = new QLineEdit(groupBox);
        lineEdit_plik_muz->setObjectName(QString::fromUtf8("lineEdit_plik_muz"));

        horizontalLayout_3->addWidget(lineEdit_plik_muz);

        toolButton_wybor_pliku_muz = new QToolButton(groupBox);
        toolButton_wybor_pliku_muz->setObjectName(QString::fromUtf8("toolButton_wybor_pliku_muz"));

        horizontalLayout_3->addWidget(toolButton_wybor_pliku_muz);


        verticalLayout_8->addLayout(horizontalLayout_3);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_8->addWidget(label_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        spinBox__sekund_dzwieku = new QSpinBox(groupBox);
        spinBox__sekund_dzwieku->setObjectName(QString::fromUtf8("spinBox__sekund_dzwieku"));
        spinBox__sekund_dzwieku->setMinimumSize(QSize(60, 0));
        spinBox__sekund_dzwieku->setMaximumSize(QSize(80, 16777215));
        spinBox__sekund_dzwieku->setMaximum(14500);

        horizontalLayout_4->addWidget(spinBox__sekund_dzwieku);

        checkBox_loop_muzyki = new QCheckBox(groupBox);
        checkBox_loop_muzyki->setObjectName(QString::fromUtf8("checkBox_loop_muzyki"));

        horizontalLayout_4->addWidget(checkBox_loop_muzyki);

        horizontalLayout_4->setStretch(0, 6);
        horizontalLayout_4->setStretch(1, 1);

        verticalLayout_8->addLayout(horizontalLayout_4);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        checkBox_flag_uruchom_program = new QCheckBox(groupBox);
        checkBox_flag_uruchom_program->setObjectName(QString::fromUtf8("checkBox_flag_uruchom_program"));

        verticalLayout_7->addWidget(checkBox_flag_uruchom_program);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        lineEdit_nazwa_programu = new QLineEdit(groupBox);
        lineEdit_nazwa_programu->setObjectName(QString::fromUtf8("lineEdit_nazwa_programu"));
        QFont font;
        font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        lineEdit_nazwa_programu->setFont(font);

        horizontalLayout_5->addWidget(lineEdit_nazwa_programu);

        toolButton_nazwy_programu = new QToolButton(groupBox);
        toolButton_nazwy_programu->setObjectName(QString::fromUtf8("toolButton_nazwy_programu"));

        horizontalLayout_5->addWidget(toolButton_nazwy_programu);


        verticalLayout_7->addLayout(horizontalLayout_5);


        verticalLayout_8->addLayout(verticalLayout_7);

        checkBox_wylacz_komputer = new QCheckBox(groupBox);
        checkBox_wylacz_komputer->setObjectName(QString::fromUtf8("checkBox_wylacz_komputer"));

        verticalLayout_8->addWidget(checkBox_wylacz_komputer);


        verticalLayout_11->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(Tedytor_alarmow_dlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_11->addWidget(buttonBox);


        retranslateUi(Tedytor_alarmow_dlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), Tedytor_alarmow_dlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Tedytor_alarmow_dlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(Tedytor_alarmow_dlg);
    } // setupUi

    void retranslateUi(QDialog *Tedytor_alarmow_dlg)
    {
        Tedytor_alarmow_dlg->setWindowTitle(QCoreApplication::translate("Tedytor_alarmow_dlg", "Definition of an alarm", nullptr));
        label->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Name", nullptr));
        label_Repeat->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "<html><head/><body><p><span style=\" color:#0145ff;\">Repeat</span> th</p><p>is</p></body></html>", nullptr));
        label_data->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Date", nullptr));
        dateEdit_daty->setDisplayFormat(QCoreApplication::translate("Tedytor_alarmow_dlg", "yyyy-MM-dd", nullptr));
        toolButton_kalendarz->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "...", nullptr));
        label_3->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Hour", nullptr));
        timeEdit_godzina->setDisplayFormat(QCoreApplication::translate("Tedytor_alarmow_dlg", "hh:mm", nullptr));
        label_interwal->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Interval in minutes", nullptr));
        pushButton_today->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "today", nullptr));
        checkBox_pn->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Monday", nullptr));
        checkBox_wt->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Tuesday", nullptr));
        checkBox_sr->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Wednesday", nullptr));
        checkBox_cz->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Thursday", nullptr));
        checkBox_pt->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Friday", nullptr));
        checkBox_so->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Saturday", nullptr));
        checkBox_nd->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Sunday", nullptr));
        label_2->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "in weeks which number is:", nullptr));
        radioButton_even->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "EVEN", nullptr));
        radioButton_odd->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "ODD", nullptr));
        label__current_week_nr->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "TextLabel", nullptr));
        label__info->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "TextLabel_INFO", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Tedytor_alarmow_dlg", "Task to do during an alarm", nullptr));
        checkBox_pokaz_okno->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Show alarm window", nullptr));
        label_5->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Message in the alarm window", nullptr));
        checkBox_odtworz_dzwiek->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Play sound / music (wav, mp3)", nullptr));
        toolButton_wybor_pliku_muz->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "...", nullptr));
        label_6->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "How many seconds of a sound (music) to play?      [0 = no limit]", nullptr));
        checkBox_loop_muzyki->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Looping", nullptr));
        checkBox_flag_uruchom_program->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Run an external program", nullptr));
        toolButton_nazwy_programu->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "...", nullptr));
        checkBox_wylacz_komputer->setText(QCoreApplication::translate("Tedytor_alarmow_dlg", "Shutdown the computer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Tedytor_alarmow_dlg: public Ui_Tedytor_alarmow_dlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEDYTOR_ALARMOW_DLG_H
