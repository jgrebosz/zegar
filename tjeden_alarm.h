#ifndef TJEDEN_ALARM_H
#define TJEDEN_ALARM_H

#include <string>
#include <iostream>

#include <QStringList>
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

using namespace  std;

//  #define tr  QObject::tr

class Togloszenie_alarmu;
class QMediaPlayer;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Tjeden_alarm
{
    friend class Alarmy;

public:   // jeśli coś dodasz do tej listy, to pamietaj, ze trzeba to też wczytać z dysku  (f. wyjecie_itemow)
    int nr_entry;
    bool flag_enable = true;
    std::string nazwa = "Alarm";
    std::string godzina;
    int czestotliwosc = 0 ;
    std::string data;
    int8_t dni_tygodnia;
    bool  flag_even_odd_week_nr = 0;
    int interwal = 15;
    bool flag_okno = true;
    std::string dodatkowy_opis;
    bool flag_dzwiek = false;
    std::string muzyka;
    int sekund_dzwieku = 0;
    bool loop = false;
    bool flag_program = false;
    std::string  nazwa_programu;
    bool  flag_wylacz_komputer = false;

    bool flag_alarm_jest_na_ekranie = false;
    Togloszenie_alarmu *dlg = nullptr;
    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutput;
    bool flag_test_alarm_now = false;


  static  QStringList list_czestotliwosc ;
//  =
//    {
//        QObject::tr("Once"),            // "Raz",
//       QObject::tr("Every day"),            //"Codziennie",
//      QObject:: tr("Every week"),            //"Co tydzień",
//        QObject:: tr("Every 2 weeks"),            //"Co dwa tygodnie",
//      QObject:: tr("Every month"),            //"Co miesiąc",
//       QObject::tr("Every year"),            //"Co rok",
//      QObject:: tr("Every N Minutes"),            //"Minuty"
//       QObject:: tr("Every EVEN week"),            //"Co PARZ  tydzień",,
//       QObject:: tr("Every ODD week"),            //"Co NIEPARZ  tydzień",
//    };





    friend
    std::ostream & operator << (std::ostream &plik, Tjeden_alarm & a)
    {
      //  std::string sep {"|||"};
#define xm(n)  "\t<" << #n << ">" << a.n << "</" << #n << ">\n"
        plik
                << "<alarm>\n"
                << xm(flag_enable)
                << xm(nazwa)
                   << xm(godzina)
                   << xm(czestotliwosc)
                   << xm(data)                      
                   <<  "\t<dni_tygodnia>" <<   (int) (a.dni_tygodnia) <<  "</dni_tygodnia>\n"
                    << xm(flag_even_odd_week_nr)
                   << xm(interwal)
                   << xm(flag_okno)
                   << xm(dodatkowy_opis)
                   << xm(flag_dzwiek)
                   << xm(muzyka)
                   << xm(sekund_dzwieku)
                   << xm(loop)
                   << xm(flag_program)
                   << xm(nazwa_programu)
                   << xm(flag_wylacz_komputer)
               << "</alarm>\n"
                << endl;

        // cout << "Zapisany "   << (int) (a.dni_tygodnia) << endl;
        return  plik;
    }


//    friend
//    std::ifstream & operator >> (std::ifstream &plik, Tjeden_alarm & a)
//    {
//        std::string sep {"|||"};

//        // wczytanie calego jednego alarmu

////        getline(plik, a.flag_enable, sep);

////       plik << a.nazwa << sep
////                   << a.godzina << sep
////                   << a.czestotliwosc << sep
////                   << a.data << sep
////                   << a.dni_tygodnia << sep
////                   << a.interwal << sep
////                   << a.flag_okno << sep
////                   << a.dodatkowy_opis << sep
////                   << a.flag_dzwiek << sep
////                   << a.muzyka << sep
////                   << a.sekund_dzwieku << sep
////                   << a.loop << sep
////                   << a.flag_program << sep
////                   << a.nazwa_programu << sep
////                   << a.flag_wylacz_komputer << sep
////                   << endl;
//        return  plik;
//    }

public:
    Tjeden_alarm();
    //-----------------------------------
    Tjeden_alarm(
            int nr,
            bool flag_enable_,
            std::string nazwa_,
            std::string godzina_,
            int czestotliwosc_,
            std::string data_,
           int8_t dni_tygodnia_,
            bool  flag_even_odd_week_nr_,
            int minuty_,
            bool flag_okno_,
            std::string dodatkowy_opis_,
            bool flag_dzwiek_,
            std::string muzyka_,
            int sekund_dzwieku_,
            bool loop_,
            bool flag_program_,
            std::string nazwa_programu_,
            bool flag_wylacz_komputer_);
};

#endif // TJEDEN_ALARM_H
