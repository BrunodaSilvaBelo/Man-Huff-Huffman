cmake_minimum_required( VERSION 2.6 )
project( manhuffhuffman )

find_package(Boost
  REQUIRED)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
endif()

set( LIB_NAME ${PROJECT_NAME} )
set(LIBRARY_TYPE STATIC)

set( SOURCE ${SOURCE}
  "${PROJECT_SOURCE_DIR}/src/io/reader.cpp"
  "${PROJECT_SOURCE_DIR}/src/coder/coder.cpp"
  )

include_directories( src ${Boost_INCLUDE_DIRS} )

add_library ( ${LIB_NAME}
  ${LIBRARY_TYPE}
  ${SOURCE}
  )

target_link_libraries( ${LIB_NAME} ${Boost_LIBRARIES} )
add_executable( "${PROJECT_NAME}code" src/main.cpp )
add_executable( "${PROJECT_NAME}test" src/test.cpp )


target_link_libraries( "${PROJECT_NAME}code" ${LIB_NAME} ${Boost_LIBRARIES} )
target_link_libraries( "${PROJECT_NAME}test" ${LIB_NAME} ${Boost_LIBRARIES} )

enable_testing()
add_test(NAME RunTests COMMAND "${PROJECT_NAME}test")