cmake_minimum_required (VERSION 2.8)
# Project Name
PROJECT(GeneticAlgorithm_TSP_Visualization)
SET(OpenCV_STATIC OFF)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
FIND_PACKAGE( OpenCV REQUIRED )

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
LINK_DIRECTORIES(${OpenCV_LIB_DIR})

# check for system
IF(WIN32)
 SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
ENDIF(WIN32)

# add the executable
ADD_EXECUTABLE(GeneticAlgorithm_TSP_Visualization GeneticAlgorithms.cpp)
# Link your application with OpenCV libraries
TARGET_LINK_LIBRARIES(GeneticAlgorithm_TSP_Visualization ${OpenCV_LIBS})

message(STATUS "OpenCV_VERSION   	: ${OpenCV_VERSION}")
message(STATUS "CMAKE_VERSION    	: ${CMAKE_VERSION}")
message(STATUS "CMAKE_SYSTEM_NAME 	: ${CMAKE_SYSTEM_NAME}") 
message(STATUS "CMAKE_GENERATOR  	: ${CMAKE_GENERATOR}")  
message(STATUS "CMAKE_C_COMPILER_ID	: ${CMAKE_C_COMPILER_ID}")
 