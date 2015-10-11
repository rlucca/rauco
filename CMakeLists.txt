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
	)

include_directories( ${INCLUDEDIRS} )

add_library(${PROJECT_NAME} SHARED ${SOURCE})

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
