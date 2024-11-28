#include "tedytor_alarmow_dlg.h"
#include "ui_tedytor_alarmow_dlg.h"

#include <QFileDialog>
#include <QTranslator>
//#define tr  QObject::tr
#include <QMessageBox>

#include <iostream>
#include <bitset>
using namespace std;

#include "tjeden_alarm.h"
#include "alarmy.h"   // dla case_czestotliwosc

//**********************************************************************************************************************************
Tedytor_alarmow_dlg::Tedytor_alarmow_dlg(QWidget *parent) :
    //    QDialog(parent),
    QDialog(parent,  Qt::Tool | Qt::WindowStaysOnTopHint   ),
    ui(new Ui::Tedytor_alarmow_dlg)
{

    // cout << __PRETTY_FUNCTION__ << endl;
    ui->setupUi(this);
    ui->comboBox_powtarzanie->insertItems(0, al.list_czestotliwosc);

    rozlozenie_po_kontrolkach();

    //   activateWindow();
    //   raise();

}
//*******************************************************************************************
Tedytor_alarmow_dlg::~Tedytor_alarmow_dlg()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    delete ui;
}
//*******************************************************************************************
void Tedytor_alarmow_dlg::rozlozenie_po_kontrolkach()
{

    // cout << __PRETTY_FUNCTION__ << endl;
    ui->lineEdit_nazwa->setText(al.nazwa.c_str());
    //    ui->comboBox_powtarzanie->setCurrentIndex(al.czestotliwosc);
    //             cout << "SPR czestotliwosci  = " << ui->comboBox_powtarzanie->currentIndex()
    //                  << ", zrobiona z "
    //                  << al.czestotliwosc
    //                  << endl;

    // gdy jest nowy, to takie dane wpisujemy
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();

    ui->label__current_week_nr->setText(
                tr("current week nr is: %1  (%2) ")
                .arg(dateTime.date().weekNumber() )
                .arg( (dateTime.date().weekNumber() % 2) ?  tr("ODD"): tr("EVEN") )
                );

    if(al.nr_entry == -1) {
        //                cout << "nowy alarm, wiec biezaca data i czas " << endl;

        //        QDateTime dateTime;
        //        dateTime = QDateTime::currentDateTime();
        ui->dateEdit_daty->setDate(dateTime.date());
        ui->timeEdit_godzina->setTime(dateTime.time());
        ui->comboBox_powtarzanie->setCurrentIndex(0);


        //        ui->label__current_week_nr->setText(
        //                    tr("(current week nr is: %1)").arg(dateTime.date().weekNumber()
        //                                                    ));

        ui->spinBox_interwal->setValue(15);

        QString detailed_description_txt  {  tr("optional user text...") } ;

        QString  date_txt( tr("Date:")) ;      // data (kalendarzowa)
        QString  start_on_txt( tr("Start on:")) ;      // "począwszy od "data (kalendarzowa)"

        QString  choose_music_txt( tr("Choose a file containing the sound/music"));
        QString  choose_program_txt( tr( "Choose a program to be exectued during alarm"));



        // ui->frame_dni_tygodnia->hide();// pojawi się na życzenie
        ui->checkBox_pokaz_okno->setCheckState(Qt::Checked);
        ui->plainTextEdit_dodatkowy_opis->setPlainText(
                    detailed_description_txt
                    );
        ui->checkBox_odtworz_dzwiek->setCheckState(Qt::Unchecked);
        ui->lineEdit_plik_muz->setText("");
        ui->spinBox__sekund_dzwieku->setValue(0);
        ui->checkBox_loop_muzyki->setCheckState(Qt::Unchecked);
        ui->checkBox_flag_uruchom_program->setCheckState(Qt::Unchecked);
        ui->lineEdit_nazwa_programu->setText("");
        ui->checkBox_wylacz_komputer->setCheckState(Qt::Unchecked);
        ui->spinBox_interwal->setValue(15);  // 15 minutes
    }
    else   // czyli to jest alarm już wczesniej zdefiniowany
    {
        ui->dateEdit_daty->setDate(QDate::fromString( al.data.c_str(), "yyyy-MM-dd"));

        QTime czas2 = QTime::fromString(al.godzina.c_str(), "hh:mm");
        ui->timeEdit_godzina->setTime(czas2);

        ui->comboBox_powtarzanie->setCurrentIndex(al.czestotliwosc);

        // ui->frame_dni_tygodnia->hide();// pojawi się na życzenie

        ui->checkBox_pn->setCheckState(al.dni_tygodnia & 1? Qt::Checked : Qt::Unchecked);
        ui->checkBox_wt->setCheckState(al.dni_tygodnia & 2? Qt::Checked : Qt::Unchecked);
        ui->checkBox_sr->setCheckState(al.dni_tygodnia & 4? Qt::Checked : Qt::Unchecked);
        ui->checkBox_cz->setCheckState(al.dni_tygodnia & 8? Qt::Checked : Qt::Unchecked);
        ui->checkBox_pt->setCheckState(al.dni_tygodnia & 16? Qt::Checked : Qt::Unchecked);
        ui->checkBox_so->setCheckState(al.dni_tygodnia & 32? Qt::Checked : Qt::Unchecked);
        ui->checkBox_nd->setCheckState(al.dni_tygodnia & 64? Qt::Checked : Qt::Unchecked);

        //        ui->checkBox_even_weeks->setCheckState(
        //                    al.flag_even_odd_week_nr == true ? Qt::Checked : Qt::Unchecked );

        // cout  << __func__ << " al.flag_even_odd_week_nr = " << al.flag_even_odd_week_nr << endl;

        ui->radioButton_even-> setChecked(al.flag_even_odd_week_nr);
        ui->radioButton_odd->    setChecked( ! al.flag_even_odd_week_nr);


        //  ui->label__current_week_nr->setText(tr("(current week nr is: %1)").arg(dateTime.date().weekNumber()) );

        ui->checkBox_pokaz_okno->setCheckState(al.flag_okno == true ? Qt::Checked : Qt::Unchecked );
        ui->plainTextEdit_dodatkowy_opis->setPlainText(al.dodatkowy_opis.c_str() );
        ui->checkBox_odtworz_dzwiek->setCheckState(al.flag_dzwiek == true ? Qt::Checked : Qt::Unchecked );
        ui->lineEdit_plik_muz->setText(al.muzyka.c_str());
        ui->spinBox__sekund_dzwieku->setValue(al.sekund_dzwieku);
        ui->spinBox_interwal->setValue(al.interwal);  //
        ui->checkBox_loop_muzyki->setCheckState(al.loop== true ? Qt::Checked : Qt::Unchecked );
        ui->checkBox_flag_uruchom_program->setCheckState(al.flag_program== true ? Qt::Checked : Qt::Unchecked );
        ui->lineEdit_nazwa_programu->setText(al.nazwa_programu.c_str());
        ui->checkBox_wylacz_komputer->setCheckState(al.flag_wylacz_komputer== true ? Qt::Checked : Qt::Unchecked );
    }
    ui->frame_dni_tygodnia->adjustSize();


    adjustSize();
}


