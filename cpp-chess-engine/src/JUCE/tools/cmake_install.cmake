# Install script for directory: /Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.0/JUCE.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.0/JUCE.cmake"
         "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/CMakeFiles/Export/lib/cmake/JUCE-6.0.0/JUCE.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.0/JUCE-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.0/JUCE.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.0" TYPE FILE FILES "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/CMakeFiles/Export/lib/cmake/JUCE-6.0.0/JUCE.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/JUCE-6.0.0" TYPE FILE FILES
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/JUCEConfigVersion.cmake"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/JUCEConfig.cmake"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/LaunchScreen.storyboard"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/PIPAudioProcessor.cpp.in"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/PIPComponent.cpp.in"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/PIPConsole.cpp.in"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/RecentFilesMenuTemplate.nib"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/UnityPluginGUIScript.cs.in"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/copyDir.cmake"
    "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/extras/Build/CMake/JUCEUtils.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/modules/cmake_install.cmake")
  include("/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/extras/Build/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/wouter/Documents/School/SysteemBasis/csd_proj/chess-project/cpp-chess-engine/src/JUCE/tools/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
