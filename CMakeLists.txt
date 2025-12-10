cmake_minimum_required(VERSION 3.16)

# nume proiect, versiune, limbaj
project(7WondersDuel VERSION 1.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# autoconfig qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
    # Windows
    set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64")
    # flag pt a ascunde consola pe Windows
    set(GUI_TYPE WIN32)

elseif(APPLE)
    # macOS
    set(CMAKE_PREFIX_PATH "/Users/pterodactylstfw/Qt/6.10.1/macos")

    # flag pt a crea pachet pe mac
    set(GUI_TYPE MACOSX_BUNDLE)
endif()

# cautare qt6 pe pc
find_package(Qt6 REQUIRED COMPONENTS Widgets)


# listare fisiere h/cpp care nu sunt legate de UI
set(CORE_SOURCES
    src/Card.cpp src/Card.h
    src/CardEffect.cpp src/CardEffect.h
    src/CardFactory.cpp src/CardFactory.h
    src/Constants.cpp src/Constants.h
    src/Cost.cpp src/Cost.h
    src/GameController.cpp src/GameController.h
    src/GameState.cpp src/GameState.h
    src/JsonUtils.cpp src/JsonUtils.h
    src/Player.cpp src/Player.h
    src/ProgressToken.cpp src/ProgressToken.h
    src/ProgressTokenFactory.cpp src/ProgressTokenFactory.h
    src/ResourceProduction.cpp src/ResourceProduction.h 
    src/Utils.h
    src/Wonder.cpp src/Wonder.h
    src/WonderFactory.cpp src/WonderFactory.h
)

# -----------------------------------------------------------
# executabil consola
# -----------------------------------------------------------
add_executable(7WondersConsole
    ${CORE_SOURCES}
    src/ConsoleUI.cpp src/ConsoleUI.h
    src/main.cpp 
)

# cale json.hpp ( din include )
target_include_directories(7WondersConsole PRIVATE include) 
# include si folder src pentru headere
target_include_directories(7WondersConsole PRIVATE src)


# -----------------------------------------------------------
# executabil qt gui
# -----------------------------------------------------------

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main_gui.cpp")
    add_executable(7WondersGUI
            ${GUI_TYPE} # WIN32 pe Windows, Bundle pe Mac - pt ascundere terminal
            ${CORE_SOURCES}
            src/main_gui.cpp
            src/MainWindow.cpp src/MainWindow.h
        src/MainWindow.ui
        src/resources.qrc
    )

    target_link_libraries(7WondersGUI PRIVATE Qt6::Widgets)
    target_include_directories(7WondersGUI PRIVATE include)
    target_include_directories(7WondersGUI PRIVATE src)

    # config iconita si nume pe mac
    if(APPLE)
        set_target_properties(7WondersGUI PROPERTIES
                MACOSX_BUNDLE_BUNDLE_NAME "7 Wonders Duel"
                MACOSX_BUNDLE_GUI_IDENTIFIER "com.pterodactyl.7wonders"
                MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        )
    endif()
endif()