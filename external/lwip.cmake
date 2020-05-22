
set(LWIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lwip)
include(${CMAKE_CURRENT_SOURCE_DIR}/lwip/src/Filelists.cmake)

# Basically include everything except PPP, 6LoWPAN, mdns, sntp, iperf
add_library(lwip
	${lwipcore_SRCS}
    ${lwipcore4_SRCS}
    ${lwipcore6_SRCS}
    ${lwipapi_SRCS}
    ${lwipnetif_SRCS}
	${lwipsnmp_SRCS}
    ${lwiphttp_SRCS}
    ${lwipsmtp_SRCS}
    ${lwipnetbios_SRCS}
    ${lwiptftp_SRCS}
    ${lwipmqtt_SRCS}
    ${lwipmbedtls_SRCS}

    ${CMAKE_CURRENT_SOURCE_DIR}/lwip-contrib/ports/freertos/sys_arch.c)


target_include_directories(lwip PUBLIC ${LWIP_DIR}/src/include
                                        ${CMAKE_CURRENT_SOURCE_DIR}/lwip-port
                                        ${CMAKE_CURRENT_SOURCE_DIR}/lwip-contrib/ports/freertos/include)
target_link_libraries(lwip hal FreeRTOS)