cmake_minimum_required(VERSION 3.16)

project(EMM_Industrial_Vibration_CPP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencias ---

# 1. Qt6 (GUI y Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Charts)

# 2. OpenCV (Visión por computador)
find_package(OpenCV REQUIRED)

# --- Configuración del Ejecutable ---

set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/MotionMagnifier.cpp
    src/VideoProcessor.cpp
)

set(HEADERS
    src/MainWindow.h
    src/MotionMagnifier.h
    src/VideoProcessor.h
)

# Manejo de archivos UI de Qt (si usas .ui)
# qt_wrap_ui(UI_HEADERS src/mainwindow.ui)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# --- Linkado de Librerías ---

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Charts
    ${OpenCV_LIBS}
)

# --- Configuración de Includes ---

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    ${OpenCV_INCLUDE_DIRS}
)

# Habilitar MOC (Meta-Object Compiler) de Qt
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOMOC ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTOUIC ON)
set_property(TARGET ${PROJECT_NAME} PROPERTY AUTORCC ON)
