cmake_minimum_required(VERSION 3.19)

project(XiuWenZhiZHuShou VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Concurrent)

file(GLOB UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")
qt_wrap_ui(GENERATED_UI_HEADERS ${UI_FILES})

file(GLOB QRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/qrc/*.qrc")

file(GLOB OSO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/osoFile/*.oso")
oso_wrap_oso(GENERATED_HEADERS ${OSO_FILES} OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/osoInclude)

FIND_SOURCES_AND_HEADERS(INCLUDE_LIST
	${CMAKE_CURRENT_SOURCE_DIR}/include 
	${CMAKE_CURRENT_SOURCE_DIR}/include/ui
	${CMAKE_CURRENT_SOURCE_DIR}/include/data
	${CMAKE_CURRENT_SOURCE_DIR}/include/func
	${CMAKE_CURRENT_SOURCE_DIR}/include/Modules
	${CMAKE_CURRENT_SOURCE_DIR}/include/NoHardWare
)

FIND_SOURCES_AND_HEADERS(SRC_LIST
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/src/ui
	${CMAKE_CURRENT_SOURCE_DIR}/src/data
	${CMAKE_CURRENT_SOURCE_DIR}/src/func
	${CMAKE_CURRENT_SOURCE_DIR}/src/DetachThread
	${CMAKE_CURRENT_SOURCE_DIR}/src/Modules
	${CMAKE_CURRENT_SOURCE_DIR}/src/NoHardWare
)

qt_add_executable(${PROJECT_NAME} 
	MANUAL_FINALIZATION
	${INCLUDE_LIST}
	${SRC_LIST}
	${GENERATED_UI_HEADERS}
	${QRC_FILES}
)


target_include_directories(${PROJECT_NAME} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/data
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/func
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/DetachThread
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/Modules
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/NoHardWare
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/osoInclude
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets Qt6::Concurrent)
target_link_libraries(${PROJECT_NAME} 
	PRIVATE ${OpenCV_LIBS}
)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE rqw
	PRIVATE oso
	PRIVATE rqwhoec
	PRIVATE hoem
	PRIVATE imgPro
	PRIVATE ime
	PRIVATE rqwscc
	PRIVATE rqwm
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${QT_PATH}/bin/windeployqt.exe --no-compiler-runtime --no-opengl-sw $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Running windeployqt to package Qt dependencies"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${TensorRT_dlls} ${TensorRT_exes} ${OnnxRuntime_dlls}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${ZMotionRutime_dlls}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# 可配置的部署输出目录（默认=目标可执行文件目录）
set(TXTOUTPUT_DIR "D:/zfkjData/HandleScanner/txt" CACHE PATH "Post-build destination for *.txt files")

# 确保输出目录存在
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TXTOUTPUT_DIR}"
)

file(GLOB TXT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sourceFile/*.txt")
if(TXT_FILES)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${TXT_FILES}
        "${TXTOUTPUT_DIR}"
    )
endif()









