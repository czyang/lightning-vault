cmake_minimum_required(VERSION 3.0.0)
project(LightningVault VERSION 0.1.0)

find_package(OpenGL REQUIRED)

include(CTest)
enable_testing()

add_subdirectory (external)

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
)

add_executable(LightningVault 
	main.cpp
)
target_link_libraries(LightningVault
	${ALL_LIBS}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
