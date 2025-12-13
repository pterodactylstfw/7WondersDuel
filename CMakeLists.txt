cmake_minimum_required(VERSION 3.16)

# Nume proiect, versiune, limbaj
project(7WondersDuel VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurare automată Qt (MOC, RCC, UIC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# -----------------------------------------------------------
# CONFIGURARE CALE QT & TIP GUI (AUTO-DETECTARE OS)
# -----------------------------------------------------------
if(WIN32)
    # --- WINDOWS ---
    set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64")
    # Flag pentru a ascunde consola pe Windows
    set(GUI_TYPE WIN32)

elseif(APPLE)
    # --- MACOS ---
    set(CMAKE_PREFIX_PATH "/Users/pterodactylstfw/Qt/6.10.1/macos")
    # Flag pentru a crea pachet .app pe Mac
    set(GUI_TYPE MACOSX_BUNDLE)

else()
    message(WARNING "OS necunoscut. Qt Path trebuie setat manual.")
endif()

# Căutare pachet Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)


# -----------------------------------------------------------
# ORGANIZARE FIȘIERE SURSĂ (Pe Foldere)
# -----------------------------------------------------------

# 1. MODEL (Date și Logică Pură)
set(SRC_MODEL
        src/model/Card.cpp src/model/Card.h
        src/model/CardEffect.cpp src/model/CardEffect.h
        src/model/Cost.cpp src/model/Cost.h
        src/model/GameState.cpp src/model/GameState.h
        src/model/Player.cpp src/model/Player.h
        src/model/ProgressToken.cpp src/model/ProgressToken.h
        src/model/ResourceProduction.cpp src/model/ResourceProduction.h
        src/model/Wonder.cpp src/model/Wonder.h
)

# 2. FACTORIES (Generatoare)
set(SRC_FACTORIES
        src/factories/CardFactory.cpp src/factories/CardFactory.h
        src/factories/ProgressTokenFactory.cpp src/factories/ProgressTokenFactory.h
        src/factories/WonderFactory.cpp src/factories/WonderFactory.h
)

# 3. CONTROLLER (Creierul)
set(SRC_CONTROLLER
        src/controller/GameController.cpp src/controller/GameController.h
        src/controller/AIController.cpp src/controller/AIController.h
)

# 4. UTILS (Utilitare)
set(SRC_UTILS
        src/utils/Constants.cpp src/utils/Constants.h
        src/utils/JsonUtils.cpp src/utils/JsonUtils.h
        src/utils/Utils.h
)

# 5. VIEW (Doar partea de MainWindow, ConsoleUI e separată jos)
set(SRC_VIEW_GUI
        src/view/MainWindow.cpp src/view/MainWindow.h
        src/view/MainWindow.ui
)

# Reunim sursele comune (folosite și de consolă și de GUI)
set(CORE_SOURCES
        ${SRC_MODEL}
        ${SRC_FACTORIES}
        ${SRC_CONTROLLER}
        ${SRC_UTILS}
)


# -----------------------------------------------------------
# EXECUTABIL 1: CONSOLA (Legacy / Debug)
# -----------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/view/ConsoleUI.cpp")
    add_executable(7WondersConsole
            ${CORE_SOURCES}
            src/view/ConsoleUI.cpp src/view/ConsoleUI.h
            src/main.cpp
    )

    # Includem toate folderele ca să putem da #include "Card.h" direct
    target_include_directories(7WondersConsole PRIVATE
            include
            src
            src/model
            src/view
            src/controller
            src/factories
            src/utils
    )
endif()


# -----------------------------------------------------------
# EXECUTABIL 2: QT GUI (Jocul Grafic)
# -----------------------------------------------------------

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main_gui.cpp")
    add_executable(7WondersGUI
            ${GUI_TYPE} # WIN32 sau MACOSX_BUNDLE
            ${CORE_SOURCES}
            ${SRC_VIEW_GUI}
            src/main_gui.cpp
            src/resources.qrc
    )

    target_link_libraries(7WondersGUI PRIVATE Qt6::Widgets)

    # Includem toate folderele și pentru GUI
    target_include_directories(7WondersGUI PRIVATE
            include
            src
            src/model
            src/view
            src/controller
            src/factories
            src/utils
    )

    # Configurare specifică Mac (Iconiță, Nume)
    if(APPLE)
        set_target_properties(7WondersGUI PROPERTIES
                MACOSX_BUNDLE_BUNDLE_NAME "7 Wonders Duel"
                MACOSX_BUNDLE_GUI_IDENTIFIER "com.pterodactyl.7wonders"
                MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        )
    endif()
endif()