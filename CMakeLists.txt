cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(PROJECT_NLBSIM)

#----------------------------------------------------------------------------

set(useROOT true)
set(MPRJ_SOURCE_DIRECTORIES src)
set(MPRJ_EXECUTABLES CACHE INTERNAL "")
set(MPRJ_SCRIPTS CACHE INTERNAL "")

include(configure.cmake)

#----------------------------------------------------------------------------

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

if(useROOT)
  list(APPEND MPRJ_SOURCE_DIRECTORIES)
  list(APPEND MPRJ_EXECUTABLES)
endif(useROOT)

set(MPRJ_SOURCES_DIRECTORIES_FULL)
foreach(_directory ${MPRJ_SOURCE_DIRECTORIES})
  list(APPEND MPRJ_SOURCE_DIRECTORIES_FULL ${PROJECT_SOURCE_DIR}/${_directory})
endforeach(_directory)

set(MPRJ_SOURCES)
foreach(_directory ${MPRJ_SOURCE_DIRECTORIES_FULL})
  list(APPEND MPRJ_SOURCES ${_directory}/*.cc)
endforeach(_directory)

set(MPRJ_HEADERS)
foreach(_directory ${MPRJ_SOURCE_DIRECTORIES_FULL})
  list(APPEND MPRJ_HEADERS ${_directory}/*.hh)
endforeach(_directory)

include(${Geant4_USE_FILE})
include_directories(${MPRJ_SOURCE_DIRECTORIES_FULL})

if(useROOT)
  EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")
endif(useROOT)

file(GLOB sources ${MPRJ_SOURCES})
file(GLOB headers ${MPRJ_HEADERS})

foreach(_executable ${MPRJ_EXECUTABLES})
  add_executable(${_executable}.g4sim ${_executable}.cc ${sources} ${headers})
if(useROOT)
  EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
endif(useROOT)
  target_link_libraries(${_executable}.g4sim ${Geant4_LIBRARIES})
endforeach(_executable)

add_custom_target(PROJECT_NLBSIM DEPENDS ${MPRJ_EXECUTABLES})

add_custom_target(link_data_submit ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/data ${PROJECT_SOURCE_DIR}/ncsubmit/data)

foreach(_executable ${MPRJ_EXECUTABLES})
  install(TARGETS ${_executable}.g4sim DESTINATION bin)
  add_custom_target(link_${_executable}_source ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_BINARY_DIR}/${_executable}.g4sim ${PROJECT_SOURCE_DIR}/${_executable}.g4sim)
  add_custom_target(link_${_executable}_submit ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_BINARY_DIR}/${_executable}.g4sim ${PROJECT_SOURCE_DIR}/ncsubmit/${_executable}.g4sim)
endforeach(_executable)
