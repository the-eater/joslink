include(../../toolchain/steamlink-toolchain.cmake)

SET(CMAKE_C_COMPILER ${CMAKE_C_COMPILER_INIT})
SET(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER_INIT})

include_directories(${CMAKE_SYSROOT}/usr/include)
link_directories(${CMAKE_SYSROOT}/usr/lib)

SET(RESOURSE_FILES
        scripts/build-external.sh)

cmake_minimum_required(VERSION 3.9)
project(joslink)

set(SOURCE_FILES
        src/main.c src/transport.c src/transport.h)

SET($ENV{JOSLINK_PREFIX} "/home/apps/joslink")
SET($ENV{STEAMLINK_CONFIGURE_OPTS} "--host=armv7a-cros-linux-gnueabi --with-sysroot=${MARVELL_ROOTFS} --prefix=/home/apps/joslink")
SET($ENV{LDFLAGS} "-I/home/apps/joslink/include $ENV{LDFLAGS}")
SET($ENV{PKG_CONFIG_LIBDIR} "$ENV{PKG_CONFIG_LIBDIR}:$ENV{MARVELL_ROOTFS}/home/apps/joslink/lib/pkg-config")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lSLAudio -ljack")

add_custom_target(external ALL
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/build-external.sh
)

add_executable(joslink ${SOURCE_FILES})

add_custom_target(joslink-all ALL)

add_dependencies(joslink-all external joslink)

install(TARGETS joslink RUNTIME DESTINATION "/")