# Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
#
# This source file is the property of Axera Semiconductor Co., Ltd. and
# may not be copied or distributed in any isomorphic form without the prior
# written consent of Axera Semiconductor Co., Ltd.
#

# Windows version configuration function
function (ConfigWindowsVersion _target _file_description _product_name)
    # Only apply to Windows platform
    if (NOT SYSTEM_IS_WINDOWS)
        return()
    endif()

    # Parse version string (format: "V3.6.4" or "3.6.4")
    string(REGEX REPLACE "^V" "" VERSION_CLEAN "${BUILD_VERSION}")
    string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${VERSION_CLEAN}")
    if (VERSION_MATCH)
        set(VERSION_MAJOR ${CMAKE_MATCH_1})
        set(VERSION_MINOR ${CMAKE_MATCH_2})
        set(VERSION_PATCH ${CMAKE_MATCH_3})
    else()
        # Default version if parsing fails
        set(VERSION_MAJOR 1)
        set(VERSION_MINOR 0)
        set(VERSION_PATCH 0)
    endif()

    # Set version information variables
    set(COMPANY_NAME "Axera Semiconductor Co., Ltd.")
    set(FILE_DESCRIPTION "${_file_description}")
    set(INTERNAL_NAME "${_target}")
    set(LEGAL_COPYRIGHT "Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.")
    set(PRODUCT_NAME "${_product_name}")

    # Set version file paths
    set(VERSION_RC_IN "${CMAKE_SOURCE_DIR}/config/version.rc.in")
    set(VERSION_RC_OUT "${CMAKE_CURRENT_BINARY_DIR}/generated/${_target}_version.rc")

    # Create generated directory if it doesn't exist
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated")

    # Configure version resource file
    configure_file("${VERSION_RC_IN}" "${VERSION_RC_OUT}" @ONLY)

    message(STATUS "Configured Windows version file: ${VERSION_RC_OUT} (version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})")

    # Add version resource file to target
    target_sources(${_target} PRIVATE "${VERSION_RC_OUT}")
endfunction()
