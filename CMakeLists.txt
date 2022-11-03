cmake_minimum_required (VERSION 3.8)

project ("HVTK")

find_package(VTK
   REQUIRED
    CommonCore
    RenderingCore
    FiltersSources
    InteractionStyle
    GUISupportQt
    RenderingOpenGL2
    IOLegacy
    FiltersCore
    IOGeometry
    IOImage
    InteractionImage
)

if (NOT VTK_FOUND)
  message("Skipping example: ${VTK_NOT_FOUND_MESSAGE}")
  return ()
endif ()

find_package(Qt5 COMPONENTS Widgets Gui)
if (NOT TARGET Qt5::Widgets OR NOT TARGET Qt5::Gui)
  message("Skipping example: ${Qt5_NOT_FOUND_MESSAGE}")
  return ()
endif ()

set(Srcs "main.cpp" "MainWindow.cpp")
set(Hdrs "MainWindow.h") 
set(UIs "MainWindow.ui")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

source_group("Resources" FILES ${UIs})

ADD_EXECUTABLE (HVTK  ${Srcs} ${Hdrs} ${UIs})

target_link_libraries(HVTK
PRIVATE
    ${VTK_LIBRARIES}
     Qt5::Gui
    Qt5::Widgets)