/** TODO:

 *
 */



#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "alarmy.h"
#include "topcje_dlg.h"

#include "togloszenie_alarmu.h"

#include <QMenu>
#include <QTimer>
#include <QTime>
#include <QTimeZone>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QGraphicsPixmapItem>
#include <QtMath>
//#include <QSound>
#include <QMediaPlayer>
#include <QPair>
#include <QDir>
#include <QFileInfoList>
#include <QOperatingSystemVersion>
#include <QProcess>
#include <QScreen>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <QProcess>
#include <QTranslator>
#include <QMessageBox>
#include  <random>
#include <QMediaPlayer>
#include <QAudioOutput>

#include "tpreferred_hands.h"


#define  KRZYZE_PROWADZACE      false //   true  //

#define COTO  ;  cout << "File = " << __FILE__ << "Linia = " << __LINE__ << endl;

bool flag_pokaz_to = false;
using namespace  std;



extern bool flag_ma_byc_restart;
bool wstepne_flag_na_wierzchu;
int wstepne_nr_jezyka;
int nr_obecnie_zainstalowanego_jezyka;
std::string nazwa_pliku_z_opcjami { "zegar_options.dat"};
std::string pathed_nazwa_pliku_z_opcjami ;

string nazwa_strefy_czasowej;
QTimeZone strefa_czasowa = QTimeZone( QString("localTime").toLatin1());              // ( QTimeZone::systemTimeZone() );

extern vector <Tpreferred_hands>  vec_pref_hands;
//******************************************************************************************************************
std::vector <Tjeden_alarm> tablica_alarmow;
//std::vector <int>   vec2_playerow_grajaych_teraz;

struct Trojka {
    QMediaPlayer * wsk_playera;
    int sekund_utworu;
    bool flag_loop;
};
std::vector < Trojka >   vec_playerow_grajaych_teraz;
//-------------------
int przezroczystosc = 255;
int przezroczystosc_dla_kursora = 255;

bool flag_na_wierzchu = false;
bool flag_ograniczenie_pozycji = false;
bool flag_blokuj_na_ekranie = false;

bool flag_uruchom_z_loginem = false;
bool flag_uruchom_przy_logowaniu = true;
bool flag_pozwalam_na_kilka_kopii_program = false;
int nr_jezyka = 0;

double gskala = 1.0;
bool flag_gskala_active = true;

bool flag_show_AM_PM = true;
bool flag_show_full_date = true;
bool flag_show_date = true;
bool flag_show_sekundnik= true;
int poz_srodka_zegara_x, poz_zegara_y;

bool flaga_wskazowki_poprzednie_a_nie_fabryczne = false;
bool flag_random_tarcza;

//***************************************************************************************
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent
                ,
                //  Qt::WindowSystemMenuHint
                //            |  Qt::FramelessWindowHint
                //                            |
                //                Qt::SplashScreen    // dla Windows jest OK (ale nie linux)
                //                |
                Qt::Tool   // to dla Linuksa działa
                ),
    ui(new Ui::MainWindow)
{   
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint, flag_dialog_bez_ramki);

    //  setWindowFlag(Qt::Tool , true);
    //     setWindowFlag(Qt::SplashScreen , true);
    // COTO ;
    ui->setupUi(this);


    //    cout << "Konstruktor Mainwindow "
    //            //         <<   tr( "example of language English ").toStdString()
    //         << endl;

    this->setWindowOpacity(przezroczystosc/255.0);


    createTrayActions_i_context_menu();
    createTrayIcon_i_dodaj_context_menu();
    trayIcon->show();


    // COTO;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    QTimer *timer_sprawdzania_alarmow = new QTimer(this);
    connect(timer_sprawdzania_alarmow, SIGNAL(timeout()),
            this, SLOT(sprawdzenie_alarmow()));
    timer_sprawdzania_alarmow->start(1000);


    timer_sprawdzania_pracy_programu =  new QTimer(this);
    connect(timer_sprawdzania_pracy_programu, SIGNAL(timeout()),
            this, SLOT(sygnalizacja_pracujacego_programu()));
    timer_sprawdzania_pracy_programu->
            start(1000 * CO_ILE_SEK_SPRAWDZENIE_AKTYWNOSCI);


    setWindowTitle("Alarm clock");    // nie tłumaczymy, bo nie bedzie widac

    // COTO;

    QDir dir;
    sciezka = QDir::currentPath().toStdString() + "/";
    //    cout << "Current directory = " << sciezka  << endl;
    pathed_nazwa_pliku_z_opcjami = sciezka + nazwa_pliku_z_opcjami;
    pathed_nazwa_pliku_z_alarmami = sciezka + nazwa_pliku_z_alarmami ;
    pathed_nazwa_pliku_z_faworytami = sciezka + nazwa_pliku_z_faworytami;

    nazwa_strefy_czasowej = "*";   // this means it should be recognised

    //----------------
    QScreen * ekran = QGuiApplication::primaryScreen();
    szer_ekranu_windows = ekran->geometry().width();
    wys_ekranu_windows = ekran->geometry().height();

    //----------------
    wstepne_wpisanie_cyferblatow();
    wstepne_wpisanie_wskazowek_godzinowych();
    wstepne_wpisanie_wskazowek_minutowych();

    zmiana_wygladu_cyferblatu(nr_tarczy,  tryb_wskazowek::fabryczne);
    wczytanie_opcji_z_dysku();

    odczytaj_ulubione_wskazowki_z_dysku();

    wstepne_flag_na_wierzchu = flag_na_wierzchu;




    strefa_czasowa = QTimeZone::systemTimeZone();
    strefa_czasowa = QTimeZone( QString(nazwa_strefy_czasowej.c_str() ).toLatin1());

    //    cout << "Po wczytaniu opcji z dysku nazwa_strefy_czasowej = "
    //         << nazwa_strefy_czasowej.c_str()
    //         << endl;

#if 0
jeszcze_raz:

    QDateTime localTime = QDateTime::currentDateTime();
    QDateTime current_zone_DateTime = localTime;
    if(time_zone.isValid())
    {
        cout << "Time_zone is valid " << endl;
        current_zone_DateTime = localTime.toTimeZone(time_zone) ;
    } else{

        cout << "Time_zone is NOT valid, so we go to localTime " << endl;
        current_zone_DateTime = localTime;
        nazwa_strefy_czasowej = "localTime";
        //             time_zone = QTimeZone( QString("localTime").toLatin1());
        time_zone = QTimeZone::systemTimeZone();
        cout << "PO poprawce  timezone display Name = "
             << nazwa_strefy_czasowej.c_str()
             << endl;


        goto jeszcze_raz;
    }
#endif
    //    cout << " gskala = " << gskala << endl;
    //    gskala = 1;

    zmiana_wygladu_cyferblatu(nr_tarczy,  tryb_wskazowek::ulubione );  // jesli sie nie udalo, to false


    wstepne_zaladowanie_tablicy_alarmow();

    //    string komenda = R"(C:\Program Files (x86)\FreeTime\FormatFactory\FormatFactory.exe)";
    //    //                system(al.nazwa_programu.c_str());
    //    komenda = kosmetyka_nazwy_programu(komenda);

    //    cout << "po kosmetyce = [" << komenda << "]" << endl;


    //    setMouseTracking(true);

    // COTO;
}
//***************************************************************************************
MainWindow::~MainWindow()
{

    //    cout << "Destruktor Mainwindow " << endl;
    delete ui;
}

//***************************************************************************************
bool MainWindow::wybor_cyferblatu(string nazwa)
{
    cout << __PRETTY_FUNCTION__ << endl;
    flag_random_tarcza = false;
    for(uint i = 0 ; i < cyferblat.size() ; ++i)
    {
        if(cyferblat[i].nazwa == nazwa)
        {

            if(i == 0)  {
                i = random_cyferblat();
            }
            zmiana_wygladu_cyferblatu(i,  tryb_wskazowek::ulubione);
            return true;
        }
    }
    return false;
}
//***************************************************************************************
void MainWindow::przelicz_wskazowke(int nr_wybranej_wsk_any,
                                    vector <Tdane_wskazowki> & vect_any,
                                    QPixmap & anyowa,
                                    bool flag_czy_bitmapowo,

                                    // dotychczasowe dane o tej wskazowce
                                    double & cyf_os_x_any,
                                    double & cyf_os_y_any,
                                    double & cyf_dlugosc_any,
                                    double & cyf_szer_any,
                                    QColor & cyf_kolor_any,
                                    // rezultaty
                                    double  & any_mimosrod_x,
                                    double  & any_mimosrod_y,
                                    double & any_length_poly,
                                    double & any_szer_poly,

                                    double & any_skalowanie_x,
                                    double & any_skalowanie_y

                                    )
{


    double dlug_bitmapy ;
    double szer_bitmapy;
    double zadana_dlugosc;
    double zadana_szerokosc;

    //    cout << "Wskazowka bitmapowa ? " << boolalpha << flag_czy_bitmapowo << endl;

    if(flag_czy_bitmapowo)
    {
        bool flag_wolno_kolorowac = false;   // raczej false, bo czasm tarcze maja swoje defaultowe kolorowe bitmapy
        //            czy może zmienona została bitmapa wskazowki any
        if( nr_wybranej_wsk_any != 0)
        {
            // auto & c = cyf;
            auto & v = vect_any[nr_wybranej_wsk_any];
            anyowa.load(v.bitmapa.c_str());

            cyf_os_x_any = v.mimosrod_x;
            cyf_os_y_any = v.mimosrod_y;
            cyf_szer_any = v.skala_x;
            //  cyf_dlugosc_any = v.skala_y;

            flag_wolno_kolorowac = v.flag_bitmapa_moze_byc_kolorowana;
            if(flag_wolno_kolorowac == false){
                cyf_kolor_any = v.kolor;   // oryginalny kolor wskazowki
            }
            //            cout << "Dane z bazy wskazowek " << v.nazwa << ", os obrotu  y = " << cyf_os_y_any << endl;
        }else{
            //                        cout << "Dane z cyferblatu,  os obrotu  y = " << cyf_os_y_any << endl;

        }



        dlug_bitmapy = anyowa.height();
        szer_bitmapy = anyowa.height();
        zadana_dlugosc = cyf_dlugosc_any;
        zadana_szerokosc = cyf_szer_any;



        any_skalowanie_y =
                (promien *  zadana_dlugosc* ( 1 + cyf_os_y_any) ) /  dlug_bitmapy ;


        any_skalowanie_x = 1.0 /((szer_bitmapy / promien) / zadana_szerokosc);
        any_mimosrod_x =  cyf_os_x_any - anyowa.width()/2.0;

        any_mimosrod_y = (cyf_os_y_any *dlug_bitmapy)
                -             (dlug_bitmapy);


        // poszerzenie skali dla nabrania szerokosci wskazowki bitmapowej?
        any_skalowanie_x = 1.0 / (any_skalowanie_x) * cyf_szer_any;
        //        any_skalowanie_y = any_wsp_korekcji_dlugosci ;

        // a może nie pozwalać na zmiane proporcji? NIE, bo deformuje bitmape
        any_skalowanie_x = any_skalowanie_y;




        // podkolorowanie bitmapy dostosowane życzenia  tarczy
        if(cyf_kolor_any != QColor(0,0,0,0) && flag_wolno_kolorowac)
        {
            QPixmap n = anyowa;
            //            n.fill(cyf_kolor_any);
            //            auto maska = anyowa.createMaskFromColor(Qt::transparent);
            //            n.setMask( maska ) ;
            //            anyowa = n;
            // cout << "kolorowanie wskazowki " << endl;

        }
    }
    else {    // wektorowa wskazowka ===========================================


        //        cout << "wektorowa  os obrotu  y = " << cyf_os_y_any << endl;

        dlug_bitmapy = any_length_poly;   // RRR  - to jest ok 2000
        szer_bitmapy = any_szer_poly;   // RRR



        zadana_dlugosc = cyf_dlugosc_any;
        zadana_szerokosc  = cyf_szer_any;

        any_skalowanie_y =
                (promien *  zadana_dlugosc* ( 1 + cyf_os_y_any) ) /  dlug_bitmapy ;
        any_skalowanie_x = 1.0 /((szer_bitmapy / promien) / zadana_szerokosc);

        any_mimosrod_x =  (cyf_os_x_any * promien);
        any_mimosrod_y = (cyf_os_y_any *dlug_bitmapy);

        //        any_skalowanie_x = any_wsp_korekcji_szerokosci;
        //        any_skalowanie_y = any_wsp_korekcji_dlugosci ;

        auto & v = vect_any[nr_wybranej_wsk_any];
        switch(v.rola_wskazowki)
        {
        case Rola_wskazowki::wsk_godzinowa:
            v.jesli_wektorowa_to_typ = nr_wsk_wektorowej_godz; break;
        case Rola_wskazowki::wsk_minutowa:
            v.jesli_wektorowa_to_typ = nr_wsk_wektorowej_min; break;
        case Rola_wskazowki::wsk_sekundowa:
            v.jesli_wektorowa_to_typ = nr_wsk_wektorowej_sek; break;
        default:
            v.jesli_wektorowa_to_typ = 0; break;
        } // end switch

    }

    if(flag_pokaz_to){
        cout  << " flag_czy_bitmapowo = " << flag_czy_bitmapowo
              << " promien_tarczy = " << promien
              << " dlug_bitmapy = "  << dlug_bitmapy
              << ", zadana dlugosc w skali promienia  "
              << zadana_dlugosc
                 //          << ", konieczne mnozenie przez any_skalowanie_x ="           << (any_skalowanie_x)
              << " any_skalowanie_y = " << any_skalowanie_y
              << ", any_mimosrod_y = " << any_mimosrod_y

              << endl;
    }

}
//***************************************************************************************
int MainWindow::znajdz_nr_wskazowki_o_bitmapie(string naz, vector<Tdane_wskazowki> &v)
{

    //     cout << "Szukam wskazowki o takiej nazwie bitmapy " << naz << endl;

    for(uint nr = 0 ; nr < v.size() ; ++nr)
    {
        //         cout << "Porownuje z " << v[nr].bitmapa << endl;;


        if( v[nr].bitmapa == naz)
        {
            // cout << " - znaleziona pod indeksem " << nr << endl;
            return nr;
        }
    }
    cout << " - wskazowka o nazwie [" << naz << "] NIE istnieje  w wektorze wskazowek "
         << " blad programisty.  "
         << "Uwaga, w favourites.dat zapisywane sa nazwy  literackie (nie filename) "
         << endl;
    exit(3);

    return -1;

}
//***************************************************************************************
int MainWindow::znajdz_nr_wskazowki_o_nazwie(string naz, vector<Tdane_wskazowki> &v)
{

    // cout << "Szukam wskazowki o nazwie literackiej lub bitmapy " << naz << endl;
    for(uint nr = 0 ; nr < v.size() ; ++nr)
    {
        //        cout << "Porownuje z nazwa " << v[nr].nazwa << endl;

        if( v[nr].nazwa == naz || v[nr].bitmapa == naz)
        {
            //            cout << " - znaleziona pod indeksem " << nr << endl;
            return nr;
        }
    }
    //        cout << " - NIE znaleziona  po nazwie "
    //             <<  " szukamy wiec po nazwach bitmapy " << endl;
    return     znajdz_nr_wskazowki_o_bitmapie (naz, v);

}
//************************************************************************************************************************
void MainWindow::wgraj_wskazowke(string nazwa,
                                 vector<Tdane_wskazowki> &vecwsk,
                                 bool * flag_bitvec, int *nr_wekt, int * nr_bitm   // tu beda odpowiedzi
                                 , QPixmap *bitmapa)
{

    //     cout << __func__
    //          << " Bedzie proba ladowanie wskazowki o nazwie : "
    //          << nazwa
    //          << " czy to bitmapowa? flag_bitvec = " << *flag_bitvec
    //          << endl;

    // jeśli zadamy bitmapowej
    if(*flag_bitvec){

        // sprawdzamy czy to naprawdę nazwa bitmapowej
        //         cout << "CZy w tym wektorze jest wskazowka o nazwie : " << nazwa << endl;
        if(czy_nazwa_naprawde_bitmapowa(nazwa) )
        {

            *flag_bitvec = true;
            // Jeśli bitmapowa, to przepisujemy stamtąd nazwe ulubionej bitmapy
            *nr_bitm = znajdz_nr_wskazowki_o_nazwie(nazwa,  vecwsk);
            //            cout << "wskazowka " << nazwa << " znaleziona pod numerem " << *nr_bitm << endl;

            auto  rezultat = bitmapa->load( vecwsk[*nr_bitm].bitmapa.c_str() );
            if(!rezultat){

                cout << "Nieudane WGRANIE  pliku bitmapy wskazowki o nazwie : "
                     << vecwsk[*nr_bitm].bitmapa
                     << "    rezultat = " << rezultat << endl;
                cout << endl;
            }
        }
        else
        {
            //  COTO
            //                    cout << "NIE MA w tym wektorze bitmapowej wskazowki o nazwie : " << nazwa
            //                     << ", więc jeśli to wskazowka wektorowa, to zmien flagę i rozkoduj numer ulubionej wsk wektorowej"
            //                            << endl;
            *flag_bitvec = false;
            *nr_wekt = stoi (nazwa );
        }
    }
    else // jeśli żadamy wektorowej
    {
        //                cout << "To jest wskazowka WEKTOROWA, a takich nie trzeba ladowac " << nazwa << endl;
        // jeśli wektorowa, to zaznacz flagę i rozkoduj numer ulubionej wsk wektorowej
        // *flag_bitvec = false;
        // *nr_wekt = stoi (nazwa);
    }
    //    cout << "Koniec funkcji dla: " << nazwa << endl;

}

//************************************************************************************************************************
bool MainWindow::czy_biezace_wskazowki_sa_fabryczne()
{

    bool rezultat = true;

    // sprawdzenie godzinowej ----------------------------------
    auto nazwa_obecnej_godzinowej = cyf.wsk_godzinowa.flag_bitmapowa ?
                vect_godzinowych[nr_wybranej_bitmapy_wsk_godzinowej].bitmapa   :
                to_string(nr_wsk_wektorowej_godz);

    auto nazwa_fab_godzinowej = cyferblat[nr_tarczy].bitmapa_godzinowa;

    if(nazwa_obecnej_godzinowej == nazwa_fab_godzinowej)
    {
        //        cout << "To jest godzinowa wskazowka fabryczyna" << endl;

    }
    else
    {
        //        cout << "To NIE jest godzinowa wskazowka fabryczyna" << endl;
        rezultat =  false;
    }

    // min ---------------------------------
    auto nazwa_obecnej_min =
            //            cyf.wsk_minutowa.flag_bitmapowa ?
            cyf.wsk_minutowa.flag_bitmapowa ?
                vect_minutowych[nr_wybranej_bitmapy_wsk_minutowej].bitmapa   :
                to_string(nr_wsk_wektorowej_min);

    auto nazwa_fab_min = cyferblat[nr_tarczy].bitmapa_minutowa;

    if(nazwa_obecnej_min == nazwa_fab_min)
    {
        //        cout << "To jest mininowa wskazowka fabryczyna" << endl;
    }
    else
    {
        //        cout << "To NIE jest mininowa wskazowka fabryczyna" << endl;
        rezultat =  false;
    }

    // sek  ---------------------------------
    auto nazwa_obecnej_sek =
            //            flag_wsk_sek_bitmapowa ?
            cyf.wsk_sekundowa.flag_bitmapowa?
                vect_sekundnikow[nr_wybranej_bitmapy_wsk_sekundowej].bitmapa   :
                to_string(nr_wsk_wektorowej_sek);

    auto nazwa_fab_sek = cyferblat[nr_tarczy].bitmapa_sekundnik;

    if(nazwa_obecnej_sek == nazwa_fab_sek)
    {
        //        cout << "To jest sekundowa wskazowka fabryczyna" << endl;
    }
    else
    {
        //        cout << "To NIE jest sekundowa wskazowka fabryczyna" << endl;
        rezultat =  false;
    }

    return rezultat;

#if 0
    // czy to naprawdę nazwa bitmapowej
    if(czy_nazwa_naprawde_bitmapowa(nazwa_fab_godzinowej) )
    {
        *flag_bitvec = true;
        // Jeśli bitmapowa, to przepisujemy stamtąd nazwe ulubionej bitmapy
        *nr_bitm = znajdz_nr_wskazowki_o_nazwie(nazwa,  vecwsk);
        bitmapa->load( nazwa.c_str());
    }
    else
    {
        // jeśli wektorowa, to zmien flagę i rozkoduj numer ulubionej wsk wektorowej
        *flag_bitvec = false;
        *nr_wekt = stoi (nazwa );
    }
#endif

}
//************************************************************************************************************************
void MainWindow::wgraj_wskazowke_fabryczna(string nazwa,
                                           vector<Tdane_wskazowki> &vecwsk,
                                           bool * flag_bitvec, int *nr_wekt, int * nr_bitm   // tu beda odpowiedzi
                                           , QPixmap *bitmapa)
{
    // czy to naprawdę nazwa bitmapowej
    if(czy_nazwa_naprawde_bitmapowa(nazwa) )
    {
        *flag_bitvec = true;
        // Jeśli bitmapowa, to przepisujemy stamtąd nazwe ulubionej bitmapy
        *nr_bitm = znajdz_nr_wskazowki_o_nazwie(nazwa,  vecwsk);
        bitmapa->load( nazwa.c_str());
    }
    else
    {
        // jeśli wektorowa, to zmien flagę i rozkoduj numer ulubionej wsk wektorowej
        *flag_bitvec = false;
        *nr_wekt = stoi (nazwa);
    }
}
//************************************************************************************************************************
bool MainWindow::czy_nazwa_naprawde_bitmapowa(string nazwa)
{
    if( nazwa.size() == 1 && isdigit(nazwa[0] ) )
    {
        return false;
    }
    else
    {
        return true;
    }
}
//************************************************************************************************************************
bool MainWindow::zmiana_wygladu_cyferblatu(int nr,  tryb_wskazowek  tryb)
{
    //          cout << __func__  << " tarcza nr " << nr
    //                << "  (z argumentem trybu wskazowek  [recent = 0 , ulubione = 1, fabryczne=2] -->"
    //                << int (tryb) << ")"<< endl;

    nr_tarczy= nr;

    auto tmp_flaga_godz = cyf.wsk_godzinowa.flag_bitmapowa;
    auto tmp_flaga_min = cyf.wsk_minutowa.flag_bitmapowa;
    auto tmp_flaga_sek = cyf.wsk_sekundowa.flag_bitmapowa;


    cyf = cyferblat[nr];


    if(tryb == tryb_wskazowek::recent)    // to poniżej niszczyło przypisanie powyższe !!!!!!!!!!!!!
    {
        cyf.wsk_godzinowa.flag_bitmapowa = tmp_flaga_godz;
        cyf.wsk_minutowa.flag_bitmapowa = tmp_flaga_min;
        cyf.wsk_sekundowa.flag_bitmapowa = tmp_flaga_sek;
    }
    //-------------------
    cyf.wsk_godzinowa.pix_ptr = &godzinowa;
    cyf.wsk_minutowa.pix_ptr = &minutowa;
    cyf.wsk_sekundowa.pix_ptr = &sekundnik;
    // extra wskazowki ustawiają to sobie same w funkcji dodaj_extra

    ustaw_lambdy_defaultowych_wskazowek();

    cyf.wsk_godzinowa.jesli_wektorowa_to_typ = nr_wsk_wektorowej_godz;
    cyf.wsk_minutowa.jesli_wektorowa_to_typ = nr_wsk_wektorowej_min;
    cyf.wsk_sekundowa.jesli_wektorowa_to_typ = nr_wsk_wektorowej_sek;

    //-------------
    cyf.pointery_wskazowek.clear();

#if 1
    if(tryb == tryb_wskazowek::ulubione)
    {
        //

        //                cout << "przejrzenie tablicy Favourites (Ulubione)" << endl;
        //                cout << __func__
        //                      << ", cyf.wsk_godzinowa.flag_bitmapowa = " << boolalpha <<  cyf.wsk_godzinowa.flag_bitmapowa
        //                     <<  " 1  cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha
        //                     <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;

        bool flag_znalezione = false;
        for(const auto & entry : vec_pref_hands)
        {
            if(entry.clock_face_name == cyf.nazwa)    // czy jest zapis ulubionych wskazowek dla tego cyferbaltu?
            {
                //                                 cout << "Znalezione entry favourites dla tarczy " << cyf.nazwa << endl;
                flag_znalezione = true;

                // novum: jest tam też skala zoom
                // if(entry.zoom != 0)
                {
                    //                    cout << "Dotychczasowy zoom gskala = " << gskala << endl;
                    gskala = entry.zoom / 100.0;
                    //                     cout << "Nowy zoom gskala = " << gskala << endl;

                    // liczba przy skalowaniu jest dobra, ale pozycja suwaka nie!   ---??
                    // uaktualnić tylko wtedy, gdy jest otwarte okno opcji...

                }
                // sprawdzenie czy ulubiona wsk godzinowa jest cyfrą (czyli wektorowa) -----------------

                wgraj_wskazowke(entry.hours_hand, vect_godzinowych,
                                & cyf.wsk_godzinowa.flag_bitmapowa,
                                & nr_wsk_wektorowej_godz,
                                & nr_wybranej_bitmapy_wsk_godzinowej,
                                & godzinowa
                                );

                wgraj_wskazowke(entry.minutes_hand , vect_minutowych,
                                &cyf.wsk_minutowa.flag_bitmapowa,
                                & nr_wsk_wektorowej_min,
                                & nr_wybranej_bitmapy_wsk_minutowej,
                                & minutowa
                                );

                wgraj_wskazowke(entry.seconds_hand , vect_sekundnikow,
                                &  cyf.wsk_sekundowa.flag_bitmapowa ,
                                & nr_wsk_wektorowej_sek,
                                & nr_wybranej_bitmapy_wsk_sekundowej,
                                & sekundnik
                                );

                //-----------------------------
                if(cyf.flaga_wsk_dnia_tygodnia){
                    nr_wybranej_bitmapy_wsk_dni_tyg =
                            znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_dni_tygodnia.bitmapa, vect_extra_wskazowek);

                    wgraj_wskazowke(
                                vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_dni_tyg].bitmapa,
                                vect_extra_wskazowek,
                                & cyf.wsk_dni_tygodnia.flag_bitmapowa,
                                & nr_wsk_wektorowej_tyg,
                                & nr_wybranej_bitmapy_wsk_dni_tyg,
                                & pix_wskazowka_dni_tygodnia
                                );
                }

                //-----------------------------
                if(cyf.flaga_wsk_dnia_miesiaca){
                    nr_wybranej_bitmapy_wsk_dni_mies =
                            znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_dni_miesiaca.bitmapa, vect_extra_wskazowek);

                    wgraj_wskazowke(
                                vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_dni_mies].bitmapa,
                                vect_extra_wskazowek,
                                & cyf.wsk_dni_miesiaca.flag_bitmapowa,
                                & nr_wsk_wektorowej_dni_mies,
                                & nr_wybranej_bitmapy_wsk_dni_mies,
                                & pix_wskazowka_dni_miesiaca
                                );
                }

                //-----------------------------
                if(cyf.flaga_wsk_miesiaca){
                    nr_wybranej_bitmapy_wsk_miesiaca =
                            znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_miesiaca.bitmapa, vect_extra_wskazowek);

                    wgraj_wskazowke(
                                vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_miesiaca].bitmapa,
                                vect_extra_wskazowek,
                                & cyf.wsk_miesiaca.flag_bitmapowa,
                                & nr_wsk_wektorowej_miesiaca,
                                & nr_wybranej_bitmapy_wsk_miesiaca,
                                & pix_wskazowka_miesiaca
                                );
                }

                //-----------------------------
                if(cyf.flaga_wsk_night_day){
                    nr_wybranej_bitmapy_wsk_night_day =
                            znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_night_day.bitmapa, vect_extra_wskazowek);

                    wgraj_wskazowke(
                                vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_night_day].bitmapa,
                                vect_extra_wskazowek,
                                & cyf.wsk_night_day.flag_bitmapowa,
                                & nr_wsk_wektorowej_night_day,
                                & nr_wybranej_bitmapy_wsk_night_day,
                                & pix_wskazowka_night_day
                                );
                }

                //-----------------------------
                if(cyf.flaga_wsk_zones){
                    nr_wybranej_bitmapy_wsk_zones =
                            znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_zones.bitmapa, vect_extra_wskazowek);

                    wgraj_wskazowke(
                                vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_zones].bitmapa,
                                vect_extra_wskazowek,
                                & cyf.wsk_zones.flag_bitmapowa,
                                & nr_wsk_wektorowej_zones,
                                & nr_wybranej_bitmapy_wsk_zones,
                                & pix_wskazowka_zones
                                );
                }


                //-----------------------------
                if(cyf.flaga_wsk_sekundnik_dodatkowy){
                    nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy =
                            znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_sekundnik_dodatkowy.bitmapa,
                                                           vect_extra_wskazowek);

                    wgraj_wskazowke(
                                vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy ].bitmapa,
                                vect_extra_wskazowek,
                                & cyf.wsk_sekundnik_dodatkowy.flag_bitmapowa,
                                & nr_wsk_wektorowej_sekundnik_dodatkowy,
                                & nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy,
                                & pix_wskazowka_sekundnik_dodatkowy
                                );
                }

                break;

                //                // sprawdzenie czy ulubiona wsk sek jest cyfrą (czytli wektorowa) -------------------
                //                if( entry.seconds_hand.size() == 1 && isdigit(entry.seconds_hand[0] ) )
                //                {
                //                     cyf.wsk_minutowa.flag_bitmapowa  = false;
                //                    nr_wsk_wektorowej_sek = stoi (entry.seconds_hand);

                //                }
                //                else
                //                {
                //                     cyf.wsk_minutowa.flag_bitmapowa  = true;
                //                    // Jeśli bitmapowa, to zapamiętujemy nazwe bitmapy
                //                    nr_wybranej_bitmapy_wsk_sekundowej = znajdz_nr_wskazowki_o_nazwie(
                //                                entry.seconds_hand,
                //                                vect_sekundnikow );
                //                }

            }
        }

        if(flag_znalezione == false)
        {
            //             cout << " Info o ulubionych wskazowkach nie  znalezione, wiec dajemy " ;
            if(flaga_wskazowki_poprzednie_a_nie_fabryczne)   // wybor w okienku opcji cyferblatu
            {
                tryb = tryb_wskazowek::recent;
                //                                cout << " poprzednie uzywane" << endl;
            }else{
                tryb = tryb_wskazowek::fabryczne;
                //                                cout << " fabryczne " << endl;
            }
        }
    }
