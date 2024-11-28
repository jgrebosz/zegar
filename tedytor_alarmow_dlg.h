#ifndef TEDYTOR_ALARMOW_DLG_H
#define TEDYTOR_ALARMOW_DLG_H

#include <QDialog>
#include "tjeden_alarm.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Ui {
class Tedytor_alarmow_dlg;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Tedytor_alarmow_dlg : public QDialog
{
    Q_OBJECT

public:
    explicit Tedytor_alarmow_dlg(QWidget *parent = nullptr);
    ~Tedytor_alarmow_dlg();

    void ustaw_alarm_do_edycji(const Tjeden_alarm & alarm);
   Tjeden_alarm daj_rezultat() {return al;}

//   QStringList list_czestotliwosc =
//   {
//       QObject::tr("Once"),            // "Raz",
//      tr("Every day"),            //"Codziennie",
//      tr("Every week"),            //"Co tydzień",
//        tr("Every 2 weeks"),            //"Co dwa tygodnie",
//      tr("Every month"),            //"Co miesiąc",
//      tr("Every year"),            //"Co rok",
//      tr("Minutes"),            //"Minuty"
//   };



private slots:
    void on_comboBox_powtarzanie_currentIndexChanged(int index);

    void on_toolButton_kalendarz_triggered(QAction *arg1);

    void on_toolButton_kalendarz_clicked();

    void on_calendarWidget_kalendarz_clicked(const QDate &date);

    void on_pushButton_today_clicked();

    void on_buttonBox_accepted();

    void on_toolButton_wybor_pliku_muz_clicked();

    void on_toolButton_nazwy_programu_clicked();

//    void on_checkBox_even_weeks_stateChanged(int arg1);

    void on_radioButton_even_clicked(bool checked);

    void on_radioButton_odd_clicked(bool checked);

private:
    Ui::Tedytor_alarmow_dlg *ui;

    void mod_raz();
    void mod_co_miesiac_rok();
    void mod_codziennie();
    void mod_co_tydzien();
    void mod_co_2tygodnie();
    void mod_minuty();
    void mod_zgas_wszystkie();
    Tjeden_alarm al;
    void rozlozenie_po_kontrolkach();
    void adjust_even_odd_to_combo();


};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif // TEDYTOR_ALARMOW_DLG_H
