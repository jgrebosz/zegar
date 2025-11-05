#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QProcess>
#include <fstream>

#include <string>

bool flag_ma_byc_restart;

extern bool flag_na_wierzchu;
extern bool wstepne_flag_na_wierzchu;
extern int nr_jezyka;
extern int wstepne_nr_jezyka;
extern int nr_obecnie_zainstalowanego_jezyka;
string nazwa_pliku_sygnalizujacego_prace {"zegar_is_active.dat" } ;
const int CO_ILE_SEK_SPRAWDZENIE_AKTYWNOSCI { 10 };
string sciezka;
//********************************************************************************
void system_srodowsko()
{
    // cout << "srodowisko " << endl;

    //! UWAGA: Nawet jeśli dowiem się ile kopii programu właśnie
    //! się wykonuje, to nie powinno mi przeszkadzać jeśli są one
    //! w róznycha katalogoach.
    //! Tylko jak dowiedzieć się w którym katalogu która kopia pracuje?
    //!
    //!
    //!
    //!
    //!


    //--------------------

    const QString &processname {"zegar*"};
    //    {"Av*"};    // wykryje Avasty

    QProcess tasklist;

#ifdef Q_OS_WINDOWS

    // TAK JEST DLA WINDOWS
    tasklist.start(
        "tasklist",
        QStringList() << "/NH"
                      << "/FO"
                      << "CSV"
                      << "/FI"
                      << QString("IMAGENAME eq %1").arg(processname)
        )

        ;
#elif defined      Q_OS_LINUX

    //       ps axco pid,command

    tasklist.start(
        "ps",
        QStringList() << "/NH"
                      << "/FO"
                      << "CSV"
                      << "/FI"
                      << QString("IMAGENAME eq %1").arg(processname));
#endif
    tasklist.waitForFinished();
    QString output = tasklist.readAllStandardOutput();

    auto wynik = output.startsWith(QString("\"%1").arg(processname));
    cout << "output = " << output.toStdString()
         << "\nwynik..." << wynik << endl;



    //      Do you want to do this on Windows or Linux or a combination of operating systems?
    //      The basic approach is to search the list of active processes for your executable.
    //      For Windows use:
    //      tasklist /fo csv /nh

    //      For Linux use:
    //      ps axco pid,command

    //      You would need to parse the results correctly for the particular OS.

    //--------------------
    // !
    // !ambershark Moderators 2 mar 2017, 03:51


    //=========================================
    //!      @Heryon For windows you can use this api
    //! call https://msdn.microsoft.com/en-us/library/windows/desktop/ms682623(v=vs.85).aspx.

    // !     And for linux I would recommend going straight to /proc rather than using ps.
    //! Using ps is fine, don't get me wrong, it's just having to allocate
    //! and run another process rather than just using the filesystem.
    //! Takes a lot more resources and time. There are open source libraries to help you
    //!  read /proc if you don't want to do it yourself. Something like http://procps.sourceforge.net.
    //!  I've never used that, just the first thing I found in google.

}
//********************************************************************************
bool czy_wolno_uruchomic(char * arg0)
{
    //! KDE (Linux) mając autostart z tym programem niepotrzebnie
    //! uruchamia dodatkowo ten program (skoro przy wylogowaniu
    //! był włączony.
    //! Robi to jednak w głównym domowym katalogu. To jest sposób
    //! by temu zapobiec
    //! Sprawdzam, czy w katalogu w który jest uruchomiony program
    //! jest sam ten program. Jeśli go nie ma, to znaczy że to może
    //! być owo dzikie uruchomienie, którego nie chcę
    //! NIESTETY zdarza się w Qtcreator Windows, że program jest
    //! w podkatalogu Release lub Debug.
    //! Na wszelki wypadek to sprawdzam także

    cout << "Nazwa programu ze sciezka "<< arg0 << endl;
    string nazwa_programu_ze_sciezka { arg0};
    string sama_nazwa_programu;
    string slesz =   // w windosach i linuxie są różne
#ifdef Q_OS_WINDOWS
        "\\";
#elif defined      Q_OS_LINUX
        "/";
#else   // nie windows (a co z Mac-iem)
#error "Oparating system not recognised "
#endif

    size_t pozycja = nazwa_programu_ze_sciezka.rfind(slesz);  // może backslash?

    if( pozycja != string::npos)
    {
        sama_nazwa_programu = nazwa_programu_ze_sciezka.substr(pozycja+1);
        // cout << "sama_nazwa_programu = "  << sama_nazwa_programu << endl;
    }

    // cout << "Sciezka z QDir  "<< sciezka << endl;
    //    czy w pod ta sciezka jest sam program?

    string wlasciwy = sama_nazwa_programu;
    string deb_progr = "Debug" + slesz + sama_nazwa_programu;
    string rel_progr = "Release" + slesz + sama_nazwa_programu;
    QDir dir;

    if(
        dir.exists(wlasciwy.c_str())
        ||
        dir.exists(deb_progr.c_str())
        ||
        dir.exists(rel_progr.c_str())
        )
    {
        //        OK
    }
    else
    {
        cout
            << "w biezacym katalogu " << sciezka
            << "\n(ani w podkatalogach  Release, Debug)   nie ma  pliku programowego ="
            << sama_nazwa_programu
            << endl;;
        return false;
    }


    // sprawdzić czy taki program już chodz/ iw bieżącym katlogu
    // jeśli tak, to nowego nie uruchamiamy
    // (jest taka specjalna opcja, która na pozwoli.)

    nazwa_pliku_sygnalizujacego_prace = sciezka + nazwa_pliku_sygnalizujacego_prace;
    ifstream plik(nazwa_pliku_sygnalizujacego_prace);

    if(plik)
    {   // jesli plik jest to może jest za stary
        //        cout << "Plik sygnalizujacy aktywnosci istnieje " << endl;
        time_t czas;
        bool czy_pozwalam = true;
        plik >> czas >> czy_pozwalam;
        auto roznica_czasu = time(nullptr) - czas;
        if( roznica_czasu >  (CO_ILE_SEK_SPRAWDZENIE_AKTYWNOSCI * 1.2) )
        {
            //            cout << " Jest jakis stary wpis dot. aktywnosci, ale to bylo "
            //                 << roznica_czasu << " s temu, "
            //                                     "\nwiec mozna uznac, ze skoro program dawno przestal dzialac to mozemy uruchomic ten "
            //                 << endl;
        }else
        {
            cout << "Juz program Zegar w tym katalogu pracowal " << roznica_czasu << " sekund temu ... "
                 << endl;
            cout << "\nczy jest pozwolenie na drugi " << czy_pozwalam << endl;
            if(czy_pozwalam == false)
            {
                cout << " wiec nie pozwalamy na drugi. Koniec programu" << endl;
                return false;  // konczymy ten program zeby nie było dwóch
            }
            // No chyba, że pozwalamy na to
        }
    }else{
        // jeśli pliku sygnalizacyjnego nie ma, to OK
        // cout << "Pliku sygnalizacyjnego nie ma, mozemy startowac program " << endl;

    }
    plik.close();
    return true; // wolno uruchomic

}
//************************************************************************************
int main(int argc, char *argv[])
{

    //    system_srodowsko();

    nr_jezyka = 0;

    QDir dir;
    sciezka = QDir::currentPath().toStdString() + "/";

    if(czy_wolno_uruchomic(argv[0]) == false) return 0;

    // At first let`s select the language
    pathed_nazwa_pliku_z_opcjami = sciezka + nazwa_pliku_z_opcjami;
    ifstream file(pathed_nazwa_pliku_z_opcjami);
    file >> nr_jezyka ;
    file.close();
    cout << "wczytany nr jezyka = " << nr_jezyka << endl;

    //  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); <-- deprecated


    while(1) {
        QApplication a(argc, argv);

        QTranslator t;

        switch (nr_jezyka)
        {
        case 0:
            qDebug() << "Język nr 0 (domyślny, angielski)";
            break;

        case 1:
        default:
        {
            QString nazwa_jezyka = ":/polski2.qm";
            qDebug() << "Wybrany język: " << nazwa_jezyka;

            bool rezultat = t.load(nazwa_jezyka);
            qDebug() << "Wynik wczytania: " << rezultat;

            if (rezultat)
                a.installTranslator(&t);
            break;
        }
        }


        nr_obecnie_zainstalowanego_jezyka = nr_jezyka;

        MainWindow w;
        w.show();
        //        cout << __LINE__ << " przed w.exec() " << endl;
        int answer = a.exec();
        //  cout << __LINE__ << " PO w.exec() " << endl;
        cout << "main,  flag_ma_byc_restart = "<< boolalpha
             << flag_ma_byc_restart << endl;
        if(flag_ma_byc_restart )
        {
            cout << "[main.c ] Powtorne pokazanie okna, z opcją flag_ma_byc_restart,   oraz  flag_na_wierzchu= "
                 << flag_na_wierzchu
                 << " i jezykiem nr " << nr_jezyka << endl;

            w.setWindowFlag(Qt::WindowStaysOnTopHint, flag_na_wierzchu );
            w.setWindowFlag(Qt::X11BypassWindowManagerHint, flag_na_wierzchu);


            w.setWindowState(w.windowState() ^ Qt::WindowNoState);
            flag_ma_byc_restart = false;
            // kontynuuj nieskonczona petle
        }
        else
        {
            cout << "Usuwam plik sygnalizacyjny " << nazwa_pliku_sygnalizujacego_prace << endl;
            dir.remove(nazwa_pliku_sygnalizujacego_prace.c_str());
            return answer;   // zwykle opuszczenie programu
        }
    }  // kon while(1)

}
