# All of these will be set as PUBLIC sources to Bonsai
set(_temp_bonsai_headers
        Bonsai/EntryPoint.h
        Bonsai/Log.h
        Bonsai/Application.h
        Bonsai/Events/Event.h
        Bonsai/Events/ApplicationEvent.h
        Bonsai/Events/KeyEvent.h
        Bonsai/Events/WindowEvent.h
        Bonsai/Events/MouseEvent.h
        )

set(_temp_bonsai_sources
        Bonsai/Log.cpp
        Bonsai/Application.cpp
        )

# Check if given C++ source compiles and links into an executable.
include(CheckCXXSourceCompiles)

# Check if we can link to `std::filesystem`. macOS < 10.15, iOS < 13, Android.
check_cxx_source_compiles("#include <filesystem>\nint main() { std::filesystem::space(\"/\"); }" HAS_STD_FILESYSTEM)
# Check if we have `std::filesystem` headers. iOS < 13.
check_cxx_source_compiles("#include <filesystem>\nint main() { using path = std::filesystem::path;\n0; }" HAS_STD_FILESYSTEM_HEADERS)

get_filename_component(CURRENT_PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} PATH)
if(BONSAI_LINK_RESOURCES)
    # Directory that Bonsai resources can be found.
    set(BONSAI_RESOURCES_DIR "${CURRENT_PARENT_DIR}/Resources")
endif()
if(BONSAI_INSTALL_RESOURCES)
    # Install resources for end-user usage because many source files use these
    install(DIRECTORY "${CURRENT_PARENT_DIR}/Resources"
            # Example: this will install the Resources dir to /usr/share/Bonsai/Resources on Linux
            DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}"
            )
endif()

# Generates a header containing export macros
include(GenerateExportHeader)
generate_export_header(Bonsai EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/Export.h")

# Adds a CMake generated config file
configure_file(Config.h.in "${CMAKE_CURRENT_BINARY_DIR}/Config.h" @ONLY)

# Sets all headers as PUBLIC sources for Bonsai
# The BUILD/INSTALL interface generator expressions are for the EXPORT command
# Otherwise it wouldn't know where to look for them
foreach(_bonsai_header IN LISTS _temp_bonsai_headers _temp_bonsai_third_party_headers)
    target_sources(Bonsai PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${_bonsai_header}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${_bonsai_header}>
            )
endforeach()
# Sets all sources (cpp) as PRIVATE sources for Bonsai
# An INSTALL_INTERFACE isn't needed, as cpp files aren't installed
foreach(_bonsai_source IN LISTS _temp_bonsai_sources _temp_bonsai_third_party_sources)
    target_sources(Bonsai PRIVATE
            $<BUILD_INTERFACE:${_bonsai_source}>
            )
endforeach()

# Changes how sources appear in IDE virtual file structures.
include(BonsaiGroupSources)
bonsai_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${_temp_bonsai_headers}")
bonsai_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${_temp_bonsai_sources}")
bonsai_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/third_party" "ThirdParty/" "${_temp_bonsai_third_party_headers}")
bonsai_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/third_party" "ThirdParty/" "${_temp_bonsai_third_party_sources}")

# Include this file in our project view.
target_sources(Bonsai PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/CMakeSources.cmake>
        )
