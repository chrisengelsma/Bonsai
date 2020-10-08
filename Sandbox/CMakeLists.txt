file(GLOB_RECURSE GAME_HEADER_FILES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "*.h" "*.hpp" "*.inl"
        )
file(GLOB_RECURSE GAME_SOURCE_FILES
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        "*.c" "*.cpp" "*.rc"
        )
add_executable(Game ${GAME_HEADER_FILES} ${GAME_SOURCE_FILES})

target_compile_features(Game PUBLIC cxx_std_17)
target_include_directories(Game PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Game PRIVATE Bonsai::Bonsai)

file(COPY ${CMAKE_SOURCE_DIR}/Resources
        DESTINATION ${CMAKE_BINARY_DIR}/bin)

if(MSVC AND "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set_target_properties(Game PROPERTIES
            LINK_FLAGS "/subsystem:windows /ENTRY:mainCRTStartup"
            )
elseif(UNIX AND APPLE)
    set_target_properties(Game PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "Test Game"
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${BONSAI_VERSION}
            MACOSX_BUNDLE_LONG_VERSION_STRING ${BONSAI_VERSION}
            MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/CMake/Info.plist.in"
            )
endif()

if(BONSAI_INSTALL_EXAMPLES)
    install(TARGETS Game
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            )
endif()

include(BonsaiGroupSources)
bonsai_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${GAME_HEADER_FILES}")
bonsai_group_sources("${CMAKE_CURRENT_SOURCE_DIR}" "/" "" "${GAME_SOURCE_FILES}")
