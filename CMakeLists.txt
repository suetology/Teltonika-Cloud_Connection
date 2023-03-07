cmake_minimum_required(VERSION 3.10)

project(app VERSION 1.0)

file(GLOB_RECURSE SRC_FILES src/*.c)
add_executable(app ${SRC_FILES})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/tuya_iot_sdk/include)
target_link_libraries(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/tuya_iot_sdk/lib/liblink_core.so)
target_link_libraries(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/tuya_iot_sdk/lib/libmiddleware_implementation.so)
target_link_libraries(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/tuya_iot_sdk/lib/libplatform_port.so)
target_link_libraries(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/deps/tuya_iot_sdk/lib/libutils_modules.so)