# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#
# Author: wanglusheng@axera-tech.com
#

# set ffmpeg library linking path

get_filename_component (FFMPEG_DIRECTORY  "${AXCL_ROOT_DIR}/3rdparty/ffmpeg" ABSOLUTE)
get_filename_component (FFMPEG_SOURCE_PATH "${FFMPEG_DIRECTORY}/FFmpeg-n7.1" ABSOLUTE)

set(FFMPEG_BUILD_PATH "${FFMPEG_DIRECTORY}")
if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*|arm64.*|ARM64.*)")
    get_filename_component (FFMPEG_PREFIX_PATH "${FFMPEG_DIRECTORY}/arm64" ABSOLUTE)
endif()
if (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|x86_64.*|AMD64.*")
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        get_filename_component (FFMPEG_PREFIX_PATH "${FFMPEG_DIRECTORY}/win64" ABSOLUTE)
        set(FFMPEG_BUILD_PATH "${FFMPEG_PREFIX_PATH}/config")
    else()
        get_filename_component (FFMPEG_PREFIX_PATH "${FFMPEG_DIRECTORY}/x64" ABSOLUTE)
    endif()
endif()
if (CMAKE_SYSTEM_PROCESSOR MATCHES "riscv.*|RISCV.*|riscv64.*|RISCV64.*")
    get_filename_component (FFMPEG_PREFIX_PATH "${FFMPEG_DIRECTORY}/riscv" ABSOLUTE)
endif()

if (NOT EXISTS "${FFMPEG_PREFIX_PATH}")
    file(MAKE_DIRECTORY "${FFMPEG_PREFIX_PATH}")
endif()

if (NOT EXISTS "${FFMPEG_BUILD_PATH}")
    file(MAKE_DIRECTORY "${FFMPEG_BUILD_PATH}")
endif()

# add cmake help function
include (AddProjectLibrary)

cmake_host_system_information(RESULT PHYSICAL_CORES QUERY NUMBER_OF_PHYSICAL_CORES)
if (PHYSICAL_CORES EQUAL 0)
    set(PHYSICAL_CORES 1)
endif()

# build ffmpeg
if (AXCL_BUILD_FFMPEG)
    if (WIN32)
        # configure ffmpeg
        set(FFMPEG_CONFIG_COMMAND
            ${CMAKE_COMMAND} -E env
            CHERE_INVOKING=1
            MSYSTEM=MINGW64
            ${WIN_MSYS2_BASH} -lc
            "${FFMPEG_SOURCE_PATH}/configure --prefix=${FFMPEG_PREFIX_PATH} --libdir=${FFMPEG_PREFIX_PATH}/lib --shlibdir=${FFMPEG_PREFIX_PATH}/lib --enable-shared --disable-static --enable-gpl --enable-ffmpeg --enable-swscale --disable-avdevice --enable-encoders --enable-filters --enable-axmm --disable-large-tests --disable-doc --disable-logging --enable-pic --disable-libxcb_shm --enable-zlib --disable-bzlib --disable-lzma"
        )
        add_custom_target(config_ffmpeg
            COMMAND ${FFMPEG_CONFIG_COMMAND}
            WORKING_DIRECTORY ${FFMPEG_BUILD_PATH}
            COMMENT "Configure ffmpeg, this may take several minutes, please wait ...\n"
            USES_TERMINAL
        )

        # build ffmpeg
        # 1. check win64/config/Makefile exist or not
        # 2. if not exist, configure ffmpeg first, otherwise build directly
        # 3. if configuration options are changed, rm win64/config/* -rf to start configure
        set(FFMPEG_BUILD_DEPENDS)
        if(NOT EXISTS ${FFMPEG_BUILD_PATH}/Makefile)
            list(APPEND FFMPEG_BUILD_DEPENDS config_ffmpeg)
        endif()
        set(FFMPEG_BUILD_COMMAND
            ${CMAKE_COMMAND} -E env
            CHERE_INVOKING=1
            MSYSTEM=MINGW64
            ${WIN_MSYS2_BASH} -lc
            "make -s CFLAGS='-DWIN32 -I${AXCL_INSTALL_INCLUDE_PATH} -I${WIN_MSYS2_PATH}/mingw64/include/SDL2' -j${PHYSICAL_CORES} && make install"
        )
        add_custom_target(build_ffmpeg ALL
            COMMAND ${FFMPEG_BUILD_COMMAND}
            WORKING_DIRECTORY ${FFMPEG_BUILD_PATH}
            COMMENT "Build ffmpeg ...\n"
            DEPENDS ${FFMPEG_BUILD_DEPENDS}
            USES_TERMINAL
        )

        # clean ffmpeg
        set(FFMPEG_CLEAN_COMMAND
            ${CMAKE_COMMAND} -E env
            CHERE_INVOKING=1
            MSYSTEM=MINGW64
            ${WIN_MSYS2_BASH} -lc
            "make clean"
        )
        add_custom_target(clean_ffmpeg
            COMMAND ${FFMPEG_CLEAN_COMMAND}
            WORKING_DIRECTORY ${FFMPEG_BUILD_PATH}
            COMMENT "Clean ffmpeg ...\n"
            USES_TERMINAL
        )

    else()
        # Linux ffmpeg build
    endif()
endif() # end of AXCL_BUILD_FFMPEG

# add avcodec interface target
set(FFMPEG_COMPONENTS
    avcodec
    avutil
    avformat
    avfilter
    swresample
    swscale
    postproc
)

foreach(component IN LISTS FFMPEG_COMPONENTS)
    if(WIN32)
        set(FFMPEG_LIB_PATH "${FFMPEG_PREFIX_PATH}/lib/lib${component}.lib")
    else()
        set(FFMPEG_LIB_PATH "${component}")
    endif()

    add_project_library(lib${component} INTERFACE
        PUBLIC
            COMPILE_OPTIONS
                $<$<C_COMPILER_ID:MSVC>:/wd4819>
            INCLUDE_DIRECTORIES
                $<BUILD_INTERFACE:${FFMPEG_PREFIX_PATH}/include>
            LINK_DIRECTORIES
                $<BUILD_INTERFACE:${FFMPEG_PREFIX_PATH}/lib>
            LINK_LIBRARIES
                ${FFMPEG_LIB_PATH}
        ALIAS
            3rdparty::ffmpeg::${component}
    )
endforeach()

add_project_library(libffmpeg INTERFACE
    PUBLIC
        LINK_LIBRARIES
            3rdparty::ffmpeg::avformat
            3rdparty::ffmpeg::avcodec
            3rdparty::ffmpeg::avutil
            3rdparty::ffmpeg::avfilter
            3rdparty::ffmpeg::swresample
            3rdparty::ffmpeg::swscale
            3rdparty::ffmpeg::postproc
    ALIAS
        3rdparty::ffmpeg
)

if (WIN32)
    file(GLOB FFMPEG_INSTALL_FILES
        ${FFMPEG_PREFIX_PATH}/lib/libav*.dll
        ${FFMPEG_PREFIX_PATH}/lib/libsw*.dll
        ${FFMPEG_PREFIX_PATH}/lib/libpostproc*.dll
        ${FFMPEG_PREFIX_PATH}/bin/ff*.exe
        # mingw64 library
        ${FFMPEG_PREFIX_PATH}/mingw64/libiconv-2.dll
        ${FFMPEG_PREFIX_PATH}/mingw64/libva_win32.dll
        ${FFMPEG_PREFIX_PATH}/mingw64/libva.dll
        ${FFMPEG_PREFIX_PATH}/mingw64/libwinpthread-1.dll
        ${FFMPEG_PREFIX_PATH}/mingw64/zlib1.dll
    )

    install(
        FILES
            ${FFMPEG_INSTALL_FILES}
        DESTINATION bin
    )
endif()
