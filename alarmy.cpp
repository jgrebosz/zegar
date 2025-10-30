#include "alarmy.h"
#include "ui_alarmy.h"

#include <QMessageBox>
#include <QTranslator>
#include "tjeden_alarm.h"
#include "tedytor_alarmow_dlg.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <bitset>
using namespace  std;
// NOTE: Do not allow to sort table of alarms on screen, because
// it only sorts on screen, but the table stays the same - and then
// it is not possible to edit paricular row-alarm.



//****************************************************************************************
Alarmy::Alarmy(QWidget *parent, string fname) :
    //    QDialog(parent, Qt::Dialog),
    QDialog(parent,  Qt::Tool | Qt::WindowStaysOnTopHint   ),
    ui(new Ui::Alarmy),
    nazwa_pliku_z_alarmami(fname)
{

    // setWindowFlag(Qt::SplashScreen , false);
    //  setModal(true);   // chyba nie działa



    // QString minutes_txt { tr("%1, ") };
    //    QString error_opening_txt {
    //        tr("Error while opening file with user-defined alarms  ")
    //    };



    ui->setupUi(this);

    QStringList labels {tr("Enable"), tr("Name"), tr("Time"),  tr("Data"),  tr("Frequency"), tr("id")};
    ui->tableWidget->setColumnCount (labels.size());
    ui->tableWidget->setHorizontalHeaderLabels ( labels );
    ui->tableWidget->resizeColumnsToContents ();
    ui->tableWidget->verticalHeader()->setStyleSheet( "background-color: rgb(255, 230, 230);");
    int   nr_rows = (int) tablica_alarmow.size();
    ui->tableWidget->setRowCount (nr_rows);

    redisplay_table();

    //    cout << "CZy modalny = " << isModal() << endl;
}
//****************************************************************************************
Alarmy::~Alarmy()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    delete ui;
}
//****************************************************************************************
//void  Alarmy::on_buttonBox_accepted()
//{
//    cout << "wcisniete OK" << endl;
//}
//****************************************************************************************
//****************************************************************************************

