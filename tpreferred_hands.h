#ifndef TDEFAULT_HANDS_H
#define TDEFAULT_HANDS_H

#include <string>

class Tpreferred_hands
{
public:
    std::string clock_face_name;
    std::string hours_hand;
//     std::string hours_hand_bmp;
    std::string minutes_hand;
//    std::string minutes_hand_bmp;
    std::string seconds_hand;
//    std::string seconds_hand_bmp
 std::string  weekdays_hand;
 std::string monthdays_hand;
 int zoom;

    Tpreferred_hands(
            std::string clock_face_name_arg,
            std::string hours_hand_arg,
            std::string minutes_hand_arg,
            std::string seconds_hand_arg,
            std::string weekdays_hand_arg,
             std::string monthdays_hand_arg,
            int zoom_arg
            );

//    void zapisz_na_dysku();  To jest gdzie indziej
//    void odczytaj_z_dysku() {}

};

#endif // TDEFAULT_HANDS_H
