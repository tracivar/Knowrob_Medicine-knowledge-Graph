cmake_minimum_required(VERSION 2.8.3)
project(medicine)

find_package(catkin REQUIRED COMPONENTS rosjava_build_tools  rosjava_bootstrap knowrob_common json_prolog_msgs rosjava_messages rosprolog rosjava_build_tools rospy roscpp
)

catkin_package(
    CATKIN_DEPENDS knowrob_common json_prolog
)

# find SWI Prolog libraries
find_package(PkgConfig)
pkg_check_modules(SWIPL REQUIRED swipl)
include_directories(${SWIPL_INCLUDE_DIRS})
link_directories(${SWIPL_LIBRARY_DIRS})

install(DIRECTORY owl/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/owl)
  
install(DIRECTORY prolog/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/prolog)

# C++ client library
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(medicineTest scripts/medicineTest.cpp)
target_link_libraries(medicineTest json_prolog ${catkin_LIBRARIES})
add_dependencies(medicineTest ${catkin_EXPORTED_TARGETS})
