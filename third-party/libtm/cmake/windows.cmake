set(OS "win")

set(WINDOWS_LIBUSB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third-party/win/libusb/include")
set(WINDOWS_BRAINSTEM2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third-party/win/BrainStem2/include")
set(WINDOWS_BRAINSTEM2_SRC_DIR "${CMAKE_SOURCE_DIR}/third-party/win/BrainStem2/src")


message("Platform is set to ${PLATFORM}")

IF(PLATFORM STREQUAL "x64")
    set(WINDOWS_LIBUSB_LIB_DIR "${CMAKE_SOURCE_DIR}/third-party/win/libusb/bin/x64")
    set(WINDOWS_BRAINSTEM2_LIB_DIR "${CMAKE_SOURCE_DIR}/third-party/win/BrainStem2/bin/x64")
else()
    set(WINDOWS_LIBUSB_LIB_DIR "${CMAKE_SOURCE_DIR}/third-party/win/libusb/bin/win32")
    set(WINDOWS_BRAINSTEM2_LIB_DIR "${CMAKE_SOURCE_DIR}/third-party/win/BrainStem2/bin/w32")
endif()

include_directories("${WINDOWS_LIBUSB_INCLUDE_DIR}")
link_directories("${WINDOWS_LIBUSB_LIB_DIR}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Zi")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")

set(LIBUSB_LIB "libusb-1.0")
set(BRAINSTEM2_LIB "BrainStem2")