#endif

    // jesli sie nie uda ladowanie to rezultatem load jest false ???


    if( tarcza.load( cyf.bitmapa_tarcza.c_str()) == false)
    {
        cout << "blad w trakcie ladowania cyferblatu "  <<  cyf.bitmapa_tarcza << endl;
        exit(2);
    }
    central.load( cyf.bitmapa_central.c_str());

    if(tryb == tryb_wskazowek::fabryczne)
    {
        //                       cout << "Wgrywam skazowki  fabryczne------------------ " << endl;
        //               cout << __func__ <<  " 2  cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;

        wgraj_wskazowke_fabryczna(cyf.bitmapa_godzinowa, vect_godzinowych,
                                  & cyf.wsk_godzinowa.flag_bitmapowa,
                                  & nr_wsk_wektorowej_godz,
                                  & nr_wybranej_bitmapy_wsk_godzinowej,
                                  & godzinowa
                                  );

        wgraj_wskazowke_fabryczna(cyf.bitmapa_minutowa , vect_minutowych,
                                  & cyf.wsk_minutowa.flag_bitmapowa,
                                  & nr_wsk_wektorowej_min,
                                  & nr_wybranej_bitmapy_wsk_minutowej,
                                  & minutowa
                                  );

        wgraj_wskazowke_fabryczna(cyf.bitmapa_sekundnik , vect_sekundnikow,
                                  &  cyf.wsk_sekundowa.flag_bitmapowa ,
                                  & nr_wsk_wektorowej_sek,
                                  & nr_wybranej_bitmapy_wsk_sekundowej,
                                  & sekundnik
                                  );
        //-----------------
        if(cyf.flaga_wsk_dnia_tygodnia)
        {
            nr_wybranej_bitmapy_wsk_dni_tyg =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_dni_tygodnia.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke_fabryczna(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_dni_tyg].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_dni_tygodnia.flag_bitmapowa,
                        & nr_wsk_wektorowej_tyg,
                        & nr_wybranej_bitmapy_wsk_dni_tyg,
                        & pix_wskazowka_dni_tygodnia
                        );

        }
        if(cyf.flaga_wsk_dnia_miesiaca){
            nr_wybranej_bitmapy_wsk_dni_mies =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_dni_miesiaca.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke_fabryczna(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_dni_mies].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_dni_miesiaca.flag_bitmapowa,
                        & nr_wsk_wektorowej_dni_mies,
                        & nr_wybranej_bitmapy_wsk_dni_mies,
                        & pix_wskazowka_dni_miesiaca
                        );
        }

        if(cyf.flaga_wsk_miesiaca){
            nr_wybranej_bitmapy_wsk_miesiaca =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_miesiaca.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke_fabryczna(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_miesiaca].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_miesiaca.flag_bitmapowa,
                        & nr_wsk_wektorowej_miesiaca,
                        & nr_wybranej_bitmapy_wsk_miesiaca,
                        & pix_wskazowka_miesiaca
                        );
        }

        if(cyf.flaga_wsk_night_day){
            nr_wybranej_bitmapy_wsk_night_day =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_night_day.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke_fabryczna(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_night_day].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_night_day.flag_bitmapowa,
                        & nr_wsk_wektorowej_night_day,
                        & nr_wybranej_bitmapy_wsk_night_day,
                        & pix_wskazowka_night_day
                        );
        }


        if(cyf.flaga_wsk_zones){
            nr_wybranej_bitmapy_wsk_zones =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_zones.bitmapa, vect_extra_wskazowek);
            wgraj_wskazowke_fabryczna(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_zones].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_zones.flag_bitmapowa,
                        & nr_wsk_wektorowej_zones,
                        & nr_wybranej_bitmapy_wsk_zones,
                        & pix_wskazowka_zones
                        );
        }

        //--------------
        if(cyf.flaga_wsk_sekundnik_dodatkowy){
            nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_sekundnik_dodatkowy.bitmapa,
                                                   vect_extra_wskazowek);

            wgraj_wskazowke_fabryczna(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy ].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_sekundnik_dodatkowy.flag_bitmapowa,
                        & nr_wsk_wektorowej_sekundnik_dodatkowy,
                        & nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy,
                        & pix_wskazowka_sekundnik_dodatkowy
                        );
        }
    }


    if(tryb == tryb_wskazowek::recent)
    {
        //          cout << "wskazowki, Nie ulubione, nie fabryczne, wiec po prostu ostatnio obowiazujace " << endl;

        wgraj_wskazowke( vect_godzinowych[nr_wybranej_bitmapy_wsk_godzinowej].bitmapa,
                         vect_godzinowych,
                         & cyf.wsk_godzinowa.flag_bitmapowa,
                         & nr_wsk_wektorowej_godz,
                         & nr_wybranej_bitmapy_wsk_godzinowej,
                         & godzinowa
                         );

        //- min ---------------------
        //        cout << __func__ <<  " 3  PRZED wgraj  cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha
        //             <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;

        wgraj_wskazowke(vect_minutowych[nr_wybranej_bitmapy_wsk_minutowej].bitmapa,
                        vect_minutowych,
                        & cyf.wsk_minutowa.flag_bitmapowa,
                        & nr_wsk_wektorowej_min,
                        & nr_wybranej_bitmapy_wsk_minutowej,
                        & minutowa
                        );

        //        cout << __func__ <<  " 33  PO cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha
        //             <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;

        //                cout << "ma to byc minutnik bitmapowy?  "
        //                     << boolalpha << cyf.wsk_minutowa.flag_bitmapowa
        //                     << " o numerze " << nr_wybranej_bitmapy_wsk_minutowej
        //                     << ", a nr wekt = " << nr_wsk_wektorowej_min
        //                     << endl;


        COTO;

        // sek -----------------


        //        cout << "ma to byc sektundnk bitmapowy?  "
        //             << boolalpha <<  cyf.wsk_minutowa.flag_bitmapowa
        //             << endl;



        wgraj_wskazowke(vect_sekundnikow[nr_wybranej_bitmapy_wsk_sekundowej].bitmapa,
                        vect_sekundnikow,
                        &  cyf.wsk_sekundowa.flag_bitmapowa ,
                        & nr_wsk_wektorowej_sek,
                        & nr_wybranej_bitmapy_wsk_sekundowej,
                        & sekundnik
                        );

        if(cyf.flaga_wsk_dnia_tygodnia ){


            //            cout << "Przed zwyklym wgraniem cyf.wsk_dni_tygodnia.flag_bitmapowa = "
            //                 << cyf.wsk_dni_tygodnia.flag_bitmapowa << endl;
            nr_wybranej_bitmapy_wsk_dni_tyg =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_dni_tygodnia.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_dni_tyg].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_dni_tygodnia.flag_bitmapowa,
                        & nr_wsk_wektorowej_tyg,
                        & nr_wybranej_bitmapy_wsk_dni_tyg,
                        & pix_wskazowka_dni_tygodnia
                        );

            //            cout << "PO wgraniem cyf.wsk_dni_tygodnia.flag_bitmapowa = "
            //                 << cyf.wsk_dni_tygodnia.flag_bitmapowa << endl;

        }



        if(cyf.flaga_wsk_dnia_miesiaca){
            nr_wybranej_bitmapy_wsk_dni_mies =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_dni_miesiaca.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_dni_mies].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_dni_miesiaca.flag_bitmapowa,
                        & nr_wsk_wektorowej_dni_mies,
                        & nr_wybranej_bitmapy_wsk_dni_mies,
                        & pix_wskazowka_dni_miesiaca
                        );
        }

        if(cyf.flaga_wsk_miesiaca){
            nr_wybranej_bitmapy_wsk_miesiaca =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_miesiaca.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_miesiaca].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_miesiaca.flag_bitmapowa,
                        & nr_wsk_wektorowej_miesiaca,
                        & nr_wybranej_bitmapy_wsk_miesiaca,
                        & pix_wskazowka_miesiaca
                        );
        }

        if(cyf.flaga_wsk_night_day){
            nr_wybranej_bitmapy_wsk_night_day =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_night_day.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_night_day].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_night_day.flag_bitmapowa,
                        & nr_wsk_wektorowej_night_day,
                        & nr_wybranej_bitmapy_wsk_night_day,
                        & pix_wskazowka_night_day
                        );
        }
        if(cyf.flaga_wsk_zones){
            nr_wybranej_bitmapy_wsk_zones =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_zones.bitmapa, vect_extra_wskazowek);

            wgraj_wskazowke(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_zones].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_zones.flag_bitmapowa,
                        & nr_wsk_wektorowej_zones,
                        & nr_wybranej_bitmapy_wsk_zones,
                        & pix_wskazowka_zones
                        );
        }

        //--------------
        if(cyf.flaga_wsk_sekundnik_dodatkowy)
        {
            nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy =
                    znajdz_nr_wskazowki_o_bitmapie(cyf.wsk_sekundnik_dodatkowy.bitmapa,
                                                   vect_extra_wskazowek);

            wgraj_wskazowke(
                        vect_extra_wskazowek[nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy ].bitmapa,
                        vect_extra_wskazowek,
                        & cyf.wsk_sekundnik_dodatkowy.flag_bitmapowa,
                        & nr_wsk_wektorowej_sekundnik_dodatkowy,
                        & nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy,
                        & pix_wskazowka_sekundnik_dodatkowy
                        );
        }



        //        cout << "Po funkcji wgraj . Sektundnki bitmapowy?  "
        //             << boolalpha <<  cyf.wsk_minutowa.flag_bitmapowa
        //             << " o numerze " << nr_wybranej_bitmapy_wsk_sekundowej
        //             << ", a nr wekt = " << nr_wsk_wektorowej_sek
        //             << endl;

    } // koniec if zwykle



    rozm_tarczy_x =  gskala*tarcza.width();
    rozm_tarczy_y =  gskala*tarcza.height();
    resize(rozm_tarczy_x, rozm_tarczy_y);

    promien = gskala* cyf.promien_tarczy;
    //          cout << "promien_ samej  = " << promien << endl;
    cyf.uskok_cienia = cyf.uskok_cienia * promien / 100.0;

    // wskazowka godzinowa ===================================

#if 1
    flag_pokaz_to = false;


    przelicz_wskazowke(
                nr_wybranej_bitmapy_wsk_godzinowej,
                vect_godzinowych,
                godzinowa,
                cyf.wsk_godzinowa.flag_bitmapowa,

                // dotychczasowe dane o tej wskazowce
                cyf.mimosrod_x_wektorowej_godzinowej,
                cyf.mimosrod_y_wektorowej_godzinowej,
                cyf.dlugosc_godzinowej,
                cyf.szer_x_godzinowej,
                cyf.kolor_godzinowej,
                // rezultaty
                cyf.wsk_godzinowa.przel_mimosrod_x,
                cyf.wsk_godzinowa.przel_mimosrod_y,
                cyf.wsk_godzinowa.przel_length_poly,
                cyf.wsk_godzinowa.przel_szer_poly,

                cyf.wsk_godzinowa.przel_skalowanie_x,
                cyf.wsk_godzinowa.przel_skalowanie_y
                );

    cyf.pointery_wskazowek.push_back(&cyf.wsk_godzinowa);


    //        cout <<  "godz_mimosrod_x =" <<   godz_mimosrod_x
    //              << ", godz_mimosrod_y = " <<   godz_mimosrod_y
    //              << ", godz_szer_poly = " <<  godz_szer_poly
    //              << ", godz_length_poly = "<< godz_length_poly
    //              << endl;

    flag_pokaz_to = false;


#endif

    // minutowa -------------------------------------------------------------
    // wskazowka minutowa ===================================



    //    cout << __func__ <<  " 5  cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;

    przelicz_wskazowke(
                nr_wybranej_bitmapy_wsk_minutowej,
                vect_minutowych,
                minutowa,
                cyf.wsk_minutowa.flag_bitmapowa,

                // dotychczasowe dane o tej wskazowce
                cyf.mimosrod_x_wektorowej_minutowej,
                cyf.mimosrod_y_wektorowej_minutowej,
                cyf.dlugosc_minutowej,
                cyf.szer_x_minutowej,
                cyf.kolor_minutowej,
                // rezultaty
                cyf.wsk_minutowa.przel_mimosrod_x,
                cyf.wsk_minutowa.przel_mimosrod_y,
                cyf.wsk_minutowa.przel_length_poly,
                cyf.wsk_minutowa.przel_szer_poly,

                cyf.wsk_minutowa.przel_skalowanie_x,
                cyf.wsk_minutowa.przel_skalowanie_y
                );

    //    cout << __func__ <<  " 55  cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha
    //         <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;

    cyf.pointery_wskazowek.push_back(&cyf.wsk_minutowa);




    // sekundnik ----------------------------------------------------------

#if 1


    przelicz_wskazowke(nr_wybranej_bitmapy_wsk_sekundowej,
                       vect_sekundnikow,
                       sekundnik,
                       cyf.wsk_sekundowa.flag_bitmapowa ,

                       // dotychczasowe dane o tej wskazowce
                       cyf.mimosrod_x_wektorowego_sekundnika,
                       cyf.mimosrod_y_wektorowego_sekundnika,
                       cyf.dlugosc_y_sekundnika,
                       cyf.szerokosc_x_sekundnika,
                       cyf.kolor_sekundnika,
                       // rezultaty
                       cyf.wsk_sekundowa.przel_mimosrod_x,
                       cyf.wsk_sekundowa.przel_mimosrod_y,
                       cyf.wsk_sekundowa.przel_length_poly,
                       cyf.wsk_sekundowa.przel_szer_poly,

                       cyf.wsk_sekundowa.przel_skalowanie_x,
                       cyf.wsk_sekundowa.przel_skalowanie_y
                       );
    //            cout << __func__ <<  " 6  cyf.wsk_minutowa.flag_bitmapowa  " << boolalpha <<  cyf.wsk_minutowa.flag_bitmapowa  << endl;
    cyf.pointery_wskazowek.push_back(&cyf.wsk_sekundowa);
#endif


    // ------- extra ----------------------

    if(cyf.wsk_dni_tygodnia.flaga_enable ){

        cyf.wsk_dni_tygodnia.flag_bitmapowa = true;

        przelicz_wskazowke(nr_wybranej_bitmapy_wsk_dni_tyg,
                           vect_extra_wskazowek,
                           pix_wskazowka_dni_tygodnia,
                           cyf.wsk_dni_tygodnia.flag_bitmapowa,

                           // dotychczasowe dane o tej wskazowce
                           cyf.wsk_dni_tygodnia.mimosrod_x,
                           cyf.wsk_dni_tygodnia.mimosrod_y,
                           cyf.wsk_dni_tygodnia.dlugosc,
                           cyf.wsk_dni_tygodnia.skala_x,
                           cyf.wsk_dni_tygodnia.kolor,
                           // rezultaty
                           cyf.wsk_dni_tygodnia.przel_mimosrod_x,
                           cyf.wsk_dni_tygodnia.przel_mimosrod_y,
                           cyf.wsk_dni_tygodnia.przel_length_poly,
                           cyf.wsk_dni_tygodnia.przel_szer_poly,

                           cyf.wsk_dni_tygodnia.przel_skalowanie_x,
                           cyf.wsk_dni_tygodnia.przel_skalowanie_y
                           );

        cyf.pointery_wskazowek.push_back(&cyf.wsk_dni_tygodnia);

    }

    if(cyf.wsk_dni_miesiaca.flaga_enable ){

        cyf.wsk_dni_miesiaca.flag_bitmapowa = true;
        // oszustwo

        przelicz_wskazowke(nr_wybranej_bitmapy_wsk_dni_mies,
                           vect_extra_wskazowek,
                           pix_wskazowka_dni_miesiaca ,
                           cyf.wsk_dni_miesiaca.flag_bitmapowa,

                           // dotychczasowe dane o tej wskazowce
                           cyf.wsk_dni_miesiaca.mimosrod_x,
                           cyf.wsk_dni_miesiaca.mimosrod_y,
                           cyf.wsk_dni_miesiaca.dlugosc,
                           cyf.wsk_dni_miesiaca.skala_x,
                           cyf.wsk_dni_miesiaca.kolor,
                           // rezultaty
                           cyf.wsk_dni_miesiaca.przel_mimosrod_x,
                           cyf.wsk_dni_miesiaca.przel_mimosrod_y,
                           cyf.wsk_dni_miesiaca.przel_length_poly,
                           cyf.wsk_dni_miesiaca.przel_szer_poly,

                           cyf.wsk_dni_miesiaca.przel_skalowanie_x,
                           cyf.wsk_dni_miesiaca.przel_skalowanie_y
                           );

        cyf.pointery_wskazowek.push_back(&cyf.wsk_dni_miesiaca);
    }

    if(cyf.wsk_miesiaca.flaga_enable ){

        cyf.wsk_miesiaca.flag_bitmapowa = true;
        // oszustwo

        przelicz_wskazowke(nr_wybranej_bitmapy_wsk_miesiaca,
                           vect_extra_wskazowek,
                           pix_wskazowka_miesiaca ,
                           cyf.wsk_miesiaca.flag_bitmapowa,

                           // dotychczasowe dane o tej wskazowce
                           cyf.wsk_miesiaca.mimosrod_x,
                           cyf.wsk_miesiaca.mimosrod_y,
                           cyf.wsk_miesiaca.dlugosc,
                           cyf.wsk_miesiaca.skala_x,
                           cyf.wsk_miesiaca.kolor,
                           // rezultaty
                           cyf.wsk_miesiaca.przel_mimosrod_x,
                           cyf.wsk_miesiaca.przel_mimosrod_y,
                           cyf.wsk_miesiaca.przel_length_poly,
                           cyf.wsk_miesiaca.przel_szer_poly,

                           cyf.wsk_miesiaca.przel_skalowanie_x,
                           cyf.wsk_miesiaca.przel_skalowanie_y
                           );

        cyf.pointery_wskazowek.push_back(&cyf.wsk_miesiaca);
    }

    if(cyf.wsk_night_day.flaga_enable ){

        cyf.wsk_night_day.flag_bitmapowa = true;
        // oszustwo

        przelicz_wskazowke(nr_wybranej_bitmapy_wsk_night_day,
                           vect_extra_wskazowek,
                           pix_wskazowka_night_day ,
                           cyf.wsk_night_day.flag_bitmapowa,

                           // dotychczasowe dane o tej wskazowce
                           cyf.wsk_night_day.mimosrod_x,
                           cyf.wsk_night_day.mimosrod_y,
                           cyf.wsk_night_day.dlugosc,
                           cyf.wsk_night_day.skala_x,
                           cyf.wsk_night_day.kolor,
                           // rezultaty
                           cyf.wsk_night_day.przel_mimosrod_x,
                           cyf.wsk_night_day.przel_mimosrod_y,
                           cyf.wsk_night_day.przel_length_poly,
                           cyf.wsk_night_day.przel_szer_poly,

                           cyf.wsk_night_day.przel_skalowanie_x,
                           cyf.wsk_night_day.przel_skalowanie_y
                           );
        cyf.pointery_wskazowek.push_back(&cyf.wsk_night_day);
    }

    if(cyf.wsk_zones.flaga_enable ){

        cyf.wsk_zones.flag_bitmapowa = true;
        // oszustwo

        przelicz_wskazowke(nr_wybranej_bitmapy_wsk_zones,
                           vect_extra_wskazowek,
                           pix_wskazowka_zones ,
                           cyf.wsk_zones.flag_bitmapowa,

                           // dotychczasowe dane o tej wskazowce
                           cyf.wsk_zones.mimosrod_x,
                           cyf.wsk_zones.mimosrod_y,
                           cyf.wsk_zones.dlugosc,
                           cyf.wsk_zones.skala_x,
                           cyf.wsk_zones.kolor,
                           // rezultaty
                           cyf.wsk_zones.przel_mimosrod_x,
                           cyf.wsk_zones.przel_mimosrod_y,
                           cyf.wsk_zones.przel_length_poly,
                           cyf.wsk_zones.przel_szer_poly,

                           cyf.wsk_zones.przel_skalowanie_x,
                           cyf.wsk_zones.przel_skalowanie_y
                           );
        cyf.pointery_wskazowek.push_back(&cyf.wsk_zones);
    }

    if(cyf.wsk_sekundnik_dodatkowy.flaga_enable ){

        cyf.wsk_sekundnik_dodatkowy.flag_bitmapowa = true;
        // oszustwo

        przelicz_wskazowke(nr_wybranej_bitmapy_wsk_sekundnik_dodatkowy,
                           vect_extra_wskazowek,
                           pix_wskazowka_sekundnik_dodatkowy ,
                           cyf.wsk_sekundnik_dodatkowy.flag_bitmapowa,

                           // dotychczasowe dane o tej wskazowce
                           cyf.wsk_sekundnik_dodatkowy.mimosrod_x,
                           cyf.wsk_sekundnik_dodatkowy.mimosrod_y,
                           cyf.wsk_sekundnik_dodatkowy.dlugosc,
                           cyf.wsk_sekundnik_dodatkowy.skala_x,
                           cyf.wsk_sekundnik_dodatkowy.kolor,
                           // rezultaty
                           cyf.wsk_sekundnik_dodatkowy.przel_mimosrod_x,
                           cyf.wsk_sekundnik_dodatkowy.przel_mimosrod_y,
                           cyf.wsk_sekundnik_dodatkowy.przel_length_poly,
                           cyf.wsk_sekundnik_dodatkowy.przel_szer_poly,

                           cyf.wsk_sekundnik_dodatkowy.przel_skalowanie_x,
                           cyf.wsk_sekundnik_dodatkowy.przel_skalowanie_y
                           );
        cyf.pointery_wskazowek.push_back(&cyf.wsk_sekundnik_dodatkowy);
    }

    //    cout << "tyg_skal_x" << tyg_skalowanie_x
    //         << ", tyg_skalowanie_y = " << tyg_skalowanie_y << endl;




    sortowanie_warstw();
    ustaw_czas_wg_strefy();

    // tu można sprawdzić czy wiece niż pół ładowanego cyferblatu
    // nie wyjdą poza brzeg ekranu



    ////    this->move(poz_x, poz_y );

    // z prawego brzegu
    //    if(poz_srodka_zegara_x  > szer_ekranu_windows  )
    //    {
    //        cout << "Ten zegar wychodzi poza prawą krawedź" << endl;
    //        poz_srodka_zegara_x = szer_ekranu_windows  ;
    //        this->move( poz_srodka_zegara_x -   (width() / 2.0), poz_zegara_y);
    //    }

    // z dolnego brzegu
    if(poz_zegara_y + (height()/2.0) > wys_ekranu_windows  )
    {
        //        poz_zegara_y = wys_ekranu_windows - (height()/2)  - 25;
        //        this->move( poz_srodka_zegara_x -  gskala *  (width() / 2), poz_zegara_y);
    }
    auto obliczona_pozycja_x_rogu =  poz_srodka_zegara_x  -  (width() / 2.0 ) ;

    this->move(obliczona_pozycja_x_rogu, poz_zegara_y);

    //        cout << __func__ << ", obliczona pozycja x rogu= "
    //             <<  obliczona_pozycja_x_rogu
    //              << ", bo pozycja poz_srodka_zegara_x = " << poz_srodka_zegara_x
    //                  << ", width  " << (width() )
    //              << ", gskala = " << gskala
    //              << ", width / 2 = " << (width() / 2.0 )
    //              << endl;


    update();

    cyf.zapis_danych_o_cyferblacie();

    return true;
}
//****************************************************************************************
void MainWindow::resize_okna_z_tarcza()
{

    zmiana_wygladu_cyferblatu(nr_tarczy, tryb_wskazowek::recent);

}
//***************************************************************************************
void MainWindow::mousePressEvent(QMouseEvent *e)
{

    //! Jakie są pullup opcje
    //!
    //!    Callendar
    //!    ------------- separator ----
    //!    Click through
    //!    Unmovable
    //!    On Top
    //!    About...
    //!    Exit


    //cout << "F. spectrum_1D::mousePressEven - button" << endl ;
    if(e->button() & Qt::RightButton)
    {
        //                cout << "F. spectrum_1D::mousePressEven - RIGHT button" << endl ;
        QMenu *context_Menu = new QMenu;

        context_Menu->addAction(tr("Options..."), this, SLOT(wywolanie_okna_opcji() ));
        context_Menu->addAction(tr("Alarms..."), this, SLOT(wywolanie_okna_alarmow() ) );

        context_Menu->addSeparator();
        //  context_Menu->addAction(tr("Change clock-face"), this, SLOT(nastepna_tarcza() ));
        //        context_Menu->addAction(tr("Change clock-hands"), this, SLOT(wywolanie_okna_wskazowek() ));

#if KRZYZE_PROWADZACE == true
        context_Menu->addAction(tr("Resource file use - report for expert"), this,
                                SLOT(resource_files_use() ));
#endif

        //        if(id_linux_czy_windows_version() == 1)
        //        {
        //            // w Linuksie moze sie to przydać, gdy chcemy, by cyferblat byl wyświetlany
        //            // na wszystkich virtual desktops. (bez tego też można dotrzeć do
        //            // cech danego programu (zegara) klawiszami Alt-F3
        //            string tekst = (flag_dialog_bez_ramki ? "Show" : "Hide") +
        //                    string(" frame of this window");

        //            context_Menu->addAction(tr(tekst.c_str()), this,
        //                                    SLOT(pokaz_ramke_dialogu() ));
        //        }

        context_Menu->addAction(tr("Exit program"), this, SLOT(koniec_programu() ));

        // uruchomienie przygotowanego menu
        context_Menu->exec(mapToGlobal(QPoint(e->position().x() ,e->position().y()   )));
        delete context_Menu;
    }

    else{
#if KRZYZE_PROWADZACE
        //if(e->button() & Qt::ControlModifier)
        cout << "zwykle klikniecie .. lewym klawiszem w pozycji x="
             << e->position().x()
             << ", y= "  << e->position().y()
             << endl;
#endif

        // do suwania tarczy po ekranie
        lastMouseX = e->position().x();
        lastMouseY = e->position().y();
    }


    //        cout << "KOniec funkcji " <<  __PRETTY_FUNCTION__ << endl;
}
//***************************************************************************************
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    auto okno_x = this->x();
    auto okno_y = this->y();

    okno_x += (event->position().x()  - lastMouseX);
    okno_y += (event->position().y()  - lastMouseY);

    if(flag_blokuj_na_ekranie == false){
        this->move(okno_x, okno_y);
        poz_srodka_zegara_x = okno_x + (width() /2.0 ) ;

        //        cout << "okno_x = " << okno_x
        //             <<  ", poz_srodka_zegara_x == " << poz_srodka_zegara_x
        //              << endl;
        poz_zegara_y = okno_y;
    }
}
//**************************************************************************************************************************
void MainWindow::paintEvent(QPaintEvent *)
{
    //    static int nr;
    //        cout << __func__ << "-----------------------" << ++nr << endl;

    //    showNormal();
    ////    show();
    //setVisible(true);


    current_zone_date_time = QDateTime::currentDateTimeUtc().toTimeZone(strefa_czasowa);

    now_time = current_zone_date_time.time();
    now_today = current_zone_date_time.date();
    //        now_time.setHMS(12, 00, 0);    // dla prób wskazowek

    //    cout << "P)  godzina w tej strefie = "
    //         << current_zone_date_timeB.toString().toStdString()
    //         << "  godzina = "
    //         << now_time.hour()

    //         << endl;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //    painter.translate(width() / 2, height() / 2);

    double translate_x = cyf.os_x_center;
    double translate_y =  cyf.os_y_center;


    painter.translate(gskala* translate_x,  gskala * translate_y);  // ustawienie pt-u 0,0 na srodku tarczy.

    painter.save();    // zapamietujemy obecny stan

    //    cout << "Zestaw wskazowek pod tarcza " << endl;

    // jesli tarcza ma okienka przezroczyste to najpierw rysujemy ich treść
    for(uint i = 0 ; i < cyf. pointery_wskazowek.size() ; ++i )
    {
        if( cyf.pointery_wskazowek[i]->warstwa >= 0  )
            break;

        rysuj_ogolna_wskazowke(painter,  *(cyf.pointery_wskazowek[i]) );
    }



    //-- rysowanie właściwej treści  tarczy ---- WARSTWA  0 ----------------------------------------------------------
    painter.scale( gskala, gskala); // skaluje wielkosc tarczy
    painter.drawPixmap( -translate_x, -translate_y,  tarcza);  // transl, bo to pozycja lewego gornego rogu.

    painter.restore();

    // -----Księzyc ----------------------------------------------
#if 1
    if(cyf.flaga_ksiezyc){
        painter.save();
        //    MOON   --------------   29.53 days on orbit

        wybierz_obrazek_ksiezyca();

        auto skale_dwie = (cyf.skala_ksiezyc * gskala);
        auto xk = (promien * cyf.x_ksiezyc)  -(skale_dwie* ksiezyc.width() / 2.0);
        auto yk =  (promien * cyf.y_ksiezyc)  -(skale_dwie * ksiezyc.height() / 2.0);
        //        cout << " xk = " <<  xk << ", yk = " <<  yk << ", gskala = " << gskala << endl;

        painter.translate( xk, yk);
        painter.scale( skale_dwie, skale_dwie);
        painter.drawPixmap(0, 0, ksiezyc);
        //-----------
        painter.restore();
    }
#endif

    // ------------------ napisy  NAPISY: full date  oraz datownik ----------------------------
    if(flag_show_full_date  || flag_show_date)
    {
        painter.save();
        painter.setPen(Qt::SolidLine);
        //painter.setBrush(QColor(0, 210, 210));
        painter.setPen(cyf.kolor_dnia_tygodnia);

        QFont font = painter.font();
        int font_w_pikselach = cyf.wys_datownika * promien/ 100;
        font.setPixelSize(font_w_pikselach);
        painter.setFont(font);

        QDate date = now_today;


        if(flag_show_AM_PM)                 // -------------------------------
        {
            QPoint p = cyf.pozycja_AM_PM;
            // przeliczenie wzgl procentu długości promienia
            p.setX(p.x() *  promien/100.0);
            p.setY(p.y() *  promien/100.0);

            //cout << " poz AM w pikselach " << p.x() << ", y " << p.y() << endl;

            int szer = 0.3 * promien;
            //const
            QRect rectangle = QRect(
                        p.x(),
                        p.y(),
                        szer,  // width
                        font_w_pikselach  // height
                        );

            QString text_AM_PM = (now_time.hour() < 12 ? " AM " : " PM ");

            // przygotowanie odpowiedniej wielkości ramki
            QRect boundingRect;

            painter.setBrush(cyf.tlo_AM_PM);   // kolor wypełnienia
            rectangle.setWidth(0);   // czyli niewidzialny ?
            painter.drawText(rectangle, 0, text_AM_PM, &boundingRect); // obliczenie
            boundingRect.moveCenter(p);

            painter.setPen(Qt::SolidLine);   // teraz już rysujemy
            painter.setPen(cyf.kolor_ramki_datownika);   // kolor brzegu

            painter.drawRect(boundingRect);    // narysowanie  ramki
            //------------
            painter.setPen(cyf.kolor_AM_PM);   // kolor tekstu

            painter.drawText(boundingRect, 0,  text_AM_PM, &boundingRect); // wypis textu AM/PM

            //------------- cień do tej ramki
            painter.setPen(przyciemnij(cyf.kolor_ramki_datownika) );   // kolor brzegu


            int skok  = 1;

            boundingRect.moveCenter(p + QPoint(skok, skok) );
            painter.drawLine(boundingRect.x(), boundingRect.y(),
                             boundingRect.x() ,
                             boundingRect.y() + boundingRect.height() - skok );
            painter.drawLine(boundingRect.x(), boundingRect.y(),
                             boundingRect.x() + boundingRect.width() - skok,
                             boundingRect.y());
            // --------------
        }


        if(flag_show_full_date){   // ------------------ napis  full date txt -------------
            QPoint p = cyf.pozycja_full_date;
            // przeliczenie wzgl procentu długości promienia
            p.setX(p.x() *  promien/100.0);
            p.setY(p.y() *  promien/100.0);

            //cout << " poz daty w pikselach " << p.x() << ", y " << p.y() << endl;

            int szer = 0.3 * promien;
            //const
            QRect rectangle = QRect(
                        p.x(),
                        p.y(),
                        szer,  // width
                        font_w_pikselach  // height
                        );

            // auto loc_long = date.toString(QLocale::LongFormat);
            auto loc_long = QLocale().toString(date, QLocale::LongFormat);

            auto dzien_tyg =  QLocale().toString(date, "dddd") ;  // dzien tygodnia
            // czy w tym jest już dzień tygodnia?
            bool flag_juz_jest = true;
            if(loc_long.contains(dzien_tyg) == false )
            {
                flag_juz_jest  = false;
            }

            auto full_date =
                    " " +
                    (flag_juz_jest ? "" : QLocale().toString(date, "dddd, ")) +  // dzień tygodnia w lokalnym języku
                    QLocale().toString(date, QLocale::LongFormat)
                    + " ";



            //++++++++++++++++++++++++++++

            QRect boundingRect;

            //            QPen pen = painter.pen();
            //            pen.setStyle(Qt::NoPen);

            rectangle.setWidth(0);   // bylo 0
            painter.drawText(rectangle, 0, full_date, &boundingRect); // obliczeie wielkości ramki

            // może wystaje poza prawy margines obrazu? ----------------
            /*  cout << "boundingRect.x() = " << boundingRect.x()
                 << " boundingRect.width() = " << boundingRect.width()
                 << " translate_x = " << translate_x
                 << endl;*/

            int konieczne_przesuniecie = (boundingRect.width() /2) - boundingRect.x() - (translate_x*gskala);
            //            cout << " LEWY BRZEG konieczne_przesuniecie = " << konieczne_przesuniecie
            //             << ", gskala = " << gskala << endl;
            if(konieczne_przesuniecie < 0)
            {
                konieczne_przesuniecie = 0;

            }
            if(konieczne_przesuniecie == 0)
            {
                // a może wystaje poza prawy brzeg? ------------------------
                int prawy_brzeg_napisu = (boundingRect.width() /2) + boundingRect.x() + (translate_x*gskala);

                //                cout << "prawy_brzeg_napisu = " << prawy_brzeg_napisu
                //                     << " rozm_tarczy_x = " << rozm_tarczy_x
                //                     << endl;

                konieczne_przesuniecie = rozm_tarczy_x - prawy_brzeg_napisu;
                if(konieczne_przesuniecie > 0)konieczne_przesuniecie = 0;
                //                cout << " PRAWY BRZEG konieczne_przesuniecie = " << konieczne_przesuniecie
                //                     << ", gskala = " << gskala << endl;
            }

            p += QPoint(konieczne_przesuniecie, 0 );
            boundingRect.moveCenter(p) ;

            // wlasciwy wypis całej daty

            painter.setPen(Qt::SolidLine);
            painter.setPen(cyf.kolor_ramki_datownika);
            painter.setBrush(cyf.kolor_tla_datownika);
            painter.drawRect(boundingRect);

            painter.setPen(cyf.kolor_dnia_tygodnia);
            painter.drawText(boundingRect, 0,full_date, &boundingRect);

            //------------- cień do tej ramki
            painter.setPen(przyciemnij(cyf.kolor_ramki_datownika) );   // kolor brzegu

            int skok  = 1;

            boundingRect.moveCenter(p + QPoint(skok, skok) );
            painter.drawLine(
                        boundingRect.x() ,
                        boundingRect.y() ,
                        boundingRect.x(),
                        boundingRect.y() + boundingRect.height() - skok );
            painter.drawLine(
                        boundingRect.x()  ,
                        boundingRect.y()  ,
                        boundingRect.x() + boundingRect.width() - skok,
                        boundingRect.y()  );
            // --------------



        }


        if(flag_show_date) {               // data ----------------------------------

            painter.setPen(Qt::NoPen);
            //     auto date.toString("ddd d").toStdString()

            // painter.scale( godz_skalowanie_x, godz_skalowanie_y); // Scales the coordinate system by (sx, sy).

            //    painter.scale( 1,1);
            QPoint p = cyf.pozycja_datownika;
            int left = p.x() * promien/100.0;
            int top = -(font_w_pikselach / 2) + (p.y()* promien/100.0) ;
            int szer  = promien * cyf.szer_datownika / 100;
            //    cout << "pozycja datownika " << left << ", top = " << top
            //         << ", skal x = " << godz_skalowanie_x << ", skal y " <<godz_skalowanie_y
            //         << endl;
            const QRect rectangle_daty = QRect(
                        left,
                        top,
                        szer,  // witdth
                        font_w_pikselach    // -(2*top) // promien/5// height;
                        );
            painter.setPen(Qt::SolidLine);
            painter.setPen(cyf.kolor_ramki_datownika);
            painter.setBrush(cyf.kolor_tla_datownika);

            painter.drawRoundedRect(rectangle_daty, 2.0, 2.0);    // narysowanie tła

            painter.setPen(cyf.kolor_datownika);
            painter.drawText(
                        rectangle_daty,
                        Qt::AlignCenter,
                        date.toString("d")
                        );

            //------------- cień do tej ramki
            painter.setPen(przyciemnij(cyf.kolor_ramki_datownika) );   // kolor brzegu

            int skok  = 1;

            // rectangle_daty.moveCenter(p + QPoint(skok, skok) );
            painter.drawLine(
                        rectangle_daty.x()+skok,
                        rectangle_daty.y()+skok,
                        rectangle_daty.x()+skok ,
                        rectangle_daty.y() + rectangle_daty.height() - skok );
            painter.drawLine(
                        rectangle_daty.x() +skok,
                        rectangle_daty.y() + skok,
                        rectangle_daty.x() + rectangle_daty.width() ,
                        rectangle_daty.y() + skok);
            // --------------


        }
        painter.restore();    // odpamietujemy showany stan, translacja tak, skala nie
    }

    //###################################################################
    //###################################################################
    //###################################################################
    //###################################################################

    painter.setPen(Qt::SolidLine);
    painter.setPen(Qt::RoundJoin | Qt::RoundCap);

    //    cout << "Zestaw wskazowek NAD tarcza " << endl;

    for(uint i = 0 ; i < cyf. pointery_wskazowek.size() ; ++i )
    {
        // if( cyf.pointery_wskazowek[i]->warstwa > 20) continue;
        if( cyf.pointery_wskazowek[i]->warstwa >= 0  )
            rysuj_ogolna_wskazowke(painter,  *(cyf.pointery_wskazowek[i]) );
    }


#if 0 // chwilowa blokada zwyklych wskazowek =====================

#if  1
    // wyswietlam wskazowka dnia tygodnia
    if(cyf.wsk_dni_tygodnia.flaga_enable){
        rysuj_ogolna_wskazowke(painter,cyf.wsk_dni_tygodnia);
    }
#endif // wyswietlam wskazowka tygodnia

#if  1
    // wyswietlam wskazowka dnia miesiaca
    if(cyf.wsk_dni_miesiaca.flaga_enable){
        rysuj_ogolna_wskazowke(painter, cyf.wsk_dni_miesiaca);
    }
#endif

#if  1
    // wyswietlam wskazowka nazwy miesiaca
    if(cyf.wsk_miesiaca.flaga_enable){
        rysuj_ogolna_wskazowke(painter, cyf.wsk_miesiaca);
    }
#endif // wyswietlanie wskazowka tygodnia

#if  1
    // wyswietlam wskazowka nazwy night_day
    if(cyf.wsk_night_day.flaga_enable){
        rysuj_ogolna_wskazowke(painter, cyf.wsk_night_day);
    }
#endif // wyswietlanie wskazowka tygodnia

#if  1
    // wyswietlam wskazowka zones (na gornej warstwie!
    if(cyf.wsk_zones.warstwa >= 0 )
    {
        if(cyf.wsk_zones.flaga_enable)
        {
            rysuj_ogolna_wskazowke(painter, cyf.wsk_zones);
        }
    }
#endif



    //---------------- koniec extra, teraz zwykłe -----------------
#if  1

    // 0  -- godzinowa =================================================
    rysuj_ogolna_wskazowke(painter, cyf.wsk_godzinowa);
#endif // 0  -- godzinowa =================================================


#if 1
    // wskazowka - minutowa,  --------------------------------------------
    {
        // cout << "Wskazowka minutowa ma byc bitmapowa? " << cyf.wsk_minutowa.flag_bitmapowa << endl;

        //        cout << __func__  << " cyf.wsk_minutowa.jesli_wektorowa_to_typ " << cyf.wsk_minutowa.jesli_wektorowa_to_typ << endl;
        rysuj_ogolna_wskazowke(painter, cyf.wsk_minutowa);
    }
#endif   // minutowa


    // wskazowka - sekundowa, --------------------------------------------
#if 1

    if(flag_show_sekundnik )
    {
        //         cout << "Wskazowka wsk_sekundowa ma byc bitmapowa? " << cyf.wsk_sekundowa.flag_bitmapowa << endl;
        //double  kat_obrotu = 6.0 * (now_time.second() );
        rysuj_ogolna_wskazowke(painter, cyf.wsk_sekundowa);
    }
#endif // 0  // sekundowa

#endif // chwilowej blokady


    //-centralna zasłonka na oś wskazowek ---------------------------------

#if KRZYZE_PROWADZACE
    painter.setPen( cyf.kolor_cienia); // kolor cienia
    painter.drawLine(0,-promien*2, 0, +promien*2);
    painter.drawLine(-promien*2, 0, promien*2, 0);

    //    QRectF rectangle(-promien/2, -promien/2, promien/2, promien/2);
    //     painter.drawArc(rectangle,0, 360);

    //painter.drawArc( (-promien*2, 0, promien*2, 0);
    // promieniscie
    //    for(int i = 0 ; i < 10 ; ++i)
    //    {
    //       painter.drawLine(0, 0, promien *sin(i/3.14) , promien*cos(i/3.14) );
    //    }
#endif

    painter.scale( cyf.skala_x_center * gskala,  cyf.skala_y_center * gskala);
    painter.drawPixmap(-central.width()/2.0, -central.height()/2.0, central);

}
//****************************************************************************************************
void MainWindow::ustaw_czas_wg_strefy()
{

    //    cout << __func__ << "============================================"<< endl;

    if( (nazwa_strefy_czasowej.empty() || nazwa_strefy_czasowej[0] == '*')  == false)
        // it is known, so just apply it
    {
        //        cout << ">>>>> Gdy nazwa strefy jest podana:  " << nazwa_strefy_czasowej << endl;
        QString strefa = nazwa_strefy_czasowej.c_str();
        // uwaga, to może nawet być ta cywilna nazwa typu Indian/Mahe, Europe/Warsaw
        strefa_czasowa = QTimeZone( strefa.toLatin1()  );


        //         QDateTime lokalny_czas = QDateTime::currentDateTime();
        //          lokalny_czas.setTimeZone(strefa_czasowa);
        //        cout << "to po ustawieniu obiektu QTimeZone "
        //             << " display name = "
        //             << strefa_czasowa.displayName(lokalny_czas, QTimeZone::OffsetName).toStdString()
        //             << endl;

        //        cout
        //                << "a)  lokalny_czas.time().hour() = "
        //                << lokalny_czas.time().hour()
        //                << endl;

        return;
    }

    //    cout << "Time zone has to  be recognised" << endl;

    QDateTime lokalny_czas = QDateTime::currentDateTime();
    //    cout
    //            << "a)  lokalny_czas.time().hour() = "
    //            << lokalny_czas.time().hour()
    //            << endl;

    auto wykryty_time_zone = lokalny_czas.timeZone();
    //   cout << "time zone abbreviation = "
    //        << lokalny_czas. timeZoneAbbreviation().toStdString()   // TO MA SENS
    //        << endl;


    //    cout
    //            << " wykryty display !!!!! = "
    //         << wykryty_time_zone.displayName(lokalny_czas, QTimeZone::QTimeZone::ShortName).toStdString()
    //         << " wykryty id = "
    //      << wykryty_time_zone.id().toStdString()
    //         << endl;

    nazwa_strefy_czasowej = wykryty_time_zone.displayName(lokalny_czas, QTimeZone::OffsetName).toStdString();
    //    cout  <<  "nazwa_strefy_czasowej " << nazwa_strefy_czasowej << endl;
    strefa_czasowa = wykryty_time_zone;

    //    QDateTime
    current_zone_date_time = lokalny_czas;
    //    cout
    //            << "b)  current_zone_date_time.time().hour() = "
    //            << current_zone_date_time.time().hour()
    //            << endl;



    //    cout << "a ) biezaca strefa czasowa ma nazwe " << nazwa_strefy_czasowej << endl;

    //    cout << " A teraz to sprawdzimy ---------" << endl;
    if(strefa_czasowa.isValid())
    {
        //        cout << "Time_zone is valid  " << endl;

        //  strefa_czasowa.abbreviation(current_zone_date_time ).toStdString();
        //QDateTime::currentDateTime();
        //        cout
        //                << "c)  teraz  current_zone_date_time.time().hour() = "
        //                << current_zone_date_time.time().hour()
        //                << ", nazwa strefy: "
        //                << strefa_czasowa.abbreviation(current_zone_date_time ).toStdString()
        //                << endl;
    } else{

        cout << "Time_zone is NOT valid " << endl;
        // time_zone = QTimeZone::systemTimeZone();
        QDateTime lokalny_czas2 = QDateTime::currentDateTime();
        current_zone_date_time = lokalny_czas2;
        //        cout
        //                << "d)  current_zone_date_time.time().hour() = "
        //                << current_zone_date_time.time().hour()
        //                << endl;
        //  strefa_czasowa =   QTimeZone::
        // goto gora;
    }

    //    cout << "wspolna czesc funkcji po sprawdzeniu valid " << endl;

    // żeby mapka świata i zones cities się zgadzały z obecnym czasem lokalnym
    // nawet jeśli jest Daylight Saving Time
    ile_minut_offsetu_DST = strefa_czasowa.daylightTimeOffset( lokalny_czas) / 60;
    //    cout << "ile_minut_offsetu = " << ile_minut_offsetu_DST << endl;
    //    QTime
    //    now_time = current_zone_date_time.time();
    //    //    QDate
    //    now_today = current_zone_date_time.date();

    now_minutes_from_UTC = current_zone_date_time.offsetFromUtc() / 60;

    //       int godzina = time.hour();
    //    // wydruk biezacego czasu w danej strefie
    //    cout << "godzina w tej strefie = "
    //         << current_zone_date_time.toString().toStdString()
    //         << " godzina = "
    //         << now_time.hour()
    //         << "      nazwa biezacej strefy ["
    //         << strefa_czasowa.abbreviation(current_zone_date_time ).toStdString()
    //         << "]"
    //         << ", now_minutes_from_UTC = "
    //         << now_minutes_from_UTC

    //            //                << " lokalny_czas.time().hour() = "
    //            //             << lokalny_czas.time().hour()
    //         << endl;

    // sprobujemy sami zbudować nazwę strefy
    int godzin =  (now_minutes_from_UTC/60) ;
    int minut = now_minutes_from_UTC - (godzin*60) ;
    char znak = (now_minutes_from_UTC < 0) ? '-' : '+';
    ostringstream sstr;
    sstr << "UTC" << znak << setfill('0')
         << setw(2) << godzin
         << ':'
         << setw(2) << minut
            ;
    nazwa_strefy_czasowej = sstr.str();
    // cout << "MOJA konstrukcja " << " nazwa_strefy_czasowej = "
    //         <<  nazwa_strefy_czasowej << endl;
    QString n { nazwa_strefy_czasowej.c_str() };
    strefa_czasowa = QTimeZone( n.toLatin1());



#if 0
    cout << "NA koniec ----------------------------" << endl;

    // zbudowanie listy nazw stref ----------------------------------
    // z ustawieniem wybranej linijki na bieżąco obowiązującej strefie czasowej
    int indeks = 0;
    QString n;
    QList<QByteArray> lista = QTimeZone::availableTimeZoneIds();
    for(int i = 0 ; i < lista.size() ; ++i)
    {
        QString nazwa = lista[i].toStdString().c_str();
        cout << nazwa.toStdString() << endl;
        if(nazwa.toStdString() .find(nazwa_strefy_czasowej) != string::npos )
        {
            indeks = i;
            n = nazwa;
        }
    }
    cout << "po znalezieniu str czasow nr " << indeks
         << " o nazwie "
         << n.toStdString()
         << endl;

    strefa_czasowa = QTimeZone( n.toLatin1());

#endif



}
//*************************************************************************************************************************
void MainWindow::rysuj_ogolna_wskazowke(
        QPainter &painter,
        Tdane_wskazowki & wskazowka
        )
{

    //    switch(wskazowka.rola_wskazowki )
    //    {
    //    case  Rola_wskazowki::wsk_sekundowa:
    //        cout << " wskazowka.rola_wskazowki == Rola_wskazowki::wsk_sekundowa" << endl;
    //        break;

    //    case  Rola_wskazowki::wsk_sekundnik_dodatkowy:
    //        cout << " wskazowka.rola_wskazowki == Rola_wskazowki::wsk_sekundnik_dodatkowy" << endl;
    //        break;

    //    case  Rola_wskazowki::wsk_godzinowa:
    //        cout << " wskazowka.rola_wskazowki == Rola_wskazowki::wsk_sekundnik_dodatkowy" << endl;
    //        break;

    //    default:
    //          cout << " wskazowka.rola_wskazowki == NIEZNANA" << endl;
    //        break;
    //    }



    if( flag_show_sekundnik == false &&
            (wskazowka.rola_wskazowki == Rola_wskazowki::wsk_sekundowa
             ||
             wskazowka.rola_wskazowki == Rola_wskazowki::wsk_sekundnik_dodatkowy)
            )
    {
        return;   // nie wyświetlamy bo odwraca uwagę od pracy
    }

    double kat_obrotu = 0;
    if(wskazowka.obliczenie_kata != nullptr)
    {
        kat_obrotu = wskazowka.obliczenie_kata();
    }

    painter.save();   // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    painter.setPen(Qt::NoPen);
    painter.setBrush(wskazowka.kolor);

    painter.translate(wskazowka.poz_x * gskala, wskazowka.poz_y * gskala);

#if KRZYZE_PROWADZACE == true
    painter.setPen( cyf.kolor_cienia); // kolor cienia
    //    painter.drawLine(0,-promien, 0, +promien);
    //    painter.drawLine(-promien, 0, promien, 0);
    //    cout << " promieniscie "<< endl;

    //    for(int i = 0 ; i < 20 ; ++i)
    //    {
    //        double kat   = i*(360/31.0) +270 ;
    //        cout << kat << endl ;

    //        painter.drawLine(0, 0,
    //                            promien *cos( qDegreesToRadians(kat)) ,
    //                            promien *sin( qDegreesToRadians((kat) )) );

    //    }


#endif


    painter.rotate(kat_obrotu);
    painter.scale(wskazowka.przel_skalowanie_x,wskazowka.przel_skalowanie_y);               // Scales the coordinate system by (sx, sy).


    //        cout << "Wskazowka wsk_sekundowa ma byc bitmapowa? " << cyf.wsk_m sekundowa.flag_bitmapowa << endl;
    //       cout << __func__ << ". Wskazowka argument  " << wskazowka.bitmapa <<  ", 8  flag_bitmapowa  = " << boolalpha
    //            <<  wskazowka.flag_bitmapowa  << endl;

    int lokalna_odl_cienia = cyf.uskok_cienia;
    if(wskazowka.odleglosc_cienia != -1)  // jesli nie defaultowy cien
    {
        lokalna_odl_cienia = wskazowka.odleglosc_cienia;
    }

    if(wskazowka.flag_bitmapowa )
    {
        //        cout << "Skalownie przed drawPixmap"
        //             << " o nazwie " << wskazowka.nazwa
        //             << ",    wskazowka.przel_skalowanie_x =  "
        //             << wskazowka.przel_skalowanie_y << "]"
        //            << ",  wskazowka.przel_mimosrod_x = " << wskazowka.przel_mimosrod_x
        //            << ", wskazowka.przel_mimosrod_y  = "<< wskazowka.przel_mimosrod_y
        //            << endl;


        double  kat_obrotu_rad = qDegreesToRadians(kat_obrotu +45 );
        double odl_cienia_x = sin(kat_obrotu_rad) * lokalna_odl_cienia /  wskazowka.przel_skalowanie_x;
        double odl_cienia_y = cos(kat_obrotu_rad) * lokalna_odl_cienia /  wskazowka.przel_skalowanie_y;

        // cień wskazówki ------------------

        QPixmap cien_pix =  * wskazowka.pix_ptr;
        cien_pix.fill(cyf.kolor_cienia);
        auto maska =  wskazowka.pix_ptr->createMaskFromColor(Qt::transparent);
        cien_pix.setMask( maska ) ;

        painter.drawPixmap(wskazowka.przel_mimosrod_x+ odl_cienia_x,
                           wskazowka.przel_mimosrod_y + odl_cienia_y,
                           cien_pix);

        // wlasciwa wskazowka --------------
        painter.drawPixmap(wskazowka.przel_mimosrod_x,   wskazowka.przel_mimosrod_y,   *wskazowka.pix_ptr);
    }
    else{
        painter.setPen(Qt::RoundJoin );
        QPen pen2;
        pen2.setWidth(2*  wskazowka.przel_szer_poly);
        painter.setPen(pen2) ;

        // -------------cien  wektorowy ----------------
        painter.setPen( cyf.kolor_cienia); // kolor cienia
        painter.setBrush(cyf.kolor_cienia);

        double  kat_obrotu_rad = qDegreesToRadians(kat_obrotu +45 );
        double odl_cienia_x = sin(kat_obrotu_rad) * lokalna_odl_cienia /   wskazowka.przel_skalowanie_x;
        double odl_cienia_y = cos(kat_obrotu_rad) * lokalna_odl_cienia /   wskazowka.przel_skalowanie_y;


        //        cout << __func__
        //             << ", wskazowka.jesli_wektorowa_to_typ ="  <<      wskazowka.jesli_wektorowa_to_typ  << endl;
        switch(wskazowka.jesli_wektorowa_to_typ)
        {
        case 0:  default:  // ostry prostokat
            // cien --------------------------------------------------------------
            rysuj_wskaz_wektorowa(painter, wskazowka.jesli_wektorowa_to_typ,
                                  wskazowka.przel_mimosrod_x ,
                                  wskazowka.przel_mimosrod_y  - wskazowka.przel_length_poly,
                                  wskazowka.przel_szer_poly,
                                  wskazowka.przel_length_poly,
                                  wskazowka.kolor,
                                  odl_cienia_x, odl_cienia_y, cyf.kolor_cienia,
                                  0,
                                  0
                                  );
            break;

        case 1 :
            // zaokraglony prostokat
            //            // wskazowka i jej cien ---------------
            rysuj_wskaz_wektorowa(painter,wskazowka.jesli_wektorowa_to_typ,
                                  wskazowka.przel_mimosrod_x ,
                                  wskazowka.przel_mimosrod_y  - wskazowka.przel_length_poly,
                                  wskazowka.przel_szer_poly,
                                  wskazowka.przel_length_poly,

                                  wskazowka.kolor,
                                  odl_cienia_x, odl_cienia_y, cyf.kolor_cienia,
                                  wskazowka.przel_szer_poly  /  wskazowka.przel_skalowanie_x,
                                  wskazowka.przel_szer_poly /  wskazowka.przel_skalowanie_y
                                  );
            break;


        case 2: // wektorowa ozdobna typu strzalki  -------------------------------------

            rysuj_wskaz_wektorowa(painter,wskazowka.jesli_wektorowa_to_typ,
                                  wskazowka.przel_mimosrod_x,
                                  wskazowka.przel_mimosrod_y,
                                  wskazowka.przel_szer_poly*2,
                                  wskazowka.przel_length_poly,
                                  wskazowka.kolor,
                                  odl_cienia_x, odl_cienia_y, cyf.kolor_cienia
                                  );
            break;
        } // end switch

    } // end else
    painter.restore();     // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
}