//*******************************************************************************************
void Tedytor_alarmow_dlg::ustaw_alarm_do_edycji(const Tjeden_alarm &alarm)
{
    // cout << __PRETTY_FUNCTION__ << endl;
    al = alarm;
//    cout << "ma byc edytowany alarm z czestotliwoscia = " << al.czestotliwosc
//         << " o godzinie " << al.godzina
//         << endl;
    rozlozenie_po_kontrolkach();
}
//*******************************************************************************************
void Tedytor_alarmow_dlg::mod_raz()
{
    // cout << __PRETTY_FUNCTION__ << endl;
    mod_zgas_wszystkie();
    // widoczna data i tool for calendar
    QString  date_txt( tr("Date:")) ;      // data (kalendarzowa)

    ui->label_data->setText(date_txt);
    ui->label_data->setVisible(true);
    ui->dateEdit_daty->setVisible(true);
    ui->toolButton_kalendarz->setVisible(true);
    adjustSize();
}
//*******************************************************************************************
void Tedytor_alarmow_dlg::mod_co_miesiac_rok()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    mod_zgas_wszystkie();

    ui->label_data->setText( tr("Starting from date:"));
    ui->label_data->setVisible(true);
    ui->dateEdit_daty->setVisible(true);
    ui->toolButton_kalendarz->setVisible(true);
    adjustSize();
}
//*******************************************************************************************
void Tedytor_alarmow_dlg::mod_codziennie()
{
    // cout << __PRETTY_FUNCTION__ << endl;
    // niewidoczne wszystko
    mod_zgas_wszystkie();
    adjustSize();
}
//*******************************************************************************************
void Tedytor_alarmow_dlg::mod_co_tydzien()
{
    // cout << __PRETTY_FUNCTION__ << endl;
    mod_zgas_wszystkie();
    // widoczny tylko frame z dniami tygodnia
    ui->frame_dni_tygodnia->setVisible(true);
    ui-> frame_week_even_odd->setVisible(false);
    ui->frame_dni_tygodnia->adjustSize();

    adjustSize();
}
//*******************************************************************************************
void Tedytor_alarmow_dlg::mod_co_2tygodnie()
{
    // cout << __PRETTY_FUNCTION__ << endl;
    //    cout << __func__ << " - jeszcze nie ukonczony" << endl;
    mod_zgas_wszystkie();

    // cout  << "obecnie al.czestotliwosc =" << al.czestotliwosc << endl;
    ui->comboBox_powtarzanie->   setCurrentIndex( al.czestotliwosc);

    // widoczny tylko frame z dniami tygodnia
    //    ui->checkBox_even_weeks->setVisible(true);
    // ui->frame_even->setVisible(true);


    ui->label__current_week_nr->setVisible(true);
    ui->frame_dni_tygodnia->setVisible(true);
    ui-> frame_week_even_odd->setVisible(true);
    //    ui->frame_even->adjustSize();

    auto now_today = QDateTime::currentDateTime().date();
    QDate sylwestra(now_today.year(), 12, 31);

    int tygodni_w_biezacym_roku = sylwestra.weekNumber();


    if(tygodni_w_biezacym_roku  == 52)
    {
        // info += "\nSo your alarm will work OK also in the next year" ;
    }
    else{   // if 53
        int this_year = now_today.year();
        QString explanation = QString(
                    tr("WARNING: This year has %1  weeks"
                       "<br>So your alarm may surprise you in January, next year<br><br>")).arg(sylwestra.weekNumber());

        explanation += QString(tr("Why?  "
                                  // "Some years have 52 weeks,<br>but some other 53.<br>"
                                  //                                  "The current year (%1) has 53 weeks - (note: 53 is an ODD number),<br> "
                                  //                                  "the first week of the new year (%2) is 1 - (note: 1 is ALSO an ODD number.<br><br>"


                                  "After week nr 53 (ODD number) in December %1, <br>"
                                  "there will be another ODD week nr 1 (in January %2)<br><br>"
                                  "This will make a following consequence:<br>"
                                  "The first alarm in the %3 week of January %4 <br>will 'fire: ' "))
                .arg(this_year)
                .arg(this_year + 1)
                .arg(al.flag_even_odd_week_nr? tr("EVEN") : tr("ODD") )
                .arg(this_year + 1);

        if(al.flag_even_odd_week_nr)
            explanation += QString(tr(" - <b> not 2 but 3! weeks after the December's alarm.</b> "));
        else
            explanation += QString(tr(" - <b> only 1 week after the December's one.</b> "));



        explanation += QString(tr("<br>(So a two weeks distance will not be kept).<br>"));


        QString kolorowy =
                QString (R"ogr(<html><head/><body><p><span style=" color:#0145ff;">%1</span></body></html>)ogr").arg(explanation);
        //  ui->label_Repeat->setText(kolorowy);

        ui->label__info->setText(kolorowy);

    }



    //    ui->frame_dni_tygodnia->adjustSize();
    adjustSize();
}
//********************************************************************************************
void  Tedytor_alarmow_dlg::mod_minuty()
{
    // cout << __PRETTY_FUNCTION__ << endl;
    mod_zgas_wszystkie();
    // widoczna data, tool kalendarza
    // oraz interwał w minutach

    QString  start_on_txt( tr("Start on:")) ;      // "począwszy od "data (kalendarzowa)"


    ui->label_data->setText(start_on_txt);
    ui->label_data->setVisible(true);
    ui->dateEdit_daty->setVisible(true);
    ui->toolButton_kalendarz->setVisible(true);
    ui->label_interwal->setVisible(true);
    ui->spinBox_interwal->setVisible(true);
    adjustSize();
}
//********************************************************************************************
void Tedytor_alarmow_dlg::mod_zgas_wszystkie()
{
    // cout << __PRETTY_FUNCTION__ << endl;


    QString  date_txt( tr("Date:")) ;      // data (kalendarzowa)

    ui->label_data->setText(date_txt);
    ui->label_data->setVisible(false);
    ui->dateEdit_daty->setVisible(false);

    ui->toolButton_kalendarz->setVisible(false);
    ui->frame_dni_tygodnia->setVisible(false);
    //    ui->checkBox_even_weeks->setVisible(false);
    //  ui->frame_even->setVisible(false);
    ui->label_interwal->setVisible(false);
    ui->spinBox_interwal->setVisible(false);
    ui->calendarWidget_kalendarz->setVisible(false);
    ui->pushButton_today->hide();
    ui->label__current_week_nr->hide();
    ui->label_Repeat->setText(tr("Repeat:") );
    ui->label__info->setText("");
    ui->label_data->setText(tr("Date:") );
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_comboBox_powtarzanie_currentIndexChanged(int index)
{
    // cout << __PRETTY_FUNCTION__ << " indeks powtarzania = " << index << endl;

    al.czestotliwosc = index;
    switch(index)
    {
    case case_czestotliwosc::raz:    mod_raz(); break;

    case case_czestotliwosc::codziennie:   mod_codziennie(); break;

    case case_czestotliwosc::co_tydzien: mod_co_tydzien(); break;

    case case_czestotliwosc::co_2_tygodnie:  // OBSOLATE
        mod_co_2tygodnie(); break;

    case case_czestotliwosc::co_PARZYSTE_tygodnie:
    case case_czestotliwosc::co_NIEPARZYSTE_tygodnie:
        adjust_even_odd_to_combo();
        mod_co_2tygodnie(); break;

    case case_czestotliwosc::co_miesiac:
        mod_co_miesiac_rok(); break;

    case case_czestotliwosc::co_rok:
        mod_co_miesiac_rok(); break;

    case case_czestotliwosc::co_iles_minut:
        mod_minuty(); break;
    }

    adjustSize();
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::adjust_even_odd_to_combo()
{
    if(al.czestotliwosc == case_czestotliwosc::co_PARZYSTE_tygodnie)
    {
        ui->radioButton_even->setChecked(true);
        ui->radioButton_odd->setChecked(false);
    }
    else if (al.czestotliwosc == case_czestotliwosc::co_NIEPARZYSTE_tygodnie)
    {
        ui->radioButton_even->setChecked(false);
        ui->radioButton_odd->setChecked(true);
    }
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_toolButton_kalendarz_triggered(QAction *  /*arg1*/)
{
    // cout  << __func__ << endl;
    //  ui->calendarWidget_kalendarz->setVisible(true);
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_toolButton_kalendarz_clicked()
{
    // cout  << __func__ << endl;
    ui->calendarWidget_kalendarz->setVisible(true);
    ui->pushButton_today->setVisible(true);
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_calendarWidget_kalendarz_clicked(const QDate &wybr_date)
{
    // cout  << __func__ << endl;
    ui->dateEdit_daty->setDate(wybr_date );
    ui->pushButton_today->setVisible(true);
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_pushButton_today_clicked()
{
    // cout  << __func__ << endl;
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();
    ui->dateEdit_daty->setDate(dateTime.date());
    ui->calendarWidget_kalendarz->setSelectedDate(dateTime.date());
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_buttonBox_accepted()
{
    // cout<< __PRETTY_FUNCTION__ << endl;

    // zapisanie biezącego stanu okna do tablicy
    // inaczej dla sytuacji new a inaczej dla edit
    vector<QCheckBox *> dni_ptr
    {            ui->checkBox_pn,
                ui->checkBox_wt,
                ui->checkBox_sr,
                ui->checkBox_cz,
                ui->checkBox_pt,
                ui->checkBox_so,
                ui->checkBox_nd };

    // obliczenie wartosci dni tygodnia ------------------------------------------
    int8_t dni = 0;
    for(uint i = 0 ; i < dni_ptr.size() ; i++)
    {
        int yes = (dni_ptr[i]->checkState() == Qt::Checked) ? 1 : 0;
        // cout << "bit nr " << i << " ma wartosc " << yes << endl;
        dni +=  (yes << i);
    }
    //    bitset<8> bbb (dni);
    //    cout << "wartosc dni = " << bbb << endl;

    al.nazwa = ui->lineEdit_nazwa->text().toStdString();


    al.czestotliwosc =
            ui->comboBox_powtarzanie->currentIndex();

    // cout  << "zapisane     al.czestotliwosc = " <<   al.czestotliwosc  << endl;

    al.data = ui->dateEdit_daty->text().toStdString();
    al.dni_tygodnia = dni;
    // al.flag_even_odd_week_nr = ui->checkBox_even_weeks->checkState()  == Qt::Checked? true : false;
    al.flag_even_odd_week_nr = ui->radioButton_even->isChecked(); // == Qt::Checked? true : false;

    // cout  << "zapisane     al.flag_even_odd_week_nr = " <<   al.flag_even_odd_week_nr  << endl;


    al.godzina = ui->timeEdit_godzina->text().toStdString();
    //    cout << "zapisana data  = " << al.data<< endl;

    al.flag_okno = ui->checkBox_pokaz_okno->checkState() == Qt::Checked? true : false;
    al.interwal = ui->spinBox_interwal->value();


    al.flag_okno = ui->checkBox_pokaz_okno-> checkState() ==  Qt::Checked? true : false;
    al.dodatkowy_opis =  ui->plainTextEdit_dodatkowy_opis->toPlainText().toStdString();
    al.flag_dzwiek = ui->checkBox_odtworz_dzwiek->checkState() ==  Qt::Checked? true : false;
    al.muzyka = ui->lineEdit_plik_muz->text().toStdString();
    al.sekund_dzwieku = ui->spinBox__sekund_dzwieku->value();
    al.loop = ui->checkBox_loop_muzyki->checkState() ==  Qt::Checked? true : false;
    al.flag_program = ui->checkBox_flag_uruchom_program->isChecked();
    al.nazwa_programu = ui->lineEdit_nazwa_programu->text().toStdString();
    al.flag_wylacz_komputer = ui->checkBox_wylacz_komputer->checkState() ==  Qt::Checked? true : false;

    // nie zapisujemy, bo inaczej dla sytuacji new a inaczej dla edit

}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_toolButton_wybor_pliku_muz_clicked()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    // cout  << "Clicked wywolanie wyboru pliku" << endl;

    QString  choose_music_txt( tr("Choose a file containing the sound/music"));


    auto fname =   QFileDialog::getOpenFileName(this,
                                                choose_music_txt ,
                                                "./",                                // const QString &dir = QString(),  directory where to start
                                                "sounds *.mp3;;sounds *.wav"                 // filter
                                                );
    //   cout << "wywolanie wyboru pliku " <<fname.toStdString() << endl;
    ui->lineEdit_plik_muz->setText(fname);
}
//******************************************************************************************************************
void Tedytor_alarmow_dlg::on_toolButton_nazwy_programu_clicked()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    QString  choose_program_txt( tr( "Choose a program to be exectued during alarm"));

    auto fname =   QFileDialog::getOpenFileName(this,
                                                choose_program_txt,
                                                "./",                                                  // directory where to start
                                                "all files *;;*.exe;;*.com;;*.bat"                       // filter
                                                );

    //   cout << "wywolanie wyboru pliku " <<fname.toStdString() << endl;
    ui->lineEdit_nazwa_programu->setText(fname);
}
//*******************************************************************************************************************
void Tedytor_alarmow_dlg::on_radioButton_even_clicked(bool checked)
{
    al.flag_even_odd_week_nr = checked;

    al.czestotliwosc = case_czestotliwosc::co_PARZYSTE_tygodnie;

    mod_co_2tygodnie();

}
//*******************************************************************************************************************
void Tedytor_alarmow_dlg::on_radioButton_odd_clicked(bool checked)
{
    al.flag_even_odd_week_nr =   !  checked;

    al.czestotliwosc = case_czestotliwosc::co_NIEPARZYSTE_tygodnie;
    mod_co_2tygodnie();

}
