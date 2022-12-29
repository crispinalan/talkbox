cmake_minimum_required(VERSION 3.22.0)

project(talkbox)

set(BUILD_MAJOR "1")
set(BUILD_MINOR "1")
set(BUILD_MICRO "0")

#set package meta data
set(PACKAGE "talkbox")
set(PACKAGE_NAME "talkbox")
set(PACKAGE_VERSION   "${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_MICRO}")
set(PACKAGE_VERSION   "${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_MICRO}")
set(PACKAGE_URL       "https://github.com/crispinalan/talkbox")

# qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set( CMAKE_BUILD_TYPE Debug )
add_definitions ( -Wall )
#add_definitions(-DQT_NO_DEBUG_OUTPUT)

# required qt packages
find_package (Qt5Widgets REQUIRED)
find_package (Qt5Core REQUIRED)
find_package (Qt5Gui REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Multimedia REQUIRED)

#header files
set (talkbox_HDRS
	dictionary.h
	speechengine.h
	wavcat.h		
	)
#source files
set (talkbox_SRCS
	main.cpp
	mainwindow.cpp
	dictionary.cpp
	speechengine.cpp
	wavcat.cpp
	)
#ui files
set (talkbox_UIS
	mainwindow.ui

	)
QT5_WRAP_UI(UIS ${talkbox_UIS})

# resource files
set (talkbox_RSCS
	icons.qrc
    )
QT5_ADD_RESOURCES(RSCS ${talkbox_RSCS})

#set MOCs
set (talkbox_MOCS
	mainwindow.h	
	)
QT5_WRAP_CPP(MOCS ${talkbox_MOCS})

# add files defined above
add_executable (talkbox ${talkbox_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS})
target_link_libraries ( talkbox  Qt5::Core Qt5::Gui Qt5::Widgets  Qt5::Xml Qt5::Multimedia)


INSTALL(TARGETS
    talkbox
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
)


