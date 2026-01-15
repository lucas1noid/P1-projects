cmake_minimum_required(VERSION 3.5)
project (BarCharRace VERSION 1.0.0 LANGUAGES CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set ( APP_NAME "bcr")

add_executable(${APP_NAME}
    "main.cpp"
    "headers/validate.cpp"
    "headers/bcr_am.cpp"
    "headers/bar_chart.cpp"
)

target_include_directories(${APP_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_compile_features( ${APP_NAME} PUBLIC cxx_std_17 )
target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -g)