void Alarmy::redisplay_table()
{
//    cout<< __PRETTY_FUNCTION__ << endl;
    auto nr_rows = (int) tablica_alarmow.size();
    ui->tableWidget->setRowCount (nr_rows);

    // creating row widgets ---------------------------------------
    int nr_columns = ui->tableWidget->columnCount ();
//    cout << "nr_columns = " << nr_columns << endl;

    for(long r = 0 ; r < nr_rows ; r++)
        for(long c = 0 ; c < nr_columns ; c++)
        {
            ui->tableWidget->setItem(r,c, new QTableWidgetItem("") );
            ui->tableWidget->item(r, c)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

    //    QStringList list  ;
    //    list << "consider this line as: true" << "consider this line as: false" ;

    //    QStringList list_czestotliwosc2 =
    //    {
    //        tr("Once"),            // "Raz",
    //        tr("Every day"),            //"Codziennie",
    //        tr("Every week"),            //"Co tydzień",
    //        tr("Every 2 weeks"),            //"Co2  tydzień",   // OBSOLATE?
    //        tr("Every month"),            //"Co miesiąc",
    //        tr("Every year"),            //"Co rok",
    //        tr("Every N minutes"),            //"Minuty"
    //        tr("Every EVEN week"),            //"Co PARZ  tydzień",
    //        tr("Every ODD week")            //"Co NIEPARZ  tydzień",
    //    };


    constexpr int kol_czestotliwosci = 4;
    constexpr int kol_daty = 3;

    for (int j = 0; j < nr_rows; ++j )
    {
        ui->tableWidget->setItem(j, 0, new QTableWidgetItem( ""));
        //                                     tablica_alarmow[j].flag_enable?
        //                                                                  "":
        //                                                                  "") );

        ui->tableWidget->item(j, 0)->setCheckState(tablica_alarmow[j].flag_enable?
                                                       Qt::Checked : Qt::Unchecked);

//         cout << j << ") tablica alarmow - nazwa   = " << tablica_alarmow[j].nazwa << flush;

        ui->tableWidget->item(j, 1) -> setText(tablica_alarmow[j].nazwa.c_str() );

        //                cout << "do tablicy alarmow wlozona wartosc godziny  = " <<
        //                        tablica_alarmow[j].godzina.c_str() << endl;

        ui->tableWidget->item(j, 2) -> setText(tablica_alarmow[j].godzina.c_str()) ;

//        cout << "  j = " << j << ", of  nr_rows = " << nr_rows << flush ;
//        cout << ",  tablica_alarmow[j].czestotliwosc = [" <<  tablica_alarmow[j].czestotliwosc
//             << "]" << endl;
//        cout << ", a  Tjeden_alarm::list_czestotliwosc.size = "
//             <<  Tjeden_alarm::list_czestotliwosc.size() << endl;

        ui->tableWidget->item(j, kol_czestotliwosci) -> setText(
                    Tjeden_alarm::list_czestotliwosc[ tablica_alarmow[j].czestotliwosc]
                ) ;


        // kolumna 3 to data, ale czasem dni tygodnia
        int co = tablica_alarmow[j].czestotliwosc;
        switch(co)
        {   // raz, codziennie, co tydzien, co miesiac, co rok, minuty
        case case_czestotliwosc::raz:                               // Raz

            ui->tableWidget->item(j, kol_daty) -> setText(tablica_alarmow[j].data.c_str())  ;
            break;
        case case_czestotliwosc::co_rok:                        // co rok
        {              // ("yyyy-MM-dd")

            map<string, QString> mies {
                {"01", tr("Jan")} ,
                {"02", tr("Feb") },
                {"03", tr("Mar") },
                { "04", tr("Apr")  },
                { "05", tr("May")  },
                { "06", tr("Jun")  },
                { "07", tr("Jul")  },
                { "08", tr("Aug")  },
                { "09", tr("Sep")  },
                { "10", tr("Oct")  },
                { "11", tr("Nov")  },
                { "12", tr("Dec")}
            };

            string txt =
                    tablica_alarmow[j].data.substr(8,2) + " " +
                    mies[ tablica_alarmow[j].data.substr(5,2) ].toStdString();
            ui->tableWidget->item(j, kol_daty) -> setText(txt.c_str())  ;
            break;
        }


        case case_czestotliwosc::co_miesiac:  {              // co miesiac
            string txt =
                    tablica_alarmow[j].data.substr(8,2) + tr(". day in month").toStdString();
            ui->tableWidget->item(j, kol_daty) -> setText(txt.c_str())  ;
        }
            break;
        case case_czestotliwosc::codziennie:   // codziennie
            break; // nic nie wypelniamy

        case case_czestotliwosc::co_tydzien:    // co tydzien
        case case_czestotliwosc::co_2_tygodnie:   // i co dwa   OBSOLATE
        case case_czestotliwosc::co_PARZYSTE_tygodnie:   // i co parzyste tygodnie
        case case_czestotliwosc::co_NIEPARZYSTE_tygodnie:   // i co parzyste tygodnie
        {
            std::vector<QString> tyg {
                QObject::tr("Mon"),
                        QObject::tr("Tue"),
                        QObject::tr("Wed"),
                        QObject::tr("Thu"),
                        QObject::tr("Fri"),
                        QObject::tr("Sat"),
                        QObject::tr("Sun")
            };

            QString rezultat;
            bitset<8>  dni = tablica_alarmow[j].dni_tygodnia;
            for(int i = 0 ; i < 7 ;++i){
                if(dni[i]) {
                    rezultat +=   tr("%1, ").arg(tyg[i]);
                }
            }
            ui->tableWidget->item(j, kol_daty) -> setText(rezultat) ;
//            cout  << "wypis dla co tydzien. tabl.dni =  "
//                  <<   tablica_alarmow[j].dni_tygodnia
//                    << ", a rezultat = "
//                    << rezultat.toStdString()
//                    << endl;
            break;
        }




        case case_czestotliwosc::co_iles_minut: // minuty

            ui->tableWidget->item(j, kol_daty) -> setText(
                        tr("%1, ").arg
                        (    tablica_alarmow[j].interwal)
                        ) ;
            break;
        } // switch

        char buff[250] = {  };
        int liczba = tablica_alarmow[j].nr_entry;
        sprintf(buff, "%d", liczba);
        ui->tableWidget->item(j, 5) -> setText( buff )   ;  // będzie potrzebne, bo może tabela została posortowana i nr entry nie jest tożsamy z nr wiersza

#if 1    // dać 0 gdy potrzebne do debugowania
        //----------- Do moich celów (usuwania alarmow), user nie musi tego widziec
        ui->tableWidget->hideColumn(5);
#endif

    }

    ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget->resizeRowsToContents();


    //     ui->tableWidget->adjustSize();
    //     auto rec = ui->tableWidget->size();
    //     resize(rec.width() *2.5 , rec.height()*2 );

    // adjustSize();
}
//***************************************************************************************************************
//***************************************************************************************************************
void Alarmy::table_value_changed()   // działa po dodaniu czegoś albo modyfikacj.
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    //    cout << " f. table_value_changed() " << endl;
    int  row = ui->tableWidget->currentRow() ;
    if((unsigned long) row >= tablica_alarmow.size() ) return ;

    unsigned col = ui->tableWidget->currentColumn() ;
    if(col == 0 )
    {
        ui->tableWidget->resizeColumnToContents(0) ;
    }
    redisplay_table();
    zapisanie_tablicy_na_dysku();
}
//***************************************************************************************************************
//***************************************************************************************************************
//***************************************************************************************************************
void Alarmy::zapisanie_tablicy_na_dysku()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    ofstream plik(nazwa_pliku_z_alarmami);
    if(!plik)
    {
        cerr <<
                tr("Error while opening file with user-defined alarms  ").toStdString()
                << nazwa_pliku_z_alarmami << endl;
        return;
    }
    for(auto x : tablica_alarmow)
    {
        //        cout << "\n================== Kolejny alarm =================" << endl;
        plik << x;
        //        cout << x;   // .nazwa  ;
    }

}
//***************************************************************************************************************


