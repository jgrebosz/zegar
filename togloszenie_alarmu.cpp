#include "togloszenie_alarmu.h"
#include "ui_togloszenie_alarmu.h"
#include "tjeden_alarm.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRandomGenerator>

#include <algorithm>

struct Trojka {
    QMediaPlayer * wsk_playera;
    int sekund_utworu;
    bool flag_loop;
};
extern std::vector < Trojka >   vec_playerow_grajaych_teraz;
extern void zatrzymaj_dzwiek_alarmu(Tjeden_alarm &al);
//*******************************************************************************************************

Togloszenie_alarmu::Togloszenie_alarmu(QWidget *parent) :
//    QDialog(parent),
      QDialog(parent,  Qt::Tool | Qt::WindowStaysOnTopHint   ),
    ui(new Ui::Togloszenie_alarmu)
{
    ui->setupUi(this);
    // close() ma skasować obiekt; destruktor sprząta dźwięk i wskaźnik w alarmie.
    setAttribute(Qt::WA_DeleteOnClose);

     // Pozycja losowa, ale tak, żeby całe okno zostało na ekranie.
     const QRect ekran = QGuiApplication::primaryScreen()
             ? QGuiApplication::primaryScreen()->availableGeometry()
             : QRect(0, 0, 1000, 1000);
     const QSize rozmiar = size();
     const int max_x = qMax(0, ekran.width() - rozmiar.width());
     const int max_y = qMax(0, ekran.height() - rozmiar.height());

     const int poz_x = ekran.x() + QRandomGenerator::global()->bounded(max_x + 1);
     const int poz_y = ekran.y() + QRandomGenerator::global()->bounded(max_y + 1);
     move(poz_x, poz_y);
}
//*******************************************************************************************************
Togloszenie_alarmu::~Togloszenie_alarmu()
{
    //    cout << "Destruktor alarmu " << wsk_alarmu->nazwa << endl;
    delete ui;

    // Nie kasujemy dlg: tym oknem jesteśmy my. Tylko odpinamy się od alarmu.
    if (wsk_alarmu != nullptr)
    {
        zatrzymaj_dzwiek_alarmu(*wsk_alarmu);
        wsk_alarmu->flag_alarm_jest_na_ekranie = false;
        if (wsk_alarmu->dlg == this)
            wsk_alarmu->dlg = nullptr;
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
    // Destruktor gasi flagę, zatrzymuje dźwięk i zeruje dlg.
    close();
}


//************************************************************************************************
void Togloszenie_alarmu::usuniecie_playera_z_wektora_grajacych()
{
// moze to zrobic jako funkcję w mainWindow?

    if (wsk_alarmu == nullptr || wsk_alarmu->player == nullptr)
        return;

    QMediaPlayer * const player = wsk_alarmu->player;
    auto &v = vec_playerow_grajaych_teraz;
    // erase() zwraca następny iterator; stary po skasowaniu jest nieważny.
    v.erase(std::remove_if(v.begin(), v.end(),
                           [player](const Trojka &t) {
                               return t.wsk_playera == player;
                           }),
            v.end());
}
