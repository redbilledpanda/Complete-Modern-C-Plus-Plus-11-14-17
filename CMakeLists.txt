cmake_minimum_required(VERSION 3.16)
project(CompleteModernCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 🔥 Step 1: Set up the top-level log file
set(TOPLEVEL_LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake_build_log.txt")

# Delete the existing log file (if it exists)
if (EXISTS ${TOPLEVEL_LOG_FILE})
    file(REMOVE ${TOPLEVEL_LOG_FILE})
endif()

# Create a new log file with a header
file(WRITE ${TOPLEVEL_LOG_FILE} "CMake Build Log for ${PROJECT_NAME}\n====================\n")
file(APPEND ${TOPLEVEL_LOG_FILE} "🔹 CMake started processing the project at ${CMAKE_CURRENT_SOURCE_DIR}\n\n")

# Function to check if a directory contains a CMakeLists.txt
function(has_cmake_lists dir result_var)
    if (EXISTS "${dir}/CMakeLists.txt")
        set(${result_var} TRUE PARENT_SCOPE)
    else()
        set(${result_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Function to check if a directory contains .cpp files
function(has_cpp_files dir result_var)
    file(GLOB_RECURSE CPP_FILES "${dir}/*.cpp")

    if (CPP_FILES)
        set(${result_var} TRUE PARENT_SCOPE)
    else()
        set(${result_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Function to check if a directory contains subdirectories
function(has_subdirectories dir result_var)
    file(GLOB SUBDIRS LIST_DIRECTORIES true "${dir}/*")

    set(dir_count 0)
    foreach(sub ${SUBDIRS})
        if (IS_DIRECTORY ${sub})
            math(EXPR dir_count "${dir_count} + 1")
        endif()
    endforeach()

    if (dir_count GREATER 0)
        set(${result_var} TRUE PARENT_SCOPE)
    else()
        set(${result_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Get all first-level subdirectories in the project root
file(GLOB SUBDIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(subdir ${SUBDIRS})
    if (IS_DIRECTORY ${subdir})
        has_cmake_lists(${subdir} contains_cmake)
        has_cpp_files(${subdir} contains_cpp)
        has_subdirectories(${subdir} contains_subdirs)

        # Convert directory name to a safe format (replace spaces with underscores)
        get_filename_component(DIR_NAME ${subdir} NAME)
        string(REPLACE " " "_" DIR_NAME ${DIR_NAME})

        if (contains_cmake)
            message(STATUS "Adding section with CMakeLists.txt: ${DIR_NAME}")
            file(APPEND ${TOPLEVEL_LOG_FILE} "✔ Adding section with CMakeLists.txt: ${DIR_NAME}\n")
            add_subdirectory(${subdir} ${DIR_NAME})  # Use the modified name
        elseif (contains_cpp AND NOT contains_subdirs)
            has_cmake_lists(${subdir} has_cmake)
            if(NOT has_cmake)
                # Convert name safely
                get_filename_component(SAFE_TARGET_NAME ${subdir} NAME)
                string(REPLACE " " "_" SAFE_TARGET_NAME ${SAFE_TARGET_NAME})
        
                file(GLOB_RECURSE SOURCE_FILES "${subdir}/*.cpp")
        
                # Generate CMakeLists.txt
                set(GENERATED_FILE "${subdir}/CMakeLists.txt")
                file(WRITE ${GENERATED_FILE}
                    "cmake_minimum_required(VERSION 3.16)\n"
                    "project(${SAFE_TARGET_NAME})\n\n"
                    "set(CMAKE_CXX_STANDARD 17)\n"
                    "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
                    "file(GLOB_RECURSE SRC_FILES \"\${CMAKE_CURRENT_SOURCE_DIR}/*.cpp\")\n"
                    "add_executable(${SAFE_TARGET_NAME} \${SRC_FILES})\n"
                )
        
                message(STATUS "📝 Auto-generated CMakeLists.txt in: ${subdir}")
                file(APPEND ${TOPLEVEL_LOG_FILE} "📝 Auto-generated leaf CMakeLists.txt in: ${subdir}\n")
            endif()
        
            message(STATUS "Adding single-source section: ${DIR_NAME}")
            file(APPEND ${TOPLEVEL_LOG_FILE} "✔ Adding single-source section: ${DIR_NAME}\n")
            add_subdirectory(${subdir} ${DIR_NAME})
        else()
            message(WARNING "Skipping redundant directory: ${subdir}")
            file(APPEND ${TOPLEVEL_LOG_FILE} "⚠ Skipping redundant directory: ${subdir}\n")
        endif()
    endif()
endforeach()

file(APPEND ${TOPLEVEL_LOG_FILE} "\n✅ CMake processing completed successfully!\n")
