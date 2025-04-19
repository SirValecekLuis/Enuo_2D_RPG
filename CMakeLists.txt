cmake_minimum_required(VERSION 3.31)
project(Enuo_2D_RPG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Nastavení kompilátoru podle build typu
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug build s Address Sanitizerem
    if(MSVC)
        # MSVC compiler (Visual Studio)
        add_compile_options(
                /W4     # Úroveň varování 4
                /WX     # Považovat varování za chyby
                /wd4100 # Nepoužité parametry (často v callbackách)
                /wd4201 # Nepojmenované struktury (běžné v grafických knihovnách)
                /EHsc   # Standard exception handling
                /DEBUG  # Generovat debug info
                /Zi     # Debug informace
                /Od     # Vypnout optimalizace pro debug
                /fsanitize=address # Address sanitizer pro MSVC (VS 2019 a novější)
        )
        add_link_options(
                /DEBUG
                /INCREMENTAL:NO
        )
    else()
        # GCC/Clang
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
    # Release build
    if(MSVC)
        # MSVC compiler
        add_compile_options(
                /W4
                /O2    # Optimalizace pro rychlost
                /GL    # Whole program optimization
                /Gy    # Function-level linking
                /Gw    # Global data optimization
                /GS-   # Vypnout buffer security check pro výkon
        )
        add_link_options(
                /LTCG  # Link-time code generation
                /OPT:REF,ICF # Remove unreferenced code & identical COMDAT folding
        )
    else()
        # GCC/Clang
        add_compile_options(
                -Wall
                -Wextra
                -O3              # Agresivní optimalizace
                -flto            # Link-time optimization
                -march=native    # Optimalizace pro konkrétní CPU
                -ffast-math      # Rychlejší matematické operace
        )
        add_link_options(
                -flto
        )
    endif()
endif()

# Přidání vlastních definic pro DEBUG/RELEASE
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

# Hledání raylib
find_package(raylib CONFIG REQUIRED)

# Vytvoření executable
add_executable(Enuo_2D_RPG main.cpp)

# Linkovací knihovny
target_link_libraries(Enuo_2D_RPG PRIVATE raylib)

# Výstupní adresáře
set_target_properties(Enuo_2D_RPG PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Kopírování závislostí do výstupního adresáře (pro Win)
if(WIN32)
    add_custom_command(TARGET Enuo_2D_RPG POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Enuo_2D_RPG>
            $<TARGET_FILE_DIR:Enuo_2D_RPG>
            COMMAND_EXPAND_LISTS
    )
endif()