cmake_minimum_required(VERSION 2.8)

project(rauco)

set(SOURCE
	${PROJECT_SOURCE_DIR}/include/rauco.h
	${PROJECT_SOURCE_DIR}/src/rauco.c
	${PROJECT_SOURCE_DIR}/src/rauco_internal.h
	${PROJECT_SOURCE_DIR}/src/rauco_internal.c
	)

set(INCLUDEDIRS
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/tools/fff/gtest
	${PROJECT_SOURCE_DIR}/tools/fff
	)

include_directories( ${INCLUDEDIRS} )
add_library(${PROJECT_NAME} SHARED ${SOURCE})

add_library(gtest STATIC
			${PROJECT_SOURCE_DIR}/tools/fff/gtest/gtest-main.cc
			${PROJECT_SOURCE_DIR}/tools/fff/gtest/gtest-all.cc
			)

enable_testing()

macro(CREATE_TEST_EXECUTABLE PREFIX BIN PARENT)
	set(TEST_SOURCE
			${PROJECT_SOURCE_DIR}/tests/${PREFIX}_${BIN}.cpp
			${PROJECT_SOURCE_DIR}/src/${BIN}.c
		)
	add_executable(${PREFIX}_${BIN}	${TEST_SOURCE})
	target_link_libraries(${PREFIX}_${BIN} gtest ${PROJECT_NAME})
	add_dependencies(${PREFIX}_${BIN} gtest ${PARENT})
	add_test(${PREFIX}_${BIN}	${PROJECT_BINARY_DIR}/${PREFIX}_${BIN})
endmacro(CREATE_TEST_EXECUTABLE)

set(UNIT_TEST_CASES
		rauco
		)

set(PREVIOUS ${PROJECT_NAME})
foreach(TEST_CASE IN LISTS UNIT_TEST_CASES)
	CREATE_TEST_EXECUTABLE("unit" ${TEST_CASE} ${PREVIOUS})
	set(PREVIOUS unit_${TEST_CASE})
endforeach(TEST_CASE)

set(INT_TEST_CASES
		rauco
		)

foreach(TEST_CASE IN LISTS INT_TEST_CASES)
	CREATE_TEST_EXECUTABLE("int" ${TEST_CASE} ${PREVIOUS})
	set(PREVIOUS int_${TEST_CASE})
endforeach(TEST_CASE)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

#force some usefull options
add_definitions(
			-W
			-Wall
			-Wextra
			-Werror
			-Winline
			-Wshadow
			-Wcast-qual
			-Wconversion
			-Wpointer-arith
		)
