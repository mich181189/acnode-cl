set(LITTLEFS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/littlefs)

set(LITTLEFS_SRCS 	${LITTLEFS_DIR}/lfs.c
					${LITTLEFS_DIR}/lfs_util.c
	)

set(LITTLEFSBD_SRCS	${LITTLEFS_DIR}/bd/lfs_filebd.c
					${LITTLEFS_DIR}/bd/lfs_rambd.c
					${LITTLEFS_DIR}/bd/lfs_testbd.c
	)

add_library(littlefs ${LITTLEFS_SRCS})

target_include_directories(littlefs PUBLIC ${LITTLEFS_DIR})

add_library(littlefs_bd ${LITTLEFSBD_SRCS})
target_link_libraries(littlefs_bd littlefs)