//*********************************************************************************************************
void MainWindow::rysuj_wskaz_wektorowa(QPainter & painter, int nr, double x, double y,
                                       double szer,
                                       double wys,
                                       QColor kolor,
                                       double cien_x,
                                       double cien_y, QColor kolor_cienia,
                                       double owal_x, double owal_y)
{

    //    cout << __func__ << " z arg typu nr = " << nr << endl;
    switch (nr)
    {
    default:
    case 0:
    case 1:
    {
        QPen pen(QColor(0,0,0,50),
                 szer /3.0,   // szerokosc
                 Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(kolor_cienia);
        painter.setPen(Qt::NoPen);

        // sam cien wskazowki
        painter.drawRoundedRect(
                    x-(szer/2) + cien_x,
                    y + cien_y,
                    szer, wys, owal_x, owal_y );

        // wlasciwa wskazowka
        painter.setPen(pen);
        painter.setBrush(kolor);
        painter.drawRoundedRect(  x-(szer/2),  y, szer, wys, owal_x, owal_y );

        break;
    }


    case 2:
    {
        // y  += wys;
        y = -y ;                              // bo dodatni Y ma oznaczac obnizenie mimosrodu
        //        szer = szer / 2.0;
        wys = -wys * 1 ; // 0.8;                       //  lubimy, tutaj, zeby oś y biegła "w górę"
        double w09 =   0.9 * wys -y;
        double w07 =   0.7 * wys - y;
        double w65 =   0.65 * wys - y;
        double w55 =   0.55 * wys - y;
        double przeciwwaga = y;

        const QPointF strzalka[] = {
            QPointF( x, 0 /* -przeciwwaga */ ),           //  srodek dół // to robimy z całym wyświetlaniem poniżej
            QPointF(x ,  w65 ),
            QPointF(x -szer,  w07 ),
            QPointF(x,  w09 ),
            QPointF(x, wys ),
            QPointF(x, w09 ),
            QPointF(x+szer,  w07 ),
            QPointF(x ,  w65  ),
            QPointF(x ,  w55  ),
            QPointF(x - szer / 2,  w55  ),
            QPointF(x + szer / 2,   w55  ),
        };



        QPen pen_cienia(kolor_cienia,
                        szer,   // szerokosc
                        Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen_cienia);

#if  1  // najpierw narysowanie cienia wskazowki
        painter.translate(cien_x, cien_y -przeciwwaga);
        painter.drawPolyline (strzalka, 11);
        painter.translate(-cien_x, -cien_y);
#endif

        // rysowanie wlaściwej wskazowki
#if 1
        QPen pen_wsk(kolor, szer,   // szerokosc
                     Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter.setPen(pen_wsk);

        painter.drawPolyline (strzalka, 11);

        // jesli chcemy ewentualne wypelnienie, to to co poniżej
        // painter.setBrush(QColor(250, 250,0) );
        //painter.drawPolygon (hourHand, 11);

#endif
    }
        break;


    }
}
//*********************************************************************************************************
void MainWindow::wywolanie_okna_alarmow()
{
    cout << "wyolanie okna alarmow " << endl;
    Alarmy *dlg = new Alarmy(this, pathed_nazwa_pliku_z_alarmami);
    smart_positioning(dlg);

    dlg->exec();
    delete dlg;
    cout << "Skonczone wywolanie    "   << __PRETTY_FUNCTION__ << endl;
}
//*********************************************************************************************************
void  MainWindow::smart_positioning(QDialog  *dlg)
{
    dlg->adjustSize();

    QRect parentRect = this->geometry();
    QRect screenRect = QApplication::primaryScreen()->availableGeometry();

    int dx = 50;
    int dy = 50;

    int newX, newY;

    // --- X position ---------------------------------------------------------

    if (parentRect.left() - dx - dlg->width() > screenRect.left()) {
        // miejsce po lewej
        newX = parentRect.left() - dlg->width() - dx;
    }
    else if (parentRect.right() + dx + dlg->width() < screenRect.right()) {
        // miejsce po prawej
        newX = parentRect.right() + dx;
    }
    else {
        // brak miejsca — centrowanie
        newX = parentRect.center().x() - dlg->width() / 2;
    }

    // --- Y position ---------------------------------------------------------

    if (parentRect.top() - dy - dlg->height() > screenRect.top()) {
        // miejsce nad rodzicem
        newY = parentRect.top() - dlg->height() - dy;
    }
    else if (parentRect.bottom() + dy + dlg->height() < screenRect.bottom()) {
        // miejsce pod rodzicem
        newY = parentRect.bottom() + dy;
    }
    else {
        // brak miejsca — centrowanie
        newY = parentRect.center().y() - dlg->height() / 2;
    }

    // --- Ostateczne zabezpieczenie przed wyjściem poza ekran ----------------

    // clamp X
    if (newX < screenRect.left())
        newX = screenRect.left();
    if (newX + dlg->width() > screenRect.right())
        newX = screenRect.right() - dlg->width();

    // clamp Y
    if (newY < screenRect.top())
        newY = screenRect.top();
    if (newY + dlg->height() > screenRect.bottom())
        newY = screenRect.bottom() - dlg->height();

    dlg->move(newX, newY);

}
//*********************************************************************************************************
void MainWindow::wywolanie_okna_opcji()
{
    wstepne_nr_jezyka = nr_jezyka;

    bool flaga__byl_tryb_random =  flag_random_tarcza;
    auto *dlg = new Topcje_dlg(this);

    smart_positioning(dlg);

    if(dlg->exec() == QDialog::Accepted)
    {
        // COTO;
        if(flaga__byl_tryb_random)
        {
            auto odp =  QMessageBox::warning ( this,
                                               tr("Random time was while you entered option menu"),
                                               tr( "Do you want to continue the 'random clock' mode ? "),
                                               QMessageBox::Yes | QMessageBox::No);
            if(odp == QMessageBox::Yes)
                flag_random_tarcza = true;
        }
        zapis_opcji_na_dysku();
    }
    delete dlg;
    // COTO;
    this->setWindowOpacity(przezroczystosc/255.0);

    //        cout << "Po dialogu opcji jezyk nr  " << nr_jezyka << endl;

    if(flag_na_wierzchu != wstepne_flag_na_wierzchu
            ||
            wstepne_nr_jezyka != nr_jezyka
            )
    {
        flag_ma_byc_restart = true;
        cout << "zamkniecie okna opcji z restartem  (przed close )" << endl;
        close();
    }
    // COTO;
}

//*********************************************************************************************************
void MainWindow::nastepna_tarcza()
{
    if(++nr_tarczy >= cyferblat.size()){
        nr_tarczy = 0;
    }
    cout << "CZy ta funkca jest używana?" << endl;
    zmiana_wygladu_cyferblatu(nr_tarczy, tryb_wskazowek::recent);
}
//*********************************************************************************************************
//void MainWindow::wywolanie_okna_wskazowek()
//{
//    // TA FUNKCJA JUZ NIE JEST POTRZEBNA

//    flag_wskazowki_bitmapowe = !flag_wskazowki_bitmapowe;
//    zmiana_wygladu_cyferblatu(nr_tarczy);

//}

//*********************************************************************************************************
void MainWindow::koniec_programu()
{
    //    cout << "111  funkcja MainWindow::koniec_programu()" << endl;
    zapis_opcji_na_dysku();
    //    cout << "222 przed close()" << endl;
    close();   // to nie dziala dla splash screen więc...
    QCoreApplication::quit();
}
//********************************************************************************************************
void MainWindow::player_position_upadate(qint64 position)
{
    //    cout << "player_position_update , (sprawdzenie długosci grania), obecnie =" << position << " ms" << endl;

    auto & v = vec_playerow_grajaych_teraz;  // bo za dluga nazwa

    //    cout << "Wektor grajacych ma elementow = " << v.size() << endl;

    // przeszukac alarmy ktory z nich ma adres tego playera
    for(uint i = 0 ; i < v.size() ; ++i)
    {
        // czy to ten palyer ?
        if( v[i].wsk_playera == sender())
        {
            //            cout << "to ten nadawca " << "ma miec sekund " << v[i].sekund_utworu << endl;

            if(position/1000 > v[i].sekund_utworu)
            {
                cout  << "Mija czas, wiec stop " << endl;
                v[i].wsk_playera->stop();

                if(v[i].flag_loop == false)
                {
                    //                    cout << "nie ma flagi 'loop' wiec usuwamy z tablicy grajacych" << endl;


                    //  v[i].wsk_playera = nullptr;
                    //    v. pop_back();

                    for( auto it =  v.begin() ;                         it != v.end() ; )
                    {
                        if(it->wsk_playera == sender() )
                        {
                            v.erase(it);
                        }
                        else
                            ++it;
                    }
                }
                else
                {
                    //                    cout << "jest flaga 'loop' nie usuwamy z tablicy grajacych " << endl;
                    v[i].wsk_playera->play();
                }
                break;
            }
        }
        else{
            //            cout << i << ") Ta pozycja w tablicy grajacych to nie TEN " << endl;

        }


        //          if(v.first = )
        //          v.second   .sekund_dzwieku
        //                  al.player
    }
    //cout << " ma byc sekund " << al.sekund_dzwieku << endl;
    //if(al.sekund_dzwieku  )
}

//*********************************************************************************************************
void MainWindow::wstepne_zaladowanie_tablicy_alarmow() {

    tablica_alarmow.clear();
    ifstream plik(pathed_nazwa_pliku_z_alarmami);
    if(!plik)
    {
        cout << tr("Can't open file with user alarms: ").toStdString()
             << pathed_nazwa_pliku_z_alarmami
             << tr("\n (if you haven't defined them yet - no problem)").toStdString()
             << endl;
        return;
    }
    string linijka;
    string tresc;
    while(
          getline(plik, linijka))
    {
        tresc += linijka + '\n';
    }
    // cout << "wczytany caly plik = " << tresc << endl;
    Tjeden_alarm a;

    size_t kursor = 0;
    for(int entry = 0; ; entry++)      // petla po alarmach
    {
        // wyjecie jednego alarmu ----------------------
        auto pocz = tresc.find("<alarm>", kursor);
        kursor = pocz;
        auto kon = tresc.find("</alarm>", kursor);
        kursor = kon;
        if(pocz == string::npos || kon == string::npos)
        {
            // cout << "Kniec peteli bo pocz = " << pocz << ", kon = " << kon << endl;
            break;
        }
        string one = tresc.substr(pocz, kon-pocz);

        // cout << "Jeden alarm nr " << entry << " ma tresc " << one << endl;
        // wyjecie poszczegolnych itemow z alarmu -------------------
        a =  wyjecie_itemow(one);
        a.nr_entry = entry;
        tablica_alarmow.push_back(a);
    }
    //    cout << "Po zakoniczeniu petli tablica alarmow  ma rozmiar " << tablica_alarmow.size() << endl;


    // Roboczo, w  przypadku konieczności wstepnego, "ręcznego" zaladowania
    //    tablica_alarmow.push_back(Tjeden_alarm{ true, "alarm A", "12:33", 1, "12-05-2020",
    //                                            0xf,  // czestotliwosc
    //                                            0,   // minuty
    //                                            true, "jakis opis",
    //                              true, "dzwiek.mp3", 0, true,   // dzwięk on, pocz_sekund, loop
    //                              false,
    //                                            "nazwa_programu",
    //                                            false  // czy wylaczyc komputer
    //                              });
    //   tablica_alarmow.push_back(Tjeden_alarm{ false, "alarm B", "17:44", 2,"12-09-2020", true, "inny  opis",
    //                                           true, "dzwiek22.mp3", 0, true,   // dzwięk on, pocz_sekund, loop
    //                                           false,
    //                                                         "gimp2",
    //                                                         false  // czy wylaczyc komputer
    //                                           });

}
//***************************************************************************************************************
Tjeden_alarm MainWindow::wyjecie_itemow(string one)
{
    Tjeden_alarm a;
    // znajdz enabled
    a.flag_enable = znajdz_int_item(one, "flag_enable");
    // znajdz zdarzenie
    a.nazwa = znajdz_txt_item(one, "nazwa");
    a.godzina = znajdz_txt_item(one, "godzina");
    a.czestotliwosc = znajdz_int_item(one, "czestotliwosc");
    a.data = znajdz_txt_item(one, "data");
    a.dni_tygodnia = znajdz_int_item(one, "dni_tygodnia");
    a.flag_even_odd_week_nr = znajdz_int_item(one, "flag_even_odd_week_nr");
    //  cout << " Wczytane a.flag_even_odd_week_nr " << a.flag_even_odd_week_nr << endl;
    a.interwal = znajdz_int_item(one, "interwal");

    a.flag_okno = znajdz_int_item(one, "flag_okno");
    a.dodatkowy_opis = znajdz_txt_item(one, "dodatkowy_opis");
    a.flag_dzwiek = znajdz_int_item(one, "flag_dzwiek");

    a.muzyka = znajdz_txt_item(one, "muzyka");
    a.sekund_dzwieku = znajdz_int_item(one, "sekund_dzwieku");
    a.loop = znajdz_int_item(one, "loop");
    a.flag_program = znajdz_int_item(one, "flag_program");
    a.nazwa_programu = znajdz_txt_item(one, "nazwa_programu");
    a.flag_wylacz_komputer = znajdz_int_item(one, "flag_wylacz_komputer");

    return a;
}
//***************************************************************************************************************
string MainWindow::znajdz_txt_item(string txt, string keyword)
{
    auto offset = string{"<" + keyword+ ">"}.size();
    auto pocz = txt.find ("<" + keyword+ ">");
    if(pocz == string::npos)
        return "";
    auto kon = txt.find("</" + keyword+ ">");
    if(kon == string::npos)
    {
        exit(2);
    }
    pocz+= offset;
    string wartosc = txt.substr(pocz, kon-pocz);

    // cout << keyword << " ma pocz = " << pocz<< ", kon= " << kon << ",   txt wartosc = [" << wartosc << "]\n\n" << endl;
    return wartosc;
}
//***************************************************************************************************************
int MainWindow::znajdz_int_item(string txt, string keyword)
{
    auto offset = string{"<" + keyword+ ">"}.size();
    auto pocz = txt.find ("<" + keyword+ ">");
    if(pocz == string::npos)
        return -1;
    auto kon = txt.find("</" + keyword+ ">");
    pocz+= offset;
    int wartosc = stoi (txt.substr(pocz, kon-pocz) ) ;
    // cout << keyword << " ma int wartosc = [" << wartosc << "]" << endl;
    return wartosc;
}
//***************************************************************************************************************
// Obecne tu dane o mimośrodach dotyczą sytuacji wskazowek wektorowych. Natomiast
// w przypadku wskazowek bitmapowych dane o mimośrodzie będą one  brane z bazy wskazowek.
//***************************************************************************************************************

void MainWindow::wstepne_wpisanie_cyferblatow()
{
    //   cout <<  __PRETTY_FUNCTION__<< endl;

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Random of one below",
                    ":/new/prefix1/content/city_hall_jg.png",       // ":/new/prefix1/content/AJ-CityHall-500.png",
                    ":/new/prefix1/content/city_hall_hours.png",
                    ":/new/prefix1/content/city_hall_minutes.png",
                    "0", // ":/new/prefix1/content/second.png",
                    ":/new/prefix1/content/center.png",

                    0,
                    0.15,// XY mimosrod wektorowej wskazowki godzinowej
                    0.05,   // grubosc w skali promienia tarczy (wazne dla wektorowych)
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0, 0.1,
                    0.01,   0.85,// os minutowej

                    QColor(0, 0, 50),

                    //                    0.0,    0,   0.005,  0.8,  // os sekundnika
                    0.0,    0,    0.01,
                    1,  // os sekundnika
                    QColor(200, 0, 0),
                    250,  250,  // srodek tarczy
                    1.0,  1.0,   // center
                    225,   // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 60),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor pełnej daty

                    QPoint(55, 0),    // pozycja datownika % z dlug promienia
                    17,   // szer okienka w % dlugosci promienia
                    8,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(50, 0, 0, 255),   // kolor daty
                    QColor(50, 50, 50, 100),   // kolor ramki_daty
                    QColor(100, 100, 100, 100),   // kolor tla_daty
                    QColor(0, 0, 0, 25) ,  // kolor cienia
                    5   // uskok cienia w procentach promienia

                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "CityHall",
                    ":/new/prefix1/content/city_hall_jg.png",       // ":/new/prefix1/content/AJ-CityHall-500.png",
                    ":/new/prefix1/content/city_hall_hours.png",
                    ":/new/prefix1/content/city_hall_minutes.png",
                    "0", // ":/new/prefix1/content/second.png",
                    ":/new/prefix1/content/center.png",

                    0,
                    0.15,// XY mimosrod wektorowej wskazowki godzinowej
                    0.05,   // grubosc w skali promienia tarczy (wazne dla wektorowych)
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0, 0.1,
                    0.01,   0.85,// os minutowej

                    QColor(0, 0, 50),

                    //                    0.0,    0,   0.005,  0.8,  // os sekundnika
                    0.0,    0,    0.01,
                    1,  // os sekundnika
                    QColor(200, 0, 0),
                    250,  250,  // srodek tarczy
                    1.0,  1.0,   // center
                    225,   // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 60),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor pełnej daty

                    QPoint(55, 0),    // pozycja datownika % z dlug promienia
                    17,   // szer okienka w % dlugosci promienia
                    8,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(50, 0, 0, 255),   // kolor daty
                    QColor(50, 50, 50, 100),   // kolor ramki_daty
                    QColor(100, 100, 100, 100),   // kolor tla_daty
                    QColor(0, 0, 0, 25) ,  // kolor cienia
                    5   // uskok cienia w procentach promienia

                }
                );



    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Target",
                    ":/new/prefix1/content/target.png",       // ":/new/prefix1/content/AJ-CityHall-500.png",
                    ":/new/prefix1/content/city_hall_hours.png",
                    ":/new/prefix1/content/city_hall_minutes.png",
                    "0", // ":/new/prefix1/content/second.png",
                    ":/new/prefix1/content/center.png",

                    // godzinowa ---
                    0,
                    0.22,// XY mimosrod wektorowej wskazowki godzinowej
                    0.05,   // grubosc w skali promienia tarczy (wazne dla wektorowych)
                    0.6,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    // minutowa ---
                    0.0, 0.1,
                    0.01,   0.7,// os minutowej

                    QColor(0, 0, 50),

                    // sekundnik ---
                    0.0,    0,   // XY mimosrod wektorowej wskazowki
                    0.01, // grubosc wektorowego sekundnika
                    0.88,  // wysokosc  w skali promienia tarczy
                    QColor(200, 0, 0),
                    250,  252,  // srodek tarczy
                    1.5,  1.5,   // center
                    225,   // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -50),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(40, 40, 40),   // kolor AM_PM
                    QPoint(0, 60),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(180, 180, 180),   // kolor pełnej daty

                    QPoint(38, -1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(150, 150, 150),   // kolor daty
                    QColor(50, 50, 50, 200),   // kolor ramki_daty
                    QColor(0, 0, 0, 100),   // kolor tla_daty
                    QColor(0, 0, 0, 25) ,  // kolor cienia
                    5,   // uskok cienia w procentach promienia
                    QColor(250,250,250,180)    //  tlo_AM_PM_

                }
                );




    //---------------------------
