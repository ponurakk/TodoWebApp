cmake_minimum_required(VERSION 3.22)
project(todowebapp VERSION 0.1.0 LANGUAGES CXX)

find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)

find_package(Crow REQUIRED)
find_package(SQLite3 REQUIRED)

set(SOURCE_FILES
  src/main.cpp
  src/logger.cpp
  src/initialize_db.cpp
  src/entities.cpp
  src/controllers/add_todo.cpp
  src/controllers/get_todo.cpp
  src/controllers/list_todo.cpp
  src/controllers/remove_todo.cpp
  src/controllers/update_todo.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PUBLIC Crow::Crow SQLite::SQLite3 ${JSONCPP_LIBRARIES})
