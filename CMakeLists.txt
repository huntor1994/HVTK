cmake_minimum_required (VERSION 3.16)

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
    CommonDataModel
    IOImage
    ImagingStencil
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

set(Srcs "main.cpp" "MainView.cpp")
set(Hdrs "MainView.h")
set(UIs "MainWindow.ui" "Views/WorkbenchView.ui" "Views/SelectView.ui")
set(Cmps 
"Components/SelectButton.cpp" "Components/SelectButton.h" "Components/SelectButton.qss"
"Components/AutoGridLayout.cpp" "Components/AutoGridLayout.h"
"Components/ImageWidget.cpp" "Components/ImageWidget.h")
set(Views 
"Views/SelectView.cpp" "Views/SelectView.h"
"Views/LoadingView.cpp" "Views/LoadingView.h"
"Views/WorkbenchView.cpp" "Views/WorkbenchView.h")
set(Cores 
"Cores/ModelHelper.cpp" "Cores/ModelHelper.h"
"Cores/PropPickerInteractorStyle.cpp" "Cores/PropPickerInteractorStyle.h"
)

set(PrefixPath ${CMAKE_CURRENT_SOURCE_DIR}"/Resources/ButtonImage")
set(ButtonImage ${PrefixPath}"/head.png")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


source_group("Resources" FILES ${UIs} ${ButtonImage})
source_group("Components" FILES ${Cmps})
source_group("Views" FILES ${Views})
source_group("Cores" FILES ${Cores})

ADD_EXECUTABLE (HVTK  ${Srcs} ${Hdrs} ${UIs} ${Cmps} ${Views} ${ButtonImage} ${Cores})

target_link_libraries(HVTK
PRIVATE
    ${VTK_LIBRARIES}
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/HVTK.res
     Qt5::Gui
    Qt5::Widgets)