#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Rollex Oyster",
                    ":/new/prefix1/content/arne22.png",
                    "1",
                    "0",
                    "0", // ":/new/prefix1/content/second.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.09,  0.05,  0.7, // os godzinowej, skala
                    QColor(30, 30, 30),

                    0.0,    0.09,
                    0.04,  0.9,// os minutowej
                    QColor(30, 30, 50),

                    0,    0.2,   0.005, 1,  // os sekundnika
                    QColor(200, 0, 0),

                    77,   77,  0.25,  0.25,   // center
                    64,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                    QPoint(0, 45),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM

                    QPoint(0, 30),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(50, 50, 50),   // kolor pełnej daty

                    QPoint(30, +1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(50, 50, 50, 255),   // kolor daty
                    QColor(30, 30, 30, 50),   // kolor ramki_daty
                    QColor(100, 100, 100, 50),   // kolor tla_daty

                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5   // uskok cienia w procentach promienia

                }
                );
#endif

    //------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Grollex silver",
                    ":/new/prefix1/content/Grolex.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",
                    0,     0.09,  0.03,  0.7, // os godzinowej, skala
                    QColor(30, 30, 30),

                    0.0,    0.09,
                    0.02,  0.9,// os minutowej
                    QColor(30, 30, 50),

                    0,    0.2,   0.005, 1,  // os sekundnika
                    QColor(200, 0, 0),

                    175,   175,  0.25,  0.25,   // center
                    150,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                    QPoint(0, 55),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM

                    QPoint(0, 30),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(100, 0, 0),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(50, 50, 50, 255),   // kolor daty
                    QColor(30, 30, 30, 50),   // kolor ramki_daty
                    QColor(100, 100, 100, 50),   // kolor tla_daty

                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5   // uskok cienia w procentach promienia

                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Grollex gold",
                    ":/new/prefix1/content/grolex_refleksy_ciemna_korona.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",
                    0,     0.09,  0.03,  0.7, // os godzinowej, skala
                    QColor(30, 30, 30),

                    0.0,    0.09,
                    0.02,  0.9,// os minutowej
                    QColor(30, 30, 50),

                    0,    0.2,   0.005, 1,  // os sekundnika
                    QColor(200, 0, 0),

                    175,   175,  0.25,  0.25,   // center
                    150,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                    QPoint(0, 55),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM

                    QPoint(0, 30),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(100, 0, 0),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(50, 50, 50, 255),   // kolor daty
                    QColor(30, 30, 30, 0),   // kolor ramki_daty
                    QColor(100, 100, 100, 20),   // kolor tla_daty

                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5   // uskok cienia w procentach promienia

                }
                );
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Grollex blue",
                    ":/new/prefix1/content/Grollex_blue_slim.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",
                    0,     0.09,  0.05,  0.6, // os godzinowej, skala
                    QColor(30, 30, 30),

                    0.0,    0.09,
                    0.02,  0.9,// os minutowej
                    QColor(30, 30, 50),

                    0,    0.2,   0.005, 1,  // os sekundnika
                    QColor(0, 150, 250),

                    198,   198,  1.,  1.,   // center
                    175,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(0, 0, 80),   // kolor AM_PM

                    QPoint(200, 105),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(0, 20, 20),   // kolor pełnej daty

                    QPoint(70, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250, 150, 50, 255),   // kolor daty
                    QColor(30, 30, 30, 5),   // kolor ramki_daty
                    QColor(100, 100, 100, 200),   // kolor tla_daty

                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5 ,  // uskok cienia w procentach promienia
                    QColor(250,250,250,180)    //  tlo_AM_PM_

                }
                );




    //---------------------------
#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "dragon",
                    ":/new/prefix1/content/dragon.png",
                    ":/new/prefix1/content/roman_hours.png",
                    ":/new/prefix1/content/roman_minut.png",
                    "0", //  ":/new/prefix1/content/second.png",
                    ":/new/prefix1/content/center.png",
                    0 ,    0,  0.04,  0.52, // os godzinowej, skala
                    QColor(30, 30, 30),
                    0.0,    0,   0.03,  0.8,// os minutowej
                    QColor(30, 30, 120),
                    0,    0,   0.005,  1,  // os sekundnika
                    QColor(200, 0, 0),
                    298,   340,  // pozycja srodka tarczy
                    0.75,  0.75,   // skalowanie zaślepki
                    60,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 180),   // kolor AM_PM
                    QPoint(0, 160),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250, 250, 250),   // kolor pełnej daty

                    QPoint(30, +1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    16,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(250, 250, 250, 255),   // kolor datownika
                    QColor(30, 30, 30, 50),   // kolor ramki_daty
                    QColor(80, 0, 100, 250),   // kolor tla_daty
                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5

                }
                );
#endif
    //-----------------------------------------------------------------

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Patek",
                    ":/new/prefix1/content/PatekPhilippe.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",
                    0,     0,  0.03    ,  0.570, // os godzinowej, skala
                    QColor(0, 0, 0),
                    0.0,    0,   0.015,  0.80,// os minutowej
                    QColor(0, 0, 0),
                    0,    0,   0.005,  1,  // os sekundnika
                    QColor(0, 0, 200),

                    128,   131,  // pozycja srodka tarczy
                    0.15,  0.15,   // skalowanie zaślepki
                    111,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                    QPoint(0, -20),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 110),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(0, 0, 40, 255),   // kolor pełnej daty

                    QPoint(30, -1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(50, 50, 50, 255),   // kolor daty
                    QColor(30, 30, 30, 50),   // kolor ramki_daty
                    QColor(100, 100, 100, 200),   // kolor tla_daty
                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Azul large",
                    ":/new/prefix1/content/Azul_large.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    //                    0,     0,  0.6    ,  0.60, // os godzinowej, skala
                    //                    QColor(0, 0, 200),
                    //                    0.0,    0,   1,  0.70,// os minutowej
                    //                    QColor(0, 0, 150),

                    0,     0.1 ,  0.025    ,  0.7, // os godzinowej, skala
                    QColor(0, 0, 200),
                    0.0,    0.07,   0.02,  0.85,// os minutowej
                    QColor(0, 0, 250),

                    0,    0,   0.003,  0.9,  // os sekundnika
                    QColor(200, 200, 0),

                    375,   375,  // pozycja srodka tarczy
                    0.35,  0.35,   // skalowanie zaślepki
                    225,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(20, 20, 80),   // kolor AM_PM

                    QPoint(0, 50),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(20, 20, 80),   // kolor pełnej daty

                    QPoint(30, -1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(0, 0, 150),   // kolor daty
                    QColor(0, 0, 150, 100),   // kolor ramki_daty
                    QColor(100, 100, 100, 50),   // kolor tla_daty
                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    3      // uskok cienia


                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Citizien",
                    ":/new/prefix1/content/Citizen.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0,  0.04 ,  0.60, // os godzinowej, skala
                    QColor(40, 10, 10),   // kolor wskaz godzinowej
                    0.0,    0,   0.02,  0.74,// os minutowej
                    QColor(40, 10, 10),   // kolor wskaz minutowej
                    0,    0,  0.005,  0.8,  // os sekundnika
                    QColor(100, 0, 0),   // kolor wskaz sekundnika

                    100,   100,  // pozycja srodka tarczy
                    0.45,  0.45,   // skalowanie zaślepki
                    80,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 15),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 110),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(180, 0, 0),   // kolor pełnej daty

                    QPoint(40, -2),    // pozycja datownika % z dlug promienia
                    38,   // szer okienka w % dlugosci promienia
                    13,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(50, 50, 50),   // kolor daty
                    QColor(0, 0, 0, 0),   // kolor ramki_daty
                    QColor(200, 200, 200, 250),   // kolor tla_daty
                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Comdex-Omega1",
                    ":/new/prefix1/content/Comdex - Omega1.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.09,  0.04 ,  0.65, // os godzinowej, skala
                    QColor(50, 50, 50, 180),   // kolor wskaz godzinowej
                    0.0,    0.1,   0.03,  0.85,// os minutowej
                    QColor(50, 50, 50),   // kolor wskaz minutowej
                    0,    0,   0.005,  1, //0.85,  // os sekundnika
                    QColor(150, 0, 0),   // kolor wskaz sekundnika

                    71,   157,  // pozycja srodka tarczy
                    0.25,  0.25,   // skalowanie zaślepki
                    50,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 50, 50),   // kolor AM_PM
                    QPoint(0, 180),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(230, 220, 220),   // kolor pełnej daty

                    QPoint(40, -1),    // pozycja datownika % z dlug promienia
                    25,   // szer okienka w % dlugosci promienia
                    20,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(40, 20, 20),   // kolor daty
                    QColor(0, 0, 0, 0),   // kolor ramki_daty
                    QColor(150, 150, 150, 0),   // kolor tla_daty
                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5      // uskok cienia
                }
                );
#endif

#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Green Marble",
                    ":/new/prefix1/content/greenmarble.png",
                    ":/new/prefix1/content/rombowe_godzinowa_czarna_plaska.png",
                    ":/new/prefix1/content/rombowe_minutowa_czarna_plaska.png",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.2,  0.06 ,  0.7, // os godzinowej, skala
                    QColor(100, 130, 120),   // kolor wskaz godzinowej
                    0.0,    0.25,   0.04,  1.05,// os minutowej
                    QColor(100, 130, 120),   // kolor wskaz minutowej
                    0,    0,   0.005,  1,  // os sekundnika
                    QColor(100, 130, 120),   // kolor wskaz sekundnika

                    140,   138,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    110,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)
                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 150, 80),   // kolor AM_PM
                    QPoint(0, 30),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(100, 130, 120),   // kolor pełnej daty

                    QPoint(40, 0),    // pozycja datownika % z dlug promienia
                    25,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100, 130, 120),   // kolor daty
                    QColor(100, 130, 120, 250),   // kolor ramki_daty
                    QColor(50, 50, 50, 50),   // kolor tla_daty
                    QColor(0, 0, 0, 80) ,  // kolor cienia
                    6      // uskok cienia
                }
                );
#endif  //

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Rammstein",
                    ":/new/prefix1/content/Ram_corr.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.0,   // os godzinowej, skala
                    0.04,   // szerokosc wskazowki
                    0.65,  // wzgledna dlugosc
                    QColor(10, 10, 210, 200),   // kolor wskaz godzinowej

                    0,    0,   0.03,  0.80,// os minutowej
                    QColor(10, 10, 10),   // kolor wskaz minutowej

                    0,    0,   0.005,  0.9,  // os sekundnika
                    QColor(140, 10, 10),   // kolor wskaz sekundnika

                    301,   301,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    290, // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -70),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 48),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(10, 10, 10, 120),   // kolor pełnej daty

                    QPoint(50, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(10, 10, 10, 120),   // kolor daty
                    QColor(40, 40, 40, 250),   // kolor ramki_daty
                    QColor(0, 0, 0, 10),   // kolor tla_daty
                    QColor(0, 0, 0, 80) ,  // kolor cienia
                    2 ,    // uskok cienia
                    QColor(25,25,25,200)    //  tlo_AM_PM_
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Wall Clock.png",
                    ":/new/prefix1/content/Wall Clock medium.png",
                    "0",
                    "0",
                    ":/new/prefix1/content/Wall-sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.15,  0.05 ,  0.55, // os godzinowej, skala
                    QColor(10, 10, 10),   // kolor wskaz godzinowej
                    0.0,    0.15,  0.04,  0.85,// os minutowej
                    QColor(10, 10, 10),   // kolor wskaz minutowej
                    0,    0.15,   0.005,  0.85,  // os sekundnika
                    QColor(80, 10, 10),   // kolor wskaz sekundnika

                    79,  79,  // pozycja srodka tarczy
                    0.12,  0.12,   // skalowanie zaślepki
                    75,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -50),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, -20),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(10, 10, 10),   // kolor pełnej daty

                    QPoint(30, -3),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(10, 10, 10),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(0, 0, 0, 10) ,
                    QColor(0, 0, 0, 50) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Banner clock",
                    ":/new/prefix1/content/banner-1240822.png",
                    "0",            //                    ":/new/prefix1/content/arnehour_pion.png",
                    "0",            // ":/new/prefix1/content/arneminute_pion.png",

                    "2",     //":/new/prefix1/content/Wall-sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.15,  0.06 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.04,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej
                    0,    0.15,   0.01,  1,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    468,  163,  // pozycja srodka tarczy
                    0.4,  0.4,   // skalowanie zaślepki
                    120,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(100, 255, 100),   // kolor AM_PM
                    QPoint(90, 120),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(100, 255, 100),   // kolor pełnej daty

                    QPoint(50, 0),    // pozycja datownika % z dlug promienia
                    40,   // szer okienka w % dlugosci promienia
                    16,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100, 250, 250),   // kolor daty
                    QColor(0, 0, 200, 150),   // kolor ramki_daty
                    QColor(0, 0, 0, 100),   // kolor tla_daty
                    QColor(0, 0, 0, 200) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //-------------------------------------------------
#if  0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Earth luminous",
                    ":/new/prefix1/content/round-luminous-earth-modern.png",
                    ":/new/prefix1/content/neon_wskazowka.png",
                    ":/new/prefix1/content/neon_wskazowka.png",
                    ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.05,  0.06 ,  0.7, // os godzinowej, skala
                    QColor(0, 0, 0, 200),   // kolor wskaz godzinowej
                    0.0,    0.05,   0.04,  0.85,// os minutowej
                    QColor(0, 0, 0, 200),   // kolor wskaz minutowej
                    0,    0.15,   0.005,  0.85,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    208,  153,  // pozycja srodka tarczy
                    0.12,  0.12,   // skalowanie zaślepki
                    150,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(0, 0, 50),   // kolor AM_PM
                    QPoint(0, -55),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(0, 0, 50),   // kolor pełnej daty

                    QPoint(9999, 45),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0, 0, 50),   // kolor daty
                    QColor(0, 0, 200, 30),   // kolor ramki_daty
                    QColor(10, 100, 255, 30),   // kolor tla_daty
                    QColor(0, 0, 0, 75) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //-------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Spacetime",
                    ":/new/prefix1/content/spacetime.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,
                    0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.03,   // grubosc w skali promienia tarczy
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0,
                    0.1,
                    0.03,
                    0.85,// os minutowej

                    QColor(0, 0, 50),

                    //                    0.0,    0,   0.005,  0.8,  // os sekundnika
                    0.0,    0,   0.01,  1,  // os sekundnika
                    QColor(200, 0, 0),
                    355,  200,  // pozycja srodka tarczy
                    0.8,  0.8,   // skalowanie zaślepki
                    70,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 130),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(250, 250, 250),   // kolor AM_PM
                    QPoint(150, -100),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250, 250, 255),   // kolor pełnej daty

                    QPoint(40, -2),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    16,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250, 250, 255),   // kolor daty
                    QColor(0, 0, 240, 180),   // kolor ramki_daty
                    QColor(10, 10, 130, 130),   // kolor tla_daty
                    QColor(0, 0, 0, 150) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

#endif // 0

    //-------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Tea-time",
                    ":/new/prefix1/content/tea-time.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.03 ,  0.6, // os godzinowej, skala
                    QColor(200, 50, 50, 250),   // kolor wskaz godzinowej
                    0.0,    0.1,   0.03,  0.75,// os minutowej
                    //                    QColor(10, 10, 50, 250),   // kolor wskaz minutowej
                    QColor(230, 230, 255, 250),   // kolor wskaz minutowej
                    0,    0.15,   0.005,  0.95,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    118,  118,  // pozycja srodka tarczy
                    0.8,  0.8,   // skalowanie zaślepki
                    118,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(70, -85),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(-40, 85),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(0, 0, 0, 200),   // kolor pełnej daty

                    QPoint(999, -70),    // pozycja datownika % z dlug promienia
                    40,   // szer okienka w % dlugosci promienia
                    11,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0, 0, 0, 200),   // kolor daty
                    QColor(0, 0, 0, 150),   // kolor ramki_daty
                    QColor(150, 150, 130, 200),   // kolor tla_daty
                    QColor(0, 0, 0, 150) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //-------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "delft-porzelan",
                    ":/new/prefix1/content/delft-porzelan.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.06 ,  0.7, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.05,  0.85,// os minutowej
                    QColor(20, 10, 10, 250),   // kolor wskaz minutowej

                    0,    0.15,   0.005,  1,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    180,  181,  // pozycja srodka tarczy
                    0.8,  0.8,   // skalowanie zaślepki
                    145,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(80, -100),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(-160, 115),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(54, 50, 37),   // kolor pełnej daty

                    QPoint(9999, -115),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0, 0, 30, 200),   // kolor daty
                    QColor(0, 0, 0, 150),   // kolor ramki_daty
                    QColor(214, 198, 146, 250),   // kolor tla_daty
                    QColor(0, 0, 0, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //-------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Blue clock",
                    ":/new/prefix1/content/clock-blue.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.03 ,  0.6, // os godzinowej, skala
                    QColor(200, 200, 200),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.04,  0.75,// os minutowej
                    QColor(200, 200, 200, 250),   // kolor wskaz minutowej

                    //  os_x_sekundnika;    os_y_sekundnika;   skala_x_sekundnika (szerokosc);    skala_y_sekundnika;
                    0,    0.15,   0.004,  0.90,  // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(150, 150, 230),   // kolor wskaz sekundnika

                    154, 154,  // pozycja srodka tarczy
                    0.8,  0.8,   // skalowanie zaślepki
                    140,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(200, 200, 200),   // kolor AM_PM
                    QPoint(0, 45),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200, 200, 200, 150),   // kolor pełnej daty

                    QPoint(50, -1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(200, 200, 200, 200),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(0, 0, 0, 10),   // kolor tla_daty
                    QColor(0, 0, 0, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //-------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Gold classic",
                    ":/new/prefix1/content/gold_KA.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.03 ,  0.6, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.02,  0.75,// os minutowej
                    QColor(20, 10, 10, 250),   // kolor wskaz minutowej

                    //0,    0.05,  0.02,  0.9,  // os sekundnika
                    0,    0.05,   0.005,  0.9,  //  // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    125, 123,  // pozycja srodka tarczy
                    0.2, 0.2, //0.8,  0.8,   // skalowanie zaślepki
                    110,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 32),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(54, 50, 37),   // kolor pełnej daty

                    QPoint(40, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0, 0, 30, 200),   // kolor daty
                    QColor(0, 0, 0, 10),   // kolor ramki_daty
                    QColor(214, 198, 146, 180),   // kolor tla_daty
                    QColor(0, 0, 0, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Silver classic",
                    ":/new/prefix1/content/silver.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.04 ,  0.7, // os godzinowej, skala x, y
                    QColor(0, 0, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.03,  0.88,// os minutowej
                    QColor(20, 10, 10, 250),   // kolor wskaz minutowej

                    0,    0.1,   0.003,  0.9,  // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika



                    125, 125,  // pozycja srodka tarczy
                    0.8,  0.8,   // skalowanie zaślepki
                    105,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 40),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(54, 50, 37),   // kolor pełnej daty

                    QPoint(9999, 65),    // pozycja datownika % z dlug promienia
                    38,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0, 0, 30, 200),                     // kolor daty
                    QColor(0, 0, 0, 00),                        // kolor ramki_daty
                    QColor(150, 150, 150, 50),              // kolor tla_daty
                    QColor(0, 0, 0, 100) ,                      // kolor cienia
                    5      // uskok cienia
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Roemische_mit_Glas",
                    ":/new/prefix1/content/roemische-Zahlen-mit-Glas-und-Luenette.png",
                    ":/new/prefix1/content/barok1_godzinowa_bevel_zloty_shining.png",
                    ":/new/prefix1/content/barok1_min_bev10.png",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.035 ,  0.6, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.03,  0.75,// os minutowej
                    QColor(20, 10, 10, 250),   // kolor wskaz minutowej

                    0,    0.15,   0.005,  0.85,   // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    150, 150,  // pozycja srodka tarczy
                    0.8,  0.8,   // skalowanie zaślepki
                    145,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(-50, 95),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(54, 50, 37),   // kolor pełnej daty

                    QPoint(30, -2),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0, 0, 30, 200),   // kolor daty
                    QColor(0, 0, 0, 10),   // kolor ramki_daty
                    QColor(214, 198, 146, 250),   // kolor tla_daty
                    QColor(0, 0, 0, 60) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    5      // uskok cienia
                }
                );

    //-------------------------------------------------
#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "clock-europe-africa",
                    ":/new/prefix1/content/clock-europe-africa.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.05 ,  0.7, // os godzinowej, skala
                    QColor(110, 110, 250),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.04,  0.9,// os minutowej
                    QColor(250, 250, 250, 190),   // kolor wskaz minutowej

                    0,    0.15,   0.005,  1,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    317, 189,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    120,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(200, 200, 200),   // kolor AM_PM
                    QPoint(0, 140),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250, 250, 250, 150),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250, 250, 250, 255),   // kolor daty
                    QColor(0, 0, 0, 150),   // kolor ramki_daty
                    QColor(0, 0, 0, 10),   // kolor tla_daty
                    QColor(50, 50, 150, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    2      // uskok cienia
                }
                );

#endif

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Chopin",
                    ":/new/prefix1/content/Chopin.png",
                    ":/new/prefix1/content/roman_hours_gold.png",
                    ":/new/prefix1/content/roman_minutgold_10.png",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.04 ,  0.6, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.04,  0.75,// os minutowej
                    QColor(20, 10, 10, 250),   // kolor wskaz minutowej

                    0,    0.15,   0.005,  0.9,   // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    150, 240,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    120,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 18),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 180),   // kolor AM_PM
                    QPoint(0, 105),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(220, 180, 0, 200),   // kolor pełnej daty

                    QPoint(35, 1),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(220, 180, 0, 200),   // kolor daty
                    QColor(0, 0, 0, 150),   // kolor ramki_daty
                    QColor(50, 0, 0, 250),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4,     // uskok cienia
                    QColor(50,50,0,200)    //  tlo_AM_PM_
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Quarz-black",
                    ":/new/prefix1/content/quarz_black.png",
                    ":/new/prefix1/content/agrafa_metal-h.png",
                    ":/new/prefix1/content/agrafa_metal-m.png",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.05 ,  0.6, // os godzinowej, skala
                    QColor(190, 190, 190),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.04,  0.75,// os minutowej
                    QColor(190, 190, 190),   // kolor wskaz minutowej

                    0,    0.15,   0.005,  1,   // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    150, 150,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    120,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 15),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 110),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(220, 180, 0, 200),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(220, 180, 0, 200),   // kolor daty
                    QColor(0, 0, 0, 250),   // kolor ramki_daty
                    QColor(50, 0, 0, 250),   // kolor tla_daty
                    QColor(150, 150, 150, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4     // uskok cienia
                }
                );


#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Wooden clock",
                    ":/new/prefix1/content/wooden-clock.png",
                    ":/new/prefix1/content/drewno_godzinowa_C_buk.png",
                    ":/new/prefix1/content/drewno_godzinowa_C_buk.png",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.06 ,  0.7, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.04,  0.95,// os minutowej
                    QColor(20, 10, 10, 250),   // kolor wskaz minutowej

                    0,    0.15,   0.01,  1.0,  // os sekundnika
                    QColor(250, 250, 0),   // kolor wskaz sekundnika

                    158, 158,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    110,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(220, 180, 0),   // kolor AM_PM
                    QPoint(-130, 130),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250, 220, 0, 250),   // kolor pełnej daty

                    QPoint(45, 0),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250, 210, 0, 200),   // kolor daty
                    QColor(0, 0, 0, 250),   // kolor ramki_daty
                    QColor(50, 0, 0, 100),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    //     QColor(255, 255, 255) ,  // kolor cienia
                    4     // uskok cienia
                }
                );
#endif  // 0
#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Rolex-datejust",
                    ":/new/prefix1/content/zifferblatt-uhr-rolex.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,  0.06 ,  0.55, // os godzinowej, skala
                    QColor(50, 20, 0),   // kolor wskaz godzinowej

                    0.0,    0.1,   0.05,  0.75,// os minutowej
                    QColor(40, 20, 10, 250),   // kolor wskaz minutowej

                    0,    0.15,   0.01,  0.9,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    192, 225,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    140,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(250,220,0,250),   // kolor AM_PM
                    QPoint(0, 115),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor (250,220,0,250),   // kolor pełnej daty

                    QPoint(50, 0),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250,250,0,250),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(60, 30, 0, 250),   // kolor tla_daty
                    QColor(0, 0, 0, 150) ,  // kolor cienia

                    4     // uskok cienia
                }
                );
#endif

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Seiko-grey",
                    ":/new/prefix1/content/seiko_grey.png",
                    "0",
                    "0",
                    "2",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.03,   // grubosc w skali promienia tarczy
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0,   0.1,   0.03,  0.85,// os minutowej
                    QColor(0, 0, 50),

                    0,    0.15,   0.005,  1,  // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 177,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    150,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 45),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(10, 10, 0, 250),   // kolor pełnej daty

                    QPoint(63, -10),    // pozycja datownika % z dlug promienia
                    25,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(10,10,0,250),   // kolor daty
                    QColor(0, 0, 0, 10),   // kolor ramki_daty
                    QColor(50, 0, 0, 20),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4,     // uskok cienia
                    QColor(250,250,250,0)    //  tlo_AM_PM_
                }
                );

#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Clock_face_India",
                    ":/new/prefix1/content/Clock_face_India.png",
                    "0",
                    "0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.03,   // grubosc w skali promienia tarczy
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0,   0.1,   0.03,  0.85,// os minutowej
                    QColor(0, 100, 250),

                    0,    0.15,   0.01,  1,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 173,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    150,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)


                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(250,250,190),   // kolor AM_PM
                    QPoint(0, 50),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250,250,190, 250),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250,250,190, 250),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(50, 0, 0, 50),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4     // uskok cienia
                }
                );
#endif

#if 0

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Clock_face_Decorative",
                    ":/new/prefix1/content/Clock_face_Decorative.png",
                    "0","0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.05,   // grubosc w skali promienia tarczy
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(100, 100, 250),

                    0.0,   0.1,   0.03,  0.85,// os minutowej
                    QColor(200, 200, 250),

                    0,    0.15,   0.015,  0.9,  // os sekundnika
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    115, 112,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    95,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 240),   // kolor AM_PM
                    QPoint(0, 50),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200, 200, 255),   // kolor pełnej daty

                    QPoint(37, 1),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(200,190, 255),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(50, 0, 0, 10),   // kolor tla_daty
                    QColor(100, 100, 100, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4     // uskok cienia
                }
                );
