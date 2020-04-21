cmake_minimum_required (VERSION 3.14)

project(Vehicle-Routing)

set(srcs
	src/Program.cpp
)
	
set(headers
	header/Graph.hpp
	headers/json.hpp
)

add_executable(${PROJECT_NAME}
	${srcs}
	${headers}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/header)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_SUPPRESS_REGENERATION true)