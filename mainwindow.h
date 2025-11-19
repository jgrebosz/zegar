#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMouseEvent>
#include <QPoint>
#include <QMediaPlayer>
//#include <QRubberBand>
#include <QDate>
#include <QTime>



#include <string>
#include <vector>
#include "tjeden_alarm.h"
#include "alarmy.h"   // dla case_czestotliwosc

extern std::string nazwa_pliku_sygnalizujacego_prace;
extern const int CO_ILE_SEK_SPRAWDZENIE_AKTYWNOSCI ;
extern std::string nazwa_pliku_z_opcjami;
extern std::string pathed_nazwa_pliku_z_opcjami ;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Ui {
class MainWindow;
}
//---------------------------------------
class Topcje_dlg;
//-------------------------------------------------
enum  class  tryb_wskazowek { recent = 0, ulubione, fabryczne };

enum  class  Rola_wskazowki { zadne = 0,
                              wsk_dzien_miesiaca = 1,
                              wsk_full_date = 1<<2,
                              wsk_miesiaca = 1<<3,
                              liczba_rok = 1<<4,
                              tarcza_faza_ksiezyca = 1<<5,
                              tarcza_night_day = 1<<6,
                              tarcza_zones = 1<<7,
                              wsk_sekundnik_dodatkowy = 1<<8,
                              wsk_godzinowa = 1 << 9,
                              wsk_minutowa = 1 <<10,
                              wsk_sekundowa = 1 << 11
                            };


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dodatkowe tarcze, wskazowki
// wsk. dzien tygodnia
// wsk. dzien miesiaca
// wsk. miesiaca
// liczba: rok
// tarcza - faza ksiezyca
// tarcza - dzien/noc godzinowo

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Tdane_wskazowki
{
    Rola_wskazowki rola_wskazowki;
    bool flaga_enable = false;
    int poz_x = 0;   // pozycja osi na tarczy
    int poz_y = 0;
    std::string nazwa;
    std::string bitmapa;
    double mimosrod_x = 0;        // w % w stosunku do p-tu obrotu
    double mimosrod_y = 0;        // w % w stosunku do p-tu obrotu
    double skala_x = 1;    // % promienia tarczy (grubosc - tylko dla wsk wektorowej)
    double dlugosc = 1;  // dugość od osi -  //  dlug w  % promienia tarczy
    bool flag_bitmapowa = true;
    QColor kolor = QColor(255, 255, 255);
    bool flag_bitmapa_moze_byc_kolorowana = false;
    int jesli_wektorowa_to_typ = 0;
    int odleglosc_cienia = -1;
    int warstwa = 0;  // na jakiej warstwie wyswietlac
    QPixmap *pix_ptr = nullptr;

    std::function<double ()> obliczenie_kata = nullptr;    // dla funkcji lambda obliczajacej kat
    //            [] (int x) { cout << "lambda obliczenie kata dla " << x << endl;
    //        return 3.3;
    //    };

    // rezultaty po przeliczeniu wskazowki na dany cyferblat
    double przel_mimosrod_x = 0;
    double przel_mimosrod_y = 0;
    double przel_length_poly = 2000;    // % promienia tarczy (grubosc - tylko dla wsk wektorowej)
    double przel_szer_poly = 10;  // dugość od osi -  //  dlug w  % promienia tarczy

    double przel_wsk_korekcji_dlugosci = 1; // dla wektorowej wsk ?
    double przel_wsk_korekcji_szerokosci = 1;   // nie uzywane?

    double przel_skalowanie_x = 1; // dla bitmapowej wsk
    double przel_skalowanie_y = 1;


    // konstruktory ----------------------------------
    Tdane_wskazowki() {};

    Tdane_wskazowki(
            Rola_wskazowki rola_,
            std::string nazwa_,
            std::string bitmapa_,
            double mimosrod_x_,        // w % w stosunku do p-tu obrotu
            double mimosrod_y_,         // w % w stosunku do p-tu obrotu
            double skala_x_,    // % promienia tarczy (grubosc - tylko dla wsk wektorowej)
            double dlugosc_,   // dugość od osi -  //  dlug w  % promienia tarczy
            QColor kolor_,
            bool flag_bitmapa_moze_byc_kolorowana_
            ):
        rola_wskazowki(rola_),
        nazwa(nazwa_),
        bitmapa(bitmapa_),
        mimosrod_x (mimosrod_x_),        // w % w stosunku do p-tu obrotu
        mimosrod_y(mimosrod_y_),        // w % w stosunku do p-tu obrotu
        skala_x(skala_x_),    // % promienia tarczy (grubosc - tylko dla wsk wektorowej)
        dlugosc(dlugosc_),  // dugość od osi -  //  dlug w  % promienia tarczy
        kolor( kolor_),
        flag_bitmapa_moze_byc_kolorowana(flag_bitmapa_moze_byc_kolorowana_)
    { }

    // konstruktor bez argumentu dot. dlugosci
    Tdane_wskazowki(
            Rola_wskazowki rola_,
            std::string nazwa_,
            std::string bitmapa_,
            double mimosrod_x_,        // w % w stosunku do p-tu obrotu
            double mimosrod_y_,         // w % w stosunku do p-tu obrotu
            double skala_x_,    // % promienia tarczy (grubosc - tylko dla wsk wektorowej)

            QColor kolor_,
            bool flag_bitmapa_moze_byc_kolorowana_
            ):
        rola_wskazowki(rola_),
        nazwa(nazwa_),
        bitmapa(bitmapa_),
        mimosrod_x (mimosrod_x_),        // w % w stosunku do p-tu obrotu
        mimosrod_y(mimosrod_y_),        // w % w stosunku do p-tu obrotu
        skala_x(skala_x_),    // % promienia tarczy (grubosc - tylko dla wsk wektorowej)
        dlugosc(1),  // dugość od osi -  //  dlug w  % promienia tarczy
        kolor( kolor_),
        flag_bitmapa_moze_byc_kolorowana(flag_bitmapa_moze_byc_kolorowana_)
    { }

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Tdane_cyferblatu
{
    std::string nazwa;
    std::string bitmapa_tarcza;
    std::string bitmapa_godzinowa;
    std::string bitmapa_minutowa;
    std::string bitmapa_sekundnik;
    std::string bitmapa_central;
    std::string bitmapa_moon;

    double mimosrod_x_wektorowej_godzinowej;
    double mimosrod_y_wektorowej_godzinowej;
    double szer_x_godzinowej;
    double dlugosc_godzinowej;
    QColor kolor_godzinowej;

    double mimosrod_x_wektorowej_minutowej;
    double mimosrod_y_wektorowej_minutowej;
    double szer_x_minutowej;
    double dlugosc_minutowej;
    QColor kolor_minutowej;

    double mimosrod_x_wektorowego_sekundnika;
    double mimosrod_y_wektorowego_sekundnika;
    double szerokosc_x_sekundnika;
    double dlugosc_y_sekundnika;
    QColor kolor_sekundnika;

    double os_x_center;  // środek tarczy x
    double os_y_center;   // srodek tarczy y
    // zaślepka osi
    double skala_x_center;
    double skala_y_center;

    double promien_tarczy;

    double  odrebna_os_x;
    double  odrebna_os_y;

    // dane dla fazy ksiezyca
    bool flaga_ksiezyc = false;
    double x_ksiezyc;  //
    double y_ksiezyc;   //
    double skala_ksiezyc;     // i wsp skali jego bitmapy


    //- Standardowe wskazowki ---------------------------
    // dane 2 wskazowki godzinowej
    bool flaga_wsk_godzinowej = true;
    Tdane_wskazowki  wsk_godzinowa;

    // dane 2 wskazowki minutowej
    bool flaga_wsk_minutowej = true;
    Tdane_wskazowki  wsk_minutowa;

    // dane 2 wskazowki sekundowej
    bool flaga_wsk_sekundowej = true;
    Tdane_wskazowki  wsk_sekundowa;

    //-- extra wskazowki ------------------

    // dane wskazowki dnia tygodnia
    bool flaga_wsk_dnia_tygodnia = false;
    Tdane_wskazowki  wsk_dni_tygodnia;


    // dane wskazowki dnia miesiaca
    bool flaga_wsk_dnia_miesiaca = false;
    Tdane_wskazowki  wsk_dni_miesiaca;

    // dane wskazowki miesiaca
    bool flaga_wsk_miesiaca = false;
    Tdane_wskazowki  wsk_miesiaca;

    // dane wskazowki-night_day
    bool flaga_wsk_night_day = false;
    Tdane_wskazowki  wsk_night_day;

    // dane wskazowki-zones
    bool flaga_wsk_zones = false;
    Tdane_wskazowki  wsk_zones;

    // dane wskazowki-dodatkowy sekundnik
    bool flaga_wsk_sekundnik_dodatkowy  = false;
    Tdane_wskazowki wsk_sekundnik_dodatkowy;


    vector<Tdane_wskazowki* > pointery_wskazowek;


    // teksty ---------------
    QPoint   pozycja_AM_PM;
    QColor kolor_AM_PM;

    QPoint   pozycja_full_date;
    QColor kolor_dnia_tygodnia;

    // pozycja Daty
    QPoint pozycja_datownika;
    double szer_datownika;
    double wys_datownika; // (i fontu) w % dlugosci promienia
    QColor kolor_datownika;
    QColor kolor_ramki_datownika;
    QColor kolor_tla_datownika;


    QColor kolor_cienia;
    double uskok_cienia;
    QColor  tlo_AM_PM;


    bool flag_fabryczna_wsk_godz_bitmapowa = true;
    bool flag_fabryczna_wsk_min_bitmapowa = true;
    bool flag_fabryczna_wsk_sek_bitmapowa = true;

    //**********************************************************************************************
    // konstruktory -----------------------------------------------------------------
    Tdane_cyferblatu( ):wsk_dni_tygodnia() { };  // domniemany

    // konstruktor zwykły, bez księżyca
    Tdane_cyferblatu(  std::string nazwa_,
                       std::string bitmapa_tarcza_,
                       std::string bitmapa_godzinowa_,
                       std::string bitmapa_minutowa_,
                       std::string bitmapa_sekundnik_,
                       std::string bitmapa_central_,


                       double os_x_godzinowej_,
                       double os_y_godzinowej_,
                       double skala_x_godzinowej_,
                       double skala_y_godzinowej_,
                       QColor kolor_godzinowej_,

                       double os_x_minutowej_,
                       double os_y_minutowej_,
                       double skala_x_minutowej_,
                       double skala_y_minutowej_,
                       QColor kolor_minutowej_,

                       double os_x_sekundnika_,
                       double os_y_sekundnika_,
                       double skala_x_sekundnika_,
                       double skala_y_sekundnika_,
                       QColor kolor_sekundnika_,

                       double os_x_center_,  // środek tarczy x
                       double os_y_center_,   // srodek tarczy y
                       // zaślepka osi
                       double skala_x_center_,
                       double skala_y_center_,

                       double promien_tarczy_,
                       double odrebna_os_x_,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                       double odrebna_os_y_,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                       // teksty ---------------

                       QPoint   pozycja_AM_PM_,
                       QColor kolor_AM_PM_,

                       QPoint   pozycja_dnia_tygodnia_,
                       QColor kolor_dnia_tygodnia_,

                       // pozycja Daty
                       QPoint pozycja_daty_,
                       double szer_daty_,
                       double wys_daty_,  // wys okienka i fontu  (w % dlugosci promienia)
                       QColor kolor_daty_,
                       QColor kolor_ramki_daty_,
                       QColor kolor_tla_daty_,
                       QColor kolor_cienia_ = QColor(0, 0, 0, 200),
                       double uskok_cienia_ = 5,
                       QColor tlo_AM_PM_ = QColor(0, 0, 0, 0)
            );

    // konstruktor tarczy z fazą księżyca
    Tdane_cyferblatu(  std::string nazwa_,
                       std::string bitmapa_tarcza_,
                       std::string bitmapa_godzinowa_,
                       std::string bitmapa_minutowa_,
                       std::string bitmapa_sekundnik_,
                       std::string bitmapa_central_,


                       double os_x_godzinowej_,
                       double os_y_godzinowej_,
                       double skala_x_godzinowej_,
                       double skala_y_godzinowej_,
                       QColor kolor_godzinowej_,

                       double os_x_minutowej_,
                       double os_y_minutowej_,
                       double skala_x_minutowej_,
                       double skala_y_minutowej_,
                       QColor kolor_minutowej_,

                       double os_x_sekundnika_,
                       double os_y_sekundnika_,
                       double skala_x_sekundnika_,
                       double skala_y_sekundnika_,
                       QColor kolor_sekundnika_,

                       double os_x_center_,  // środek tarczy x
                       double os_y_center_,   // srodek tarczy y
                       // zaślepka osi
                       double skala_x_center_,
                       double skala_y_center_,

                       double promien_tarczy_,
                       double odrebna_os_x_,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                       double odrebna_os_y_,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                       double x_ksiezyc_,  // pozycja obrazka z fazą księżyca
                       double y_ksiezyc_,   //
                       double skala_ksiezyc_,   //   // i wsp skali jego bitmapy
                       // teksty ---------------

                       QPoint   pozycja_AM_PM_,
                       QColor kolor_AM_PM_,

                       QPoint   pozycja_dnia_tygodnia_,
                       QColor kolor_dnia_tygodnia_,

                       // pozycja Daty
                       QPoint pozycja_daty_,
                       double szer_daty_,
                       double wys_daty_,  // wys okienka i fontu  (w % dlugosci promienia)
                       QColor kolor_daty_,
                       QColor kolor_ramki_daty_,
                       QColor kolor_tla_daty_,
                       QColor kolor_cienia_ = QColor(0, 0, 0, 200),
                       double uskok_cienia_ = 5,
                       QColor tlo_AM_PM_ = QColor(0, 0, 0, 0)
            ) ;




    void dodaj_extra_wskazowke(
            Tdane_wskazowki * wsk,
            enum  Rola_wskazowki extra,
            int os_x, int os_y,
            std::string nazwa,
            std::string bitmapa,
            double mimosrod_x,        // w % w stosunku do p-tu obrotu
            double mimosrod_y,        // w % w stosunku do p-tu obrotu
            double skala_x,   // % promienia tarczy (grubosc - tylko dla wsk wektorowej)
            double dlugosc,  //    // (długość od osi -  //  dlug w  % promienia tarczy NIEISTOTNE, dlugosc brana jest z cyferblatu
            QPixmap * pixmapa,
            QColor kolor,
            bool flag_bitmapa_moze_byc_kolorowana,
            int odl_cienia,
            int warstwa, // na jakiej warstwie rysowac
            std::function<double ()> lambda_obliczenie_kata
            );

    void zapis_danych_o_cyferblacie();
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class Topcje_dlg;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    std::string sciezka;


    bool zmiana_wygladu_cyferblatu(int nr, tryb_wskazowek  tryb);    // tryb 0 = zwykly, 1 uzyj favourites (jesli sa),  2 - wróć do fabrycznych

    void resize_okna_z_tarcza();
    void zmien_przezroczystosc(int value);

    bool spr_rozne_typy_alarmu(Tjeden_alarm &a, QTime time, QDate date);
    string kosmetyka_nazwy_programu(string nazwa);
    void zmiana_opcji_on_top(bool flag );
    void zmiana_jezyka(int nr_jezyka);

    void zapisz_ulubione_wskazowki_na_dysku();
    void odczytaj_ulubione_wskazowki_z_dysku();


private slots:

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *event)override;
    //    void mouseReleaseEvent([[maybe_unused]] QMouseEvent *   event)override
    //    {

    //    }
    //        void closeEvent(QCloseEvent *event) override;  // dla system_tray


    void sprawdzenie_alarmow();
    void sygnalizacja_pracujacego_programu();

    void wywolanie_okna_alarmow();
    void wywolanie_okna_opcji();


    // void wywolanie_okna_wskazowek();
    void resource_files_use();
    void nastepna_tarcza();
    void koniec_programu();
    void player_position_upadate(qint64 position);

    void player_loop_needed(QMediaPlayer::MediaStatus status);

    void pokaz_ramke_dialogu()
    {
        flag_dialog_bez_ramki = !flag_dialog_bez_ramki;
        cout << "czy bez ramki = " << flag_dialog_bez_ramki << endl;
        setWindowFlag(Qt::FramelessWindowHint, flag_dialog_bez_ramki);
        showNormal();

        setVisible(true);
    }

protected:
    void paintEvent(QPaintEvent *event) override;
protected:

private:

    QTime now_time;
    QDate now_today;
    QDateTime current_zone_date_time;
    void ustaw_czas_wg_strefy();
    Ui::MainWindow *ui;

    QTimer *timer_sprawdzania_pracy_programu = nullptr;


    QPixmap minutowa;
    QPixmap godzinowa;
    QPixmap sekundnik;
    QPixmap tarcza;
    QPixmap central;
    QPixmap ksiezyc;
    QPixmap pix_wskazowka_dni_tygodnia;
    QPixmap pix_wskazowka_dni_miesiaca;
    QPixmap pix_wskazowka_miesiaca;
    QPixmap pix_wskazowka_night_day;
    QPixmap pix_wskazowka_zones;
    QPixmap pix_wskazowka_sekundnik_dodatkowy;


    int nr_biezacej_fazy_ksiezyca = -1 ;

    //vector<string> klatki_faz { "00",   "01", "02", "03", "04", "05", "06",
    //                                  "07",   "08", "09", "10", "11", "12", "13",
    //                                 "13",  "14", "15", "16", "17", "18", "19",
    //                                 "19",   "21", "21", "22", "22", "23", "23", "00",
    //                        //  "00"   // <-- czasami jest 29 gdy
    //                          };
    vector<string> klatki_faz { "01", "02", "03", "04", "05", "06",
                                "07",   "08", "09", "10", "11", "12", "13",
                                "14", "15", "16", "17", "18", "19",
                                "20",   "21", "22", "23", "24", "25", "26", "27", "28",
                                  "01"   // <-- czasami jest 29 gdy
                              };

    bool wybor_cyferblatu(string nazwa);


    double rozm_tarczy_x = 200;
    double rozm_tarczy_y = 200;


    //-------------

    void wstepne_zaladowanie_tablicy_alarmow();
    Tjeden_alarm wyjecie_itemow(std::string one);
    std::string znajdz_txt_item(std::string txt, std::string keyword);
    int znajdz_int_item(std::string txt, std::string keyword);

    void wstepne_wpisanie_cyferblatow();
    void wstepne_wpisanie_wskazowek_godzinowych();
    void wstepne_wpisanie_wskazowek_minutowych();
    // void wstepne_wczytanie_faz_ksiezyca();



    std::string nazwa_pliku_z_alarmami { "user_alarms.dat"};
    std::string pathed_nazwa_pliku_z_alarmami ;
    std::string  nazwa_pliku_z_faworytami { "favorites.dat" };
    std::string  pathed_nazwa_pliku_z_faworytami;

    void zapis_opcji_na_dysku();
    void wczytanie_opcji_z_dysku();

    void rysuj_wskaz_wektorowa(QPainter & painter, int nr, double x, double y, double dlug, double szer,
                               QColor kolor,
                               double cien_x, double cien_y,  QColor kolor_cienia,
                               double owal_x =0, double owal_y = 0);



    vector<Tdane_cyferblatu> cyferblat;
    unsigned int nr_tarczy = 0;
    Tdane_cyferblatu cyf; // biezacy cyferblat;




    int nr_wybranej_bitmapy_wsk_godzinowej = 0;
    int nr_wybranej_bitmapy_wsk_minutowej = 0;
    int nr_wybranej_bitmapy_wsk_sekundowej = 0;
    int nr_wybranej_bitmapy_wsk_dni_tyg = 0;
    int nr_wybranej_bitmapy_wsk_dni_mies = 0;
    int nr_wybranej_bitmapy_wsk_miesiaca = 0;
    int nr_wybranej_bitmapy_wsk_night_day = 0;
    int nr_wybranej_bitmapy_wsk_zones = 0;
    int nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy = 0;
    vector <Tdane_wskazowki> vect_godzinowych;
    vector <Tdane_wskazowki> vect_minutowych;
    vector <Tdane_wskazowki> vect_sekundnikow;
    vector <Tdane_wskazowki> vect_extra_wskazowek;

    int nr_wsk_wektorowej_godz = 0;
    int nr_wsk_wektorowej_min = 0;
    int nr_wsk_wektorowej_sek = 0;

    int nr_wsk_wektorowej_tyg = 0;
    int nr_wsk_wektorowej_dni_mies = 0;
    int nr_wsk_wektorowej_miesiaca = 0;
    int nr_wsk_wektorowej_night_day = 0;  // nigdy nie używane
    int nr_wsk_wektorowej_zones = 0;  // nigdy nie używane
    int nr_wsk_wektorowej_sekundnik_dodatkowy = 0;  //

    // do suwania tarczy po ekranie
    int lastMouseX = 0;
    int lastMouseY = 0;

    // dla lambda obliczenie_kata NOW
    int now_minutes_from_UTC;
    int now_utc_hour = 0;
    int now_utc_minute = 0;

    // koniec danych lambd

    int  ile_minut_offsetu_DST = 0;

    void przelicz_wskazowke(int nr_wybranej_wsk_any,
                            vector <Tdane_wskazowki> & vect_any,
                            QPixmap & anyowa,
                            bool flag_czy_bitmapowo,

                            // dotychczasowe dane o tej wskazowce
                            double & cyf_os_x_any,
                            double & cyf_os_y_any,
                            double & cyf_dlugosc_any,
                            double & cyf_szer_any,
                            QColor &cyf_kolor_any,
                            // rezultaty
                            double  & any_mimosrod_y,
                            double  & any_mimosrod_x,
                            double & any_length_poly,
                            double & any_szer_poly,
                            //                            double & any_wsp_korekcji_dlugosci,
                            //                            double &any_wsp_korekcji_szerokosci

                            //                            // dla bitmapowej  wsk godzinowej
                            double & any_skalowanie_x,
                            double & any_skalowanie_y

                            );


    double promien = 100;

    //-------

    int  znajdz_nr_wskazowki_o_bitmapie(string nazwa, vector<Tdane_wskazowki> & v);
    int  znajdz_nr_wskazowki_o_nazwie(string naz, vector<Tdane_wskazowki> &v);

    void wgraj_wskazowke(string nazwa,  vector <Tdane_wskazowki> & vecwsk,
                         bool *flag_bitvec, int * nr_wekt, int * nr_bitm, QPixmap *bitmapa);

    void wgraj_wskazowke_fabryczna(string nazwa,  vector <Tdane_wskazowki> & vecwsk,
                                   bool *flag_bitvec, int * nr_wekt, int * nr_bitm, QPixmap *bitmapa);
    bool czy_nazwa_naprawde_bitmapowa(string nazwa);
    bool czy_biezace_wskazowki_sa_fabryczne();
    int  id_linux_czy_windows_version();
    void make_shutdown();

    void wybierz_obrazek_ksiezyca();

    void rysuj_ogolna_wskazowke( QPainter &painter,  Tdane_wskazowki & wskazowka);
    void sortowanie_warstw();
    QColor przyciemnij(QColor k)
    {
        return QColor{ k.red() / 8,   k.green() / 8,  k.blue() / 8,  k.alpha()};
    }

    int random_cyferblat();

    //**********************************************************************************************
    void ustaw_lambdy_defaultowych_wskazowek()   // czyli godz, min, sek
    {
        cyf.wsk_godzinowa.obliczenie_kata =
                [this] () {
            double wynik =
                    30.0 * ((this->now_time.hour() + this->now_time.minute() / 60.0));
            //cout << "lambda obliczenie kata dla godzinowej " << " = " << wynik  << endl;
            return wynik;
        };

        cyf.wsk_godzinowa.warstwa = 20;
        cyf.wsk_godzinowa.rola_wskazowki = Rola_wskazowki::wsk_godzinowa;

        cyf.wsk_minutowa.obliczenie_kata =
                [this] () {
            double wynik =
                    6.0 * (now_time.minute() + now_time.second() / 60.0);
            //cout << "lambda obliczenie kata dla minutowej " << " = " << wynik  << endl;
            return wynik;
        };

        cyf.wsk_minutowa.warstwa = 21;
        cyf.wsk_minutowa.rola_wskazowki = Rola_wskazowki::wsk_minutowa;

        cyf.wsk_sekundowa.obliczenie_kata =
                [this] () {
            double wynik = 6.0 * (this->now_time.second() );
            //cout << "lambda obliczenie kata dla sekundnika " << " = " << wynik  << endl;
            return wynik;
        };

        cyf.wsk_sekundowa .warstwa = 22;  // this is a main second pointer. (The secondary can have different )
        cyf.wsk_sekundowa.rola_wskazowki = Rola_wskazowki::wsk_sekundowa;
    }

    // System tray

    void createTrayActions_i_context_menu();
    void createTrayIcon_i_dodaj_context_menu();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    // akcje na popup menu ---------------- CHWILOWO, potem z tego zrezygnowac i dać zwykle
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    bool flag_dialog_bez_ramki = true;
    int szer_ekranu_windows = 800;
    int wys_ekranu_windows = 600;

    void  smart_positioning(QDialog * dlg);

};

#endif // MAINWINDOW_H