#endif


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Clock_face_Modern",
                    ":/new/prefix1/content/Clock_face_Modern.png",
                    "0","0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.03,   // grubosc w skali promienia tarczy
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0,   0.1,   0.03,  0.85,// os minutowej
                    QColor(0, 0, 50),

                    0,    0.15,   0.005,  1,   // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    112, 112,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    95,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 25),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, -40),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(10, 10, 0, 250),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    36,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(10,10,0,250),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(50, 0, 0, 50),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4     // uskok cienia
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Clock_face_Paris2",
                    ":/new/prefix1/content/Clock_face_Paris2.png",
                    "0","0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.06,   // grubosc w skali promienia tarczy
                    0.65,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0,   0.1,   0.04,  0.85,// os minutowej
                    QColor(120, 50, 00),

                    0,    0.15,   0.005,  1,  // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    161, 162,  // pozycja srodka tarczy
                    1,  1,   // skalowanie zaślepki
                    156,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(70, -90),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(200, 150, 40),   // kolor AM_PM
                    QPoint(-40, 90),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200, 200, 100, 250),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(230,230,210,255),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(50, 0, 0, 250),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4 ,    // uskok cienia
                    QColor(50, 0, 0, 250)    //  tlo_AM_PM_
                }
                );



    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Neon-modern",
                    ":/new/prefix1/content/neon.png",
                    ":/new/prefix1/content/neon_wskazowka.png",
                    ":/new/prefix1/content/neon_wskazowka.png",
                    ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.1,// XY mimosrod wektorowej wskazowki godzinowej
                    0.04,   // grubosc w skali promienia tarczy
                    0.65,// wysokosc  w skali promienia tarczy
                    QColor(0, 220, 255),

                    0.0,   0.1,   0.04,  0.8,// os minutowej
                    QColor(0, 220, 255),

                    0,    0.19,   0.005,  0.9,  // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    170, 174,  // pozycja srodka tarczy
                    0.3,  0.3,   // skalowanie zaślepki
                    140,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 45),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(0,200,250,250),   // kolor AM_PM
                    QPoint(0, 117),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(0, 200, 250, 250),   // kolor pełnej daty

                    QPoint(45, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0,200,250,250),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(50, 0, 0, 50),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4 ,    // uskok cienia
                    QColor(250,250,250,0)    //  tlo_AM_PM_
                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Galaxy",
                    ":/new/prefix1/content/galaxy.png",
                    "0","0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.15,  0.04 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.03,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.19,   0.005,  1,   // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 175,  // pozycja srodka tarczy
                    0.6,  0.6,   // skalowanie zaślepki
                    150,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 75),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(250, 200, 80),   // kolor AM_PM
                    QPoint(0, 100),    // pozycja pełnej daty ( % z dlug promienia)
                    //                    QColor(0, 200, 250, 250),   // kolor pełnej daty
                    QColor(200, 200, 100, 150),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(0,250,250,250),   // kolor daty
                    QColor(0, 0, 0, 250),   // kolor ramki_daty
                    QColor(50, 0, 0, 250),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4,     // uskok cienia
                    QColor(250,250,250,50)    //  tlo_AM_PM_
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Ocean_nordic",
                    ":/new/prefix1/content/ocean_nordic.png",
                    "0","0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.15,  0.04 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.03,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.19,   0.005,  1,   // os sekundnika x, y, skala  x (szerokosc), y (dlugosc)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    150, 152,  // pozycja srodka tarczy
                    0.3,  0.3,   // skalowanie zaślepki
                    135,  // promien_ tarczy
                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 45),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(0, 40, 0),   // kolor AM_PM
                    QPoint(0, 100),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200, 200, 250, 250),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    25,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100,250,100),   // kolor daty
                    QColor(0, 0, 0, 10),   // kolor ramki_daty
                    QColor(0, 40, 30, 150),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4,     // uskok cienia
                    QColor(250,250,250,100)    //  tlo_AM_PM_
                }
                );
#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "mechanic",
                    ":/new/prefix1/content/mechanic.png",
                    "0","0",
                    "0",
                    ":/new/prefix1/content/center.png",

                    0,     0.15,  0.07 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.05,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.045,  // grubosc sekundnika
                    1.3,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    177, 108,  // pozycja srodka tarczy
                    0.3,  0.3,   // skalowanie zaślepki
                    60,  // promien_ tarczy

                    0, 113,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 150),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(0, 260),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250, 100, 100),   // kolor pełnej daty

                    QPoint(140, 110),    // pozycja datownika % z dlug promienia
                    40,   // szer okienka w % dlugosci promienia
                    25,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(250, 50, 50),   // kolor daty
                    QColor(100, 100, 100),   // kolor ramki_daty
                    QColor(0, 0, 0, 150),   // kolor tla_daty
                    QColor(50, 50, 50, 100) ,  // kolor cienia
                    6     // uskok cienia
                }
                );
#endif

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Seamaster",
                    //                    ":/new/prefix1/content/Seamaster.png",
                    // ":/new/prefix1/content/Seamaster_mask_mniejszy.png",
                    ":/new/prefix1/content/Seamaster_bez_ramy.png",
                    ":/new/prefix1/content/Omega-hours.png",
                    ":/new/prefix1/content/Omega-minutes.png",
                    ":/new/prefix1/content/Omega-seconds.png",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.20,  0.07 ,  0.65, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.05,  0.9,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.005,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    196, 201,  // pozycja srodka tarczy
                    0.03,  0.03,   // skalowanie zaślepki
                    150,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -80),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(230, 230, 230),   // kolor AM_PM
                    QPoint(0, 122),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(250,200,0),   // kolor pełnej daty

                    QPoint(-5, 83),    // pozycja datownika % z dlug promienia
                    15,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(30,150,30),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(0, 0, 0, 0),   // kolor tla_daty
                    QColor(40, 40, 40, 150) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4,     // uskok cienia
                    QColor(20,25,25,200)    //  tlo_AM_PM_
                }
                );

    //    cyferblat.back().dodaj_extra_wskazowke(
    //                &  cyferblat.back().wsk_world,
    //                zestaw_wskazowek_dodatkowych::tarcza_world,
    //                0,  // x pozycja osi  na tarczy
    //                0, // y pozycja osi  na tarczy
    //                "noname",
    //                ":/new/prefix1/content/Omega-Seamaster-WorldCircle.png",
    //                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
    //                0, //  os_y;  mimosrod       // w %
    //                0.01, // skala_x (grubosc dla wsk wektorowej),
    //                0.75, // % dlugosc wsk (wekt i bitmapowej)
    //                &pix_wskazowka_world ,
    //                QColor(255, 0, 0),  //  QColor kolor;
    //                false //  flag_bitmapa_moze_byc_kolorowana;
    //                );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_night_day,
                Rola_wskazowki::tarcza_night_day,
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/Omega-Seamaster-WorldCircle2.png",
                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.88, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_night_day ,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                0, // bez cienia
                1, // warstwa


                // kat_obrotu = 15.0 * ((utc_hour + utc_minute / 60.0)) + 0;
                // lambda
                [this] () {
        double wynik =  15.0 * (this->now_time.hour() + this->now_time.minute() / 60.0) + 0;
        //        cout << "lambda obliczenie kata dla zones " << " = " << wynik
        //             << ", now_utc_hour " << now_utc_hour
        //             << ", now_utc_minute " << now_utc_minute
        //             << ", ile_minut_offsetu_DST " << ile_minut_offsetu_DST
        //             << endl;
        return wynik;
    }
    );


    //    cyferblat.back().dodaj_extra_wskazowke(
    //                &  cyferblat.back().wsk_zones,
    //                Rola_wskazowki::tarcza_zones,
    //                -1,  // x pozycja osi  na tarczy
    //                -1, // y pozycja osi  na tarczy
    //                "Seamaster_zones3",
    //                ":/new/prefix1/content/Seamaster_zones3.png",
    //                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
    //                0, //  os_y;  mimosrod       // w %
    //                0.01, // skala_x (grubosc dla wsk wektorowej),
    //                1.50, // % dlugosc wsk (wekt i bitmapowej)
    //                &pix_wskazowka_zones ,
    //                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza
    //                false, //  flag_bitmapa_moze_byc_kolorowana
    //                0, // bez cienia
    //                -1 // poniżej tarczy
    //                ,             [this] () {
    //        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
    //        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
    //        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
    //        return wynik;
    //    }
    //    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_zones,
                Rola_wskazowki::tarcza_zones,
                0,  // x pozycja osi  na tarczy
                -1, // y pozycja osi  na tarczy
                "Seamaster_zones_north",
                ":/new/prefix1/content/Seamaster_zones_north.png",
                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                1.57,  //1.25, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_zones ,
                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza
                false, //  flag_bitmapa_moze_byc_kolorowana
                0, // bez cienia
                -1 // poniżej tarczy
                ,             [this] () {
        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
        return wynik;
    }
    );



    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "World_rowki",

                    ":/new/prefix1/content/zegar_world_sam_cyferblat_rowkiM.png",
                    ":/new/prefix1/content/agrafa_godz.png",
                    ":/new/prefix1/content/agrafa_min.png",
                    ":/new/prefix1/content/Omega-seconds.png",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.20,  0.07 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.05,  0.9,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    1+(357/2), 1+(357/2),  // pozycja srodka tarczy
                    0.03,  0.03,   // skalowanie zaślepki
                    120,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(60, -140),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(20, 20, 20),   // kolor AM_PM
                    QPoint(0, 155),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200,100,0),   // kolor pełnej daty

                    QPoint(9999, 77),    // pozycja datownika % z dlug promienia
                    15,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100,250,100),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(0, 40, 30, 150),   // kolor tla_daty
                    QColor(40, 40, 40, 150) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4 ,    // uskok cienia
                    QColor(250,250,250,160)    //  tlo_AM_PM_
                }
                );



    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_night_day,
                Rola_wskazowki::tarcza_night_day,
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "zegar_world_24h_grey",
                //                ":/new/prefix1/content/Omega-Seamaster-WorldCircle2.png",
                ":/new/prefix1/content/zegar_world_24h_grey.png",
                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                1.58, // % dlugosc wsk (wekt i bitmapowej)
                //                1.525, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_night_day ,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                3, // bez cienia
                3, // warstwa


                // kat_obrotu = 15.0 * ((utc_hour + utc_minute / 60.0)) + 0;
                // lambda
                [this] () {
        double wynik =  15.0 * (this->now_time.hour() + this->now_time.minute() / 60.0) + 0;
        //        cout << "lambda obliczenie kata dla zones " << " = " << wynik
        //             << ", now_utc_hour " << now_utc_hour
        //             << ", now_utc_minute " << now_utc_minute
        //             << ", ile_minut_offsetu_DST " << ile_minut_offsetu_DST
        //             << endl;
        return wynik;
    }
    );




    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_zones,
                Rola_wskazowki::tarcza_zones,
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "zegar_world_zones_i_mapa",   // nazwa cywilna, poniżej nazwa pliku

                //       ":/new/prefix1/content/zegar_world_zones_big_black.png",
                ":/new/prefix1/content/world_cities.png",

                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                //                1.95, // % dlugosc wsk (wekt i bitmapowej)
                1.93, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_zones ,
                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza (?)
                false, //  flag_bitmapa_moze_byc_kolorowana
                0, // bez cienia
                1 // warstwa tarczy
                ,             [this] () {
        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
        return wynik;
    }
    );



    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,   // oszustwo
                Rola_wskazowki::wsk_miesiaca,  // oszustwo
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "Azimuthal_projection_SW",
                ":/new/prefix1/content/Azimuthal_projection_SW.png",
                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                1, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,   // oszustwo
                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza
                false, //  flag_bitmapa_moze_byc_kolorowana
                0, // uskok cienia
                4 // warstwa  tarczy
                ,             [this] () {
        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
        return wynik;
    }
    );

    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "world_globus",
                    //                    ":/new/prefix1/content/cyferblat_glob.png",
                    ":/new/prefix1/content/glob_golden.png",
                    ":/new/prefix1/content/agrafa_godz.png",
                    ":/new/prefix1/content/agrafa_min.png",
                    ":/new/prefix1/content/Omega-seconds.png",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.20,  0.07 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.05,  0.9,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    0.9,  // dlugosc sekundnika (w n% promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    //                   419/2, 421/2,  // pozycja srodka tarczy
                    760/2, 760/2,  // pozycja srodka tarczy
                    0.03,  0.03,   // skalowanie zaślepki
                    //                    240/2,  // promien_ tarczy
                    230,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(120, -140),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(0, 0, 30),   // kolor AM_PM
                    QPoint(-100, -155),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(100,250,100, 250),   // kolor pełnej daty

                    QPoint(9999, 77),    // pozycja datownika % z dlug promienia
                    15,   // szer okienka w % dlugosci promienia
                    8,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100,250,100),   // kolor daty
                    QColor(0, 0, 0, 50),   // kolor ramki_daty
                    QColor(0, 40, 30, 150),   // kolor tla_daty
                    QColor(40, 40, 40, 150) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4,     // uskok cienia
                    QColor(250,250,250,160)    //  tlo_AM_PM_
                }
                );



    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_night_day,
                Rola_wskazowki::tarcza_night_day,
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "zegar_world_24h_band_outline",
                //                ":/new/prefix1/content/Omega-Seamaster-WorldCircle2.png",
                ":/new/prefix1/content/zegar_world_24h_grey.png",
                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),

                //                1.2, // n% dlugosc wsk (wekt i bitmapowej)
                //                1.65, // n% dlugosc wsk (wekt i bitmapowej)
                1.7, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_night_day ,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                4, // bez cienia
                3, // warstwa


                // kat_obrotu = 15.0 * ((utc_hour + utc_minute / 60.0)) + 0;
                // lambda
                [this] () {
        double wynik =  15.0 * (this->now_time.hour() + this->now_time.minute() / 60.0) + 0;
        //        cout << "lambda obliczenie kata dla zones " << " = " << wynik
        //             << ", now_utc_hour " << now_utc_hour
        //             << ", now_utc_minute " << now_utc_minute
        //             << ", ile_minut_offsetu_DST " << ile_minut_offsetu_DST
        //             << endl;
        return wynik;
    }
    );




    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_zones,
                Rola_wskazowki::tarcza_zones,
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "zegar_world_zones_i_mapa",
                ":/new/prefix1/content/world_cities.png",
                //                 ":/new/prefix1/content/zegar_world_zones_big_black.png",

                //                ":/new/prefix1/content/zegar_world_zones_transparentD.png",

                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                2.1, // % dlugosc wsk (wekt i bitmapowej)
                //                    1.60, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_zones ,
                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza (?)
                false, //  flag_bitmapa_moze_byc_kolorowana
                0, // bez cienia
                1 // warstwa tarczy
                ,             [this] () {
        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
        return wynik;
    }
    );


    //    cyferblat.back().dodaj_extra_wskazowke(
    //                &  cyferblat.back().wsk_zones,
    //                Rola_wskazowki::tarcza_zones,
    //                0,  // x pozycja osi  na tarczy
    //                0, // y pozycja osi  na tarczy
    //                "zegar_world_zones_i_mapa",
    ////                ":/new/prefix1/content/zegar_world_zones_i_mapa.png",
    //                ":/new/prefix1/content/zegar_world_zones_transparentD.png",

    //                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
    //                0, //  os_y;  mimosrod       // w %
    //                0.01, // skala_x (grubosc dla wsk wektorowej),
    //                1.90, // % dlugosc wsk (wekt i bitmapowej)
    ////                    1.60, // % dlugosc wsk (wekt i bitmapowej)
    //                &pix_wskazowka_zones ,
    //                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza (?)
    //                false, //  flag_bitmapa_moze_byc_kolorowana
    //                0, // bez cienia
    //                1 // warstwa tarczy
    //                ,             [this] () {
    //        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
    //        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
    //        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
    //        return wynik;
    //    }
    //    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,   // oszustwo
                Rola_wskazowki::wsk_miesiaca,  // oszustwo
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "Azimuthal_projection_SW",
                ":/new/prefix1/content/Azimuthal_projection_SW.png",
                0, // os_x    mimosrod       // w %  // uwaga, dla bitmap, to jest cechą samej wskazowki!!
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                1.17, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,   // oszustwo
                QColor(0, 0, 0),  //  QColor kolor czarny oznacza ze chcemy wyswietlic pod tarcza
                false, //  flag_bitmapa_moze_byc_kolorowana
                0, // uskok cienia
                4 // warstwa  tarczy
                ,             [this] () {
        double wynik =  15.0 * (( (-ile_minut_offsetu_DST+  this->now_minutes_from_UTC) / 60.0)) + 0;
        // ta tarcza jest nieruchoma. Zmienia ją tylko zmiana zone
        //cout << "lambda obliczenie kata dla zones " << wynik  << endl;
        return wynik;
    }
    );

    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Morgan",
                    ":/new/prefix1/content/Morgan_crop.png",
                    ":/new/prefix1/content/morgan_hours.png",
                    ":/new/prefix1/content/morgan_minutes.png",
                    ":/new/prefix1/content/Omega-seconds.png",   // uwaga: to jest zwykły sekundnik , więc jego warstwa nie jest definiowana specjalnie
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.10,  0.07 ,  0.65, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.19,   0.05,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.24,
                    0.02,  // grubosc sekundnika
                    0.25,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    162, 192,  // pozycja srodka tarczy  (162, 276,)
                    0.3,  0.3,   // skalowanie zaślepki
                    140,  // promien_ tarczy

                    0, 68,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(-45, 14),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(100,200,100),   // kolor AM_PM
                    //                    QPoint(0, -25),    // pozycja pełnej daty ( % z dlug promienia)
                    QPoint(0, 120),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200, 200, 50, 250),   // kolor pełnej daty
                    //                    QColor(0, 0, 0),   // kolor pełnej daty


                    QPoint(35, 14),    // pozycja datownika % z dlug promienia
                    40,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100,200,100),   // kolor daty
                    QColor(110, 110, 110, 50),   // kolor ramki_daty
                    QColor(60, 40, 20, 50),   // kolor tla_daty
                    QColor(0, 0, 0, 180) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4 ,    // uskok cienia
                    QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );
    //--------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Aviator",
                    ":/new/prefix1/content/Morgan_Gard.png",
                    ":/new/prefix1/content/morgan_hours.png",
                    ":/new/prefix1/content/morgan_minutes.png",
                    ":/new/prefix1/content/Omega-seconds.png",  // uwaga: to jest zwykły sekundnik , więc jego warstwa nie jest definiowana specjalnie
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.10,  0.07 ,  0.65, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.19,   0.05,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.24,
                    0.02,  // grubosc sekundnika
                    0.25,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    162, 192,  // pozycja srodka tarczy  (162, 276,)
                    0.3,  0.3,   // skalowanie zaślepki
                    140,  // promien_ tarczy

                    0, 68,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(-45, 14),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(100,200,100),   // kolor AM_PM
                    //                    QPoint(0, -25),    // pozycja pełnej daty ( % z dlug promienia)
                    QPoint(0, 120),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(200, 200, 250, 250),   // kolor pełnej daty
                    //                    QColor(0, 0, 0),   // kolor pełnej daty


                    QPoint(35, 14),    // pozycja datownika % z dlug promienia
                    40,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100,200,100),   // kolor daty
                    QColor(110, 110, 110, 50),   // kolor ramki_daty
                    QColor(60, 40, 20, 50),   // kolor tla_daty
                    QColor(0, 0, 0, 180) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4 ,    // uskok cienia
                    QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );

#if 1
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "gear-wall-clocks",
                    ":/new/prefix1/content/gear-wall-clocks.png",
                    "0","0",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.15,  0.07 ,  0.7, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.15,   0.05,  0.95,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.15,
                    0.02,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    176, 180,  // pozycja srodka tarczy
                    0.3,  0.3,   // skalowanie zaślepki
                    150,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 20),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(250, 250, 250),   // kolor AM_PM
                    QPoint(0, 50),    // pozycja pełnej daty ( % z dlug promienia)
                    QColor(255, 200, 50),   // kolor pełnej daty

                    QPoint(35, 14),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(100,200,100),   // kolor daty
                    QColor(110, 110, 110, 50),   // kolor ramki_daty
                    QColor(60, 40, 20, 150),   // kolor tla_daty
                    QColor(50, 50, 50, 150) ,  // kolor cienia
                    //                       QColor(255, 255, 255) ,  // kolor cienia
                    4     // uskok cienia
                }
                );
#endif

    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "IWC_Schaffhausen",
                                      ":/new/prefix1/content/iwc_schaffhausen_short.png",
                                      ":/new/prefix1/content/iwc_hours.png",
                                      ":/new/prefix1/content/iwc_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.115,
                                      0.01 ,  0.6, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06,
                                      0.01,  0.9,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      0.18,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      167, 205,  // pozycja srodka tarczy    // 167, 293,  // pozycja srodka tarczy
                                      0.005,  0.005,   // skalowanie zaślepki
                                      150,  // promien_ tarczy

                                      -77, -1,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.62, // pozycja księzyca
                                      0.12 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 65),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(120, 120, 120),   // kolor AM_PM
                                      QPoint(0, 125),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor pełnej daty

                                      QPoint(9999, 110),    // pozycja datownika % z dlug promienia
                                      30,   // szer okienka w % dlugosci promienia
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(200,100,0),   // kolor daty
                                      QColor(110, 110, 0, 10),   // kolor ramki_daty
                                      QColor(60, 15, 0, 240),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      3 ,    // uskok cienia
                                      QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -77,  // x pozycja osi  na tarczy
                -1, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.28, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) * (now_today.dayOfWeek());
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_miesiaca,
                Rola_wskazowki::wsk_dzien_miesiaca,
                77,  // x pozycja osi  na tarczy
                -1, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                2, // odl cienia
                0, // warstwa
                // lambda
                [this] () {
        double kat =
                (360.0/31) * (now_today.day() );
        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
        return kat;
    }
    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                0,  // x pozycja osi  na tarczy
                78, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik =
                (360.0/12) * (now_today.month() );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );

    //---------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "GWC_Graffhausen_skin",
                                      ":/new/prefix1/content/gwc_graffhausen_skin.png",
                                      ":/new/prefix1/content/iwc_hours.png",
                                      ":/new/prefix1/content/iwc_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.115,
                                      0.01 ,  0.6, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06,
                                      0.01,  1,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      167, 200,  // pozycja srodka tarczy    // 167, 293,  // pozycja srodka tarczy
                                      0.005,  0.005,   // skalowanie zaślepki
                                      139,  // promien_ tarczy

                                      0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.68, // pozycja księzyca
                                      0.11 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 65),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(120, 120, 120),   // kolor AM_PM
                                      QPoint(0, 125),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor pełnej daty

                                      QPoint(9999, 110),    // pozycja datownika % z dlug promienia
                                      30,   // szer okienka w % dlugosci promienia
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(200,100,0),   // kolor daty
                                      QColor(110, 110, 0, 10),   // kolor ramki_daty
                                      QColor(60, 15, 0, 240),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      3 ,    // uskok cienia
                                      QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -80,  // x pozycja osi  na tarczy
                -0, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.28, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) *
                (now_today.dayOfWeek());

        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_miesiaca,
                Rola_wskazowki::wsk_dzien_miesiaca,
                80,  // x pozycja osi  na tarczy
                -2, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                2, // odl cienia
                0, // warstwa
                // lambda
                [this] () {
        double kat =
                (360.0/31) *
                (now_today.day() -1);
        //              (now_time.second() % 31);
        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
        return kat;
    }
    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                0,  // x pozycja osi  na tarczy
                80, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik = -360/24 +
                (360.0/12) *
                (now_today.month() );
        //         (now_time.second() % 12);
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );

    //---------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "GWC_Graffhausen_metal",
                                      ":/new/prefix1/content/gwc_graffhausen_metal.png",
                                      ":/new/prefix1/content/iwc_hours.png",
                                      ":/new/prefix1/content/iwc_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.115,
                                      0.01 ,  0.6, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06,
                                      0.01,  1,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      167, 200,  // pozycja srodka tarczy    // 167, 293,  // pozycja srodka tarczy
                                      0.005,  0.005,   // skalowanie zaślepki
                                      139,  // promien_ tarczy

                                      0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.7, // pozycja księzyca
                                      0.11 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 65),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(120, 120, 120),   // kolor AM_PM
                                      QPoint(0, 125),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor pełnej daty

                                      QPoint(9999, 110),    // pozycja datownika % z dlug promienia
                                      30,   // szer okienka w % dlugosci promienia
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(200,100,0),   // kolor daty
                                      QColor(110, 110, 0, 10),   // kolor ramki_daty
                                      QColor(60, 15, 0, 240),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      3 ,    // uskok cienia
                                      QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -80,  // x pozycja osi  na tarczy
                -0, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.28, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) *
                (now_today.dayOfWeek());

        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_miesiaca,
                Rola_wskazowki::wsk_dzien_miesiaca,
                80,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                2, // odl cienia
                0, // warstwa
                // lambda
                [this] () {
        double kat =
                (360.0/31) *
                (now_today.day() -1);
        //              (now_time.second() % 31);
        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
        return kat;
    }
    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                0,  // x pozycja osi  na tarczy
                80, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik = -360/24 +
                (360.0/12) *
                (now_today.month() );
        //         (now_time.second() % 12);
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );
    //---------------------------------------------------------
#if 0
    //---------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "GWC_Graffhausen_dark",
                                      ":/new/prefix1/content/gwc_graffhausen_dark.png",
                                      ":/new/prefix1/content/iwc_hours.png",
                                      ":/new/prefix1/content/iwc_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.115,
                                      0.01 ,  0.6, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06,
                                      0.01,  1,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      166, 200,  // pozycja srodka tarczy
                                      0.5,  0.5,   // skalowanie zaślepki
                                      134,  // promien_ tarczy

                                      0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.7, // pozycja księzyca
                                      0.11 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 128),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor AM_PM
                                      QPoint(0, 142),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(240, 110, 0),   // kolor pełnej daty

                                      QPoint(9999, 110),    // pozycja datownika % z dlug promienia
                                      30,   // szer okienka w % dlugosci promienia
                                      12,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(200,100,0),   // kolor daty
                                      QColor(110, 110, 0, 150),   // kolor ramki_daty
                                      QColor(60, 15, 0, 240),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      3     // uskok cienia
                }
                );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -80,  // x pozycja osi  na tarczy
                -0, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.28, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) *
                (now_today.dayOfWeek());

        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_miesiaca,
                Rola_wskazowki::wsk_dzien_miesiaca,
                80,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                2, // odl cienia
                0, // warstwa
                // lambda
                [this] () {
        double kat =
                (360.0/31) *
                (now_today.day() -1);
        //              (now_time.second() % 31);
        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
        return kat;
    }
    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                0,  // x pozycja osi  na tarczy
                80, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.26, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik = -360/24 +
                (360.0/12) *
                (now_today.month() );
        //         (now_time.second() % 12);
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );
    //---------------------------------------------------------
#endif

    //---------------------------------------------------------

    //---------------------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Large-Kitchen-Wall-Clock",
                    ":/new/prefix1/content/Large-Kitchen-Wall-Clock.png",
                    ":/new/prefix1/content/rombowe_godzinowa_ciemna.png",
                    ":/new/prefix1/content/rombowe_minutowa_czarna.png",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.12,  0.07 ,  0.6, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.075,   0.05,  0.80,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.15,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    165, 277,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    120,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, -110),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(240, 120, 0),   // kolor AM_PM
                    QPoint(180, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 120, 0),   // kolor pełnej daty

                    QPoint(9999, -50),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,120,0),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(60, 0, 0, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 180) ,  // kolor cienia
                    5 ,    // uskok cienia

                    QColor(60, 0, 0, 240),  //  tlo_AM_PM_
                }
                );



    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "rzymski_chrom",
                    ":/new/prefix1/content/rzymski_chrom.png",
                    ":/new/prefix1/content/rombowe_godzinowa_ciemna.png",
                    ":/new/prefix1/content/rombowe_minutowa_czarna.png",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.12,  0.07 ,  0.5, // os godzinowej, skala
                    QColor(190, 255, 0),   // kolor wskaz godzinowej
                    0.0,    0.075,   0.05,  0.750,// os minutowej
                    QColor(255, 255, 0),   // kolor wskaz minutowej

                    0,    0.15,
                    0.01,  // grubosc sekundnika
                    0.9,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 175,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    155,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(-40, -0),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(240, 110, 0),   // kolor AM_PM
                    QPoint(40, -105),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 110, 0),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    25,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,120,0),   // kolor daty
                    QColor(110, 110, 0, 0),   // kolor ramki_daty
                    QColor(60, 0, 0, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    5  ,   // uskok cienia
                    QColor(240, 110, 0, 0)    //  tlo_AM_PM_
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "metal_chrom",
                    ":/new/prefix1/content/metal_chrom.png",
                    ":/new/prefix1/content/rombowe_godzinowa_ciemna.png",
                    ":/new/prefix1/content/rombowe_minutowa_czarna.png",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.12,  0.07 ,  0.7, // os godzinowej, skala
                    QColor(190, 230, 0),   // kolor wskaz godzinowej
                    0.0,    0.075,   0.05,  0.90,// os minutowej
                    QColor(15, 15, 15),   // kolor wskaz minutowej

                    0.0,    0.15,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 175,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    150,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(-40, -105),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(240, 120, 0),   // kolor AM_PM
                    QPoint(50, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 120, 0),   // kolor pełnej daty

                    QPoint(9999, -70),    // pozycja datownika % z dlug promienia
                    30,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,120,0),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(60, 0, 0, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 70) ,  // kolor cienia
                    5     // uskok cienia
                }
                );


    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Plum",
                    ":/new/prefix1/content/Clock_face_Plum.png",
                    "0","0",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,
                    0.23,// XY mimosrod wektorowej wskazowki godzinowej
                    0.03,   // grubosc w skali promienia tarczy
                    0.7,// wysokosc  w skali promienia tarczy
                    QColor(255, 0, 0, 255),

                    0.0, 0.1,
                    0.03,   0.85,// os minutowej

                    QColor(0, 0, 50),

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 175,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    160,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 35),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 180),   // kolor AM_PM
                    QPoint(30, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 240, 240),   // kolor pełnej daty

                    QPoint(25, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,240,240),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    //                     QColor(255, 255, 0),   // kolor ramki_daty
                    QColor(50, 50, 50, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 70) ,  // kolor cienia
                    5     // uskok cienia
                }
                );




#if 0
    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "roman black empty",
                    ":/new/prefix1/content/rzymskie.png",
                    "0","0",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.3,  0.07 ,  0.55, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej
                    0.0,    0.2,   0.05,  0.90,// os minutowej
                    QColor(0, 0, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 175,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    175,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 35),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(30, -90),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 240, 240),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    9,   // wys okienka i fontu  (w % dlugosci promienia)
                    QColor(240,240,240),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(50, 50, 50, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    5     // uskok cienia
                }
                );

