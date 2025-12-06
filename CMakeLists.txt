cmake_minimum_required(VERSION 3.20)
project(ManagmentGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

#set(CMAKE_AUTORCC ON)
                ## qt_add_resources(my_app "app_images"
                ##     PREFIX "/"
                ##     FILES
                ##         images/copy.png
                ##        images/cut.png
                ##         images/new.png
                ##         images/open.png
                ##         images/paste.png
                ##         images/save.png
                ## )


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

include_directories(
    include/controller
    include/ui
    include/ui/mainGameWindow
    include/ui/menu
    include/model

)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

## Заголовки
file(GLOB_RECURSE HEADERS
    include/**/*.h
)

add_executable(${PROJECT_NAME} WIN32
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Network)
