#include "tpreferred_hands.h"
#include <iostream>
#include <fstream>
using namespace  std;

Tpreferred_hands::Tpreferred_hands(
        std::string clock_face_name_arg,
        std::string hours_hand_arg,
        std::string minutes_hand_arg,
        std::string seconds_hand_arg,
        std::string weekdays_hand_arg,
         std::string monthdays_hand_arg,
        int zoom_arg)
    :
      clock_face_name(clock_face_name_arg),
      hours_hand(hours_hand_arg),
      minutes_hand(minutes_hand_arg),
      seconds_hand(seconds_hand_arg),
       weekdays_hand( weekdays_hand_arg),
        monthdays_hand(monthdays_hand_arg),
       zoom(zoom_arg)
{

}





//************************************************************************************************************************************
//************************************************************************************************************************************
//************************************************************************************************************************************