#endif

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "roman_shadow_empty",
                    ":/new/prefix1/content/rzymskie_cien.png",
                    "0","0",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,  0.04 ,  0.7, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej
                    0.0,    0.23,   0.03,  0.90,// os minutowej
                    QColor(0, 0, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    180, 180,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    175,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 35),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(30, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 240, 240),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,240,240),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(50, 50, 50, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    5     // uskok cienia
                }
                );



    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "gold_metalic_roman",
                    ":/new/prefix1/content/radialny_rzymski.png",
                    "0","0",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.23,  0.07 ,  0.55, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej
                    0.0,    0.23,   0.05,  0.90,// os minutowej
                    QColor(0, 0, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    0.95,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 177,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    145,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(0, 45),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(80, 80, 80),   // kolor AM_PM
                    QPoint(20, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 240, 240),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,240,240),   // kolor daty
                    QColor(110, 110, 0, 100),   // kolor ramki_daty
                    QColor(50, 50, 50, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    5     // uskok cienia
                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Old Cathedral",
                    ":/new/prefix1/content/rom_dark_gold.png",
                    ":/new/prefix1/content/cebulasta_godzinowa_zlota.png",
                    ":/new/prefix1/content/cebulasta_minutowa_zlota.png",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.33,  0.07 ,  0.55, // os godzinowej, skala
                    QColor(200, 230, 150),   // kolor wskaz godzinowej
                    0.0,    0.25,   0.05,  0.90,// os minutowej
                    QColor(240, 230, 250),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    205, 188,  // pozycja srodka tarczy
                    0.5,  0.5,   // skalowanie zaślepki
                    150,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(80, 65),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 0),   // kolor AM_PM
                    QPoint(50, -115),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 40, 40),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,240, 0),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(50, 50, 50, 40),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    5,     // uskok cienia
                    QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Sailors",
                    ":/new/prefix1/content/zaglowiec.png",
                    ":/new/prefix1/content/cebulasta_godzinowa_zlota.png",
                    ":/new/prefix1/content/cebulasta_minutowa_zlota.png",
                    "0",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.33,  0.07 ,  0.65, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej
                    0.0,    0.25,   0.05,  0.95,// os minutowej
                    QColor(0, 0, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    1,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    175, 175,  // pozycja srodka tarczy
                    0.1,  0.1,   // skalowanie zaślepki
                    160,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(80, 90),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 180),   // kolor AM_PM
                    QPoint(30, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 240, 240),   // kolor pełnej daty

                    QPoint(35, 0),    // pozycja datownika % z dlug promienia
                    18,   // szer okienka w % dlugosci promienia
                    10,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,240,240),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(50, 50, 50, 140),   // kolor tla_daty
                    QColor(0, 0, 0, 120) ,  // kolor cienia
                    5     // uskok cienia
                }
                );




    cyferblat.push_back(
                Tdane_cyferblatu  {
                    "Modern-design",
                    ":/new/prefix1/content/Modern-Design.png",
                    ":/new/prefix1/content/Modern-Design-hours.png",
                    ":/new/prefix1/content/Modern-Design-minutes.png",
                    ":/new/prefix1/content/Modern-Design-seconds.png",
                    //  ":/new/prefix1/content/neon_sekundnik.png",
                    ":/new/prefix1/content/center.png",

                    0,     0.0,  0.07 ,  0.5, // os godzinowej, skala
                    QColor(0, 0, 0),   // kolor wskaz godzinowej
                    0.0,    0.25,   0.05,  0.65,// os minutowej
                    QColor(0, 0, 0),   // kolor wskaz minutowej

                    0,    0.1,
                    0.01,  // grubosc sekundnika
                    0.7,  // dlugosc sekundnika (w % promienia tarczy)
                    QColor(250, 10, 10),   // kolor wskaz sekundnika

                    173, 175,  // pozycja srodka tarczy
                    0.1,  0.1,   // skalowanie zaślepki
                    160,  // promien_ tarczy

                    0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                    QPoint(80, 90),    // pozycja AM_PM ( % z dlug promienia)
                    QColor(180, 180, 180),   // kolor AM_PM
                    QPoint(30, -100),    // pozycja pełnej daty ( % z dlug promienia)

                    QColor(240, 240, 240),   // kolor pełnej daty

                    QPoint(30, 0),    // pozycja datownika % z dlug promienia
                    20,   // szer okienka w % dlugosci promienia
                    12,   // wys okienka i fontu  (w % dlugosci promienia)

                    QColor(240,240,240),   // kolor daty
                    QColor(110, 110, 0, 150),   // kolor ramki_daty
                    QColor(50, 50, 50, 240),   // kolor tla_daty
                    QColor(0, 0, 0, 70) ,  // kolor cienia
                    3     // uskok cienia
                }
                );


    /*
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Lunascope",
                                      ":/new/prefix1/content/Lunascope.png",
                                      ":/new/prefix1/content/morgan_hours.png",
                                      ":/new/prefix1/content/morgan_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.1,
                                      0.04 ,  0.6, // os godzinowej, skala
                                      QColor(100, 100, 255),   // kolor wskaz godzinowej

                                      -0.0001,    0.1,
                                      0.03,  0.85,// os minutowej
                                      QColor(100, 100, 255),   // kolor wskaz minutowej

                                      0,    0.1,
                                      0.01,  // grubosc sekundnika
                                      0.90,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      170, 175,  // pozycja srodka tarczy
                                      0.5,  0.5,   // skalowanie zaślepki
                                      150,  // promien_ tarczy

                                      0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.4,  // pozycja księzyca
                                      0.28 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 150),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor AM_PM
                                      QPoint(0, 125),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor pełnej daty

                                      QPoint(-15, 55),    // pozycja datownika % z dlug promienia
                                      30,   // szer okienka w % dlugosci promienia
                                      12,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(200,100,0),   // kolor daty
                                      QColor(110, 110, 0, 150),   // kolor ramki_daty
                                      QColor(0, 0, 80, 240),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      3     // uskok cienia
                }
                );
*/
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Lunascope2",
                                      ":/new/prefix1/content/Lunascope2.png",
                                      ":/new/prefix1/content/morgan_hours.png",
                                      ":/new/prefix1/content/morgan_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.1,
                                      0.04 ,  0.6, // os godzinowej, skala
                                      QColor(100, 100, 255),   // kolor wskaz godzinowej

                                      -0.0001,    0.1,
                                      0.03,  0.9,// os minutowej
                                      QColor(100, 100, 255),   // kolor wskaz minutowej

                                      0,    0.1,
                                      0.01,  // grubosc sekundnika
                                      0.95,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      180, 187,  // pozycja srodka tarczy
                                      0.5,  0.5,   // skalowanie zaślepki
                                      157,  // promien_ tarczy

                                      0, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.4,  // pozycja księzyca
                                      0.28 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 60),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor AM_PM
                                      QPoint(0, 115),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(200, 160, 0),   // kolor pełnej daty

                                      QPoint(-7, 82),    // pozycja datownika % z dlug promienia
                                      12,   // szer okienka w % dlugosci promienia
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(0,150,255),   // kolor daty
                                      QColor(110, 110, 0, 50),   // kolor ramki_daty
                                      QColor(0, 20, 80, 240),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      5     // uskok cienia
                }
                );

    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Lunascope GrebMeister",
                                      ":/new/prefix1/content/Lunascope_greb.png",
                                      ":/new/prefix1/content/morgan_hours.png",
                                      ":/new/prefix1/content/morgan_minutes.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001,
                                      0.115,  // XY mimosrod wektorowej wskazowki
                                      0.01 ,  0.8, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06, // XY mimosrod wektorowej wskazowki
                                      0.01,  1.0, // 0.9,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.1,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      175, 200,  // pozycja srodka tarczy
                                      0.8,  0.8,   // skalowanie zaślepki
                                      145,  // promien_ tarczy

                                      -0, -0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, -0.49, // pozycja księzyca (w % promienia tarczy)
                                      0.35 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 70),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(100,100,20),   // kolor AM_PM
                                      QPoint(0, 128),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(50, 50, 200),   // kolor pełnej daty

                                      QPoint(41, -2),    // pozycja datownika % z dlug promienia
                                      15,   // szer okienka w % dlugosci promienia
                                      14,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(100,100,20),   // kolor daty
                                      QColor(110, 110, 110, 0),   // kolor ramki_daty
                                      QColor(0, 0, 60, 200),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      2  ,   // uskok cienia
                                      QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );


    //cout << __LINE__ << " <------TUTAS 0 ---------" << endl;
    //############################################

    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "ChronoSwiss",
                                      ":/new/prefix1/content/wtla-chronoswiss-cyferblat.png",
                                      ":/new/prefix1/content/wtla-chronoswiss-godziny.png",
                                      ":/new/prefix1/content/wtla-chronoswiss-minuty.png",
                                      ":/new/prefix1/content/wtla-chronoswiss-sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001, 0.115,
                                      0.01 ,  0.65, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06,
                                      0.01,  0.83,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      0.93,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika


                                      173, 228,  // pozycja srodka tarczy
                                      0.05,  0.05,   // skalowanie zaślepki
                                      150,  // promien_ tarczy

                                      0, 0,  // -70, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0.47, -0.14, // pozycja księzyca
                                      0.09 ,        // i wsp skali jego bitmapy

                                      QPoint(0, -37),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(220, 220, 220),   // kolor AM_PM
                                      QPoint(0, 125),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(180, 240, 240, 150),   // kolor pełnej daty

                                      QPoint(-15, 110),    // pozycja datownika % z dlug promienia
                                      0,   // szer okienka w % dlugosci promienia  // nie chcemy wyswietlac
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(200,100,0),   // kolor daty
                                      QColor(110, 110, 0, 0),   // kolor ramki_daty
                                      QColor(0, 0, 80, 200),   // kolor tla_daty
                                      QColor(0, 0, 0, 120) ,  // kolor cienia
                                      5,     // uskok cienia
                                      QColor(0,0,0,0)    //  tlo_AM_PM_
                }
                );

    //cout << __LINE__ << " <------TUTAS 1 ---------" << endl;

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_miesiaca,
                Rola_wskazowki::wsk_dzien_miesiaca,
                0,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/wtla-chronoswiss-days.png",
                0, // os_x    mimosrod       // w %
                0.05, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.97, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                3, // odl cienia
                1, // warstwa
                // lambda
                [this] () {
        double wynik =
                (360.0/31) * (now_today.day() );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_sekundnik_dodatkowy,
                Rola_wskazowki::wsk_sekundnik_dodatkowy,
                -69,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "extra: chronoswiss-sekundnik_maly",
                ":/new/prefix1/content/wtla-chronoswiss-sekundnik_maly.png",
                0, // os_x    mimosrod       // w %
                0, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.22, // % dlugosc wsk (wekt i bitmapowej)

                &pix_wskazowka_sekundnik_dodatkowy,


                QColor(255, 0, 0),  //  QColor k olor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                3, // odl cienia
                15,  // warstwa
                //
                // lambda dla SEKUNDNIKA !
                [this] () {
        double wynik = 6.0 * (this->now_time.second() );
        // cout << "lambda obliczenie kata dla malego sekundnika " << " = " << wynik  << endl;
        return wynik; }
    );




    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                0,  // x pozycja osi  na tarczy
                68, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/wtla-chronoswiss-miesiace.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda dla SEKUNDNIKA !
                [this] () {
        double wynik =
                (360.0/12) * (now_today.month() );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }

    );

    // NOWE ---------------
    //********************************************************************************************
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Chronometer",
                                      ":/new/prefix1/content/chronograph2.png",
                                      ":/new/prefix1/content/wtla-chronoswiss-godziny.png",
                                      ":/new/prefix1/content/wtla-chronoswiss-minuty.png",
                                      ":/new/prefix1/content/wtla-chronoswiss-sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001,  0.015,
                                      0.10 ,  0.72, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06,
                                      0.01,  0.88,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika


                                      189, 189,  // pozycja srodka tarczy
                                      0.05,  0.05,   // skalowanie zaślepki
                                      150,  // promien_ tarczy

                                      0, 0,  // -70, 0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0.50, -0.15, // pozycja księzyca
                                      0.09 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 38),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(250,250, 250),   // kolor AM_PM
                                      QPoint(0, 120),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(250, 40, 40, 190),   // kolor pełnej daty

                                      QPoint(-15, 110),    // pozycja datownika % z dlug promienia
                                      0,   // szer okienka w % dlugosci promienia  // nie chcemy wyswietlac
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(100,10,10),   // kolor daty
                                      QColor(110, 110, 0, 150),   // kolor ramki_daty
                                      QColor(0, 0, 40, 100),   // kolor tla_daty
                                      QColor(0, 0, 0, 120) ,  // kolor cienia
                                      5 ,    // uskok cienia,
                                      QColor(250,250,250,0)    //  tlo_AM_PM_
                }
                );


    cyferblat.back().dodaj_extra_wskazowke(  // Wielka księzycowata strzała z datami
                                             &  cyferblat.back().wsk_dni_miesiaca,
                                             Rola_wskazowki::wsk_dzien_miesiaca,
                                             0,  // x pozycja osi  na tarczy
                                             0, // y pozycja osi  na tarczy
                                             "noname",
                                             ":/new/prefix1/content/wtla-chronoswiss-days.png",
                                             0, // os_x    mimosrod       // w %
                                             0.05, //  os_y;  mimosrod       // w %
                                             0.01, // skala_x (grubosc dla wsk wektorowej),
                                             1.08, // % dlugosc wsk (wekt i bitmapowej)
                                             &pix_wskazowka_dni_miesiaca,
                                             QColor(255, 0, 0),  //  QColor kolor;
                                             false, //  flag_bitmapa_moze_byc_kolorowana;
                                             3, // odl cienia
                                             1, // warstwa
                                             // lambda
                                             [this] () {
        double wynik =
                (360.0/31) * (now_today.day() );
        //           (360.0/31) * (this->now_time.second() );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );

    cyferblat.back().dodaj_extra_wskazowke(   // SEKUNDNIK DODATKOWY
                                              &  cyferblat.back().wsk_sekundnik_dodatkowy,
                                              Rola_wskazowki::wsk_sekundnik_dodatkowy,
                                              -78,  // x pozycja osi  na tarczy
                                              0, // y pozycja osi  na tarczy
                                              "extra: chronoswiss-sekundnik_maly",
                                              ":/new/prefix1/content/wtla-chronoswiss-sekundnik_maly.png",
                                              0, // os_x    mimosrod       // w %
                                              0, //  os_y;  mimosrod       // w %
                                              0.01, // skala_x (grubosc dla wsk wektorowej),
                                              0.22, // % dlugosc wsk (wekt i bitmapowej)

                                              &pix_wskazowka_sekundnik_dodatkowy,


                                              QColor(255, 0, 0),  //  QColor k olor;
                                              false, //  flag_bitmapa_moze_byc_kolorowana;
                                              3, // odl cienia
                                              15,  // warstwa
                                              //
                                              // lambda dla SEKUNDNIKA !
                                              [this] () {
        double wynik = 6.0 * (this->now_time.second() );
        // cout << "lambda obliczenie kata dla malego sekundnika " << " = " << wynik  << endl;
        return wynik; }
    );




    cyferblat.back().dodaj_extra_wskazowke(    // NAZWA  MIESIACA
                                               &  cyferblat.back().wsk_miesiaca,
                                               Rola_wskazowki::wsk_miesiaca,
                                               0,  // x pozycja osi  na tarczy
                                               73, // y pozycja osi  na tarczy
                                               "noname",
                                               ":/new/prefix1/content/wtla-chronoswiss-miesiace.png",
                                               0, // os_x    mimosrod       // w %
                                               0.12, //  os_y;  mimosrod       // w %
                                               0.01, // skala_x (grubosc dla wsk wektorowej),
                                               0.2, // % dlugosc wsk (wekt i bitmapowej)
                                               &pix_wskazowka_miesiaca,
                                               QColor(255, 0, 0),  //  QColor kolor;
                                               false,   //  flag_bitmapa_moze_byc_kolorowana;
                                               1, // odl cienia
                                               0,          // warstwa

                                               // lambda dla SEKUNDNIKA !
                                               [this] () {
        double wynik =
                (360.0/12) * (now_today.month() );
        return wynik;
    }
    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,   //MONDAY, TUESDAY...
                Rola_wskazowki::wsk_full_date,
                0,  // x pozycja osi  na tarczy
                -77, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/wtla-chronoswiss-miesiace.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda dla wsk _dni_tygodnia
                [this] () {
        double wynik =
                (360.0/7) * (now_today.dayOfWeek());
        return wynik ; }


    );
#if 1
    //-------------------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Geizmont",
                                      ":/new/prefix1/content/geizmont.png",
                                      ":/new/prefix1/content/geizmont_wsk_hours_h.png",
                                      ":/new/prefix1/content/geizmont_wsk_minut_h.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001,
                                      0.115,  // XY mimosrod wektorowej wskazowki
                                      0.01 ,  0.65, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06, // XY mimosrod wektorowej wskazowki
                                      0.01,  0.9,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.1,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      216, 249,  // pozycja srodka tarczy
                                      0.5,  0.5,   // skalowanie zaślepki
                                      163,  // promien_ tarczy

                                      -0, -0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, 0.43, // pozycja księzyca (w % promienia tarczy)
                                      0.2 ,        // i wsp skali jego bitmapy

                                      QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(50, 50, 50, 150),   // kolor AM_PM
                                      QPoint(0, 146),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(40, 20, 0),   // kolor pełnej daty

                                      QPoint(34, 38),    // pozycja datownika % z dlug promienia
                                      20,   // szer okienka w % dlugosci promienia
                                      11,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(20,20,20),   // kolor daty
                                      QColor(110, 110, 110, 150),   // kolor ramki_daty
                                      QColor(160, 160, 160, 150),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      2 ,    // uskok cienia
                                      QColor(250,250,250,0)    //  tlo_AM_PM_
                }
                );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -72,  // x pozycja osi  na tarczy
                -0, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) * (now_today.dayOfWeek());
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    //    cyferblat.back().dodaj_extra_wskazowke(
    //                &  cyferblat.back().wsk_dni_miesiaca,
    //                Rola_wskazowki::wsk_dzien_miesiaca,
    //                72,  // x pozycja osi  na tarczy
    //                0, // y pozycja osi  na tarczy
    //                "noname",
    //                ":/new/prefix1/content/iwc_hours.png",
    //                0, // os_x    mimosrod       // w %
    //                0.05, //  os_y;  mimosrod       // w %
    //                0.01, // skala_x (grubosc dla wsk wektorowej),
    //                0.26, // % dlugosc wsk (wekt i bitmapowej)
    //                &pix_wskazowka_dni_miesiaca,
    //                QColor(255, 0, 0),  //  QColor kolor;
    //                false, //  flag_bitmapa_moze_byc_kolorowana;
    //                2, // odl cienia
    //                0, // warstwa
    //                // lambda
    //                [this] () {
    //        double kat =
    //                (360.0/31) * (now_today.day() -1);
    //        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
    //        return kat;
    //    }
    //    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                72,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik =
                (360.0/12) * (now_today.month()-1 );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );
#endif

    //-------------------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Geizmont_light",
                                      ":/new/prefix1/content/geizmont_light.png",
                                      ":/new/prefix1/content/geizmont_wsk_hours_h.png",
                                      ":/new/prefix1/content/geizmont_wsk_minut_h.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001,
                                      0.115,    // XY mimosrod wektorowej wskazowki
                                      0.01 ,  0.65, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      0,    0.06, // XY mimosrod wektorowej wskazowki
                                      0.01,  0.9,// szer i dlug minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      209, 249,  // pozycja srodka tarczy
                                      0.5,  0.5,   // skalowanie zaślepki
                                      163,  // promien_ tarczy

                                      -0, -0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, 0.43, // pozycja księzyca (w % promienia tarczy)
                                      0.2 ,        // i wsp skali jego bitmapy

                                      QPoint(0, -30),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(50, 50, 50, 150),   // kolor AM_PM
                                      QPoint(0, 146),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(40, 20, 0),   // kolor pełnej daty

                                      QPoint(34, 38),    // pozycja datownika % z dlug promienia
                                      20,   // szer okienka w % dlugosci promienia
                                      10,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(20,20,20),   // kolor daty
                                      QColor(110, 110, 110, 50),   // kolor ramki_daty
                                      QColor(160, 160, 160, 150),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      2,     // uskok cienia
                                      QColor(200,200,200,100)    //  tlo_AM_PM_
                }
                );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -72,  // x pozycja osi  na tarczy
                -0, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) * (now_today.dayOfWeek());
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    //    cyferblat.back().dodaj_extra_wskazowke(
    //                &  cyferblat.back().wsk_dni_miesiaca,
    //                Rola_wskazowki::wsk_dzien_miesiaca,
    //                72,  // x pozycja osi  na tarczy
    //                0, // y pozycja osi  na tarczy
    //                "noname",
    //                ":/new/prefix1/content/iwc_hours.png",
    //                0, // os_x    mimosrod       // w %
    //                0.05, //  os_y;  mimosrod       // w %
    //                0.01, // skala_x (grubosc dla wsk wektorowej),
    //                0.26, // % dlugosc wsk (wekt i bitmapowej)
    //                &pix_wskazowka_dni_miesiaca,
    //                QColor(255, 0, 0),  //  QColor kolor;
    //                false, //  flag_bitmapa_moze_byc_kolorowana;
    //                2, // odl cienia
    //                0, // warstwa
    //                // lambda
    //                [this] () {
    //        double kat =
    //                (360.0/31) * (now_today.day() -1);
    //        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
    //        return kat;
    //    }
    //    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                70,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik =
                (360.0/12) * (now_today.month()-1 );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );
#if 0
    //-------------------------------------------------------------------
    cyferblat.push_back(
                Tdane_cyferblatu  {   // konstruktor z ksiezycem
                                      "Geizmont_dark",
                                      ":/new/prefix1/content/geizmont_dark.png",
                                      ":/new/prefix1/content/geizmont_wsk_hours_h.png",
                                      ":/new/prefix1/content/geizmont_wsk_minut_h.png",
                                      "0",                      //  ":/new/prefix1/content/neon_sekundnik.png",
                                      ":/new/prefix1/content/center.png",

                                      0.0001,
                                      0.115,  // XY mimosrod wektorowej wskazowki
                                      0.01 ,  0.85, // os godzinowej, skala
                                      QColor(66, 21, 2),   // kolor wskaz godzinowej

                                      -0.0001,    0.06, // XY mimosrod wektorowej wskazowki
                                      0.01,  1.05, // 0.9,// os minutowej
                                      QColor(66, 21, 2),   // kolor wskaz minutowej

                                      0,    0.0,
                                      0.01,  // grubosc sekundnika
                                      1,  // dlugosc sekundnika (w % promienia tarczy)
                                      QColor(250, 10, 10),   // kolor wskaz sekundnika

                                      209, 249,  // pozycja srodka tarczy
                                      0.5,  0.5,   // skalowanie zaślepki
                                      163,  // promien_ tarczy

                                      -0, -0,  // translacja do odrebnej osi sekundnika (x,y w % promienia)

                                      0, 0.43, // pozycja księzyca (w % promienia tarczy)
                                      0.2 ,        // i wsp skali jego bitmapy

                                      QPoint(0, 150),    // pozycja AM_PM ( % z dlug promienia)
                                      QColor(240, 120, 0),   // kolor AM_PM
                                      QPoint(0, 146),    // pozycja pełnej daty ( % z dlug promienia)
                                      QColor(40, 20, 0),   // kolor pełnej daty

                                      QPoint(34, 38),    // pozycja datownika % z dlug promienia
                                      20,   // szer okienka w % dlugosci promienia
                                      12,   // wys okienka i fontu  (w % dlugosci promienia)

                                      QColor(20,20,20),   // kolor daty
                                      QColor(110, 110, 110, 150),   // kolor ramki_daty
                                      QColor(160, 160, 160, 150),   // kolor tla_daty
                                      QColor(0, 0, 0, 100) ,  // kolor cienia
                                      2     // uskok cienia
                }
                );

    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_dni_tygodnia,
                Rola_wskazowki::wsk_full_date,
                -70,  // x pozycja osi  na tarczy
                -0, // y pozycja osi  na tarczy
                "extra: iwc_hours",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_dni_tygodnia,
                QColor(255, 0, 0),  //  QColor kolor;
                false, //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,  // warstwa
                // lambda
                [this] () {
        double kat =   (360.0/7) * (now_today.dayOfWeek());
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return kat;
    }
    );

    //    cyferblat.back().dodaj_extra_wskazowke(
    //                &  cyferblat.back().wsk_dni_miesiaca,
    //                Rola_wskazowki::wsk_dzien_miesiaca,
    //                72,  // x pozycja osi  na tarczy
    //                0, // y pozycja osi  na tarczy
    //                "noname",
    //                ":/new/prefix1/content/iwc_hours.png",
    //                0, // os_x    mimosrod       // w %
    //                0.05, //  os_y;  mimosrod       // w %
    //                0.01, // skala_x (grubosc dla wsk wektorowej),
    //                0.26, // % dlugosc wsk (wekt i bitmapowej)
    //                &pix_wskazowka_dni_miesiaca,
    //                QColor(255, 0, 0),  //  QColor kolor;
    //                false, //  flag_bitmapa_moze_byc_kolorowana;
    //                2, // odl cienia
    //                0, // warstwa
    //                // lambda
    //                [this] () {
    //        double kat =
    //                (360.0/31) * (now_today.day() -1);
    //        //cout << "lambda obliczenie kata dla zones " << " = " << kat  << endl;
    //        return kat;
    //    }
    //    );


    cyferblat.back().dodaj_extra_wskazowke(
                &  cyferblat.back().wsk_miesiaca,
                Rola_wskazowki::wsk_miesiaca,
                70,  // x pozycja osi  na tarczy
                0, // y pozycja osi  na tarczy
                "noname",
                ":/new/prefix1/content/iwc_hours.png",
                0, // os_x    mimosrod       // w %
                0.12, //  os_y;  mimosrod       // w %
                0.01, // skala_x (grubosc dla wsk wektorowej),
                0.2, // % dlugosc wsk (wekt i bitmapowej)
                &pix_wskazowka_miesiaca,
                QColor(255, 0, 0),  //  QColor kolor;
                false,   //  flag_bitmapa_moze_byc_kolorowana;
                1, // odl cienia
                0,          // warstwa

                // lambda
                [this] () {
        double wynik =
                (360.0/12) * (now_today.month()-1 );
        //cout << "lambda obliczenie kata dla zones " << " = " << wynik  << endl;
        return wynik;
    }
    );
#endif











}
//*************************************************************************************************************************
// UWAGA:  //  dlug wskazowki w  % promienia tarczy jest w poniższych danych  NIEISTOTNA,
// bo brana jest z cyferblatu
// NATOMIAST mimośród x, y są waże, bo są cechą konkretnej wskazowki (gdzie na niej jest oś obrotu)
//*************************************************************************************************************************
void MainWindow::wstepne_wpisanie_wskazowek_godzinowych()
{


#if 0
    // Ta pierwsza nigdy nie będzie użyta, zamiast niej będzia ta (bitmapowa) która jest opisana w tarczy
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  tr("factory hours hand  (for a particular clock face)").toStdString(),
                                  ":/new/prefix1/content/arnehour_pion.png",
                                  0,     0.23,      // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.025,       // szer w  % promienia tarczy
                                  QColor(0, 0, 0), // kolor wskazowki
                                  false // nie chcemy podkolorowac tej bitmapy
                });
#endif
    // zwykle, arne

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "arne-hour ",
                                  //                                  ":/new/prefix1/content/arnehour_pion.png",
                                  ":/new/prefix1/content/arne-3D_hour_hand.png",
                                  0,     0.05,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer
                                  //  dlug wsk w  % promienia tarczy NIEISTOTNE, dlugosc brana jest z cyferblatu
                                  QColor(0, 0, 0), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  // //  0.02,     0.02,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // nie chcemy podkolorowac tej bitmapy



                });

    //    vect_godzinowych.push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::wsk_godzinowa,
    //                    "CityHall-hour",
    //                    ":/new/prefix1/content/AJ-CityHall-500-hour-pion.png",
    //                    0,     0.23,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    1.0,        // szer w  % promienia tarczy
    //                    QColor(0, 0, 0), // kolor wskazowki
    //                    QColor(150, 150, 150, 150), // kolor cienia
    //                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    false // nie chcemy podkolorowac tej bitmapy
    //                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "CityHall-hour-JG",
                                  ":/new/prefix1/content/city_hall_hours.png",
                                  0,     0.22,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.0,       // szer w  % promienia tarczy
                                  QColor(0, 0, 0), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // nie chcemy podkolorowac tej bitmapy



                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "barok1-kameleon",
                                  ":/new/prefix1/content/barok1_godzinowa.png",
                                  0,     // XY mimosrod wektorowej wskazowki X
                                  0.1,     // mimosrod os y w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,    // szer
                                  //  dlug w  % promienia tarczy NIEISTOTNE, dlugosc brana jest z cyferblatu

                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // flag_bitmapa



                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "barok1-silver-hour",
                                  ":/new/prefix1/content/barok1_godzinowa_bevel_a.png",
                                  0,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac?
                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "barok1-gold-hour",
                                  ":/new/prefix1/content/barok1_godzinowa_bevel_zloty.png",
                                  0,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac?
                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "barok1-gold-shining-hour",
                                  ":/new/prefix1/content/barok1_godzinowa_bevel_zloty_shining.png",
                                  0,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac?
                });
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "barok1-red",
                                  ":/new/prefix1/content/barok_godzinowa_red.png",
                                  0,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac?
                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "barok1-dark-hour",
                                  ":/new/prefix1/content/barok1_godzinowa_bevel_dark.png",
                                  0,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac?
                });



    //    vect_godzinowych.push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::wsk_godzinowa,
    //                    "marble-hour",
    //                    ":/new/prefix1/content/marble_minutowa.png",
    //                    0,     0.26,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    1.2,  0.8,       // szer w  % promienia tarczy
    //                    QColor(0, 0, 200), // kolor wskazowki
    //                    QColor(150, 150, 150, 150), // kolor cienia
    //                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    true // wolno podkolorowac bitmape



    //                });



    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "crystal-ball-hours",
                                  ":/new/prefix1/content/crystal_ball_hours.png",
                                  0,     0.02,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1, // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 0, 0), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape?



                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "roman-hours",
                                  ":/new/prefix1/content/roman_hours.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "roman-gold-hours",
                                  ":/new/prefix1/content/roman_hours_gold.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,      // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "roman-white",
                                  ":/new/prefix1/content/roman_hours_white.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,  // // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "roman-grey",
                                  ":/new/prefix1/content/roman_hours_grey.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "cebulasta-gold",
                                  ":/new/prefix1/content/cebulasta_godzinowa_zlota.png",
                                  -2,     0.326,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,    // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "cebulasta-czarna",
                                  ":/new/prefix1/content/cebulasta_godzinowa_czarna.png",
                                  0,     0.326,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });
    //----------------------
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "cebulasta-biala",
                                  ":/new/prefix1/content/cebulasta_godzinowa_biala.png",
                                  0,     0.326,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });

    //----------------------
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "drewniana-B",
                                  ":/new/prefix1/content/drewano_godzinowaB.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });

    //----------------------
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "drewniana-kameleon",
                                  ":/new/prefix1/content/drewano_godzinowaB.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "drewniana-Cwide",
                                  ":/new/prefix1/content/drewno_minutowa_wide_contr.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "drewniana-C",
                                  ":/new/prefix1/content/drewano_godzinowa_C.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "drewno_godzinowa_kameleon",
                                  ":/new/prefix1/content/drewno_godzinowa_C_black.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "drewno_godzinowa_C_black",
                                  ":/new/prefix1/content/drewno_godzinowa_C_buk.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "agrafa_metal",
                                  ":/new/prefix1/content/agrafa_metal-h.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki {  Rola_wskazowki::wsk_godzinowa,
                                   "agrafa_gold",
                                   ":/new/prefix1/content/agrafa_godz.png",
                                   0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                   5,        // szer w  % promienia tarczy
                                   QColor(0, 0, 200), // kolor wskazowki
                                   //                    QColor(150, 150, 250, 50), // kolor cienia
                                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                   false // wolno podkolorowac bitmape ?
                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "tower_empty_gold",
                                  ":/new/prefix1/content/tower_empty_gold_hours.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "tower_full_gold",
                                  ":/new/prefix1/content/tower_gold_hours.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "zestaw_barok_gold",
                                  ":/new/prefix1/content/zestaw_barok_gold_h.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });



    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "tower_metal-h",
                                  ":/new/prefix1/content/tower_metal-h.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "tower_empty_metal-h",
                                  ":/new/prefix1/content/tower_empty_metal-h.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "tower_barock_metal-h",
                                  ":/new/prefix1/content/tower_barock_metal-h.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "rombowe_godzinowa_zlota",
                                  ":/new/prefix1/content/rombowe_godzinowa_zlota.png",
                                  0,     0.11,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "rombowe_godzinowa_biala",
                                  ":/new/prefix1/content/rombowe_godzinowa_biala.png",
                                  0,     0.11,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });
    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "rombowe_godzinowa_ciemna",
                                  ":/new/prefix1/content/rombowe_godzinowa_ciemna.png",
                                  0,     0.11,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "rombowe_godzinowa_kameleon",
                                  ":/new/prefix1/content/rombowe_godzinowa_czarna_plaska.png",
                                  0,     0.11,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true    // wolno podkolorowac bitmape ?
                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "iwc_hours",
                                  ":/new/prefix1/content/iwc_hours.png",
                                  -1,     0.115,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false    // wolno podkolorowac bitmape ?
                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "Morgan_hours",
                                  ":/new/prefix1/content/morgan_hours.png",
                                  -2,     0.26,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,     // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false    // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "neon_hours",
                                  ":/new/prefix1/content/neon_wskazowka.png",
                                  0,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,         // szer w  % promienia tarczy
                                  QColor(0, 220, 255), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false    // wolno podkolorowac bitmape ?

                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "Omega-hours",
                                  ":/new/prefix1/content/Omega-hours.png",
                                  0,     0.18,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,        // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false    // wolno podkolorowac bitmape ?

                });


    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "Modern-Design",
                                  ":/new/prefix1/content/Modern-Design-hours.png",
                                  0,     0.15,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false    // wolno podkolorowac bitmape ?

                });



    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "ChronoSwiss",
                                  ":/new/prefix1/content/wtla-chronoswiss-godziny.png",
                                  0,     0.12,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  false    // wolno podkolorowac bitmape ?
                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "Geizmont",
                                  ":/new/prefix1/content/geizmont_wsk_hours.png",
                                  0,     0.19,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  false    // wolno podkolorowac bitmape ?
                });

    vect_godzinowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_godzinowa,
                                  "Geizmont_empty",
                                  ":/new/prefix1/content/geizmont_wsk_hours_h.png",
                                  0,     0.15,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  false    // wolno podkolorowac bitmape ?
                });
}

//**********************************************************minutowe **********************************
void MainWindow::wstepne_wpisanie_wskazowek_minutowych()
{


#if 0
    // pierwsza minutowa nigdy nie będzie użyta, bo zatąpi ją ta, która jest opisana w tarczy
    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  tr("factory minutes hand  (for a particular clock face)").toStdString(),
                                  ":/new/prefix1/content/arneminute_pion.png",
                                  0,     0.15,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 0), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  // //  0.02,     0.02,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false  // wolno podkolorowac bitmape ?
                });

