cmake_minimum_required(VERSION 3.0.2)
project(livelybotserial)
add_compile_options(-std=c++11)
find_package(catkin REQUIRED)
# find_package(Boost REQUIRED COMPONENTS system)
find_library(FTD2XX_LIBRARY 
    NAMES ftd2xx
    PATHS /usr/local/lib
    )
# 检查是否找到了库
if(NOT FTD2XX_LIBRARY)
    message(FATAL_ERROR "libftd2xx.so not found")
endif()
catkin_package(
 INCLUDE_DIRS include
 LIBRARIES ${PROJECT_NAME}
)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/serialPort.cpp
# )
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
file(GLOB_RECURSE SRC_LIST
"src/*.cc"
)
set(serial_SRCS
    src/livelybot_serial.cc
    include/livelybot_serial.h
)
add_executable(sp_t src/test.cpp ${SRC_LIST})
target_link_libraries(sp_t ${catkin_LIBRARIES} ${FTD2XX_LIBRARY})

add_library(${PROJECT_NAME} ${serial_SRCS})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${FTD2XX_LIBRARY})
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)
install(FILES
  include/livelybotserial.h
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/livelybotserial
)
