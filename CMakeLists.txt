# Version minima para ejecturarse
cmake_minimum_required(VERSION 3.16)

# Nombre del proyecto - lenguajes
project(Calculadora LANGUAGES CXX)

#version de compilador
set(CMAKE_CXX_STANDARD 17)

# El programa no corre si no tiene esa version de compilador
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt instalado por Homebrew
# Ajusta esta ruta si usas Intel (sería /usr/local/opt/qt)
# Linux no necesita esto, puedes comentarlo
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# busca los paquetes de widgets, necesario para botones y UI
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# define cuales son los ejecutables
add_executable(calculator

)

# Enlaza el compilador con la libreria de Qt
target_link_libraries(calculator PRIVATE Qt6::Widgets)