#endif


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "arne-minute",
                                  //                                  ":/new/prefix1/content/arneminute_pion.png",
                                  ":/new/prefix1/content/arne-3Dhand.png",
                                  0,     0.10,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 0), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.02,     0.02,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false  // wolno podkolorowac bitmape ?



                });




    vect_minutowych.push_back(
                Tdane_wskazowki{ Rola_wskazowki::wsk_minutowa,
                                 "CityHall-minutes-JG",
                                 ":/new/prefix1/content/city_hall_minutes.png",
                                 0,     0.15,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                 2,        // szer w  % promienia tarczy
                                 QColor(0, 0, 0), // kolor wskazowki
                                 //                    QColor(150, 150, 150, 150), // kolor cienia
                                 //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                 false // wolno podkolorowac bitmape ?
                }
                );


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "barok1-minutes",
                                  ":/new/prefix1/content/barok1_minutowa.png",
                                  0.001,     0.062,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true //wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "barok1-gold-minutes",
                                  ":/new/prefix1/content/barok1_minutowa_gold.png",
                                  0,     0.062,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false //wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "barok1-gold-min_bv6",
                                  ":/new/prefix1/content/barok1_min_bev06.png",
                                  0,     0.062,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false //wolno podkolorowac bitmape ?

                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "barok1-gold-min_bv10",
                                  ":/new/prefix1/content/barok1_min_bev10.png",
                                  0,     0.062,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false //wolno podkolorowac bitmape ?

                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "barok1_red",
                                  ":/new/prefix1/content/barok1_minutowa_red.png",
                                  0,     0.062,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false //wolno podkolorowac bitmape ?

                });


    //    vect_minutowych.push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::wsk_minutowa,
    //                    "marble-minutes",
    //                    ":/new/prefix1/content/marble_minutowa.png",
    //                    0,     0.25,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    1.0,        // szer w  % promienia tarczy
    //                    QColor(0, 0, 200), // kolor wskazowki
    //                    QColor(150, 150, 150, 150), // kolor cienia
    //                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    true //wolno podkolorowac bitmape ?



    //                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "crystal-ball-minutes",
                                  ":/new/prefix1/content/crystal_ball_minutes.png",
                                  0,     0.02,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.5,  // 0.75,       // szer w  % promienia tarczy
                                  QColor(0, 0, 0, 0), // kolor wskazowki  (nie będzie zmiany koloru)
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });



    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_minut",
                                  ":/new/prefix1/content/roman_minut.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // wolno podkolorowac bitmape ?



                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_gold_soft_minut",
                                  ":/new/prefix1/content/roman_gold_soft_minut.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_minutgold_10",
                                  ":/new/prefix1/content/roman_minutgold_10.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_minut_light",
                                  ":/new/prefix1/content/roman_minut_light.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });

    //    vect_minutowych.push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::wsk_minutowa,
    //                    "roman-new-dark-minutes",
    //                    ":/new/prefix1/content/roman_minut_dark.png",
    //                    0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    1.2,        // szer w  % promienia tarczy
    //                    QColor(0, 0, 200), // kolor wskazowki
    //                    QColor(150, 150, 250, 250), // kolor cienia
    //                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    false // wolno podkolorowac bitmape ?
    //
    //
    //
    //                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_min_black",
                                  ":/new/prefix1/content/roman_min_black.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_min_grey",
                                  ":/new/prefix1/content/roman_min_grey.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "roman_min_gold",
                                  ":/new/prefix1/content/roman_min_gold.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "cebulasta_minutowa_zlota",
                                  ":/new/prefix1/content/cebulasta_minutowa_zlota.png",
                                  0,     0.255,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "cebulasta-biala",
                                  ":/new/prefix1/content/cebulasta_minutowa_biala.png",
                                  0,     0.255,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });
    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "cebulasta-czarna",
                                  ":/new/prefix1/content/cebulasta_minutowa_czarna.png",
                                  0,     0.255,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewniana",
                                  ":/new/prefix1/content/drewno_minutowa.png",
                                  0,     0.16,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 250, 250), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewniana-wide",
                                  ":/new/prefix1/content/drewno_minutowa_wide.png",
                                  0,     0.16,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewniana-Cwide",
                                  ":/new/prefix1/content/drewno_minutowa_wide_contr.png",
                                  0,     0.16,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewniana-Coloured",
                                  ":/new/prefix1/content/drewno_minutowa_wide_contr.png",
                                  0,     0.16,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(250, 0, 0, 205), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewniana-B",
                                  ":/new/prefix1/content/drewano_godzinowaB.png",
                                  0,     0.16,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1.2,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // // wolno podkolorowac bitmape ?



                });







    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewno_godzinowa_C_black",
                                  ":/new/prefix1/content/drewno_godzinowa_C_black.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // wolno podkolorowac bitmape ?



                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "drewno_C_buk",
                                  ":/new/prefix1/content/drewno_godzinowa_C_buk.png",
                                  0,     0.175,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "agrafa_gold",
                                  ":/new/prefix1/content/agrafa_min.png",
                                  0,     0.05,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });




    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "tower_empty_gold_minutes",
                                  ":/new/prefix1/content/tower_empty_gold_minutes.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "tower_gold_minutes",
                                  ":/new/prefix1/content/tower_gold_minutes.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "zestaw_barok_gold",
                                  ":/new/prefix1/content/zestaw_barok_gold_m.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "agrafa_metal-m",
                                  ":/new/prefix1/content/agrafa_metal-m.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "tower_metal-m",
                                  ":/new/prefix1/content/tower_metal-m.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "tower_empty_metal-m",
                                  ":/new/prefix1/content/tower_empty_metal-m.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "tower_barock_metal-m",
                                  ":/new/prefix1/content/tower_barock_metal-m.png",
                                  0,     0.045,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "rombowe_minutowa_zlota",
                                  ":/new/prefix1/content/rombowe_minutowa_zlota.png",
                                  0,     0.08,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "rombowe_minutowa_kameleon",
                                  ":/new/prefix1/content/rombowe_minutowa_czarna_plaska.png",
                                  0,     0.08,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  true // wolno podkolorowac bitmape ?
                });
    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "rombowe_minutowa_czarna",
                                  ":/new/prefix1/content/rombowe_minutowa_czarna.png",
                                  0,     0.08,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "rombowe_minutowa_biala",
                                  ":/new/prefix1/content/rombowe_minutowa_biala.png",
                                  0,     0.08,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "iwc_minutes",
                                  ":/new/prefix1/content/iwc_minutes.png",
                                  0.0,    0.06,   // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.05,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "morgan_minutes",
                                  ":/new/prefix1/content/morgan_minutes.png",
                                  0.0,    0.19,   // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.05,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "neon_minutes",
                                  ":/new/prefix1/content/neon_wskazowka.png",
                                  0.0,    0.1,   // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "Omega-minutes",
                                  ":/new/prefix1/content/Omega-minutes.png",
                                  0.0,    0.15,   // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,    // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });


    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "Modern-design",
                                  ":/new/prefix1/content/Modern-Design-minutes.png",
                                  0.0,    0.1,   // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,       // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki
                                  //   QColor(150, 150, 250, 50), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "ChronoSwiss",
                                  ":/new/prefix1/content/wtla-chronoswiss-minuty.png",
                                  0,     0.07,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.04,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  false    // wolno podkolorowac bitmape ?
                });

    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "Geizmont",
                                  ":/new/prefix1/content/geizmont_wsk_minut.png",
                                  0,     0.18,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.02,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  false    // wolno podkolorowac bitmape ?
                });
    vect_minutowych.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_minutowa,
                                  "Geizmont_empty",
                                  ":/new/prefix1/content/geizmont_wsk_minut_h.png",
                                  0,     0.14,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.02,         // szer w  % promienia tarczy
                                  QColor(0, 200, 200), // kolor wskazowki
                                  false    // wolno podkolorowac bitmape ?
                });

    //==== SEKUNDNIKI =============================================

    //    vect_sekundnikow. push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::wsk_sekundowa,
    //                   tr("factory seconds hand  (for a particular clock face)"),
    //                    // tr ("default hand - for a current clock face").toStdString() ,
    //                    ":/new/prefix1/content/second.png",
    //                    0.30,     0.0,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    1,  0.8,       // szer w  % promienia tarczy
    //                    QColor(200, 0, 0), // kolor wskazowki
    //                    QColor(150, 150, 150, 150), // kolor cienia
    //                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    true // wolno podkolorowac bitmape ?



    //                }
    //                );

    //    vect_sekundnikow. push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::wsk_sekundowa,
    //                    "arne-seconds",
    //                    ":/new/prefix1/content/second.png",
    //                    0.20,     0.1,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    1,  0.8,       // szer w  % promienia tarczy
    //                    QColor(200, 0, 200), // kolor wskazowki
    //                    QColor(150, 150, 150, 150), // kolor cienia
    //                   //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    false // wolno podkolorowac bitmape ?



    //                }
    //                );



    vect_sekundnikow. push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_sekundowa,
                                  "Wall Clock seconds HD",
                                  ":/new/prefix1/content/Wall-sekundnik.png",
                                  0.0,     0.0,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  1,        // szer w  % promienia tarczy
                                  QColor(200, 0, 0), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                }
                );

    vect_sekundnikow. push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_sekundowa,
                                  "Neon seconds",
                                  ":/new/prefix1/content/neon_sekundnik.png",
                                  0.0,     0.19,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.005,    // szer w  % promienia tarczy
                                  QColor(0, 230, 230), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?

                }
                );

    vect_sekundnikow. push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_sekundowa,
                                  "Omega-seconds.png",
                                  ":/new/prefix1/content/Omega-seconds.png",
                                  0.0,     0.24,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.005,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 230, 230), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                }
                );




    vect_sekundnikow. push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_sekundowa,
                                  "Modern-Design.png",
                                  ":/new/prefix1/content/Modern-Design-seconds.png",
                                  0.0,     0.28,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.005,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 230, 230), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                }
                );




    vect_sekundnikow. push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_sekundowa,
                                  "Chrono Swiss",
                                  ":/new/prefix1/content/wtla-chronoswiss-sekundnik.png",
                                  0.0,     0.32,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.005,  // 0.6,       // szer w  % promienia tarczy
                                  QColor(0, 230, 230), // kolor wskazowki
                                  //                    QColor(150, 150, 150, 150), // kolor cienia
                                  //  0.05,     0.05,     // przes cienia os w stosunku do p-tu centralnego (w % prom tarczy)
                                  false // wolno podkolorowac bitmape ?
                }
                );

    //===============================================================
    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "iwc_hours",
                                  ":/new/prefix1/content/iwc_hours.png",
                                  -1,     0.5215,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });


    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { zestaw_wskazowek_dodatkowych::zadne,
    //                    "night-day world clockhours",
    //                    ":/new/prefix1/content/Omega-Seamaster-WorldCircle.png",
    //                    0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                    0.07,        // szer w  % promienia tarczy
    //                    QColor(0, 0, 200), // kolor wskazowki

    //                    false    // wolno podkolorowac bitmape ?
    //                });
    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "night-day world clockhours",
                                  ":/new/prefix1/content/Omega-Seamaster-WorldCircle2.png",
                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });



    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zones world clockhours",
    //                                  ":/new/prefix1/content/Seamaster_zones.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });
    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zones2 world clockhours",
    //                                  ":/new/prefix1/content/Seamaster_zones2.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });

    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "zones3 world clockhours",
                                  ":/new/prefix1/content/Seamaster_zones_north.png",
                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });
    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zones2 world clockhours",
    //                                  ":/new/prefix1/content/Seamaster_zones3.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });

    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "zegar_world_24h_grey",
                                  ":/new/prefix1/content/zegar_world_24h_grey.png",
                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });

    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "zegar_world_24h_band_outline",
                                  ":/new/prefix1/content/zegar_world_24h_band_outline.png",
                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });


    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zegar_world_zones_i_mapa",
    //                                  ":/new/prefix1/content/zegar_world_zones_i_mapa.png",
    //                                  //                                     ":/new/prefix1/content/zegar_world_zones_transparent.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });

    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zegar_world_zones_i_mapa",
    //                                  ":/new/prefix1/content/zegar_world_zones_transparent.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });

    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zegar_world_zones_i_mapa",
    //                                  ":/new/prefix1/content/zegar_world_zones_transparentC.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki
    //                                  false    // wolno podkolorowac bitmape ?
    //                });
    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zegar_world_zones_i_mapa",
    //                                  ":/new/prefix1/content/zegar_world_zones_transparentD.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });



    //    vect_extra_wskazowek.push_back(
    //                Tdane_wskazowki { Rola_wskazowki::zadne,
    //                                  "zegar_world_zones_i_mapa_big",
    //                                  ":/new/prefix1/content/zegar_world_zones_big_black.png",
    //                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
    //                                  0.07,        // szer w  % promienia tarczy
    //                                  QColor(0, 0, 200), // kolor wskazowki

    //                                  false    // wolno podkolorowac bitmape ?
    //                });






    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "world_cities",
                                  ":/new/prefix1/content/world_cities.png",
                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });


    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::wsk_miesiaca,
                                  "Azimuthal_projection_SW",
                                  ":/new/prefix1/content/Azimuthal_projection_SW.png",
                                  0,     0.5,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.07,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });

    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "ChronoSwiss-days",
                                  ":/new/prefix1/content/wtla-chronoswiss-days.png",
                                  0,     0.08,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });

    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "ChronoSwiss-months",
                                  ":/new/prefix1/content/wtla-chronoswiss-miesiace.png",
                                  0,     0.15,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });
    vect_extra_wskazowek.push_back(
                Tdane_wskazowki { Rola_wskazowki::zadne,
                                  "ChronoSwiss-seconds (small)",
                                  ":/new/prefix1/content/wtla-chronoswiss-sekundnik_maly.png",
                                  0,
                                  0.35,     // os w stosunku do p-tu centralnego (w % prom tarczy)
                                  0.5,        // szer w  % promienia tarczy
                                  QColor(0, 0, 200), // kolor wskazowki

                                  false    // wolno podkolorowac bitmape ?
                });
}

//***************************************************************************************************************
void MainWindow::zapis_opcji_na_dysku()
{
    ofstream plik(pathed_nazwa_pliku_z_opcjami);
    if(!plik)
    {
        cerr << "Blad zapisu pliku " << pathed_nazwa_pliku_z_opcjami << endl;
        exit(3);
    }
    plik
            << nr_jezyka << "\n"   // must be the first
            << przezroczystosc << "\n"
            << przezroczystosc_dla_kursora << "\n"
            << flag_na_wierzchu << "\n"
            <<  flag_ograniczenie_pozycji << "\n"
             <<  flag_blokuj_na_ekranie << "\n"
              <<  flag_uruchom_z_loginem << "\n"
               <<  flag_uruchom_przy_logowaniu<< "\n"
                << flag_pozwalam_na_kilka_kopii_program<< "\n"
                << flag_gskala_active << '\n'
                <<  gskala  << '\n'
                 << flag_show_full_date << '\n'
                 <<  flag_show_date << '\n'
                  << flag_show_sekundnik << '\n'
                  << nr_tarczy << '\n'
                  << poz_srodka_zegara_x << '\n'
                  << poz_zegara_y << '\n'
                  << nazwa_strefy_czasowej << '\n'
                  << flag_show_AM_PM << '\n'
                  <<   cyf.wsk_godzinowa.flag_bitmapowa  << '\n'
                    <<  cyf.wsk_minutowa.flag_bitmapowa   << '\n'
                     << cyf.wsk_sekundowa.flag_bitmapowa  << '\n'

                     <<  nr_wsk_wektorowej_godz << '\n'
                      <<  nr_wsk_wektorowej_min << '\n'
                       <<  nr_wsk_wektorowej_sek << '\n'

                        << nr_wybranej_bitmapy_wsk_godzinowej << '\n'
                        << nr_wybranej_bitmapy_wsk_minutowej << '\n'
                        << nr_wybranej_bitmapy_wsk_sekundowej << '\n'
                        <<  flag_random_tarcza << '\n';



    cerr << "Po zapisie " << nazwa_pliku_z_opcjami << endl;
}
//***************************************************************************************************************
int  MainWindow::id_linux_czy_windows_version()
{
    //    cout << __func__ << " function " << endl;

#ifdef Q_OS_WIN

    auto
            current
            [[maybe_unused]]
            = QOperatingSystemVersion::current();
    //    cout <<  current.name().toStdString() << endl;    // pisze: "Windows"

    string product_version =
            QSysInfo::productVersion().toStdString();

    // cout << " to productVersion  = " <<  product_version << endl; // pisze: "7sp1"


    if(product_version.substr(0, 1) == "7 ")
    {
        // to jest windows 7
        return 7;
    }
    else   if(product_version.substr(0, 1) == "8")
    {
        // to jest windows 8
        return 8;
    }
    else   if(product_version.substr(0, 2) == "10")
    {
        // to jest windows 10
        return 10;
    }

    // w przeciwnym razie wersja Windows nierozpoznana
    return 0;


#elif defined Q_OS_LINUX
    return 1;  // to jest linux
#else
#error  "Your operating system is not supported "
    return 0;
#endif

}
//***************************************************************************************************************
void MainWindow::wczytanie_opcji_z_dysku()
{
    // rozpoznanie typu systemu operacyjnego. Jeśli to LInux, to powinien istniec katalog  ~/.configure
    // 	cout << __PRETTY_FUNCTION__ <<  __LINE__ << endl;

    id_linux_czy_windows_version();


    ifstream plik(pathed_nazwa_pliku_z_opcjami);
    if(!plik)
    {
        cerr << "Error while reading-in an options file:  "
             << pathed_nazwa_pliku_z_opcjami
             << "\n so build-in values will be used"
             << endl;
        przezroczystosc = 255;
        return ;
    }
    plik
            >> nr_jezyka   // musi byc piewszy, bo w main to czyt
            >> przezroczystosc
            >> przezroczystosc_dla_kursora
            >> flag_na_wierzchu
            >>  flag_ograniczenie_pozycji   // nie uzywamy
            >>  flag_blokuj_na_ekranie
            >>  flag_uruchom_z_loginem  // ???
            >>  flag_uruchom_przy_logowaniu // ???
            >> flag_pozwalam_na_kilka_kopii_program
            >> flag_gskala_active
            >>  gskala
            >> flag_show_full_date
            >> flag_show_date
            >> flag_show_sekundnik
            >> nr_tarczy
            >> poz_srodka_zegara_x
            >> poz_zegara_y
            >> nazwa_strefy_czasowej
            >>  flag_show_AM_PM
            >> cyf.wsk_godzinowa.flag_bitmapowa
            >> cyf.wsk_minutowa.flag_bitmapowa
            >>  cyf.wsk_sekundowa.flag_bitmapowa
            >> nr_wsk_wektorowej_godz
            >> nr_wsk_wektorowej_min
            >> nr_wsk_wektorowej_sek

            >> nr_wybranej_bitmapy_wsk_godzinowej
            >> nr_wybranej_bitmapy_wsk_minutowej
            >> nr_wybranej_bitmapy_wsk_sekundowej
            >> flag_random_tarcza;



    this->setWindowOpacity(przezroczystosc/255.0);
    auto pozycja_x_rogu = poz_srodka_zegara_x  - (width() / 2.0) ;
    this->move(pozycja_x_rogu, poz_zegara_y);

    //    cout << __func__ <<  "pozycja rogu na ekranie  = "
    //            << pozycja_x_rogu
    //            << " width() = " << width()
    //            << " a sama poz_srodka_zegara_x = " << poz_srodka_zegara_x
    //            << ", skala = " << gskala
    //            << endl;

    if(flag_random_tarcza)
        nr_tarczy = random_cyferblat();

    //    cout << __func__ << endl;
    //    cout  << "wczytane timezone = " << nazwa_strefy_czasowej
    //          << ", poz_zegara_y = " << poz_zegara_y
    //          << endl;

    zmiana_opcji_on_top(flag_na_wierzchu);

    if(nr_jezyka != nr_obecnie_zainstalowanego_jezyka)
    {
        zmiana_jezyka(nr_jezyka);
        flag_ma_byc_restart = true;
        cout << "ma byc restart z powodu zmiany jezyka w we wczytanych opcjach (przed close )" << endl;
        close();
    }

    if(gskala < 0.01) gskala = 1;   // in case something was stored wrong
    gskala = 1;   // CHWILOWO

    //    cout << __PRETTY_FUNCTION__ <<  __LINE__ << endl;
}


//***************************************************************************************************************
void MainWindow::zmiana_opcji_on_top(bool flag_top)
{
    flag_na_wierzchu = flag_top;

    //  cout << "Flaga - zegar ma byc on top ? " << boolalpha << flag_top << endl;

    setWindowFlag(Qt::WindowStaysOnTopHint, flag_top );
    setWindowFlag(Qt::X11BypassWindowManagerHint, flag_top);
    //  Informs the window system that the window should stay on top of all other windows.
    // Note that on some window managers on X11 you also have
    // to pass Qt::X11BypassWindowManagerHint for this flag to work correctly.

}

//***************************************************************************************************************
void MainWindow::zmiana_jezyka(int nr_jezyka)
{
    cout << __func__ << "  Flaga - nowy jezyk  ma  nr " << nr_jezyka << endl;



    setWindowFlag(Qt::WindowStaysOnTopHint, flag_na_wierzchu );
    setWindowFlag(Qt::X11BypassWindowManagerHint, flag_na_wierzchu);

    //  Informs the window system that the window should stay on top of all other windows.
    // Note that on some window managers on X11 you also have
    // to pass Qt::X11BypassWindowManagerHint for this flag to work correctly.

}


//***************************************************************************************************************
void MainWindow::zmien_przezroczystosc(int value)
{
    setWindowOpacity(value/255.0);
}
//*********************************************************************************************************
bool MainWindow::spr_rozne_typy_alarmu(Tjeden_alarm & al, QTime time, QDate date)
{
    // UWAGA
    //    enum  case_czestotliwosc { raz, codziennie, co_tydzien,
    //                               co_2_tygodnie,   //  <---- OBSOLATE
    //                               co_miesiac, co_rok, co_iles_minut,
    //                               co_PARZYSTE_tygodnie,
    //                               co_NIEPARZYSTE_tygodnie
    //                                };


    if(al.flag_test_alarm_now)
    {
        al.flag_test_alarm_now = false;
        return true;
    }

    //    QTime time = QTime::currentTime();
    //    QDate date = QDate::currentDate();

    //        cout << "Porownanie  alarmu " << (al.godzina + ":00") << endl;
    //        cout << "  z czasem " << time.toString().toStdString()    << endl;

    if( (al.godzina + ":00")  == time.toString().toStdString() )
    {
        cout << "bezacy miesiac " << date.month();
        cout << ", czestotliwosc  " << al.czestotliwosc << endl;
        cout << ", al.data  " << al.data << endl;
        cout << ", date.toString().toStdString() =  " << date.toString().toStdString()  << endl;

        switch (al.czestotliwosc){
        case case_czestotliwosc::codziennie:    // codziennie
            return true;

        case case_czestotliwosc::raz:   // raz
            if(al.data == date.toString("yyyy-MM-dd").toStdString() )
            {
                cout << "Zgadza sie godzina i data " << endl;
                return true;
            }
            break;

        case case_czestotliwosc::co_tydzien: // co tydzien
        {
            cout << "dzien tygodnia ma byc ma byc 0x"  << hex  << (int) al.dni_tygodnia   << endl;
            int dzisejszy_full_date = date.dayOfWeek()-1;
            if( al.dni_tygodnia & (1 << dzisejszy_full_date))
            {
                //                cout << "zgadza sie dzien tygodnia 0x " << hex << (int) al.dni_tygodnia  << endl;

                return true;
            }
            break;
        }

        case case_czestotliwosc::co_2_tygodnie:   // <--- OBSOLATE
            // co 2 tygodnie
        case co_PARZYSTE_tygodnie:
        case co_NIEPARZYSTE_tygodnie:
        {
            cout << "dzien DWU tygodnia ma byc ma byc 0x"  << hex  << (int) al.dni_tygodnia   << endl;
            int dzisejszy_full_date = date.dayOfWeek()-1;
            if( al.dni_tygodnia & (1 << dzisejszy_full_date))
            {
                cout << "zgadza sie dzien tygodnia 0x " << hex
                     << (int) al.dni_tygodnia  << endl;
                cout << " a czy zgadza się nr tygodnia? " << date.weekNumber()
                     << " (date.weekNumber()  % 2) ) --->"
                     << (date.weekNumber()  % 2)
                     << " a ma byc " << al.flag_even_odd_week_nr << endl;

                if( (date.weekNumber()  % 2)  ==  ! al.flag_even_odd_week_nr  )  //
                {
                    cout << "Zgadza sie parzystosc numeru tygodnia ALARM TRUE" << endl;
                    return true;
                }
                else {
                    cout << "NIE Zgadza sie parzystosc numeru tygodnia " << endl;
                }
            }
            break;
        }

        case case_czestotliwosc::co_miesiac:   // co miesiac
            //            cout << "dzien miesiaca ma byc " << al.data.substr(8,2) << endl;

            if( al.data.substr(8,2) == date.toString("dd").toStdString() )
            {
                //                cout << "Zgadza sie godzina i dzien w miesiacu " << endl;
                return true;
            }
            break;

        case case_czestotliwosc::co_rok:  // co rok
            cout << " miesiac i dzien miesaca ma byc " << al.data.substr(5,5) << endl;
            if( al.data.substr(5,5) == date.toString("MM-dd").toStdString() )
            {
                cout << "Zgadza sie godzina i dzien w miesiacu i miesiac " << endl;
                return true;
            }
            break;

        default:   // ignorujemy na razie interwal, to bedzie ponizej
            break;



        } // koniec switch



    } // kon if, zgadza sie godzina
    else if (al.czestotliwosc == case_czestotliwosc::co_iles_minut)   // czyli: co interwal minut, poczawszy od..
    {

        // cout << " co interwal, " << al.interwal << " poczawszy od " << al.godzina << endl;

        string s_godz = al.godzina.substr(0,2);
        string s_min = al.godzina.substr(3,2);
        int g = stoi(s_godz);
        int m = stoi(s_min);

        QTime start;
        start.setHMS(g, m, 0);

        if(al.interwal != 0){
            if(( time.secsTo(start) % (al.interwal*60) ) == 0)
            {
                cout << " zgodzil sie interwal "  << endl;
                return true;
            }
        }
    }
    else
        return false;

    return false;
}
//*********************************************************************************************************
string MainWindow::kosmetyka_nazwy_programu(string nazwa)
{
    auto gdzie = string::npos;
    for(int i = 0 ; i < 100 ; ++i)
    {
        gdzie = nazwa.rfind('\\', gdzie);
        if(gdzie == string::npos)
        {
            break;
        }
        nazwa.replace(gdzie, 1, R"(\\)" );
        //cout << "Obecnie nazwa = " << nazwa << ", kursor na pozycji " << gdzie << endl;
        --gdzie;



    }
    nazwa = R"(")"   + nazwa + R"(")";
    cout << "ostatecznie [" << nazwa << "]" << endl;


    return nazwa;
}
//*********************************************************************************************************
void MainWindow::player_loop_needed(QMediaPlayer::MediaStatus status)
{
    //cout << "player  zmienil status na = " << status << endl;

    switch (status)
    {
    case QMediaPlayer::LoadedMedia:
        //  cout << "--------->  QMediaPlayer::LoadedMedia " << endl;
        break;

    case QMediaPlayer::BufferedMedia:
        //        cout << " ----------->QMediaPlayer::BufferedMedia " << endl;
        break;

    case  QMediaPlayer::EndOfMedia:
    {

        //cout << "---------> player zakonczyl media" << endl;

        QMediaPlayer *wsk = dynamic_cast< QMediaPlayer*> (sender());

        wsk->play();

    }
        break;

    default:
        cout << "nieznany status " << endl;
        break;


    }
}

// *********************************************************************************************************
/* Uwaga, tu może być problem, bo jeśli wykonanie
 * jednego alarmu zajmie więcej czasu, to drugi, który ma być
 * o tej samej porze może już stać się o sekundę nieaktualny
 * dlatego napierw pytam tu o czas i ten sam czas jest
 * używany do wszystkich sprawdzanych alarmów
 */
// ********************************************************************************************************
void MainWindow::sprawdzenie_alarmow()
{

    //cout <<  __func__ << endl;

    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();

    for(auto & al : tablica_alarmow)
    {

        //        cout <<  "\n" << al.nazwa << " ma godzine " << al.godzina.substr(0,5)
        //              <<  ", porownanie ze stringiem [" << time.toString().toStdString().substr(0,5) << "]" << endl;

        //        if(al.godzina.substr(0,5)  == time.toString().toStdString().substr(0,5) )

        if(al.flag_enable == false) continue;

        if(spr_rozne_typy_alarmu( al, time, date) )
        {

            cout << "!!!!!! ALARM !!!!!!!!" << al.nazwa
                 << ", flag okno = " << al.flag_okno
                 << endl;

            // otwarcie okienka z tekstem
            if(al.flag_okno){
                if( al.flag_alarm_jest_na_ekranie == false  ) {
                    al.flag_alarm_jest_na_ekranie = true;
                    al.dlg = new Togloszenie_alarmu(this);
                    al.dlg->adres_mojego_alarmu(&al);
                    al.dlg->wstaw_opis(al.nazwa.c_str(),  al.dodatkowy_opis.c_str());
                    al.dlg->show();
                } // if nie ma na ekranie

            }// if flag okno

            // wywołanie muzyki -----------------------------------------------------
            if(al.flag_dzwiek)
            {
                //                cout << "!!!!!! Proba uruchomienia muzyki!!!!!!!!" << al.muzyka
                //                     << ", flag dzwiek = " << al.flag_dzwiek << endl;
                if(al.muzyka.empty() == false)
                {
                    cout << "!!!!!! Przed funkcja play !!!!!!!!" << al.muzyka << endl;

                    //QSound::play(al.muzyka.c_str());
                    // if(al.player ==nullptr)   al.player = new QMediaPlayer;
                    // ...
                    // al.player->setMedia(QUrl::fromLocalFile(al.muzyka.c_str()) );
                    // al.player->setVolume(50);

                    al.player = new QMediaPlayer(this);
                    al.audioOutput = new QAudioOutput(this);    // nowy element w Qt6
                    al.player->setAudioOutput(al.audioOutput);  // powiązanie

                    al.player->setSource(QUrl::fromLocalFile(QString::fromStdString(al.muzyka)));
                    al.audioOutput->setVolume(0.5);  // zakres 0.0–1.0

                    al.player->play();


                    if(al.sekund_dzwieku > 0)
                    {
                        connect(al.player, SIGNAL(positionChanged(qint64)),
                                this, SLOT(player_position_upadate(qint64)));

                        vec_playerow_grajaych_teraz.push_back (
                                    Trojka { al.player, al.sekund_dzwieku, al.loop}
                                    );
                        //                        auto itt = vec_playerow_grajaych_teraz.front();
                        //                        vec_playerow_grajaych_teraz.erase(itt);
                    }
                    // Co zrobić, żeby pętla się zarzymała, gdy zamkniemy okno.
                    if(al.loop)
                    {
                        cout << "Wejscie do opcji loop w muzyce" << endl;
                        connect(al.player,
                                SIGNAL(
                                    mediaStatusChanged(QMediaPlayer::MediaStatus)
                                    ),
                                this,
                                SLOT(player_loop_needed(QMediaPlayer::MediaStatus )));
                    }
                    // delete player;
                }
            }

            if(al.flag_program)
            {
                cout << "!!!!!! Przed uruchomieniem zewnetrznego programu  !!!!!!!!" << al.nazwa_programu << endl;
                string komenda =  al.nazwa_programu ;
                komenda = kosmetyka_nazwy_programu(komenda);

                QProcess::startDetached(komenda.c_str()); // nie zatrzyma sekundnika
            }

            if(al.flag_wylacz_komputer)
            {
                cout << "!!!!!! Przed uruchomieniem shutdown !!!!!!!!" << endl;
                make_shutdown();
            }

        } // to alarm

    } // end petli po alarmach
}

