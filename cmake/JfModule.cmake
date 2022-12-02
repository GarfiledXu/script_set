# short command for adding simple OpenCV module
# see ocv_add_module for argument details
# Usage:
# ocv_define_module(module_name  [INTERNAL] [EXCLUDE_CUDA] [REQUIRED] [<list of dependencies>] [OPTIONAL <list of optional dependencies>] [WRAP <list of wrappers>])







macro(ocv_define_module module_name)
  ocv_debug_message("ocv_define_module(" ${module_name} ${ARGN} ")")
  set(_argn ${ARGN})
  set(exclude_cuda "")
  foreach(arg ${_argn})
    if("${arg}" STREQUAL "EXCLUDE_CUDA")
      set(exclude_cuda "${arg}")
      list(REMOVE_ITEM _argn ${arg})
    endif()
  endforeach()

  ocv_add_module(${module_name} ${_argn})
  ocv_glob_module_sources(${exclude_cuda})
  ocv_module_include_directories()
  ocv_create_module()

  ocv_add_accuracy_tests()
  ocv_add_perf_tests()
  ocv_add_samples()
endmacro()

# declare new OpenCV module in current folder
# Usage:
#   ocv_add_module(<name> [INTERNAL|BINDINGS] [REQUIRED] [<list of dependencies>] [OPTIONAL <list of optional dependencies>] [WRAP <list of wrappers>])
# Example:
#   ocv_add_module(yaom INTERNAL opencv_core opencv_highgui opencv_flann OPTIONAL opencv_cudev)
macro(ocv_add_module _name)
  ocv_debug_message("ocv_add_module(" ${_name} ${ARGN} ")")
  string(TOLOWER "${_name}" name)
  set(the_module opencv_${name})

  # the first pass - collect modules info, the second pass - create targets
  if(OPENCV_INITIAL_PASS)
    #guard against redefinition
    if(";${OPENCV_MODULES_BUILD};${OPENCV_MODULES_DISABLED_USER};" MATCHES ";${the_module};")
      message(FATAL_ERROR "Redefinition of the ${the_module} module.
  at:                    ${CMAKE_CURRENT_SOURCE_DIR}
  previously defined at: ${OPENCV_MODULE_${the_module}_LOCATION}
")
    endif()

    if(NOT DEFINED the_description)
      set(the_description "The ${name} OpenCV module")
    endif()

    if(NOT DEFINED BUILD_${the_module}_INIT)
      set(BUILD_${the_module}_INIT ON)
    endif()

    # create option to enable/disable this module
    option(BUILD_${the_module} "Include ${the_module} module into the OpenCV build" ${BUILD_${the_module}_INIT})

    # remember the module details
    set(OPENCV_MODULE_${the_module}_DESCRIPTION "${the_description}" CACHE INTERNAL "Brief description of ${the_module} module")
    set(OPENCV_MODULE_${the_module}_LOCATION    "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Location of ${the_module} module sources")

    set(OPENCV_MODULE_${the_module}_LINK_DEPS "" CACHE INTERNAL "")

    set(ADD_MODULE_ARGN ${ARGN})
    ocv_cmake_hook(PRE_ADD_MODULE)
    ocv_cmake_hook(PRE_ADD_MODULE_${the_module})

    # parse list of dependencies
    if(" ${ARGV1}" STREQUAL " INTERNAL" OR " ${ARGV1}" STREQUAL " BINDINGS")
      set(OPENCV_MODULE_${the_module}_CLASS "${ARGV1}" CACHE INTERNAL "The category of the module")
      set(__ocv_argn__ ${ADD_MODULE_ARGN})
      list(REMOVE_AT __ocv_argn__ 0)
      ocv_add_dependencies(${the_module} ${__ocv_argn__})
      unset(__ocv_argn__)
    else()
      set(OPENCV_MODULE_${the_module}_CLASS "PUBLIC" CACHE INTERNAL "The category of the module")
      ocv_add_dependencies(${the_module} ${ADD_MODULE_ARGN})
      if(BUILD_${the_module})
        set(OPENCV_MODULES_PUBLIC ${OPENCV_MODULES_PUBLIC} "${the_module}" CACHE INTERNAL "List of OpenCV modules marked for export")
      endif()
    endif()

    # add self to the world dependencies
    if((NOT DEFINED OPENCV_MODULE_IS_PART_OF_WORLD
        AND NOT OPENCV_MODULE_${the_module}_CLASS STREQUAL "BINDINGS"
        AND (NOT DEFINED OPENCV_MODULE_${the_module}_IS_PART_OF_WORLD OR OPENCV_MODULE_${the_module}_IS_PART_OF_WORLD)
        AND (NOT OPENCV_PROCESSING_EXTRA_MODULES OR NOT OPENCV_WORLD_EXCLUDE_EXTRA_MODULES)
        AND (NOT BUILD_SHARED_LIBS OR NOT "x${OPENCV_MODULE_TYPE}" STREQUAL "xSTATIC"))
        OR OPENCV_MODULE_IS_PART_OF_WORLD
        )
      set(OPENCV_MODULE_${the_module}_IS_PART_OF_WORLD ON CACHE INTERNAL "")
      ocv_add_dependencies(opencv_world OPTIONAL ${the_module})
    else()
      set(OPENCV_MODULE_${the_module}_IS_PART_OF_WORLD OFF CACHE INTERNAL "")
    endif()

    if(NOT DEFINED the_label)
      if(OPENCV_PROCESSING_EXTRA_MODULES)
        set(the_label "Extra")
      else()
        set(the_label "Main")
      endif()
    endif()
    set(OPENCV_MODULE_${the_module}_LABEL "${the_label};${the_module}" CACHE INTERNAL "")

    if(BUILD_${the_module})
      set(OPENCV_MODULES_BUILD ${OPENCV_MODULES_BUILD} "${the_module}" CACHE INTERNAL "List of OpenCV modules included into the build")
    else()
      set(OPENCV_MODULES_DISABLED_USER ${OPENCV_MODULES_DISABLED_USER} "${the_module}" CACHE INTERNAL "List of OpenCV modules explicitly disabled by user")
    endif()

    # stop processing of current file
    ocv_cmake_hook(POST_ADD_MODULE)
    ocv_cmake_hook(POST_ADD_MODULE_${the_module})
    return()
  else()
    set(OPENCV_MODULE_${the_module}_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "")
    if(NOT BUILD_${the_module})
      return() # extra protection from redefinition
    endif()
    if(NOT OPENCV_MODULE_${the_module}_IS_PART_OF_WORLD OR NOT ${BUILD_opencv_world})
      if (NOT ${the_module} STREQUAL opencv_world)
        project(${the_module})
      endif()
      add_definitions(
        -D_USE_MATH_DEFINES  # M_PI constant in MSVS
        -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS  # to use C libraries from C++ code (ffmpeg)
      )
    endif()
  endif()
endmacro()

# finds and sets headers and sources for the standard OpenCV module
# Usage:
# ocv_glob_module_sources([EXCLUDE_CUDA] [EXCLUDE_OPENCL] <extra sources&headers in the same format as used in ocv_set_module_sources>)
macro(ocv_glob_module_sources)
  ocv_debug_message("ocv_glob_module_sources(" ${ARGN} ")")
  set(_argn ${ARGN})
  list(FIND _argn "EXCLUDE_CUDA" exclude_cuda)
  if(NOT exclude_cuda EQUAL -1)
    list(REMOVE_AT _argn ${exclude_cuda})
  endif()
  list(FIND _argn "EXCLUDE_OPENCL" exclude_opencl)
  if(NOT exclude_opencl EQUAL -1)
    list(REMOVE_AT _argn ${exclude_opencl})
  endif()

  file(GLOB_RECURSE lib_srcs
       "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
  )
  file(GLOB_RECURSE lib_int_hdrs
       "${CMAKE_CURRENT_LIST_DIR}/src/*.hpp"
       "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
  )
  file(GLOB lib_hdrs
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/*.hpp"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/*.hpp"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/*.h"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/hal/*.hpp"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/hal/*.h"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/utils/*.hpp"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/utils/*.h"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/legacy/*.h"
  )
  file(GLOB lib_hdrs_detail
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/detail/*.hpp"
       "${CMAKE_CURRENT_LIST_DIR}/include/opencv2/${name}/detail/*.h"
  )
  if (APPLE)
    file(GLOB_RECURSE lib_srcs_apple
         "${CMAKE_CURRENT_LIST_DIR}/src/*.mm"
         "${CMAKE_CURRENT_LIST_DIR}/src/*.swift"
    )
    list(APPEND lib_srcs ${lib_srcs_apple})
  endif()

  ocv_source_group("Src" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/src" FILES ${lib_srcs} ${lib_int_hdrs})
  ocv_source_group("Include" DIRBASE "${CMAKE_CURRENT_LIST_DIR}/include" FILES ${lib_hdrs} ${lib_hdrs_detail})

  set(lib_cuda_srcs "")
  set(lib_cuda_hdrs "")
  if(HAVE_CUDA AND exclude_cuda EQUAL -1)
    file(GLOB lib_cuda_srcs
         "${CMAKE_CURRENT_LIST_DIR}/src/cuda/*.cu"
    )
    file(GLOB lib_cuda_hdrs
         "${CMAKE_CURRENT_LIST_DIR}/src/cuda/*.hpp"
    )
    source_group("Src\\Cuda"      FILES ${lib_cuda_srcs} ${lib_cuda_hdrs})
  endif()

  file(GLOB cl_kernels
       "${CMAKE_CURRENT_LIST_DIR}/src/opencl/*.cl"
  )
  if(cl_kernels AND exclude_opencl EQUAL -1)
    set(OCL_NAME opencl_kernels_${name})
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.cpp"  # don't add .hpp file here to optimize build process
      COMMAND ${CMAKE_COMMAND} "-DMODULE_NAME=${name}" "-DCL_DIR=${CMAKE_CURRENT_LIST_DIR}/src/opencl" "-DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.cpp" -P "${OpenCV_SOURCE_DIR}/cmake/cl2cpp.cmake"
      DEPENDS ${cl_kernels} "${OpenCV_SOURCE_DIR}/cmake/cl2cpp.cmake"
      COMMENT "Processing OpenCL kernels (${name})"
    )
    ocv_source_group("Src\\opencl\\kernels" FILES ${cl_kernels})
    ocv_source_group("Src\\opencl\\kernels\\autogenerated" FILES "${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.hpp")
    set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.hpp"
        PROPERTIES GENERATED TRUE
    )
    list(APPEND lib_srcs ${cl_kernels} "${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.cpp" "${CMAKE_CURRENT_BINARY_DIR}/${OCL_NAME}.hpp")
  endif()

  ocv_set_module_sources(${_argn} HEADERS ${lib_hdrs} ${lib_hdrs_detail}
                         SOURCES ${lib_srcs} ${lib_int_hdrs} ${lib_cuda_srcs} ${lib_cuda_hdrs})
endmacro()


# setup include path for OpenCV headers for specified module
# ocv_module_include_directories(<extra include directories/extra include modules>)
macro(ocv_module_include_directories)
  if(ENABLE_PRECOMPILED_HEADERS OR OPENCV_INCLUDE_DIR_APPEND_MODULE_SRC)
    ocv_target_include_directories(${the_module} "${OPENCV_MODULE_${the_module}_LOCATION}/src")
  endif()
  ocv_target_include_directories(${the_module}
      "${OPENCV_MODULE_${the_module}_LOCATION}/include"
      "${CMAKE_CURRENT_BINARY_DIR}" # for precompiled headers
      )
  ocv_target_include_modules(${the_module} ${OPENCV_MODULE_${the_module}_DEPS} ${ARGN})
endmacro()

# creates OpenCV module in current folder
# creates new target, configures standard dependencies, compilers flags, install rules
# Usage:
#   ocv_create_module(<extra link dependencies>)
#   ocv_create_module()
macro(ocv_create_module)
  ocv_debug_message("${the_module}: ocv_create_module(" ${ARGN} ")")
  if(OPENCV_MODULE_${the_module}_CLASS STREQUAL "BINDINGS")
    message(FATAL_ERROR "Bindings module can't call ocv_create_module()")
  endif()
  if(NOT " ${ARGN}" STREQUAL " ")
    set(OPENCV_MODULE_${the_module}_LINK_DEPS "${OPENCV_MODULE_${the_module}_LINK_DEPS};${ARGN}" CACHE INTERNAL "")
  endif()
  if(BUILD_opencv_world AND OPENCV_MODULE_${the_module}_IS_PART_OF_WORLD)
    # nothing
    set(the_module_target opencv_world)
  else()
    _ocv_create_module(${ARGN})
    set(the_module_target ${the_module})
  endif()

  if(WINRT AND BUILD_TESTS)
    # removing APPCONTAINER from modules to run from console
    # in case of usual starting of WinRT test apps output is missing
    # so starting of console version w/o APPCONTAINER is required to get test results
    # also this allows to use opencv_extra test data for these tests
    if(NOT "${the_module}" STREQUAL "opencv_ts" AND NOT "${the_module}" STREQUAL "opencv_hal")
      add_custom_command(TARGET ${the_module}
                         POST_BUILD
                         COMMAND link.exe /edit /APPCONTAINER:NO $(TargetPath))
    endif()

    if("${the_module}" STREQUAL "opencv_ts")
      # copy required dll files; WinRT apps need these dlls that are usually substituted by Visual Studio
      # however they are not on path and need to be placed with executables to run from console w/o APPCONTAINER
      add_custom_command(TARGET ${the_module}
        POST_BUILD
        COMMAND copy /y "\"$(VCInstallDir)redist\\$(PlatformTarget)\\Microsoft.VC$(PlatformToolsetVersion).CRT\\msvcp$(PlatformToolsetVersion).dll\"" "\"${CMAKE_BINARY_DIR}\\bin\\$(Configuration)\\msvcp$(PlatformToolsetVersion)_app.dll\""
        COMMAND copy /y "\"$(VCInstallDir)redist\\$(PlatformTarget)\\Microsoft.VC$(PlatformToolsetVersion).CRT\\msvcr$(PlatformToolsetVersion).dll\"" "\"${CMAKE_BINARY_DIR}\\bin\\$(Configuration)\\msvcr$(PlatformToolsetVersion)_app.dll\""
        COMMAND copy /y "\"$(VCInstallDir)redist\\$(PlatformTarget)\\Microsoft.VC$(PlatformToolsetVersion).CRT\\vccorlib$(PlatformToolsetVersion).dll\"" "\"${CMAKE_BINARY_DIR}\\bin\\$(Configuration)\\vccorlib$(PlatformToolsetVersion)_app.dll\"")
    endif()
  endif()
endmacro()

# this is a command for adding OpenCV accuracy/regression tests to the module
# ocv_add_accuracy_tests(<list of extra dependencies>)
function(ocv_add_accuracy_tests)
  ocv_debug_message("ocv_add_accuracy_tests(" ${ARGN} ")")

  set(test_path "${CMAKE_CURRENT_LIST_DIR}/test")
  if(BUILD_TESTS AND EXISTS "${test_path}"
      AND (NOT DEFINED OPENCV_BUILD_TEST_MODULES_LIST
          OR OPENCV_BUILD_TEST_MODULES_LIST STREQUAL "all"
          OR ";${OPENCV_BUILD_TEST_MODULES_LIST};" MATCHES ";${name};"
      )
  )
    __ocv_parse_test_sources(TEST ${ARGN})

    # opencv_imgcodecs is required for imread/imwrite
    set(test_deps opencv_ts ${the_module} opencv_imgcodecs opencv_videoio ${OPENCV_MODULE_${the_module}_DEPS} ${OPENCV_MODULE_opencv_ts_DEPS})
    ocv_check_dependencies(${test_deps})
    if(OCV_DEPENDENCIES_FOUND)
      set(the_target "opencv_test_${name}")
      # project(${the_target})

      if(NOT OPENCV_TEST_${the_module}_SOURCES)
        file(GLOB_RECURSE test_srcs "${test_path}/*.cpp")
        file(GLOB_RECURSE test_hdrs "${test_path}/*.hpp" "${test_path}/*.h")
        ocv_source_group("Src" DIRBASE "${test_path}" FILES ${test_srcs})
        ocv_source_group("Include" DIRBASE "${test_path}" FILES ${test_hdrs})
        set(OPENCV_TEST_${the_module}_SOURCES ${test_srcs} ${test_hdrs})
      endif()

      if(OPENCV_MODULE_${the_module}_TEST_SOURCES_DISPATCHED)
        list(APPEND OPENCV_TEST_${the_module}_SOURCES ${OPENCV_MODULE_${the_module}_TEST_SOURCES_DISPATCHED})
      endif()
      ocv_compiler_optimization_process_sources(OPENCV_TEST_${the_module}_SOURCES OPENCV_TEST_${the_module}_DEPS ${the_target})

      if(NOT BUILD_opencv_world)
        get_native_precompiled_header(${the_target} test_precomp.hpp)
      endif()

      source_group("Src" FILES "${${the_target}_pch}")
      ocv_add_executable(${the_target} ${OPENCV_TEST_${the_module}_SOURCES} ${${the_target}_pch})
      ocv_target_include_modules(${the_target} ${test_deps})
      if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/test")
        ocv_target_include_directories(${the_target} "${CMAKE_CURRENT_BINARY_DIR}/test")
      endif()
      ocv_target_link_libraries(${the_target} PRIVATE ${test_deps} ${OPENCV_MODULE_${the_module}_DEPS} ${OPENCV_LINKER_LIBS} ${OPENCV_TEST_${the_module}_DEPS})
      add_dependencies(opencv_tests ${the_target})

      if(TARGET opencv_videoio_plugins)
        add_dependencies(${the_target} opencv_videoio_plugins)
      endif()
      if(TARGET opencv_highgui_plugins)
        add_dependencies(${the_target} opencv_highgui_plugins)
      endif()

      if(HAVE_HPX)
        message("Linking HPX to Perf test of module ${name}")
        ocv_target_link_libraries(${the_target} LINK_PRIVATE "${HPX_LIBRARIES}")
      endif()

      set_target_properties(${the_target} PROPERTIES LABELS "${OPENCV_MODULE_${the_module}_LABEL};AccuracyTest")
      set_source_files_properties(${OPENCV_TEST_${the_module}_SOURCES} ${${the_target}_pch}
        PROPERTIES LABELS "${OPENCV_MODULE_${the_module}_LABEL};AccuracyTest")

      # Additional target properties
      set_target_properties(${the_target} PROPERTIES
        DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
      )

      ocv_append_target_property(${the_target} COMPILE_DEFINITIONS "__OPENCV_TESTS=1")

      if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${the_target} PROPERTIES FOLDER "tests accuracy")
      endif()

      if(OPENCV_TEST_BIGDATA)
        ocv_append_target_property(${the_target} COMPILE_DEFINITIONS "OPENCV_TEST_BIGDATA=1")
      endif()

      if(NOT BUILD_opencv_world)
        _ocv_add_precompiled_headers(${the_target})
      endif()

      if(OPENCV_TEST_EXTRA_CXX_FLAGS_Release)
        target_compile_options(${the_target} PRIVATE "$<$<CONFIG:Release>:${OPENCV_TEST_EXTRA_CXX_FLAGS_Release}>")
      endif()

      ocv_add_test_from_target("${the_target}" "Accuracy" "${the_target}")
    else(OCV_DEPENDENCIES_FOUND)
      # TODO: warn about unsatisfied dependencies
    endif(OCV_DEPENDENCIES_FOUND)

    if(INSTALL_TESTS)
      install(TARGETS ${the_target} RUNTIME DESTINATION ${OPENCV_TEST_INSTALL_PATH} COMPONENT tests)
    endif()
  endif()
endfunction()


# this is a command for adding OpenCV performance tests to the module
# ocv_add_perf_tests(<extra_dependencies>)
function(ocv_add_perf_tests)
  ocv_debug_message("ocv_add_perf_tests(" ${ARGN} ")")

  if(WINRT)
    set(OPENCV_DEBUG_POSTFIX "")
  endif()

  set(perf_path "${CMAKE_CURRENT_LIST_DIR}/perf")
  if(BUILD_PERF_TESTS AND EXISTS "${perf_path}"
      AND (NOT DEFINED OPENCV_BUILD_PERF_TEST_MODULES_LIST
          OR OPENCV_BUILD_PERF_TEST_MODULES_LIST STREQUAL "all"
          OR ";${OPENCV_BUILD_PERF_TEST_MODULES_LIST};" MATCHES ";${name};"
      )
  )
    __ocv_parse_test_sources(PERF ${ARGN})

    # opencv_imgcodecs is required for imread/imwrite
    set(perf_deps opencv_ts ${the_module} opencv_imgcodecs ${OPENCV_MODULE_${the_module}_DEPS} ${OPENCV_MODULE_opencv_ts_DEPS})
    ocv_check_dependencies(${perf_deps})

    if(OCV_DEPENDENCIES_FOUND)
      set(the_target "opencv_perf_${name}")
      # project(${the_target})

      if(NOT OPENCV_PERF_${the_module}_SOURCES)
        file(GLOB_RECURSE perf_srcs "${perf_path}/*.cpp")
        file(GLOB_RECURSE perf_hdrs "${perf_path}/*.hpp" "${perf_path}/*.h")
        ocv_source_group("Src" DIRBASE "${perf_path}" FILES ${perf_srcs})
        ocv_source_group("Include" DIRBASE "${perf_path}" FILES ${perf_hdrs})
        set(OPENCV_PERF_${the_module}_SOURCES ${perf_srcs} ${perf_hdrs})
      endif()

      ocv_compiler_optimization_process_sources(OPENCV_PERF_${the_module}_SOURCES OPENCV_PERF_${the_module}_DEPS ${the_target})

      if(NOT BUILD_opencv_world)
        get_native_precompiled_header(${the_target} perf_precomp.hpp)
      endif()

      source_group("Src" FILES "${${the_target}_pch}")
      ocv_add_executable(${the_target} ${OPENCV_PERF_${the_module}_SOURCES} ${${the_target}_pch})
      ocv_target_include_modules(${the_target} ${perf_deps})
      ocv_target_link_libraries(${the_target} PRIVATE ${perf_deps} ${OPENCV_MODULE_${the_module}_DEPS} ${OPENCV_LINKER_LIBS} ${OPENCV_PERF_${the_module}_DEPS})
      add_dependencies(opencv_perf_tests ${the_target})

      if(TARGET opencv_videoio_plugins)
        add_dependencies(${the_target} opencv_videoio_plugins)
      endif()
      if(TARGET opencv_highgui_plugins)
        add_dependencies(${the_target} opencv_highgui_plugins)
      endif()

      if(HAVE_HPX)
        message("Linking HPX to Perf test of module ${name}")
        ocv_target_link_libraries(${the_target} LINK_PRIVATE "${HPX_LIBRARIES}")
      endif()

      set_target_properties(${the_target} PROPERTIES LABELS "${OPENCV_MODULE_${the_module}_LABEL};PerfTest")
      set_source_files_properties(${OPENCV_PERF_${the_module}_SOURCES} ${${the_target}_pch}
        PROPERTIES LABELS "${OPENCV_MODULE_${the_module}_LABEL};PerfTest")

      # Additional target properties
      set_target_properties(${the_target} PROPERTIES
        DEBUG_POSTFIX "${OPENCV_DEBUG_POSTFIX}"
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
      )
      if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${the_target} PROPERTIES FOLDER "tests performance")
      endif()

      if(WINRT)
        # removing APPCONTAINER from tests to run from console
        # look for detailed description inside of ocv_create_module macro above
        add_custom_command(TARGET "opencv_perf_${name}"
                           POST_BUILD
                           COMMAND link.exe /edit /APPCONTAINER:NO $(TargetPath))
      endif()

      if(NOT BUILD_opencv_world)
        _ocv_add_precompiled_headers(${the_target})
      endif()

      ocv_add_test_from_target("${the_target}" "Performance" "${the_target}")
      ocv_add_test_from_target("opencv_sanity_${name}" "Sanity" "${the_target}"
                               "--perf_min_samples=1"
                               "--perf_force_samples=1"
                               "--perf_verify_sanity")
    else(OCV_DEPENDENCIES_FOUND)
      # TODO: warn about unsatisfied dependencies
    endif(OCV_DEPENDENCIES_FOUND)
    if(INSTALL_TESTS)
      install(TARGETS ${the_target} RUNTIME DESTINATION ${OPENCV_TEST_INSTALL_PATH} COMPONENT tests)
    endif()
  endif()
endfunction()

function(ocv_add_samples)
  ocv_debug_message("ocv_add_samples(" ${ARGN} ")")

  set(samples_path "${CMAKE_CURRENT_SOURCE_DIR}/samples")
  if(NOT EXISTS "${samples_path}")
    return()
  endif()

  string(REGEX REPLACE "^opencv_" "" module_id ${the_module})

  if(BUILD_EXAMPLES)
    set(samples_deps ${the_module} ${OPENCV_MODULE_${the_module}_DEPS} opencv_imgcodecs opencv_videoio opencv_highgui ${ARGN})
    ocv_check_dependencies(${samples_deps})

    if(OCV_DEPENDENCIES_FOUND)
      file(GLOB sample_sources "${samples_path}/*.cpp")

      foreach(source ${sample_sources})
        get_filename_component(name "${source}" NAME_WE)
        set(the_target "example_${module_id}_${name}")

        ocv_add_executable(${the_target} "${source}")
        ocv_target_include_modules(${the_target} ${samples_deps})
        ocv_target_link_libraries(${the_target} PRIVATE ${samples_deps})

        set_target_properties(${the_target} PROPERTIES
          PROJECT_LABEL "(sample) ${name}"
          LABELS "${OPENCV_MODULE_${the_module}_LABEL};Sample")
        set_source_files_properties("${source}" PROPERTIES
          LABELS "${OPENCV_MODULE_${the_module}_LABEL};Sample")
        if(ENABLE_SOLUTION_FOLDERS)
          set_target_properties(${the_target} PROPERTIES
            FOLDER "samples/${module_id}")
        endif()
        # Add single target to build all samples for the module: 'make opencv_samples_bioinspired'
        set(parent_target opencv_samples_${module_id})
        if(NOT TARGET ${parent_target})
          add_custom_target(${parent_target})
          add_dependencies(opencv_samples ${parent_target})
        endif()
        add_dependencies(${parent_target} ${the_target})

        if(TARGET opencv_videoio_plugins)
          add_dependencies(${the_target} opencv_videoio_plugins)
        endif()
        if(TARGET opencv_highgui_plugins)
          add_dependencies(${the_target} opencv_highgui_plugins)
        endif()

        if(INSTALL_BIN_EXAMPLES)
          install(TARGETS ${the_target} RUNTIME DESTINATION "${OPENCV_SAMPLES_BIN_INSTALL_PATH}/${module_id}" COMPONENT samples)
        endif()
      endforeach()
    endif()
  endif()

  if(INSTALL_C_EXAMPLES)
    file(GLOB DEPLOY_FILES_AND_DIRS "${samples_path}/*")
    foreach(ITEM ${DEPLOY_FILES_AND_DIRS})
        IF( IS_DIRECTORY "${ITEM}" )
            LIST( APPEND sample_dirs "${ITEM}" )
        ELSE()
            LIST( APPEND sample_files "${ITEM}" )
        ENDIF()
    endforeach()
    install(FILES ${sample_files}
            DESTINATION "${OPENCV_SAMPLES_SRC_INSTALL_PATH}/${module_id}"
            COMPONENT samples)
    install(DIRECTORY ${sample_dirs}
            DESTINATION "${OPENCV_SAMPLES_SRC_INSTALL_PATH}/${module_id}"
            COMPONENT samples)
  endif()
endfunction()