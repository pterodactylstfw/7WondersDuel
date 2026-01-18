cmake_minimum_required(VERSION 3.16)

# Nume proiect, versiune, limbaj
project(7WondersDuel VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurare automata Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# config path qt si tip gui

if(WIN32)
    set(GUI_TYPE WIN32)
    # Setam doar daca nu e setat deja de utilizator/IDE
    if(NOT DEFINED CMAKE_PREFIX_PATH)
        set(CMAKE_PREFIX_PATH "C:/Qt/6.10.1/msvc2022_64") 
    endif()
elseif(APPLE)
    set(GUI_TYPE MACOSX_BUNDLE)
    # Setam doar daca nu e setat deja
    if(NOT DEFINED CMAKE_PREFIX_PATH)
        set(CMAKE_PREFIX_PATH "/Users/pterodactylstfw/Qt/6.10.1/macos") # hardcodat pt mac-ul meu
    endif()

else()
    message(WARNING "OS necunoscut. Qt Path trebuie setat manual.")
endif()

# Cautare pachet qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)


# organizare fisiere source

# model
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

# factories
set(SRC_FACTORIES
        src/factories/CardFactory.cpp src/factories/CardFactory.h
        src/factories/ProgressTokenFactory.cpp src/factories/ProgressTokenFactory.h
        src/factories/WonderFactory.cpp src/factories/WonderFactory.h
)

# controller
set(SRC_CONTROLLER
        src/controller/GameController.cpp src/controller/GameController.h
        src/controller/AIController.cpp src/controller/AIController.h
)

# utils
set(SRC_UTILS
        src/utils/Constants.cpp src/utils/Constants.h
        src/utils/JsonUtils.cpp src/utils/JsonUtils.h
        src/utils/Utils.cpp src/utils/Utils.h
        include/CoreExport.h
)
# viewInterface
set(SRC_VIEW_INTERFACE
        src/view/IGameView.h
)


add_library(7WondersCore SHARED
        ${SRC_MODEL}
        ${SRC_FACTORIES}
        ${SRC_CONTROLLER}
        ${SRC_UTILS}
        ${SRC_VIEW_INTERFACE}
)

target_compile_definitions(7WondersCore PRIVATE SEVEN_WONDERS_CORE_LIBRARY)

target_include_directories(7WondersCore PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/model
        ${CMAKE_CURRENT_SOURCE_DIR}/src/view
        ${CMAKE_CURRENT_SOURCE_DIR}/src/controller
        ${CMAKE_CURRENT_SOURCE_DIR}/src/factories
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

target_link_libraries(7WondersCore PRIVATE Qt6::Widgets Qt6::Network)

# exec consola

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/view/ConsoleUI.cpp")
    add_executable(7WondersConsole
            src/view/ConsoleUI.cpp src/view/ConsoleUI.h
            src/main.cpp
    )

    target_link_libraries(7WondersConsole PRIVATE 7WondersCore)
endif()


# exec qt gui

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main_gui.cpp")

    set(SRC_VIEW_IMPL
            src/view/MainWindow.cpp src/view/MainWindow.h
            src/view/MainWindow.ui
            src/view/NetworkClient.cpp src/view/NetworkClient.h
            src/server/GameServer.cpp src/server/GameServer.h src/server/ServerGameView.h
    )

    add_executable(7WondersGUI
            ${GUI_TYPE} # WIN32 / macOS X Bundle
            ${SRC_VIEW_IMPL}
            src/main_gui.cpp
            src/resources.qrc
    )

    target_link_libraries(7WondersGUI PRIVATE Qt6::Widgets Qt6::Network 7WondersCore)

    # Configurare specifica Mac (iconita si nume)
    if(APPLE)
        set_target_properties(7WondersGUI PROPERTIES
                MACOSX_BUNDLE_BUNDLE_NAME "7 Wonders Duel"
                MACOSX_BUNDLE_GUI_IDENTIFIER "com.constructorii.7wonders"
                MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        )
    endif()

endif()


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/server/main_server.cpp")
    add_executable(7WondersServer
                src/server/main_server.cpp
                src/server/GameServer.cpp
                src/server/GameServer.h
    )

    target_link_libraries(7WondersServer PRIVATE Qt6::Network 7WondersCore)
endif()


# unit tests cu Google Test
enable_testing()

# descarcare google test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# setare pentru Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# definire teste
add_executable(7WondersTests
    tests/main_test.cpp
    tests/TestPlayer.cpp
    tests/TestFactories.cpp
    tests/TestGameController.cpp
    tests/TestGameState.cpp
    tests/TestCard.cpp
    tests/TestCost.cpp
    tests/TestWonder.cpp
    tests/MockGameView.h
)

# legam testele cu bibliotecile necesare
target_link_libraries(7WondersTests
    PRIVATE
    GTest::gtest_main
    7WondersCore     
    Qt6::Core         
)

# includem directoarele sursa pentru teste
target_include_directories(7WondersTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/model
    ${CMAKE_CURRENT_SOURCE_DIR}/src/view
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/src/factories
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# inregistrare teste
include(GoogleTest)
gtest_discover_tests(7WondersTests)