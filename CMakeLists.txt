cmake_minimum_required(VERSION 3.0.0)
project(lightning_vault VERSION 0.0.1)

#set(NAME "lightning_vault")

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# IF(NOT CMAKE_BUILD_TYPE)
# 	SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
# ENDIF(NOT CMAKE_BUILD_TYPE)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# if(WIN32)
# 	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
# endif(WIN32)

#link_directories(${CMAKE_SOURCE_DIR}/lib)

# if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# endif()

# find_package(X11 REQUIRED) # Make sure x is there 
# if(NOT X11_FOUND)
#     message("ERROR: x11 not found")
# endif(NOT X11_FOUND)


add_subdirectory (external)

include_directories(
	external/glfw-3.1.2/include/
	external/glad/include/
	external/freetype-2.9/include/
	external/glm-0.9.7.1/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLAD
	freetype
)

add_library(GLAD "external/glad/src/glad.c")

if(WIN32)
	#set(ALL_LIBS glfw3 opengl32 assimp)
elseif(UNIX AND NOT APPLE)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
	find_package(OpenGL REQUIRED)
	add_definitions(${OPENGL_DEFINITIONS})
	find_package(X11 REQUIRED)
	# note that the order is important for setting the libs
	# use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
	set(ALL_LIBS ${ALL_LIBS} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread ${ASSIMP_LIBRARY})
	set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
	INCLUDE_DIRECTORIES(/System/Library/Frameworks)
	FIND_LIBRARY(COCOA_LIBRARY Cocoa)
	FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
	FIND_LIBRARY(IOKit_LIBRARY IOKit)
	FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
	MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
	SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
	SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY})
	set(ALL_LIBS ${ALL_LIBS} ${APPLE_LIBS})
else()
	set(ALL_LIBS )
endif(WIN32)


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)

file(GLOB SRC
    "*.h"
	"*.hpp"
    "*.cpp"
)

add_executable(lightning_vault ${SRC})	

target_link_libraries(lightning_vault
	${ALL_LIBS}
)

# set(CPACK_PROJECT_NAME ${NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
