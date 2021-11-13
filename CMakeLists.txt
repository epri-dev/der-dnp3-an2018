cmake_minimum_required(VERSION 3.2)
project(der-dnp3-an2018)

include(FetchContent)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

FetchContent_Declare(
  opendnp3
  GIT_REPOSITORY https://github.com/dnp3/opendnp3.git
  GIT_TAG        3.1.1
)

FetchContent_MakeAvailable(opendnp3)

set(DER_SOURCES
    der-an-2018/CSVReaderMeasurement.cpp
    der-an-2018/DERCommandHandler.cpp
    der-an-2018/DERCommandHandlerCallbackDefault.cpp
    der-an-2018/DEROutstation.cpp
    der-an-2018/GenericCurve.cpp
    der-an-2018/main.cpp
    der-an-2018/Schedule.cpp
)

add_executable(der_dnp3_an2018 ${DER_SOURCES})
target_include_directories(der_dnp3_an2018 PRIVATE "${EXTERNAL_INSTALL_LOCATION}/include")
target_link_directories(der_dnp3_an2018 PRIVATE "${EXTERNAL_INSTALL_LOCATION}/lib")
target_link_libraries(der_dnp3_an2018 opendnp3)