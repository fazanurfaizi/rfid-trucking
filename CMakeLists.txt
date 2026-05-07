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

set(CPPTUI_BUILD_EXAMPLES OFF)
FetchContent_Declare(
  cpptui
  GIT_REPOSITORY https://github.com/jonoton/cpp-tui
  GIT_TAG main
)
FetchContent_MakeAvailable(cpptui)

find_package(PahoMqttCpp REQUIRED)

# file(GLOB SOURCES "src/*.cpp")
set(SOURCES
  src/config/AppConfig.cpp
  src/main.cpp
  src/mqtt/MqttReader.cpp
  src/mqtt/TopicDispatcher.cpp
  src/db/Database.cpp
  src/handlers/TruckCycle.cpp
  src/ui/LogBox.cpp
  src/ui/MessageStore.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  fmt::fmt
  PahoMqttCpp::paho-mqttpp3
  nlohmann_json::nlohmann_json
  cpptui::cpptui
  SQLiteCpp
  sqlite3
  pthread
  dl
)
