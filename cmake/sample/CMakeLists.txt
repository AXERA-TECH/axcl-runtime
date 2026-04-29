# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set sample working path
get_filename_component (AXCL_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample" ABSOLUTE)

# add cmake help function
include (AddProjectLibrary)
include (AddProjectExecutable)

# base sample app add function
function (add_sample_app path)
    cmake_parse_arguments (ARG
        ""
        "NAME"
        "PATH;SOURCES;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;LINK_OPTIONS;LINK_DIRECTORIES;LINK_LIBRARIES;"
        ${ARGN}
    )

    if (NOT ARG_NAME)
        get_filename_component(path_name ${path} NAME_WE)
        set (name axcl_sample_${path_name})
    else()
        set (name ${ARG_NAME})
    endif ()

    aux_source_directory (${AXCL_SAMPLE_PATH}/${path} AXCL_SAMPLE_SRCS)
    foreach (dir ${ARG_PATH})
        aux_source_directory (${AXCL_SAMPLE_PATH}/${dir} AXCL_SAMPLE_SRCS)
    endforeach()

    add_project_executable (${name}
        SOURCES
            ${ARG_SOURCES}
            ${AXCL_SAMPLE_SRCS}
        INCLUDE_DIRECTORIES
            ${ARG_INCLUDE_DIRECTORIES}
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/${path}>
            $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/utils>
            $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
            $<BUILD_INTERFACE:${AXCL_ROOT_DIR}/toolkit>
        COMPILE_DEFINITIONS
            ${ARG_COMPILE_DEFINITIONS}
        COMPILE_OPTIONS
            ${ARG_COMPILE_OPTIONS}
        LINK_OPTIONS
            ${ARG_LINK_OPTIONS}
        LINK_DIRECTORIES
            ${AXCL_SAMPLE_PATH}/${ARG_LINK_DIRECTORIES}
        LINK_LIBRARIES
            ${ARG_LINK_LIBRARIES}
            axcl::host::comm
            axcl::runtime
            3rdparty::cmdline
            3rdparty::spdlog
    )
endfunction()

# base sample lib add function
function (add_sample_lib path)
    cmake_parse_arguments (ARG
        ""
        "NAME"
        "PATH;SOURCES;INCLUDE_DIRECTORIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;LINK_OPTIONS;LINK_DIRECTORIES;LINK_LIBRARIES;"
        ${ARGN}
    )

    if (NOT ARG_NAME)
        get_filename_component(path_name ${path} NAME_WE)
        set (name axcl_sample_${path_name})
    else()
        set (name ${ARG_NAME})
    endif ()

    aux_source_directory (${AXCL_SAMPLE_PATH}/${path} AXCL_SAMPLE_SRCS)
    foreach (dir ${ARG_PATH})
        aux_source_directory (${AXCL_SAMPLE_PATH}/${dir} AXCL_SAMPLE_SRCS)
    endforeach()

    add_project_library (${name} SHARED
        SOURCES
            ${ARG_SOURCES}
            ${AXCL_SAMPLE_SRCS}
        PRIVATE
            INCLUDE_DIRECTORIES
                ${ARG_INCLUDE_DIRECTORIES}
                $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/${path}>
                $<BUILD_INTERFACE:${AXCL_SAMPLE_PATH}/utils>
                $<BUILD_INTERFACE:${AXCL_INSTALL_INCLUDE_PATH}>
                ${AXCL_ROOT_DIR}/toolkit
            COMPILE_DEFINITIONS
                ${ARG_COMPILE_DEFINITIONS}
            COMPILE_OPTIONS
                $<$<NOT:$<BOOL:${SYSTEM_IS_WINDOWS}>>:-shared>
                ${ARG_COMPILE_OPTIONS}
            LINK_OPTIONS
                ${ARG_LINK_OPTIONS}
            LINK_DIRECTORIES
                ${AXCL_SAMPLE_PATH}/${ARG_LINK_DIRECTORIES}
            LINK_LIBRARIES
                ${ARG_LINK_LIBRARIES}
                axcl::host::comm
                axcl::runtime
                3rdparty::cmdline
                3rdparty::spdlog
    )

    target_include_directories(${name} PUBLIC ${ARG_INCLUDE_DIRECTORIES})
endfunction()

# add memory sample path
include (memory.cmake)

# add runtime sample path
include (runtime.cmake)

# add sys sample path
include (sys.cmake)

# add usrworker sample path
include (usrworker.cmake)

# add dmadim sample path
include (dmadim.cmake)

# add engine sample path
include (engine.cmake)

# add axclite sample path
include (axclite.cmake)

# add ppl sample path
include (ppl.cmake)

# add vdec sample path
include (vdec.cmake)

# add ffmpeg sample path
include (ffmpeg.cmake)

if (NOT SYSTEM_IS_WINDOWS)
    include (ive.cmake)
    include (skel.cmake)
    include (venc.cmake)
    include (ivps.cmake)
    include (aicard.cmake)
endif()
