cmake_minimum_required(VERSION 3.16)
project(Section07)  # Modify dynamically for other sections if needed

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Create a log file at the section level
set(SECTION_LOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/section_build_log.txt")
file(WRITE ${SECTION_LOG_FILE} "CMake Build Log for ${PROJECT_NAME}\n====================\n")

# 🔥 Step 1: Remove all existing CMakeLists.txt files inside this section
file(GLOB_RECURSE EXISTING_CMAKE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt")
foreach(cmake_file ${EXISTING_CMAKE_FILES})
    file(REMOVE ${cmake_file})
    file(APPEND ${SECTION_LOG_FILE} "🗑 Deleted old CMakeLists.txt: ${cmake_file}\n")
endforeach()

# Function to check if a directory has subdirectories (i.e., is NOT a leaf)
function(has_subdirectories dir result_var)
    file(GLOB SUBDIRS LIST_DIRECTORIES true "${dir}/*")

    set(dir_has_subdirs FALSE)
    foreach(sub ${SUBDIRS})
        if (IS_DIRECTORY ${sub})
            set(dir_has_subdirs TRUE)
            break()
        endif()
    endforeach()

    set(${result_var} ${dir_has_subdirs} PARENT_SCOPE)
endfunction()

# Function to check if a directory contains .cpp files
function(has_cpp_files dir result_var)
    file(GLOB CPP_FILES "${dir}/*.cpp")

    if (CPP_FILES)
        set(${result_var} TRUE PARENT_SCOPE)
    else()
        set(${result_var} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Function to recursively traverse down to the leaf directories
function(traverse_to_leaf_dirs dir)
    has_subdirectories(${dir} has_subdirs)

    if (NOT has_subdirs)
        # If we are at a true leaf directory, check for .cpp files
        has_cpp_files(${dir} contains_cpp)

        if (contains_cpp)
            # Convert directory name to a safe format (replace spaces with underscores)
            get_filename_component(DIR_NAME ${dir} NAME)
            string(REPLACE " " "_" DIR_NAME ${DIR_NAME})            

            # Ensure a CMakeLists.txt exists
            set(CMAKE_FILE "${dir}/CMakeLists.txt")
            if (NOT EXISTS ${CMAKE_FILE} AND NOT DIR_NAME STREQUAL "")
                # Generate CMakeLists.txt for this leaf directory
                file(WRITE ${CMAKE_FILE} "cmake_minimum_required(VERSION 3.16)\n")
                file(APPEND ${CMAKE_FILE} "project(${DIR_NAME})\n\n")
                file(APPEND ${CMAKE_FILE} "set(CMAKE_CXX_STANDARD 17)\n")
                file(APPEND ${CMAKE_FILE} "set(CMAKE_CXX_STANDARD_REQUIRED True)\n\n")
                file(APPEND ${CMAKE_FILE} "file(GLOB SOURCES \"\${CMAKE_CURRENT_SOURCE_DIR}/*.cpp\")\n\n")
                file(APPEND ${CMAKE_FILE} "add_executable(${DIR_NAME} \${SOURCES})\n")
                file(APPEND ${CMAKE_FILE} "target_include_directories(${DIR_NAME} PRIVATE \${CMAKE_CURRENT_SOURCE_DIR})\n")
                file(APPEND ${SECTION_LOG_FILE} "🛠 Auto-generated CMakeLists.txt in ${dir}\n")
            endif()

            if (EXISTS ${CMAKE_FILE})
                message(STATUS "Adding leaf subdirectory: ${dir}")
                file(APPEND ${SECTION_LOG_FILE} "✔ Adding leaf subdirectory: ${dir}\n")
                add_subdirectory(${dir} ${DIR_NAME})
            endif()
        else()
            message(WARNING "Skipping empty leaf directory: ${dir}")
            file(APPEND ${SECTION_LOG_FILE} "⚠ Skipping empty leaf directory: ${dir}\n")
        endif()
    else()
        # If not a leaf, recursively traverse deeper
        file(GLOB SUBDIRS LIST_DIRECTORIES true "${dir}/*")
        foreach(sub ${SUBDIRS})
            if (IS_DIRECTORY ${sub})
                traverse_to_leaf_dirs(${sub})
            endif()
        endforeach()
    endif()
endfunction()

# Start traversing from the section root
file(GLOB SUBDIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(subdir ${SUBDIRS})
    if (IS_DIRECTORY ${subdir})
        traverse_to_leaf_dirs(${subdir})
    endif()
endforeach()