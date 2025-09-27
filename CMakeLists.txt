# Version minima para ejecturarse
cmake_minimum_required(VERSION 3.16)

# Nombre del proyecto - lenguajes

project(Calculadora LANGUAGES CXX)

# Enable Qt's automoc/uic/rcc for QObject (Q_OBJECT) classes
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#version de compilador
set(CMAKE_CXX_STANDARD 17)

# El programa no corre si no tiene esa version de compilador
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt path hint (macOS Homebrew only). On Windows/Linux, pass -DCMAKE_PREFIX_PATH at configure time or rely on Qt being in the default CMake paths.
if(APPLE)
  if(EXISTS "/opt/homebrew/opt/qt")
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
  elseif(EXISTS "/usr/local/opt/qt") # Intel macs
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
  endif()
endif()

# Qt path hint for Windows (optional). Prefer passing -DCMAKE_PREFIX_PATH, but fall back to env/common installs.
if(WIN32)
  if(DEFINED ENV{QT_DIR})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QT_DIR}")
  elseif(DEFINED ENV{QTDIR})
    list(APPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
  elseif(EXISTS "C:/Qt")
    # Try to autodetect a Qt6 MSVC kit, e.g., C:/Qt/6.6.3/msvc2022_64
    file(GLOB _qt_kits "C:/Qt/6.*.*/msvc20*")
    list(SORT _qt_kits DESC)
    list(LENGTH _qt_kits _qt_len)
    if(_qt_len GREATER 0)
      list(GET _qt_kits 0 _qt_guess)
      list(APPEND CMAKE_PREFIX_PATH "${_qt_guess}")
    endif()
  endif()
endif()

# busca los paquetes de widgets, necesario para botones y UI
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# define cuales son los ejecutables
add_executable(calculator MACOSX_BUNDLE
  src/main.cpp
  src/UICalculator.cpp
  src/engine.cpp
)

# Enlaza el compilador con la libreria de Qt
target_link_libraries(calculator PRIVATE Qt6::Widgets)

# --- Installation & Packaging helpers ---
# Install the app bundle/EXE to the top-level of the package
install(TARGETS calculator
  BUNDLE DESTINATION .
  RUNTIME DESTINATION .
)

# Platform-specific packaging (optional): use CPack
if(APPLE)
  set(CPACK_GENERATOR DragNDrop)
  set(CPACK_PACKAGE_NAME "Calculator")
  set(CPACK_PACKAGE_FILE_NAME "calculator-mac")
  # Optional: provide an icon if you have it (uncomment and set the path)
  # set(CPACK_DMG_VOLUME_NAME "Calculator")
  include(CPack)
elseif(WIN32)
  set(CPACK_GENERATOR ZIP)
  set(CPACK_PACKAGE_NAME "Calculator")
  set(CPACK_PACKAGE_FILE_NAME "calculator-win")
  include(CPack)
else()
  set(CPACK_GENERATOR ZIP)
  set(CPACK_PACKAGE_NAME "Calculator")
  set(CPACK_PACKAGE_FILE_NAME "calculator-linux")
  include(CPack)
endif()