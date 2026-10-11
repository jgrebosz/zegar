#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>
#include <QDir>
#include <fstream>

#include <string>

// bool flag_ma_byc_restart;

extern bool flag_na_wierzchu;
extern bool wstepne_flag_na_wierzchu;
extern int nr_jezyka;
// extern int wstepne_nr_jezyka;
// extern int nr_obecnie_zainstalowanego_jezyka;
string nazwa_pliku_sygnalizujacego_prace {"zegar_is_active.dat" } ;
const int CO_ILE_SEK_SPRAWDZENIE_AKTYWNOSCI { 10 };
// Ustawiane tuż przed startem nowej kopii (XWayland). Stara kopia nie może
// wtedy skasować zegar_is_active.dat, bo nowa już z niego korzysta.
bool flag_nowa_kopia_przejmuje_zamek = false;
string sciezka;
//********************************************************************************
// Qt nie ma funkcji do przeglądania innych procesów (ani na Windows, ani na
// Linuxie, ani na macOS). Da się odczytać tylko tę kopię: pid, katalog roboczy
// i ścieżkę pliku. Wołać po utworzeniu QApplication, inaczej ścieżka pliku jest pusta.
// Inna kopia w tym samym katalogu jest sprawdzana w czy_wolno_uruchomic()
// przez plik zegar_is_active.dat.
void system_srodowsko()
{
    const qint64 pid = QCoreApplication::applicationPid();
    const QString katalog = QDir::currentPath();
    const QString plik = QCoreApplication::applicationFilePath();

    cout << "pid " << pid
         << "\nkatalog " << katalog.toStdString()
         << "\nplik " << plik.toStdString()
         << endl;
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

    // cout << "Nazwa programu ze sciezka "<< arg0 << endl;
    string nazwa_programu_ze_sciezka { arg0};
    string sama_nazwa_programu;
    string slesz =
#ifdef Q_OS_WINDOWS
        "\\";
#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
        "/";
#else
#error "Operating system not recognised"
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
QTranslator translator;
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
    int na_wierzchu_z_pliku = 0;
    {
        ifstream file(pathed_nazwa_pliku_z_opcjami);
        int pomin_przezroczystosc = 0;
        int pomin_przezroczystosc_kursora = 0;
        file >> nr_jezyka
                >> pomin_przezroczystosc
                >> pomin_przezroczystosc_kursora
                >> na_wierzchu_z_pliku;
        (void)pomin_przezroczystosc;
        (void)pomin_przezroczystosc_kursora;
    }
    // cout << "wczytany z pliku nr jezyka = " << nr_jezyka << endl;
    if(nr_jezyka < 0 ) nr_jezyka = 0 ;

#ifdef Q_OS_LINUX
    // GNOME na Waylandzie nie ma prośby „zawsze na wierzchu” dla klienta.
    // Okno XWayland dostaje _NET_WM_STATE_ABOVE, więc przy włączonej opcji
    // zostajemy przy X11. QT_QPA_PLATFORM ustawione przez użytkownika zostawiamy.
    const bool sesja_wayland =
            qgetenv("XDG_SESSION_TYPE") == "wayland"
            || !qgetenv("WAYLAND_DISPLAY").isEmpty();
    if(na_wierzchu_z_pliku
            && sesja_wayland
            && qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")
            && !qgetenv("DISPLAY").isEmpty())
    {
        qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    }
#endif

    //  QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); <-- deprecated

    // Restart całego QApplication (zmiana języka / flagi "zawsze na wierzchu")
    // jest zakomentowany niżej. Pętla while(1) i tak wychodziła przy pierwszym return,
    // więc program startuje raz.
    QApplication a(argc, argv);

    // MainWindow jest oknem Qt::Tool (patrz konstruktor). Okno narzędziowe
    // nie liczy się jako okno trzymające proces przy życiu. Dialog opcji
    // (Topcje_dlg) jest zwykłym Qt::Dialog z WA_QuitOnClose, więc accept()
    // i reject() z buttonBox zamykają ostatnie „prawdziwe” okno i Qt
    // wychodzi z a.exec() — program kończy się w return poniżej.
    // Zegar ma żyć dalej (wyjście jest jawne w koniec_programu() / tray Quit).
    a.setQuitOnLastWindowClosed(false);

    switch (nr_jezyka)
    {
    case 0:
        // qDebug() << "Jezyk nr 0 (domyslny, angielski)";
        break;

    case 1:
    default:
    {
        QString nazwa_jezyka = ":/polski2.qm";
        qDebug() << "Wybrany język: " << nazwa_jezyka;

        qDebug() << "Zasób istnieje:" << QFile(":/polski2.qm").exists();
        bool rezultat = translator.load(nazwa_jezyka);
        qDebug() << "Wynik wczytania: " << rezultat;

        if (rezultat)
            a.installTranslator(&translator);
        break;
    }
    }


    // nr_obecnie_zainstalowanego_jezyka = nr_jezyka;

    MainWindow w;
    w.show();
    //        cout << __LINE__ << " przed w.exec() " << endl;
    int answer = a.exec();
    //  cout << __LINE__ << " PO w.exec() " << endl;
    // cout << "main,  flag_ma_byc_restart = "<< boolalpha
    //      << flag_ma_byc_restart << endl;
    // if(flag_ma_byc_restart )
    // {
    //     cout << "[main.c ] Powtorne pokazanie okna, z opcją flag_ma_byc_restart,   oraz  flag_na_wierzchu= "
    //          << flag_na_wierzchu
    //          << " i jezykiem nr " << nr_jezyka << endl;

    //     w.setWindowFlag(Qt::WindowStaysOnTopHint, flag_na_wierzchu );
    //     w.setWindowFlag(Qt::X11BypassWindowManagerHint, flag_na_wierzchu);


    //     w.setWindowState(w.windowState() ^ Qt::WindowNoState);
    //     flag_ma_byc_restart = false;
    //     // kontynuuj nieskonczona petle
    // }
    // else
    {
        // cout << "Usuwam plik sygnalizacyjny " << nazwa_pliku_sygnalizujacego_prace << endl;
        if(!flag_nowa_kopia_przejmuje_zamek)
            dir.remove(nazwa_pliku_sygnalizujacego_prace.c_str());
        return answer;   // zwykle opuszczenie programu
    }

}
