add_library(Bonsai)
add_library(Bonsai::Bonsai ALIAS Bonsai)

# This file is quite big, so for readability we include instead...
# This uses target_sources(Bonsai) to define its headers & source files
include(CMakeSources.cmake)
include(FetchContent)

# Used to track if we're using ONLY system libs
# Prevents errors with EXPORT
set(_BONSAI_ALL_SYSTEM_LIBS true)

find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
find_package(GLFW REQUIRED)
find_package(glslang REQUIRED)
find_package(spdlog REQUIRED)

if (APPLE)
    set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework IOKit -framework Foundation -framework CoreFoundation")
endif ()

target_compile_features(Bonsai PUBLIC c_std_11 cxx_std_17)
target_compile_definitions(Bonsai
        PUBLIC
        # If the CONFIG is Debug or RelWithDebInfo, define BONSAI_DEBUG
        # Works on both single and mutli configuration
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:DEBUG BONSAI_DEBUG>
        # 32-bit
        $<$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>:BONSAI_BUILD_32BIT>
        # 64-bit
        $<$<EQUAL:8,${CMAKE_SIZEOF_VOID_P}>:BONSAI_BUILD_64BIT>
        # Windows
        $<$<PLATFORM_ID:Windows>:BONSAI_BUILD_WINDOWS WIN32_LEAN_AND_MEAN NOMINMAX>
        # Linux
        $<$<PLATFORM_ID:Linux>:BONSAI_BUILD_LINUX>
        # macOS
        $<$<PLATFORM_ID:Darwin>:BONSAI_BUILD_MACOS>
        # MSVC
        $<$<CXX_COMPILER_ID:MSVC>:BONSAI_BUILD_MSVC _SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS>
        # Clang/AppleClang
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:BONSAI_BUILD_CLANG>
        # GNU/GCC
        $<$<CXX_COMPILER_ID:GNU>:BONSAI_BUILD_GNU __USE_MINGW_ANSI_STDIO=0>
        )
target_compile_options(Bonsai
        PUBLIC
        # Disables symbol warnings.
        $<$<CXX_COMPILER_ID:MSVC>:/wd4251 /wd4592 /wd4018 /wd4101 /wd4244 /wd4267 /wd4756 /wd4309 /wd26812>
        PRIVATE
        # Enables SSE4.1, it is also possible to use SSE2 with -msse2
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.1>
        # Enabled SSE2 for MSVC for 32-bit.
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<EQUAL:4,${CMAKE_SIZEOF_VOID_P}>>:/arch:SSE2>
        )
target_include_directories(Bonsai
        PUBLIC
        # Project source includes
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        # Project third_party includes
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party>
        # Generated headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        # Helps the includes find what they need at runtime
        # Although this also allows people to not prefix "Bonsai" before includes as well
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
        PRIVATE
        )
target_link_libraries(Bonsai
        PUBLIC
        # All IMPORTED targets, which automatically handles includes
        $<$<CXX_COMPILER_ID:GNU>:stdc++fs> # std::filesystem
        $<$<PLATFORM_ID:Windows>:ws2_32> # Winsock 2.0
        $<$<PLATFORM_ID:Android>:log> # log support
        ${OTHER_LDFLAGS}
        ${CMAKE_DL_LIBS} # dlopen and dlclose

        Threads::Threads # pthread
        Vulkan::Vulkan
        spdlog::spdlog # Logging

        PRIVATE

        # More IMPORTED
        "-L/usr/local/lib"
        "-lglfw"
        "-framework Cocoa"
        "-framework Metal"
        "-framework IOKit"
        "-framework CoreFoundation"
        "-framework CoreVideo"
        "-framework Carbon"

        )

set_target_properties(Bonsai PROPERTIES
        #        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
        FOLDER "Bonsai"
        )

# Installs all headers, preserving their file structure
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Config.h"
        "${CMAKE_CURRENT_BINARY_DIR}/Export.h"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
        )
foreach (_bonsai_header IN LISTS _temp_bonsai_headers _temp_bonsai_third_party_headers)
    get_filename_component(_bonsai_header_dir ${_bonsai_header} DIRECTORY)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_bonsai_header}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_bonsai_header_dir}"
            )
endforeach ()
# If strictly using system libs, we can generate an export & install it
if (_BONSAI_ALL_SYSTEM_LIBS)
    install(TARGETS Bonsai
            # Auto-generates an export to install
            EXPORT
            BonsaiTargets
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            )
    # Install the auto-generated export
    install(EXPORT BonsaiTargets
            # Makes the end-user imported library Bonsai::Bonsai instead of just Bonsai
            NAMESPACE Bonsai::
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
            )
    # Defines write_basic_package_version_file
    include(CMakePackageConfigHelpers)
    # Dynamically writes the path to the resources dir (if installed) into the BonsaiConfig.cmake file
    configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/BonsaiConfig.cmake.in"
            "${PROJECT_BINARY_DIR}/BonsaiConfig.cmake"
            INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
            PATH_VARS CMAKE_INSTALL_DATADIR
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
            )
    # Auto-generates a version file based on the current version specified in project()
    write_basic_package_version_file("${PROJECT_BINARY_DIR}/BonsaiConfigVersion.cmake"
            COMPATIBILITY SameMajorVersion
            )
    # Install the auto-generated version file and BonsaiConfig
    install(FILES
            "${PROJECT_BINARY_DIR}/BonsaiConfig.cmake"
            "${PROJECT_BINARY_DIR}/BonsaiConfigVersion.cmake"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
            )


else ()
    # Install without an export since we're using 1 (or more) non-system libs
    install(TARGETS Bonsai
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            )
endif ()