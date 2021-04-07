add_library(demoPass1 MODULE
    demoPass1.cpp
)
# 使用c++11
target_compile_features(demoPass1 PRIVATE cxx_range_for cxx_auto_type)
# 不使用C++ RTTI.
set_target_properties(demoPass1 PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)