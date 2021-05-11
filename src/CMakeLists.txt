add_library(originalSample MODULE
    originalSample.cpp
)
# 使用c++11
target_compile_features(originalSample PRIVATE cxx_range_for cxx_auto_type)
# 不使用C++ RTTI.
set_target_properties(originalSample PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)