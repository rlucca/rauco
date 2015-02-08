# Versao onde foi desenvolvido o projeto
cmake_minimum_required(VERSION 2.8)

# Nome do projeto
project(uo_client)

set (SOURCE
	${PROJECT_SOURCE_DIR}/src/main.c
	)

set (INCLUDEDIRS
	${PROJECT_SOURCE_DIR}/../libnetcaller/include
	)

set (LINKDIRS
	${PROJECT_SOURCE_DIR}/../libnetcaller/bin
	)

include_directories( ${INCLUDEDIRS} )
link_directories( ${LINKDIRS} )

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries (${PROJECT_NAME} netcaller)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE Release)

# Coloca umas opcoes a mais no compilador
add_definitions(
			-W
			-Wall
			-Werror
			-Winline
			-Wshadow
			-Wcast-qual
			-Wconversion
			-Wpointer-arith
		)
