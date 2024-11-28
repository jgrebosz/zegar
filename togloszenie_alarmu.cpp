#include "togloszenie_alarmu.h"
#include "ui_togloszenie_alarmu.h"
#include "tjeden_alarm.h"

#include <QMediaPlayer>
#include <QRandomGenerator>

class Tjeden_alarm;


struct Trojka {
    QMediaPlayer * wsk_playera;
    int sekund_utworu;
    bool flag_loop;
};
extern std::vector < Trojka >   vec_playerow_grajaych_teraz;
//*******************************************************************************************************

Togloszenie_alarmu::Togloszenie_alarmu(QWidget *parent) :
//    QDialog(parent),
      QDialog(parent,  Qt::Tool | Qt::WindowStaysOnTopHint   ),
    ui(new Ui::Togloszenie_alarmu)
{
    ui->setupUi(this);

     QRandomGenerator prng1( time(nullptr) );

     // tu można sprawdzić czy kawałki tego okna nie wyjdą poza brzeg ekranu
     auto rec = this->size();
     auto wys = rec.height();
     auto szer = rec.width();

     auto poz_x = prng1.generate() % 1000;
     auto poz_y = prng1.generate() % 1000;
     if(poz_x + szer > 1000) poz_x -= szer;
     if(poz_y + wys > 1000) poz_y -= wys;


     this->move(poz_x, poz_y );
}
//*******************************************************************************************************
Togloszenie_alarmu::~Togloszenie_alarmu()
{
    //    cout << "Destruktor alarmu " << wsk_alarmu->nazwa << endl;
    delete ui;
    delete wsk_alarmu->dlg;
    wsk_alarmu->dlg = nullptr;

    // muzyka
    if(wsk_alarmu->player != nullptr){
        wsk_alarmu->player->stop();
        delete wsk_alarmu->player;
        wsk_alarmu->player = nullptr;
    }
}
//*******************************************************************************************************
void Togloszenie_alarmu::adres_mojego_alarmu (Tjeden_alarm * wsk )
{
    wsk_alarmu = wsk;
}


//*******************************************************************************************************
void Togloszenie_alarmu::wstaw_opis( QString tytul, QString dodatkowy)
{
    setWindowTitle(tytul);
    ui->plainTextEdit->setPlainText(dodatkowy);
}
//*******************************************************************************************************
void Togloszenie_alarmu::on_pushButton_close_clicked()
{
    //cout << "Close dla alarmu " << wsk_alarmu->nazwa << endl;
    // zgaszenie flagi ekranowej w alarmie
    wsk_alarmu->flag_alarm_jest_na_ekranie = false;

    if(wsk_alarmu->player != nullptr)
    {
        wsk_alarmu->player->stop();
        usuniecie_playera_z_wektora_grajacych();
    }

    // skasowanie dialogu z pamięci
    destroy(true); // wywola destruktor?...

    //    cout << "po destroy " << wsk_alarmu->nazwa << endl;


}


//struct Trojka {
//    QMediaPlayer * wsk_playera;
//    int sekund_utworu;
//    bool flag_loop;
//};
//extern std::vector < Trojka >   vec_playerow_grajaych_teraz;

//************************************************************************************************
void Togloszenie_alarmu::usuniecie_playera_z_wektora_grajacych()
{
// moze to zrobic jako funkcję w mainWindow?

    for( auto it =  vec_playerow_grajaych_teraz.begin() ;
         it != vec_playerow_grajaych_teraz.end() ; )
    {
        if(it->wsk_playera == wsk_alarmu->player)
        {
            vec_playerow_grajaych_teraz.erase(it);
        }
        else
            ++it;

    }


}
