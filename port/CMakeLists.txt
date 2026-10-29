cmake_minimum_required(VERSION 3.16)
project(byzantine_port LANGUAGES CXX)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(byzantine_formats STATIC
    src/all.cpp
    src/ambient_audio.cpp
    src/anims.cpp
    src/cb.cpp
    src/cc.cpp
    src/compose.cpp
    src/conversation.cpp
    src/cursors.cpp
    src/inventory.cpp
    src/jpeg.cpp
    src/menu.cpp
    src/rle.cpp
    src/panorama.cpp
    src/panorama_view.cpp
    src/records.cpp
    src/save.cpp
    src/scene_setup.cpp
    src/settings.cpp
    src/language.cpp
    src/text.cpp
    src/transition.cpp
    src/variables.cpp
    src/video.cpp
)
target_include_directories(byzantine_formats PUBLIC include)
target_link_libraries(byzantine_formats PUBLIC SDL2::SDL2)

add_executable(byzantine-all src/all_main.cpp)
target_link_libraries(byzantine-all PRIVATE byzantine_formats)

add_executable(byzantine-cc src/cc_main.cpp)
target_link_libraries(byzantine-cc PRIVATE byzantine_formats)

add_executable(byzantine-rle src/main.cpp)
target_link_libraries(byzantine-rle PRIVATE byzantine_formats)

add_executable(byzantine-play src/play_main.cpp)
target_link_libraries(byzantine-play PRIVATE byzantine_formats SDL2::SDL2)

include(CTest)
if(BUILD_TESTING)
    add_executable(panorama-test tests/panorama.cpp)
    target_link_libraries(panorama-test PRIVATE byzantine_formats)
    add_test(NAME panorama COMMAND panorama-test)
    add_executable(panorama-view-test tests/panorama_view.cpp)
    target_link_libraries(panorama-view-test PRIVATE byzantine_formats)
    add_test(NAME panorama-view COMMAND panorama-view-test)
    add_executable(scene-state-test tests/scene_state.cpp)
    target_link_libraries(scene-state-test PRIVATE byzantine_formats)
    add_test(NAME scene-state COMMAND scene-state-test)
    add_executable(variable-metadata-test tests/variable_metadata.cpp)
    target_link_libraries(variable-metadata-test PRIVATE byzantine_formats)
    add_test(NAME variable-metadata COMMAND variable-metadata-test)
    add_executable(records-test tests/records.cpp)
    target_link_libraries(records-test PRIVATE byzantine_formats)
    add_test(NAME records COMMAND records-test)
    add_executable(anims-test tests/anims.cpp)
    target_link_libraries(anims-test PRIVATE byzantine_formats)
    add_test(NAME anims COMMAND anims-test)
    add_executable(compose-test tests/compose.cpp)
    target_link_libraries(compose-test PRIVATE byzantine_formats)
    add_test(NAME compose COMMAND compose-test)
    add_executable(save-test tests/save.cpp)
    target_link_libraries(save-test PRIVATE byzantine_formats)
    add_test(NAME save COMMAND save-test)
    add_executable(cursor-test tests/cursors.cpp)
    target_link_libraries(cursor-test PRIVATE byzantine_formats)
    add_test(NAME cursors COMMAND cursor-test)
    add_executable(menu-test tests/menu.cpp)
    target_link_libraries(menu-test PRIVATE byzantine_formats)
    add_test(NAME menu COMMAND menu-test)
    add_executable(inventory-test tests/inventory.cpp)
    target_link_libraries(inventory-test PRIVATE byzantine_formats)
    add_test(NAME inventory COMMAND inventory-test)
    add_executable(jpeg-test tests/jpeg.cpp)
    target_link_libraries(jpeg-test PRIVATE byzantine_formats)
    add_executable(settings-test tests/settings.cpp)
    target_link_libraries(settings-test PRIVATE byzantine_formats)
    add_test(NAME settings COMMAND settings-test)
    add_executable(language-test tests/language.cpp)
    target_link_libraries(language-test PRIVATE byzantine_formats)
    add_test(NAME language COMMAND language-test)
    add_test(NAME jpeg COMMAND jpeg-test
        ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/rea/jpeg_sample/BCLINTEL.jpg
        ${CMAKE_CURRENT_SOURCE_DIR}/../analysis/rea/jpeg_sample/vc05.jpg)
endif()
