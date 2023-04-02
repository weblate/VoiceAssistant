get_target_property(__qtBuildQtType Qt${QT_VERSION_MAJOR}::Core TYPE)
if ("${__qtBuildQtType}" STREQUAL "STATIC_LIBRARY")
    set(QT_BUILD_STATIC ON)
else()
    set(QT_BUILD_STATIC OFF)
endif()

if (Qt6_FOUND)
    # Use the built-in qt_add_plugin function if Qt6 is available
    function(add_plugin)
        qt_add_plugin(${ARGV})
    endfunction()
else()
    # Define our own function for Qt5
    function(add_plugin target)
        set(options SHARED STATIC)
        set(one_value_args CLASS_NAME)
        set(multi_value_args OUTPUT_TARGETS)

        cmake_parse_arguments(QT_ADD_PLUGIN "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

        if(QT_BUILD_STATIC)
            add_library(${target} STATIC ${QT_ADD_PLUGIN_UNPARSED_ARGUMENTS})
            target_compile_definitions(${target} PRIVATE QT_PLUGIN QT_STATICPLUGIN)
            if (TARGET voiceassistant)
                target_link_libraries(voiceassistant PRIVATE ${target})
            endif()
            if (TARGET test_load_plugins)
                target_link_libraries(test_load_plugins PRIVATE ${target})
            endif()
        else()
            add_library(${target} MODULE ${QT_ADD_PLUGIN_UNPARSED_ARGUMENTS})
            set_property(TARGET ${target} PROPERTY QT_PLUGIN)
        endif()

        if(QT_ADD_PLUGIN_CLASS_NAME)
            target_compile_definitions(${target} PRIVATE QT_PLUGIN_CLASS_NAME=${QT_ADD_PLUGIN_CLASS_NAME})
            set_target_properties(${target} PROPERTIES QT_PLUGIN_CLASS_NAME ${QT_ADD_PLUGIN_CLASS_NAME})
        endif()
    endfunction()
endif()
