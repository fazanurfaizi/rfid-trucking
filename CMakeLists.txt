cmake_minimum_required(VERSION 3.14)

project(RFIDTrucking VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.1.1
)

FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
  SQLiteCpp
  GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp
  GIT_TAG 3.3.1
)

FetchContent_MakeAvailable(SQLiteCpp)

set(CPPTUI_BUILD_EXAMPLES OFF CACHE INTERNAL "Disable cpptui examples")
FetchContent_Declare(
  cpptui
  GIT_REPOSITORY https://github.com/jonoton/cpp-tui
  GIT_TAG main
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(cpptui)

FetchContent_Declare(
  laserpants_dotenv
  GIT_REPOSITORY https://github.com/laserpants/dotenv-cpp
  GIT_TAG master
)
FetchContent_MakeAvailable(laserpants_dotenv)

find_package(PahoMqttCpp REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fmt::fmt
  PahoMqttCpp::paho-mqttpp3
  nlohmann_json::nlohmann_json
  cpptui::cpptui
  dotenv
  SQLiteCpp
  sqlite3
  pthread
  dl
)
