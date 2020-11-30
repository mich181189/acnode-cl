include(FreeRTOS.cmake)

if(NOT CMAKE_CROSSCOMPILING)
include(googletest.cmake)
endif()

if(USE_LWIP)
include(lwip.cmake)
endif()

include(littlefs.cmake)