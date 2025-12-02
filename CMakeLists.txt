cmake_minimum_required(VERSION 3.20)
project(ManagmentGame LANGUAGES CXX)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Включаем автогенерацию Qt
set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

## Подключаем Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Включаем директории
include_directories(
    include/core
    include/network
    include/ui
    include/utils
)

## Собираем исходники
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

## Заголовки
file(GLOB_RECURSE HEADERS
    include/**/*.h
    include/**/*.h
)

## Создаем исполняемый файл
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

## Линкуем библиотеки Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)
