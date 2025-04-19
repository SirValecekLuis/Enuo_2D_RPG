cmake_minimum_required(VERSION 3.31)
project(Enuo_2D_RPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(
                /W4
                /WX
                /wd4100
                /wd4201
                /EHsc
                /DEBUG
                /Zi
                /Od
        )

        if(MSVC_VERSION GREATER_EQUAL 1929)
            add_compile_options(/fsanitize=address)
            add_link_options(/fsanitize=address)
        endif()

        add_link_options(
                /DEBUG
                /INCREMENTAL:NO
        )
    else()
        add_compile_options(
                -Wall
                -Wextra
                -Wpedantic
                -Werror
                -Wno-unused-parameter
                -g3
                -O0
                -fsanitize=address
                -fno-omit-frame-pointer
        )
        add_link_options(
                -fsanitize=address
        )
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(
                /W4
                /O2
                /GL
                /Gy
                /Gw
                /GS-
        )
        add_link_options(
                /LTCG
                /OPT:REF,ICF
        )
    else()
        add_compile_options(
                -Wall
                -Wextra
                -O3
                -flto
                -march=native
                -ffast-math
        )
        add_link_options(
                -flto
        )
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(
            DEBUG=1
            _DEBUG=1
    )
else()
    add_compile_definitions(
            NDEBUG=1
    )
endif()

find_package(raylib CONFIG REQUIRED)

add_executable(Enuo_2D_RPG main.cpp)

target_link_libraries(Enuo_2D_RPG PRIVATE raylib)

set_target_properties(Enuo_2D_RPG PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

if(WIN32)
    add_custom_command(TARGET Enuo_2D_RPG POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Enuo_2D_RPG>
            $<TARGET_FILE_DIR:Enuo_2D_RPG>
            COMMAND_EXPAND_LISTS
    )

    # Přidání runtime DLL pro AddressSanitizer (pouze pro Debug)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET Enuo_2D_RPG POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.43.34808/bin/Hostx64/x64/clang_rt.asan_dynamic-x86_64.dll"
                $<TARGET_FILE_DIR:Enuo_2D_RPG>
        )
    endif()
endif()