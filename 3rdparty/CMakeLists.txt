cmake_minimum_required(VERSION 3.5)

project(3rdparty LANGUAGES CXX)

###############################################################################
## CONFIGURATION
## 3rd-party submodules configuration
###############################################################################

set(UTILITIES_SUBMODULE	   ON CACHE BOOL "" FORCE)

###############################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the 3rd-party configuration
###############################################################################

if (UTILITIES_SUBMODULE)
    add_subdirectory(Utilities)
endif()
