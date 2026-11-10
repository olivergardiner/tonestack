cmake_minimum_required(VERSION 3.19)
project(tonestack LANGUAGES CXX)
include(GNUInstallDirs)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Help)

qt_standard_project_setup()

if(MSVC)
    # Embed Debug symbols in objects to avoid the compiler PDB-server dependency.
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    add_compile_options(/FS)
endif()

qt_add_executable(tonestack
    WIN32 MACOSX_BUNDLE
    main.cpp
    simulationcontroller.cpp simulationcontroller.h
    capacitor.cpp
    capacitor.h
    circuit.cpp
    circuit.h
    circuitelement.cpp
    circuitelement.h
    helpbrowser.cpp
    helpbrowser.h
    helpbrowser.ui
    helptextviewer.cpp
    helptextviewer.h
    inductor.cpp
    inductor.h
    namedialog.cpp
    namedialog.h
    namedialog.ui
    potentiometer.cpp
    potentiometer.h
    potsweep.h
    resistor.cpp
    resistor.h
    settingsdialog.cpp
    settingsdialog.h
    settingsdialog.ui
    tonestackcalculator.cpp
    tonestackcalculator.h
    tonestackcalculator.ui
    ngspice/sharedspice.h
)

target_include_directories(tonestack PRIVATE ngspice)

target_link_libraries(tonestack
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::PrintSupport
        Qt6::Help
        ${CMAKE_SOURCE_DIR}/ngspice/ngspice.lib
)

# Copy ngspice.dll to the build output directory
add_custom_command(TARGET tonestack POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/ngspice/ngspice.dll"
        "$<TARGET_FILE_DIR:tonestack>/ngspice.dll"
    COMMENT "Copying ngspice.dll"
)

# Copy help files to a help/ subdirectory alongside the executable
add_custom_command(TARGET tonestack POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:tonestack>/help"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/manual/help/tonestack.qhc"
        "$<TARGET_FILE_DIR:tonestack>/help/tonestack.qhc"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/manual/help/tonestack.qch"
        "$<TARGET_FILE_DIR:tonestack>/help/tonestack.qch"
    COMMENT "Copying help files"
)

install(TARGETS tonestack
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET tonestack
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

include(CTest)
# Deployed application plugins may omit offscreen. Use the matching Qt SDK
# plugins for tests, independently of what windeployqt copied beside the app.
get_target_property(_qt_core_dll Qt6::Core IMPORTED_LOCATION_RELEASE)
get_filename_component(_qt_bin_dir "${_qt_core_dll}" DIRECTORY)
get_filename_component(_qt_sdk_dir "${_qt_bin_dir}" DIRECTORY)
if(BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    qt_add_executable(tst_configuration
        tests/tst_configuration.cpp simulationcontroller.cpp simulationcontroller.h
        tonestackcalculator.cpp tonestackcalculator.ui
        namedialog.cpp namedialog.ui helpbrowser.cpp helpbrowser.ui helptextviewer.cpp
        circuit.cpp circuitelement.cpp resistor.cpp capacitor.cpp inductor.cpp
        potentiometer.cpp settingsdialog.cpp settingsdialog.ui
    )
    target_include_directories(tst_configuration PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(tst_configuration PRIVATE Qt6::Widgets Qt6::Test Qt6::Help Qt6::PrintSupport)
    # The test executable supplies the shared-library entry points itself.
    target_compile_definitions(tst_configuration PRIVATE SHARED_MODULE)
    add_test(NAME configuration COMMAND tst_configuration)
    set_tests_properties(configuration PROPERTIES
        TIMEOUT 60
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_PLUGIN_PATH=${_qt_sdk_dir}/plugins"
    )
endif()

if(BUILD_TESTING)
    qt_add_executable(tst_numerical
        tests/tst_numerical.cpp simulationcontroller.cpp simulationcontroller.h
        tonestackcalculator.cpp tonestackcalculator.ui namedialog.cpp namedialog.ui
        helpbrowser.cpp helpbrowser.ui helptextviewer.cpp circuit.cpp circuitelement.cpp resistor.cpp
        capacitor.cpp inductor.cpp potentiometer.cpp settingsdialog.cpp settingsdialog.ui
    )
    target_include_directories(tst_numerical PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(tst_numerical PRIVATE Qt6::Widgets Qt6::Test Qt6::Help Qt6::PrintSupport
        ${CMAKE_SOURCE_DIR}/ngspice/ngspice.lib)
    add_custom_command(TARGET tst_numerical POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/ngspice/ngspice.dll"
            "$<TARGET_FILE_DIR:tst_numerical>/ngspice.dll")
    add_test(NAME numerical COMMAND tst_numerical rcLowPass toneStacks restoredResponse)
    set_tests_properties(numerical PROPERTIES
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_PLUGIN_PATH=${_qt_sdk_dir}/plugins"
        TIMEOUT 60)
endif()

if(BUILD_TESTING)
    qt_add_executable(tst_controller tests/tst_controller.cpp
        simulationcontroller.cpp simulationcontroller.h)
    target_include_directories(tst_controller PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(tst_controller PRIVATE SHARED_MODULE)
    target_link_libraries(tst_controller PRIVATE Qt6::Core Qt6::Test)
    add_test(NAME controller COMMAND tst_controller)
    set_tests_properties(controller PROPERTIES TIMEOUT 30)
endif()

# Refresh runtime data even when no source file needs relinking.
add_custom_target(runtime_assets ALL
    COMMAND ${CMAKE_COMMAND}
        "-DSOURCE_DIR=${CMAKE_SOURCE_DIR}"
        "-DOUTPUT_DIR=$<TARGET_FILE_DIR:tonestack>"
        -P "${CMAKE_SOURCE_DIR}/cmake/stage-assets.cmake")
add_dependencies(tonestack runtime_assets)
if(BUILD_TESTING)
    add_dependencies(tst_configuration runtime_assets)
    add_dependencies(tst_numerical runtime_assets)
endif()
include(GNUInstallDirs)
install(DIRECTORY circuits DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILES_MATCHING PATTERN "*.cir" PATTERN "*.tsc" PATTERN "*.png"
    PATTERN "*.lib" PATTERN "*.mod" PATTERN "*.spice" PATTERN "*.inc")
install(FILES ngspice/ngspice.dll DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES manual/help/tonestack.qhc manual/help/tonestack.qch
    DESTINATION ${CMAKE_INSTALL_BINDIR}/help)

if(BUILD_TESTING)
    foreach(case IN ITEMS namedComparisonRestoresResponse controllerUsesRealEngine realWindowUpdatesAndRecovers pathsWithSpacesAndUnrelatedCwd windowFromUnrelatedDirectory)
        add_test(NAME ${case} COMMAND tst_numerical ${case})
        set_tests_properties(${case} PROPERTIES
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
            ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QT_PLUGIN_PATH=${_qt_sdk_dir}/plugins"
            TIMEOUT 60)
    endforeach()
endif()
install(FILES LICENSE DESTINATION .)
