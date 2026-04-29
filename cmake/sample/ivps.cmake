# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#


# set ivps sample path
get_filename_component (AXCL_IVPS_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ivps" ABSOLUTE)
get_filename_component (AXCL_IVPS_TOOLS_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ivps/tools" ABSOLUTE)
get_filename_component (AXCL_IVPS_INI_PARSER_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ivps/tools/ini_parser" ABSOLUTE)
get_filename_component (AXCL_IVPS_SYNC_API_SAMPLE_PATH "${AXCL_ROOT_DIR}/sample/ivps/sync_api" ABSOLUTE)

# collect all source files
aux_source_directory (${AXCL_IVPS_SAMPLE_PATH} AXCL_IVPS_SAMPLE_SRCS)
aux_source_directory (${AXCL_IVPS_TOOLS_SAMPLE_PATH} AXCL_IVPS_TOOLS_SAMPLE_SRCS)
aux_source_directory (${AXCL_IVPS_INI_PARSER_SAMPLE_PATH} AXCL_IVPS_INI_PARSER_SAMPLE_SRCS)
aux_source_directory (${AXCL_IVPS_SYNC_API_SAMPLE_PATH} AXCL_IVPS_SYNC_API_SAMPLE_SRCS)

# add ivps sample (Linux only)
if (NOT SYSTEM_IS_WINDOWS)
    add_sample_app (ivps
        NAME
            axcl_sample_ivps
        SOURCES
            ${AXCL_IVPS_SAMPLE_SRCS}
            ${AXCL_IVPS_TOOLS_SAMPLE_SRCS}
            ${AXCL_IVPS_INI_PARSER_SAMPLE_SRCS}
            ${AXCL_IVPS_SYNC_API_SAMPLE_SRCS}
        LINK_LIBRARIES
            axcl::runtime
            axcl::native::sys
            axcl::native::ivps
    )
endif ()