//***************************************************************************************************************
//			QMessageBox::warning ( this,
//								   "No spectrum selected",
//								   QString ( "You can not disable this page, because now its table has already some contents" ),
//								   QMessageBox::Ok,
//								   QMessageBox::NoButton,
//								   QMessageBox::NoButton );

//***************************************************************************************************************
void Alarmy::on_pushButton_usun_rzad_clicked()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    //    cout << "Operacja usuwania rzedu nr" << endl;


    QList<QTableWidgetSelectionRange> range = ui->tableWidget->selectedRanges() ;

    // cout << "Selection has " << range.count() << " ranges" << endl;

    for(int s = range.count()-1; s >=0 ; s--)    // can be many separated ranges on this list
    {
        cout << "range[s].bottomRow()= " << range[s].bottomRow() << ", range[s].topRow()= " <<  range[s].topRow() << endl;
        for(unsigned int row = range[s].bottomRow() ; row >= (unsigned int) range[s].topRow() ; row--)
        {
            //  unsigned row = ui->tableWidget->selectRow();    ro sel currentRow() ;
            if(row >= tablica_alarmow.size() ) return ;

            QString mess_usuwanie = tr(
                                        "You are going to remove the alarm = nr %1\n\n"
                                        "called  : %2\n"
                                        "hour: %3\n"
                                        "date: %4\n"
                                        "polygon : %5\n\n"
                                        "Are you sure?")
                                        .arg(row + 1)
                                        .arg(QString::fromStdString(tablica_alarmow[row].nazwa))
                                        .arg(QString::fromStdString(tablica_alarmow[row].godzina))
                                        .arg(QString::fromStdString(tablica_alarmow[row].data))
                                        .arg(tablica_alarmow[row].czestotliwosc);

            QMessageBox::StandardButton reply = QMessageBox::warning(
                this,
                tr("ARE YOU SURE?"),
                mess_usuwanie,
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                QMessageBox::No // domyślnie zaznaczony
                );

            switch (reply) {
            case QMessageBox::Yes:
                tablica_alarmow.erase(tablica_alarmow.begin() + row);
                redisplay_table();
                break;

            case QMessageBox::No:
                // użytkownik wybrał "No"
                break;

            case QMessageBox::Cancel:
            default:
                // anulowanie lub zamknięcie okna
                break;
            }

        } // for row
    } // for sel
    zapisanie_tablicy_na_dysku();
}
//***************************************************************************************************************
void Alarmy::on_tableWidget_cellClicked(int row, int column)
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    //    cout << "------------ F.  on_tableWidget_cellClicked row=" << row
    //         << ", column= " << column << endl;
    if(column == 0)
    {
        bool stan = ui->tableWidget->item(row, column)->checkState();
        // change the text in
        // ui->tableWidget->item(row, column)->setText( "");
        tablica_alarmow[row].flag_enable = stan;
        zapisanie_tablicy_na_dysku();
        ui->tableWidget->resizeColumnsToContents();
    }
    else {
        auto ile = ui->tableWidget->columnCount();
        //        cout << "select inne kolumy - to select till column " << ile  << endl;
        ui->tableWidget->setRangeSelected(
                    QTableWidgetSelectionRange(row, 1, row,
                                               ile-1
                                               ),
                    true );
    }
}
//***************************************************************************************************************

