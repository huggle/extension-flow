# This is a build file for Huggle (used with cmake)
# You will need to update it by hand!
cmake_minimum_required (VERSION 2.8.7)
PROJECT(huggle_flow)
#  cmake <folder> -DQT5_BUILD=true
option(QT5_BUILD "Build huggle using the Qt5 framework" FALSE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)
#SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOMOC ON)
include_directories(../../)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
set(QT_INCLUDES
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Xml_INCLUDE_DIRS}
    ${Qt5Svg_INCLUDE_DIRS}
)
include_directories(${QT_INCLUDES})

file(GLOB flow_srcx "src/*.cpp")
file(GLOB flow_resx "src/*.qrc")
file(GLOB flow_uixx "src/*.ui")

SET(flow_SOURCES ${flow_srcx})
SET(flow_FORMS ${enwiki_uixx})
SET(flow_RESOURCES ${flow_resx})
if(QT5_BUILD)
    QT5_WRAP_CPP(flow_HEADERS_MOC ${flow_HEADERS})
	QT5_WRAP_UI(flow_FORMS_HEADERS ${flow_FORMS})
    QT5_ADD_RESOURCES(flow_RESOURCES_RCC ${flow_RESOURCES})
else()
    QT4_WRAP_CPP(flow_HEADERS_MOC ${flow_HEADERS})
	QT4_WRAP_UI(flow_FORMS_HEADERS ${flow_FORMS})
    QT4_ADD_RESOURCES(flow_RESOURCES_RCC ${flow_RESOURCES})
endif()
if (NOT QT5_BUILD)
  INCLUDE(${QT_USE_FILE})
endif()
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS( -DQT_USE_QSTRINGBUILDER )
ADD_LIBRARY(huggle_flow SHARED ${flow_SOURCES} ${flow_FORMS_HEADERS} ${flow_HEADERS_MOC} ${flow_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(huggle_flow ${QT_LIBRARIES})
TARGET_LINK_LIBRARIES(huggle_flow huggle_core huggle_ui)
TARGET_LINK_LIBRARIES(huggle_flow Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
if (LINUX_SNAP)
    INSTALL(TARGETS huggle_flow DESTINATION bin/extensions)
else()
    INSTALL(TARGETS huggle_flow DESTINATION share/huggle/extensions)
endif()