//*******************************************************************************************************************
void MainWindow::sygnalizacja_pracujacego_programu()
{

    ofstream plik(nazwa_pliku_sygnalizujacego_prace);

    if(plik)
    {
        //        cout << " zapis biezacych sekund " << time(nullptr)
        //             << "\t" << flag_pozwalam_na_kilka_kopii_program
        //             << " do pliku " << nazwa_pliku_sygnalizujacego_prace
        //             << "\n" << endl;

        plik << time(nullptr) << "\t" << flag_pozwalam_na_kilka_kopii_program;
    }else
    {
        cerr << "blad otwarcia pliku do pisania: " << nazwa_pliku_sygnalizujacego_prace << endl;

    }
}
//********************************************************************************************************************
void MainWindow::zapisz_ulubione_wskazowki_na_dysku()
{
    string def_filename {pathed_nazwa_pliku_z_faworytami};
    ofstream plik(def_filename);
    if(!plik){
        cout << "Can't open file for writing defaults (" << def_filename << endl;
        return;
    }


    for(const auto & e : vec_pref_hands)
    {
        //        cout << "Tarcza " << e.clock_face_name << " lubi wsk godz:  "
        //             << e.hours_hand
        //             << " i wsk min: " << e.minutes_hand
        //             << " i sekundnik:  " << e.seconds_hand
        //             << endl;


#define xkeyword(n)  "\t<" << #n << ">" << e.n << "</" << #n << ">\n"

        plik
                << "<faworyt>\n"
                << xkeyword(clock_face_name)
                << xkeyword(hours_hand)
                << xkeyword(minutes_hand)
                << xkeyword(seconds_hand)
                << xkeyword(monthdays_hand)
                << xkeyword(zoom)
                << "</faworyt>\n"
                << endl;

        // cout << "Zapisany "   << (int) (a.dni_tygodnia) << endl;

    }
}
//******************************************************************************************************
void MainWindow::odczytaj_ulubione_wskazowki_z_dysku()
{

    ifstream plik(pathed_nazwa_pliku_z_faworytami);
    if(!plik)
    {
        cout << "Error while opening file " << pathed_nazwa_pliku_z_faworytami
             << "\nno problem, perhaps it was not defined yet" << endl;
        return;
    }

    string tresc;
    string linijka;
    while(
          getline(plik, linijka))
    {
        tresc += linijka + '\n';
    }

    //

    size_t kursor = 0;
    for(int entry  [[__maybe_unused__]] = 0 ; ; entry++)      // petla po faworytach
    {
        // cout << "obieg nr " << entry << endl;
        // wyjecie jednego alarmu ----------------------
        auto pocz = tresc.find("<faworyt>", kursor);
        kursor = pocz;
        auto kon = tresc.find("</faworyt>", kursor);
        kursor = kon;
        if(pocz == string::npos || kon == string::npos)
        {
            // cout << "Koniec petli bo pocz = " << pocz << ", kon = " << kon << endl;
            break;
        }
        string one = tresc.substr(pocz, kon-pocz);

        // wyjecie poszczegolnych itemow z alarmu -------------------

        auto tarcza = znajdz_txt_item(one, "clock_face_name");
        auto godzinowa = znajdz_txt_item(one, "hours_hand");
        auto minutowa = znajdz_txt_item(one, "minutes_hand");
        auto sek = znajdz_txt_item(one, "seconds_hand");
        auto tyg = znajdz_txt_item(one, "weekdays_hand");
        auto dmies = znajdz_txt_item(one, "monthdays_hand");
        auto zoom = znajdz_int_item(one, "zoom");

        //        cout << "Jedno entry nr " << entry << " ma tresc " << one << endl;
        //        cout << "tarcza " << tarcza
        //                //                     << " | godz = " << godzinowa
        //                //                     << " | min "  << minutowa
        //                //                     << " | sek "  << sek
        //                //                     << " | tyg "  << tyg
        //                //                     << " | dmies "  << dmies
        //             << " zoom = " << zoom
        //             << endl;

        vec_pref_hands.push_back(
                    Tpreferred_hands(
                        tarcza, godzinowa, minutowa, sek, tyg, dmies, zoom));
    }

}
//************************************************************************************************************
// Konstruktor zwykły (bez księżyca)
Tdane_cyferblatu::Tdane_cyferblatu(string nazwa_, string bitmapa_tarcza_,
                                   string bitmapa_godzinowa_, string bitmapa_minutowa_, string bitmapa_sekundnik_,
                                   string bitmapa_central_, double os_x_godzinowej_, double os_y_godzinowej_,
                                   double skala_x_godzinowej_, double skala_y_godzinowej_,
                                   QColor kolor_godzinowej_,
                                   double os_x_minutowej_, double os_y_minutowej_,
                                   double skala_x_minutowej_, double skala_y_minutowej_,
                                   QColor kolor_minutowej_,
                                   double os_x_sekundnika_, double os_y_sekundnika_,
                                   double skala_x_sekundnika_, double skala_y_sekundnika_,
                                   QColor kolor_sekundnika_,
                                   double os_x_center_, double os_y_center_,
                                   double skala_x_center_, double skala_y_center_,
                                   double promien_tarczy_, double odrebna_os_x_,
                                   double odrebna_os_y_,
                                   QPoint pozycja_AM_PM_,
                                   QColor kolor_AM_PM_, QPoint pozycja_dnia_tygodnia_,
                                   QColor kolor_dnia_tygodnia_, QPoint pozycja_daty_, double szer_daty_,
                                   double wys_daty_, QColor kolor_daty_, QColor kolor_ramki_daty_,
                                   QColor kolor_tla_daty_, QColor kolor_cienia_,
                                   double uskok_cienia_,
                                   QColor  tlo_AM_PM_
                                   )

    :  // lista inicjalizacyjna
      nazwa(nazwa_), bitmapa_tarcza(bitmapa_tarcza_),
      bitmapa_godzinowa(bitmapa_godzinowa_),
      bitmapa_minutowa(bitmapa_minutowa_),
      bitmapa_sekundnik(bitmapa_sekundnik_),
      bitmapa_central(bitmapa_central_),

      mimosrod_x_wektorowej_godzinowej(os_x_godzinowej_),
      mimosrod_y_wektorowej_godzinowej(os_y_godzinowej_),
      szer_x_godzinowej(skala_x_godzinowej_),
      dlugosc_godzinowej(skala_y_godzinowej_),
      kolor_godzinowej(kolor_godzinowej_),

      mimosrod_x_wektorowej_minutowej(os_x_minutowej_),
      mimosrod_y_wektorowej_minutowej(os_y_minutowej_),
      szer_x_minutowej(skala_x_minutowej_),
      dlugosc_minutowej(skala_y_minutowej_),
      kolor_minutowej(kolor_minutowej_),

      mimosrod_x_wektorowego_sekundnika(os_x_sekundnika_),
      mimosrod_y_wektorowego_sekundnika(os_y_sekundnika_),
      szerokosc_x_sekundnika(skala_x_sekundnika_),
      dlugosc_y_sekundnika(skala_y_sekundnika_),

      kolor_sekundnika(kolor_sekundnika_),

      os_x_center(os_x_center_),
      os_y_center(os_y_center_),
      skala_x_center(skala_x_center_),
      skala_y_center(skala_y_center_),

      promien_tarczy(promien_tarczy_),

      odrebna_os_x(odrebna_os_x_),  // translacja do odrebnej osi sekundnika (x,y w % promienia)

      odrebna_os_y(odrebna_os_y_),  // translacja do odrebnej osi sekundnika (x,y w % promienia)


      pozycja_AM_PM ( pozycja_AM_PM_),
      kolor_AM_PM(  kolor_AM_PM_),

      pozycja_full_date(pozycja_dnia_tygodnia_),
      kolor_dnia_tygodnia(kolor_dnia_tygodnia_),
      pozycja_datownika(pozycja_daty_),
      szer_datownika(szer_daty_),
      wys_datownika(wys_daty_),
      kolor_datownika(kolor_daty_),
      kolor_ramki_datownika(kolor_ramki_daty_),
      kolor_tla_datownika(kolor_tla_daty_),
      kolor_cienia(kolor_cienia_),
      uskok_cienia(uskok_cienia_),
      tlo_AM_PM (tlo_AM_PM_)
{
    //     cout << "dziala konstruktor" << __PRETTY_FUNCTION__<< endl;

    wsk_sekundowa.poz_x = odrebna_os_x_;
    wsk_sekundowa.poz_y = odrebna_os_y_;
    wsk_sekundowa.kolor = kolor_sekundnika_;




    // Wykryć, czy wskazowka jest bitmapowa czy wektorowa
    // wektorowa ma tylko jeden znak w nazwie bitmapy i jest to cyfra będąca
    // numerem wzoru wektorowego.

    // ustawić flagę mówiącą czy wskazowka fabryczna jest wektorowa czy bitmapowa.

    // sprawdzenie czy ulubiona wsk godzinowa jest cyfrą (czytli wektorowa) -----------------
    if( bitmapa_godzinowa.size() == 1 && isdigit(bitmapa_godzinowa[0] ) )
    {
        flag_fabryczna_wsk_godz_bitmapowa = false;
        // nr_wsk_wektorowej_godz = stoi (bitmapa_godzinowa );
    }

    if( bitmapa_minutowa.size() == 1 && isdigit(bitmapa_minutowa[0] ) )
    {
        flag_fabryczna_wsk_min_bitmapowa = false;
    }

    if( bitmapa_sekundnik.size() == 1 && isdigit(bitmapa_sekundnik[0] ) )
    {
        flag_fabryczna_wsk_sek_bitmapowa = false;
    }
}
//************************************************************************************************************
// Konstruktor z fazą księzyca
Tdane_cyferblatu::Tdane_cyferblatu(string nazwa_, string bitmapa_tarcza_,
                                   string bitmapa_godzinowa_, string bitmapa_minutowa_, string bitmapa_sekundnik_,
                                   string bitmapa_central_, double os_x_godzinowej_, double os_y_godzinowej_,
                                   double skala_x_godzinowej_, double skala_y_godzinowej_,
                                   QColor kolor_godzinowej_,
                                   double os_x_minutowej_, double os_y_minutowej_,
                                   double skala_x_minutowej_, double skala_y_minutowej_,
                                   QColor kolor_minutowej_,
                                   double os_x_sekundnika_, double os_y_sekundnika_,
                                   double skala_x_sekundnika_, double skala_y_sekundnika_,
                                   QColor kolor_sekundnika_,
                                   double os_x_center_, double os_y_center_,
                                   double skala_x_center_, double skala_y_center_,
                                   double promien_tarczy_, double odrebna_os_x_,
                                   double odrebna_os_y_,
                                   double x_ksiezyc_,  // pozycja obrazka z fazą księżyca
                                   double y_ksiezyc_,
                                   double  skala_ksiezyc_,
                                   QPoint pozycja_AM_PM_,
                                   QColor kolor_AM_PM_, QPoint pozycja_dnia_tygodnia_,
                                   QColor kolor_dnia_tygodnia_, QPoint pozycja_daty_, double szer_daty_,
                                   double wys_daty_, QColor kolor_daty_, QColor kolor_ramki_daty_,
                                   QColor kolor_tla_daty_, QColor kolor_cienia_,
                                   double uskok_cienia_,
                                   QColor  tlo_AM_PM_)

    :  // lista inicjalizacyjna
      nazwa(nazwa_), bitmapa_tarcza(bitmapa_tarcza_),
      bitmapa_godzinowa(bitmapa_godzinowa_),
      bitmapa_minutowa(bitmapa_minutowa_),
      bitmapa_sekundnik(bitmapa_sekundnik_),
      bitmapa_central(bitmapa_central_),

      mimosrod_x_wektorowej_godzinowej(os_x_godzinowej_),
      mimosrod_y_wektorowej_godzinowej(os_y_godzinowej_),
      szer_x_godzinowej(skala_x_godzinowej_),
      dlugosc_godzinowej(skala_y_godzinowej_),
      kolor_godzinowej(kolor_godzinowej_),

      mimosrod_x_wektorowej_minutowej(os_x_minutowej_),
      mimosrod_y_wektorowej_minutowej(os_y_minutowej_),
      szer_x_minutowej(skala_x_minutowej_),
      dlugosc_minutowej(skala_y_minutowej_),
      kolor_minutowej(kolor_minutowej_),

      mimosrod_x_wektorowego_sekundnika(os_x_sekundnika_),
      mimosrod_y_wektorowego_sekundnika(os_y_sekundnika_),
      szerokosc_x_sekundnika(skala_x_sekundnika_),
      dlugosc_y_sekundnika(skala_y_sekundnika_),

      kolor_sekundnika(kolor_sekundnika_),

      os_x_center(os_x_center_),
      os_y_center(os_y_center_),

      skala_x_center(skala_x_center_),
      skala_y_center(skala_y_center_),

      promien_tarczy(promien_tarczy_),


      odrebna_os_x(odrebna_os_x_),  // translacja do odrebnej osi sekundnika (x,y w % promienia)
      odrebna_os_y(odrebna_os_y_),  // translacja do odrebnej osi sekundnika (x,y w % promienia)

      x_ksiezyc(x_ksiezyc_),  // pozycja obrazka z fazą księżyca
      y_ksiezyc(y_ksiezyc_),

      skala_ksiezyc(skala_ksiezyc_),

      pozycja_AM_PM ( pozycja_AM_PM_),
      kolor_AM_PM(  kolor_AM_PM_),

      pozycja_full_date(pozycja_dnia_tygodnia_),
      kolor_dnia_tygodnia(kolor_dnia_tygodnia_),
      pozycja_datownika(pozycja_daty_),
      szer_datownika(szer_daty_),
      wys_datownika(wys_daty_),
      kolor_datownika(kolor_daty_),
      kolor_ramki_datownika(kolor_ramki_daty_),
      kolor_tla_datownika(kolor_tla_daty_),
      kolor_cienia(kolor_cienia_),
      uskok_cienia(uskok_cienia_),
      tlo_AM_PM(tlo_AM_PM_)
{
    //     cout << "dziala konstruktor z ksiezycem " << endl;
    //         cout << "dziala konstruktor" << __PRETTY_FUNCTION__<< endl;

    wsk_sekundowa.poz_x = odrebna_os_x_;
    wsk_sekundowa.poz_y = odrebna_os_y_;
    wsk_sekundowa.kolor = kolor_sekundnika_;



    // Wykryć, czy wskazowka jest bitmapowa czy wektorowa
    // wektorowa ma tylko jeden znak w nazwie bitmapy i jest to cyfra będąca
    // numerem wzoru wektorowego.

    // ustawić flagę mówiącą czy wskazowka fabryczna jest wektorowa czy bitmapowa.

    // sprawdzenie czy ulubiona wsk godzinowa jest cyfrą (czytli wektorowa) -----------------
    if( bitmapa_godzinowa.size() == 1 && isdigit(bitmapa_godzinowa[0] ) )
    {
        flag_fabryczna_wsk_godz_bitmapowa = false;
        // nr_wsk_wektorowej_godz = stoi (bitmapa_godzinowa );
    }

    if( bitmapa_minutowa.size() == 1 && isdigit(bitmapa_minutowa[0] ) )
    {
        flag_fabryczna_wsk_min_bitmapowa = false;
    }

    if( bitmapa_sekundnik.size() == 1 && isdigit(bitmapa_sekundnik[0] ) )
    {
        flag_fabryczna_wsk_sek_bitmapowa = false;
    }
    flaga_ksiezyc = true;
}
//**********************************************************************************************************
void Tdane_cyferblatu::dodaj_extra_wskazowke(
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
        int warstwa // na jakiej warstwie rysowac
        , std::function<double ()> lambda_obliczenie_kata
        )
{
    //    cout << __func__
    //         << "  nazwa wskazowki  " << nazwa
    //         << " nazwa bitmapa = " << bitmapa
    //         << endl;

    wsk->flaga_enable = true;

    wsk->poz_x = os_x;
    wsk->poz_y = os_y;

    wsk->nazwa = nazwa;
    wsk->bitmapa = bitmapa;

    wsk->mimosrod_x = mimosrod_x;
    wsk->mimosrod_y = mimosrod_y;
    wsk->skala_x  = skala_x;
    wsk->dlugosc = dlugosc;
    wsk->kolor = kolor;
    wsk->flag_bitmapa_moze_byc_kolorowana = flag_bitmapa_moze_byc_kolorowana;
    wsk->warstwa = warstwa;
    wsk->obliczenie_kata = lambda_obliczenie_kata;
    wsk->odleglosc_cienia = odl_cienia;
    wsk->pix_ptr = pixmapa;
    wsk->rola_wskazowki =  extra;

    switch(extra){
    case  Rola_wskazowki::wsk_full_date:
        flaga_wsk_dnia_tygodnia = true;

        break;
    case  Rola_wskazowki::wsk_dzien_miesiaca:
        flaga_wsk_dnia_miesiaca = true;

        break;

    case  Rola_wskazowki::wsk_miesiaca:
        flaga_wsk_miesiaca = true;

        break;

    case  Rola_wskazowki::tarcza_night_day:
        flaga_wsk_night_day = true;

        break;
    case  Rola_wskazowki::tarcza_zones:
        flaga_wsk_zones = true;
        break;

    case  Rola_wskazowki::wsk_sekundnik_dodatkowy:
        flaga_wsk_sekundnik_dodatkowy = true;
        break;

    default:
        break;
    }

}
//**********************************************************************************************************
void Tdane_cyferblatu::zapis_danych_o_cyferblacie()
{
    string q = "\"";
    cout
            << q << nazwa << q << ", \t// nazwa cyferblatu \n"
            << q << bitmapa_tarcza << q << ",  \t// nazwa bitmapy_tarczy \n"
            << q << bitmapa_godzinowa  << q << ", \t// nazwa_bitmapy_wskazowki_godzinowej \n"
            << q << bitmapa_minutowa << q << ", \t// nazwa_bitmapy_wskazowki_minutowej \n"
            << q << bitmapa_sekundnik << q << ", \t// nazwa_bitmapy_wskazowki_sekundowej  \n"
            << q << bitmapa_central  << q << ", \t// nazwa_bitmapy_zaslepki centralnej   \n\n"

            <<  os_x_center  << " ,  "
             << os_y_center << " ,  \t// srodek tarczy na bitmapie  x, y \n"
             << promien_tarczy << " , \t// promien tarczy (liczony do rozmiarow sekund) \n"

             <<  mimosrod_x_wektorowej_godzinowej << " , \t// X mimosrod wektorowej wskazowki_godzinowej  \n"
              << mimosrod_y_wektorowej_godzinowej << " , \t// Y mimosrod wektorowej wskazowki_godzinowej \n"
              << szer_x_godzinowej  << ",  \t// szerokosc wsk godzinowej (w %)\n"
              << dlugosc_godzinowej  << ",  \t// dlugosc wsk godzinowej (w %)\n"
              << kolor_godzinowej.red() << ", "
              << kolor_godzinowej.green()  << ", "
              << kolor_godzinowej.blue()  << ",  \t// kolor_wsk_godzinowej ( RGB) \n\n"

              <<  mimosrod_x_wektorowej_minutowej <<" , \t// X mimosrod wektorowej wskazowki_minutowej \n"
               << mimosrod_y_wektorowej_minutowej << " , \t// Y mimosrod wektorowej wskazowki_minutowej  \n"
               << szer_x_minutowej  << ",  \t// szerokosc wsk minutowej (w %)\n"
               << dlugosc_minutowej  << ",  \t// dlugosc wsk minutowej (w %)\n"
               << kolor_minutowej.red() << ", "
               << kolor_minutowej.green()  << ", "
               << kolor_minutowej.blue()  << ",  \t// kolor_wsk_minutowej ( RGB) \n\n"



               <<  mimosrod_x_wektorowego_sekundnika <<" , \t// X mimosrod wektorowej wskazowki_sekundowej (w %)\n"
                << mimosrod_y_wektorowego_sekundnika <<" , \t// X mimosrod wektorowej wskazowki_sekundowej (w %)\n"
                << szerokosc_x_sekundnika  << ",  \t// szerokosc wsk sekundnika (w %) \n"
                << dlugosc_y_sekundnika  << ",  \t// dlugosc wsk sekundnika (w %)\n"
                << kolor_sekundnika.red() << ", "
                << kolor_sekundnika.green()  << ", "
                << kolor_sekundnika.blue()  << ",  \t// kolor_wsk_sekundnika ( RGB) \n\n"


                   // zaślepka osi
                << skala_x_center << " ,  \t// skala x zaslepki srodka tarczy (w %) \n"
                << skala_y_center  << " ,  \t// skala y zaslepki srodka tarczy (w %)\n"

                << odrebna_os_x << "  ,  \t// translacja do odrebnej osi sekundnika (x, w % promienia) \n"
                << odrebna_os_y << " ,  \t// translacja do odrebnej osi sekundnika (y w % promienia)\n\n"

                   // teksty ---------------

                <<  pozycja_AM_PM.x() << " , "
                 <<  pozycja_AM_PM.y() << " ,    \t// pozycja napisu AM/PM   (w %) \n"

                  << kolor_AM_PM.red() << " , "
                  << kolor_AM_PM.green() << " , "
                  << kolor_AM_PM.blue() << " , "
                  << kolor_AM_PM.alpha() << " ,  \t // kolor napisu AM_PM  RGBA \n "

                  << tlo_AM_PM.red() << " , "
                  << tlo_AM_PM.green() << " , "
                  << tlo_AM_PM.blue() << " , "
                  << tlo_AM_PM.alpha() << " , \t// kolor RGBA tla  napisu AM_PM  \n "

                  << pozycja_full_date.x() << " , "
                  << pozycja_full_date.y() <<  ",   \t// pozycja pelnej daty X,Y \n"
                  << kolor_dnia_tygodnia.red() << " , "
                  << kolor_dnia_tygodnia.green() << " , "
                  << kolor_dnia_tygodnia.blue() << " , "
                  << kolor_dnia_tygodnia.alpha() << " ,  \t// kolor RGBA  napisu pelnej daty \n"


                     // pozycja Daty
                  << pozycja_datownika.x() << " ,  "
                  << pozycja_datownika.y() << " ,  \t// pozycja XY datownika (dzien miesiaca) \n"
                  << szer_datownika << " ,  \t// szerokosc okienka datownika (%) \n"
                  << wys_datownika  << " , \t// wysokosc okienka datownika  i  wys fontu  (w % dlugosci promienia) \n"
                  <<   kolor_datownika.red()  << " , "
                    <<   kolor_datownika.green()  << " , "
                      <<   kolor_datownika.blue() << " , "
                        <<   kolor_datownika.alpha() << " , \t// kolor RGBA cyfr datownika \n"

                          << kolor_ramki_datownika.red()  << " , "
                          << kolor_ramki_datownika.green()  << " , "
                          << kolor_ramki_datownika.blue()  << " , "
                          << kolor_ramki_datownika.alpha()  << " ,  \t// kolor RGBA ramki datownika \n"

                          << kolor_tla_datownika.red() << " , "
                          << kolor_tla_datownika.green() << " , "
                          << kolor_tla_datownika.blue() << " , "
                          << kolor_tla_datownika.alpha() << " ,   \t// kolor RGBA tla datownika \n"

                          << kolor_cienia.red() <<  " , "
                          << kolor_cienia.green() <<  " , "
                          << kolor_cienia.blue() <<  " , "
                          << kolor_cienia.alpha() <<  " ,    \t// kolor RGBA cienia wskazowek \n"


                          << uskok_cienia <<  " ,   \t// Uskok cienia wskazowek \n"
                             // KSIEZYC
                          << flaga_ksiezyc <<  " ,  \t// flaga_ksiezyc \n"
                          <<  x_ksiezyc <<  ", "
                           <<  y_ksiezyc <<  ",  \t// pozycja x y ksiezyca \n"
                            << skala_ksiezyc  <<  ",  \t//  wsp skali bitmapy ksiezyca \n"

                            << endl;
    //         cout << "KOniec konstruktora " << __PRETTY_FUNCTION__<< endl;
}
//**********************************************************************************************************
void MainWindow::make_shutdown()
{
    QProcess proc;

#ifdef Q_OS_WINDOWS

    // Dla Windowsow ----------------------
    QStringList argumenty1;
    // litera L - mala robi jedynie logout.
    // Do powier off trzeba chyba opcji /s
    argumenty1 << "/s";

    proc.startDetached("shutdown" , argumenty1) ;

    //cout << "Command executed " << endl;
    if(!proc.waitForStarted())
    {
        // cout << "After waitForStarded " << endl;
    }
#elif defined      Q_OS_LINUX
    // Dla linuxa ------------------------------
    QStringList argumenty2;
    argumenty2 << "--poweroff" ;

    proc.startDetached("shutdown" , argumenty2) ;

    if(!proc.waitForStarted()){
        cout << "After waitForStarded 2 " << endl;

    }
#endif

}
//**********************************************************************************************************

void MainWindow::wybierz_obrazek_ksiezyca()
{
    static int orbit = 2953;   // 29.530 pomnożone przez 100 żeby możliwa była operacja modulo

    //    QDate day_of_newmon(2020, 4, 23);  // Jul 5, 2020 - was a new moon
    //QDate day_of_newmon(2020, 9, 17);  // Sep, 17  2020 (13:00) - was a new moon
    QDate day_of_newmon(2024, 01, 11);

    int days_from_newmoon = day_of_newmon.daysTo(now_today);          // returns days from some known newmoon

    // obliczenie faza, czyli ile dni po ostatnim nowiu
    double faza =( (100 * days_from_newmoon) % orbit) / 100.0;

    //        cout
    //                << "days_from_newmoon = " << days_from_newmoon
    //                << ", Dni po nowiu (faza) = " << faza  << endl;


    int faza_int =  faza  +0.5 ;

    // nr_biezacej_fazy_ksiezyca <--- czyli dotychczas wyświetlanej  !


    //    static int nday;
    //    faza_int = (nday++) % 29;
    //    COTO;
    //        cout
    //                << "nr_biezacej_fazy_ksiezyca = " << nr_biezacej_fazy_ksiezyca
    //                << ", faza_int = " << faza_int  << endl;

    if(faza_int == nr_biezacej_fazy_ksiezyca)
        return ; // nie trzeba zmieniać bitmapy księzyca

    nr_biezacej_fazy_ksiezyca = faza_int;

    //--------------
    //        COTO;
    //    cout << " faza_int = " << faza_int << endl;
    //    cout << "rozmaira kaltki_faz = " << klatki_faz.size() << endl;
    //     cout << " modulo = "  << (faza_int % 29) << endl;
    //    COTO;

    auto nr_obrazka = klatki_faz[faza_int % 29] ;   // % zeby nie przekroczyc rozmiaru tablicy
    //string  nazwa = (string)(":/new/prefix1/content/Moon_day_" )+ nr_obrazka + ".png";
    string  nazwa = (string)(":/new/prefix1/content/moon_" )+ nr_obrazka + ".png";


    //    cout << ", Dni po nowiu = " << faza
    //                     << ", faza_int = "
    //                     << faza_int
    //                    << " nr_obrazka = " <<  nr_obrazka
    //                     << endl;

    //             cout << ", Potrzebny plik " << nazwa  << endl;

    ksiezyc.load(nazwa.c_str());

}
//**********************************************************************************************************
void MainWindow::sortowanie_warstw()
{
    // sortowanie tablicy wskaznikow do wskazowek, tak by potem
    // móc rysować począwszy od najniższych warstw do najwyższych


    //    for(uint i = 0 ; i < cyf.pointery_wskazowek.size() ; ++i)
    //    {
    //        cout << "Warstwa "  <<  cyf. pointery_wskazowek[i]->warstwa
    //                     << endl;
    //    }

    sort(
                cyf. pointery_wskazowek.begin(),
                cyf. pointery_wskazowek.end(),
                [] (Tdane_wskazowki * w1,  Tdane_wskazowki *w2)   // funkcja lambda
    { return (w1->warstwa < w2->warstwa); }
    );


    //    for(uint i = 0 ; i < cyf.pointery_wskazowek.size() ; ++i)
    //    {
    //        cout << "Warstwa "  <<  cyf. pointery_wskazowek[i]->warstwa
    //                     << endl;
    //    }

}
//*********************************************************************************************************
int  MainWindow::random_cyferblat()
{
    std::mt19937_64 dre(time(0) );

    // use engine to generate integral numbers between 10 and 20 (both included)
    int zakres =cyferblat.size()-1;

    std::uniform_int_distribution<int> di(0, zakres);
    nr_tarczy = di(dre);
    flag_random_tarcza = true;


    return nr_tarczy;

}
//*********************************************************************************************************
// this function helps to know which resource file are really in use
// (to distinguish them from these old files, which can be removed from the resouce file
void MainWindow::resource_files_use()
{


    vector <string> used_bitmaps;

    for (auto &x : cyferblat) used_bitmaps.push_back( x.bitmapa_tarcza);
    for (auto &x : vect_godzinowych) used_bitmaps.push_back( x.bitmapa  );
    for (auto &x : vect_minutowych) used_bitmaps.push_back( x.bitmapa);
    for (auto &x : vect_sekundnikow) used_bitmaps.push_back( x.bitmapa);
    for (auto &x : vect_extra_wskazowek) used_bitmaps.push_back( x.bitmapa);

    sort(
                used_bitmaps.begin(),
                used_bitmaps.end()
                //                ,
                //                [] (string  w1,  string  w2)   // funkcja lambda
                //    { return (w1 < w2); }
                );

    vector <string> raport_extra;
    for (auto &x : vect_extra_wskazowek) raport_extra.push_back( x.bitmapa);

    // W cyferblacie są zapisane tzw. extra wskazowki (tarcze np. day/night). Teraz sprawdzimy
    // czy w wektorze extra wsk są elementy, ktorych tarcze nigdy nie skorzystaja
    for (auto &x : cyferblat) {

        //     bool flag_in_use = false;
        for(auto & r : raport_extra){
            if(r == x.wsk_zones.bitmapa )  // tzn jest użyty
            {
                //             flag_in_use = true;
                r += "  <-- extra used by: " + x.bitmapa_tarcza ;
            }

            if(r == x.wsk_miesiaca.bitmapa )   r += "  <-- extra used by: " + x.bitmapa_tarcza ;
            if(r == x.wsk_night_day.bitmapa )   r += "  <-- extra used by: " + x.bitmapa_tarcza ;
            if(r == x.wsk_dni_miesiaca.bitmapa )   r += "  <-- extra used by: " + x.bitmapa_tarcza ;
            if(r == x.wsk_dni_tygodnia.bitmapa )   r += "  <-- extra used by: " + x.bitmapa_tarcza ;
            if(r == x.wsk_sekundnik_dodatkowy.bitmapa )   r += "  <-- extra used by: " + x.bitmapa_tarcza ;
        }
    }

    //    cout << "\n\nRaport only EXTRA , (case sensitive)" << endl;
    //    for(auto x : raport_extra)
    //    {
    //        cout << x << endl;
    //    }

    //    cout << "\n\nRaport (full) , (case sensitive)" << endl;
    //    for(auto x : used_bitmaps)
    //    {
    //        cout << x << endl;
    //    }

    // ---------------------------------------------
    cout << "\n\n1. Checking if every Disk file (from 'content' sudirectory) is used in resource file" << endl;
    // a przynajmniej czy jest potem użyty w programie

    bool flag_some_issues = false;

    QDir katalog(R"#(content)#" );
    katalog.setFilter(QDir::Files);
    QStringList listaf = katalog.entryList();

    QDir res(R"#(:/new/prefix1/content)#" );
    QStringList resource_entry = res.entryList();

    for(auto &n : listaf)
    {
        //       cout << n.toStdString() << endl;
        // is this file used in resource
        //        cout << "czy ten file jest uzyty w resources ?" << endl;

        bool flag_yes = false;
        for(auto &r : resource_entry)
        {
            // cout << n.toStdString() << endl;
            // sprawdzenie czy ten file jest w resource
            if(n == r){
                flag_yes = true;
                break;
            }
        }
        if(flag_yes == false)
        {
            flag_some_issues = true;
            cout << "   Disk file " <<  n.toStdString()
                 << " not used in your resource " << endl;
        }
    }
    if(flag_some_issues == false)
        cout << "   OK, all disk files from contents subdirectory files are in use in the resource file " << endl;

    //===================================================
    cout << "2. Checking if every resource item used in the program " << endl;

    flag_some_issues = false;
    for(auto &res_ent : resource_entry)
    {
        // cout << n.toStdString() << endl;
        // sprawdzenie czy ten file jest w uzyciu

        // Najpierw wyjątki, których nie ma w wektorach wskazowek i tarcz
        if(res_ent.toStdString().find("Moon_day") != string::npos)
            continue;
        if(res_ent.toStdString().find("center.png") != string::npos)
            continue;
        if(res_ent.toStdString().find("pencil.png") != string::npos)
            continue;
        if(res_ent.toStdString().find("zegar_ikona.png") != string::npos)
            continue;


        // koniec wyjątków ----------------------

        bool flag_yes = false;

        for(auto &ub : used_bitmaps){
            //            cout << "Porownanie res " <<  res_ent.toStdString()
            //                 << " z used_bitmaps " << ub << endl;
            auto sama_nazwa = res_ent.toStdString();

            //            cout << "czy w "
            //                 << ub
            //                 << " jest  "
            //                 << sama_nazwa
            //                 << endl;

            //            if(sama_nazwa.find(ub) != string::npos)
            if(ub.find(sama_nazwa) != string::npos)
            {
                flag_yes = true;
                // cout << "TAK !!!!!!!!!!!!!!!!!!!!! " << sama_nazwa << " jest w programie " << endl;
                break;
            }
        }
        if(flag_yes == false)
        {
            cout << "   Resource File " <<  res_ent.toStdString()
                 << " is not used in program  " << endl;
            flag_some_issues = true;
        }
    }
    if(flag_some_issues == false)
        cout << "   OK, all the resource files are in use in the program" << endl;



}
//******************************************************************************************
void MainWindow::createTrayActions_i_context_menu()   // akcje jak pull up menu
{
#if 0
#endif
    //    minimizeAction = new QAction(tr("Mi&nimize"), this);
    //    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    //    maximizeAction = new QAction(tr("Ma&ximize"), this);
    //    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore on screen"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit Zegar program"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}
//******************************************************************************************
void MainWindow::createTrayIcon_i_dodaj_context_menu()   // tworzenie i dodanie pullup menu
{
    trayIconMenu = new QMenu(this);
#if 0
    trayIconMenu->addAction(minimizeAction);
    //  trayIconMenu->addAction(maximizeAction);
#endif
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);

    QIcon icon(":/new/prefix1/content/zegar_ikona.png"), tr("zegar - a clock reminder");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip(("'Zegar', a clock-reminder "));   // <- nazwa tekstowa ikonki


    trayIcon->setContextMenu(trayIconMenu);

}
#if 0
//******************************************************************************************
void MainWindow::closeEvent(QCloseEvent *event)
{

    // cout<< __PRETTY_FUNCTION__ << endl;
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
    if( flag_ma_byc_restart != true)     // gdy to nie jest konieczny restart
    {
        if (trayIcon->isVisible()) {
            QMessageBox::information(this, tr("Systray"),
                                     tr("The program will keep running in the "
                                        "system tray. To terminate the program, "
                                        "choose <b>Quit</b> in the context menu "
                                        "of the system tray entry."));
            hide();
            event->ignore();
        }
    }
    else{
        cout << "To ma byc restart " << endl;
        close();
    }

}
#endif   // 0