void Alarmy::on_pushButton_nowy_clicked()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    Tedytor_alarmow_dlg  * dlg = new Tedytor_alarmow_dlg();


    Tjeden_alarm nowy;
    nowy.nr_entry = -1;
    dlg->ustaw_alarm_do_edycji(nowy);

    if( dlg->exec() == QDialog::Accepted)
    {
        auto zmieniony = dlg->daj_rezultat();
        zmieniony.nr_entry = (int) tablica_alarmow.size();
        tablica_alarmow.push_back(zmieniony);
        redisplay_table();
        // zapis na dysk?
        zapisanie_tablicy_na_dysku();
    }
    delete dlg;
    // cout << " Koniec funkcji: " << __PRETTY_FUNCTION__ << endl;
}
//***************************************************************************************************************
void Alarmy::on_pushButton_edytuj_clicked()
{

//    cout<< __PRETTY_FUNCTION__ << endl;
    Tedytor_alarmow_dlg  * dlg = new Tedytor_alarmow_dlg();

    // sprawdzenie ktory wiersz w tabliycy jest selected

    // odczytanie, ktory ma nr_entry, bo nie musi być ten sam co nr wiersza,
    //    jako ze można sortować wiersze tabeli wg roznych kolumn
    // który to alarm (wiersz lub entry)
    auto range_list = ui->tableWidget->selectedRanges();
//    cout << "selected range list size = ", range_list.size();
    if(range_list.empty())     // jesli zaden, to error message
    {
       // QMessageBox::StandardButton reply =
            QMessageBox::warning
            ( this,
                               tr("No alarm entry selected"),
                               tr( "At first select some alarm by clicking on its name "),
                               QMessageBox::Ok);
        return;
    }

    // sprawdzamy nr_entry wybranego alarmu
    int wiersz = range_list[0].topRow();

     auto txt = ui->tableWidget->item(wiersz, 5)->text().toStdString();
    int nr_entry = stoi(txt.c_str());      // przeliczamy na
    // int nr_entry = wiersz; //    stoi(txt.c_str());

   // int nr_entry =  ui->tableWidget->item(wiersz, 5)->text().toInt();
//         cout << "znaleziony nr_entry =" << nr_entry << endl;

    dlg->ustaw_alarm_do_edycji(tablica_alarmow[nr_entry]); // bo numeracja jest od 0

    if( dlg->exec() == QDialog::Accepted)
    {
        auto zmodyfikowany = dlg->daj_rezultat();
        //    entry.nr_entry = tablica_alarmow.size();
        tablica_alarmow[nr_entry] = zmodyfikowany;
        redisplay_table();
        // zapis na dysk?
        zapisanie_tablicy_na_dysku();
    }
    redisplay_table();
    delete dlg;

}
//***************************************************************************************************************
void Alarmy::on_pushButton_OK_clicked()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    //     cout << "Alarmy:: on_pushButton_OK_clicked()"  << endl;
    zapisanie_tablicy_na_dysku();   // bo moglismy cos usunac
    close();   // zamykamy to okno wszystkich alarmow
}
//***************************************************************************************************************
void Alarmy::on_tableWidget_cellDoubleClicked(int row, int column)
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    on_tableWidget_cellClicked(row, column);
    on_pushButton_edytuj_clicked();

}
//***************************************************************************************************************

void Alarmy::on_pushButton_test_clicked()
{
    // cout<< __PRETTY_FUNCTION__ << endl;
    //  cout << "Test alarmu " << endl;
    QList<QTableWidgetSelectionRange> range = ui->tableWidget->selectedRanges() ;
    if( range.count() ) {
        int nr =  range[0].bottomRow();

        // wywołanie funkcji alarmowej
        //     int nr = range.topRow();

        tablica_alarmow[nr].flag_test_alarm_now = true;
    }
}
//***************************************************************************************************************
void Alarmy::on_pushButton_clicked()
{
//         cout<< __PRETTY_FUNCTION__ << " - Wywolanie f. redisplay table " << endl;
        redisplay_table();
}
//***************************************************************************************************************
