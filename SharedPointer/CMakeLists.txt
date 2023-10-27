Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(shared_pointer_tests "test/test.cpp")
target_link_libraries(shared_pointer_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(shared_pointer_tests PRIVATE inc)

target_compile_options(shared_pointer_tests PUBLIC "$<$<CONFIG:DEBUG>:${GCC_COMPILE_DEBUG_OPTIONS}>")
target_compile_options(shared_pointer_tests PUBLIC "$<$<CONFIG:RELEASE>:${GCC_COMPILE_RELEASE_OPTIONS}>")