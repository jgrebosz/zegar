cmake_minimum_required(VERSION 3.16)
project(zegar VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
)

qt_standard_project_setup()

qt_add_executable(zegar WIN32 MACOSX_BUNDLE
    alarmy.cpp alarmy.h alarmy.ui
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    tedytor_alarmow_dlg.cpp tedytor_alarmow_dlg.h tedytor_alarmow_dlg.ui
    tjeden_alarm.cpp tjeden_alarm.h
    togloszenie_alarmu.cpp togloszenie_alarmu.h togloszenie_alarmu.ui
    topcje_dlg.cpp topcje_dlg.h topcje_dlg.ui
    tpreferred_hands.cpp tpreferred_hands.h
)
target_compile_definitions(zegar PRIVATE
    QT_DEPRECATED_WARNINGS
)

target_link_libraries(zegar PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Multimedia
)



# Resources:
set(translations_resource_files
    "polski2.qm"
)

qt_add_resources(zegar "translations"
    PREFIX
        "/"
    FILES
        ${translations_resource_files}
)
set(zegar_elementy_resource_files
    "content/Azimuthal_projection_SW.png"
    "content/Azul_large.png"
    "content/Chopin.png"
    "content/Citizen.png"
    "content/Clock_face_Decorative.png"
    "content/Clock_face_India.png"
    "content/Clock_face_Modern.png"
    "content/Clock_face_Paris2.png"
    "content/Clock_face_Plum.png"
    "content/Comdex - Omega1.png"
    "content/Grolex.png"
    "content/Grollex_blue_slim.png"
    "content/Large-Kitchen-Wall-Clock.png"
    "content/Lunascope2.png"
    "content/Lunascope_greb.png"
    "content/Modern-Design-hours.png"
    "content/Modern-Design-minutes.png"
    "content/Modern-Design-seconds.png"
    "content/Modern-Design.png"
    "content/Moon_day_00.png"
    "content/Moon_day_01.png"
    "content/Moon_day_02.png"
    "content/Moon_day_03.png"
    "content/Moon_day_04.png"
    "content/Moon_day_05.png"
    "content/Moon_day_06.png"
    "content/Moon_day_07.png"
    "content/Moon_day_08.png"
    "content/Moon_day_09.png"
    "content/Moon_day_10.png"
    "content/Moon_day_11.png"
    "content/Moon_day_12.png"
    "content/Moon_day_13.png"
    "content/Moon_day_14.png"
    "content/Moon_day_15.png"
    "content/Moon_day_16.png"
    "content/Moon_day_17.png"
    "content/Moon_day_18.png"
    "content/Moon_day_19.png"
    "content/Moon_day_20.png"
    "content/Moon_day_21.png"
    "content/Moon_day_22.png"
    "content/Moon_day_23.png"
    "content/Morgan.png"
    "content/Morgan_Gard.png"
    "content/Morgan_crop.png"
    "content/Morgan_crop_szablon.png"
    "content/Omega-Seamaster-WorldCircle2.png"
    "content/Omega-hours.png"
    "content/Omega-minutes.png"
    "content/Omega-seconds.png"
    "content/PatekPhilippe.png"
    "content/Ram_corr.png"
    "content/Seamaster_bez_ramy.png"
    "content/Seamaster_mask_mniejszy.png"
    "content/Seamaster_zones_north.png"
    "content/Swiss Quartz Pocket Watch.png"
    "content/Swiss Quartz Pocket Watch_JG.png"
    "content/Wall Clock medium.png"
    "content/Wall-sekundnik.png"
    "content/agrafa_godz.png"
    "content/agrafa_metal-h.png"
    "content/agrafa_metal-m.png"
    "content/agrafa_min.png"
    "content/arne-3D_hour_hand.png"
    "content/arne-3Dhand.png"
    "content/arne22.png"
    "content/arnehour_pion.png"
    "content/arneminute_pion.png"
    "content/banner-1240822.png"
    "content/barok1_godzinowa.png"
    "content/barok1_godzinowa_bevel_a.png"
    "content/barok1_godzinowa_bevel_dark.png"
    "content/barok1_godzinowa_bevel_zloty.png"
    "content/barok1_godzinowa_bevel_zloty_shining.png"
    "content/barok1_min_bev06.png"
    "content/barok1_min_bev10.png"
    "content/barok1_minutowa.png"
    "content/barok1_minutowa_gold.png"
    "content/barok1_minutowa_red.png"
    "content/barok_godzinowa_red.png"
    "content/cebulasta_godzinowa_biala.png"
    "content/cebulasta_godzinowa_czarna.png"
    "content/cebulasta_godzinowa_zlota.png"
    "content/cebulasta_minutowa_biala.png"
    "content/cebulasta_minutowa_czarna.png"
    "content/cebulasta_minutowa_zlota.png"
    "content/center.png"
    "content/chronograph.png"
    "content/chronograph2.png"
    "content/city_hall_hours.png"
    "content/city_hall_jg.png"
    "content/city_hall_minutes.png"
    "content/clock-blue.png"
    "content/clock-europe-africa.png"
    "content/clock.png"
    "content/crystal_ball_hours.png"
    "content/crystal_ball_minutes.png"
    "content/delft-porzelan.png"
    "content/dragon.png"
    "content/drewano_godzinowaB.png"
    "content/drewano_godzinowa_C.png"
    "content/drewno_godzinowa_C_black.png"
    "content/drewno_godzinowa_C_buk.png"
    "content/drewno_minutowa.png"
    "content/drewno_minutowa_wide.png"
    "content/drewno_minutowa_wide_contr.png"
    "content/galaxy.png"
    "content/gear-wall-clocks.png"
    "content/geizmont.png"
    "content/geizmont_dark.png"
    "content/geizmont_light.png"
    "content/geizmont_wsk_hours.png"
    "content/geizmont_wsk_hours_h.png"
    "content/geizmont_wsk_minut.png"
    "content/geizmont_wsk_minut_h.png"
    "content/glob_golden.png"
    "content/gold_KA.png"
    "content/greenmarble.png"
    "content/grolex_refleksy_ciemna_korona.png"
    "content/gwc_graffhausen_black.png"
    "content/gwc_graffhausen_dark.png"
    "content/gwc_graffhausen_light.png"
    "content/gwc_graffhausen_metal.png"
    "content/gwc_graffhausen_skin.png"
    "content/iwc_hours.png"
    "content/iwc_minutes.png"
    "content/iwc_schaffhausen_short.png"
    "content/mechanic.png"
    "content/metal_chrom.png"
    "content/moon_01.png"
    "content/moon_02.png"
    "content/moon_03.png"
    "content/moon_04.png"
    "content/moon_05.png"
    "content/moon_06.png"
    "content/moon_07.png"
    "content/moon_08.png"
    "content/moon_09.png"
    "content/moon_10.png"
    "content/moon_11.png"
    "content/moon_12.png"
    "content/moon_13.png"
    "content/moon_14.png"
    "content/moon_15.png"
    "content/moon_16.png"
    "content/moon_17.png"
    "content/moon_18.png"
    "content/moon_19.png"
    "content/moon_20.png"
    "content/moon_21.png"
    "content/moon_22.png"
    "content/moon_23.png"
    "content/moon_24.png"
    "content/moon_25.png"
    "content/moon_26.png"
    "content/moon_27.png"
    "content/moon_28.png"
    "content/morgan_hours.png"
    "content/morgan_minutes.png"
    "content/neon.png"
    "content/neon_sekundnik.png"
    "content/neon_wskazowka.png"
    "content/ocean_nordic.png"
    "content/pencil.png"
    "content/quarz_black.png"
    "content/radialny_rzymski.png"
    "content/roemische-Zahlen-mit-Glas-und-Luenette.png"
    "content/rom_dark_gold.png"
    "content/roman_gold_soft_minut.png"
    "content/roman_hours.png"
    "content/roman_hours_gold.png"
    "content/roman_hours_grey.png"
    "content/roman_hours_white.png"
    "content/roman_min_black.png"
    "content/roman_min_gold.png"
    "content/roman_min_grey.png"
    "content/roman_minut.png"
    "content/roman_minut_light.png"
    "content/roman_minutgold_10.png"
    "content/rombowe_godzinowa_biala.png"
    "content/rombowe_godzinowa_ciemna.png"
    "content/rombowe_godzinowa_czarna_plaska.png"
    "content/rombowe_godzinowa_zlota.png"
    "content/rombowe_minutowa_biala.png"
    "content/rombowe_minutowa_czarna.png"
    "content/rombowe_minutowa_czarna_plaska.png"
    "content/rombowe_minutowa_zlota.png"
    "content/round-luminous-earth-modern.png"
    "content/rzymski_chrom.png"
    "content/rzymskie.png"
    "content/rzymskie_cien.png"
    "content/seiko_grey.png"
    "content/silver.png"
    "content/spacetime.png"
    "content/target.png"
    "content/tea-time.png"
    "content/tower_barock_metal-h.png"
    "content/tower_barock_metal-m.png"
    "content/tower_empty_gold_hours.png"
    "content/tower_empty_gold_minutes.png"
    "content/tower_empty_metal-h.png"
    "content/tower_empty_metal-m.png"
    "content/tower_gold_hours.png"
    "content/tower_gold_minutes.png"
    "content/tower_metal-h.png"
    "content/tower_metal-m.png"
    "content/wooden-clock.png"
    "content/world_cities.png"
    "content/wtla-chronoswiss-cyferblat.png"
    "content/wtla-chronoswiss-days.png"
    "content/wtla-chronoswiss-godziny.png"
    "content/wtla-chronoswiss-miesiace.png"
    "content/wtla-chronoswiss-minuty.png"
    "content/wtla-chronoswiss-sekundnik.png"
    "content/wtla-chronoswiss-sekundnik_maly.png"
    "content/zaglowiec.png"
    "content/zegar_ikona.png"
    "content/zegar_world_24h_band_outline.png"
    "content/zegar_world_24h_grey.png"
    "content/zegar_world_sam_cyferblat_rowkiM.png"
    "content/zestaw_barok_gold_h.png"
    "content/zestaw_barok_gold_m.png"
    "content/zifferblatt-uhr-rolex.png"
)

qt_add_resources(zegar "zegar_elementy"
    PREFIX
        "/new/prefix1"
    FILES
        ${zegar_elementy_resource_files}
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(zegar PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS zegar
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET zegar
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
