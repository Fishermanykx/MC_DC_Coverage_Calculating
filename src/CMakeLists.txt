add_library(MCDCCoverage MODULE
    MCDCCoverage.cpp
)
# 使用c++11
target_compile_features(MCDCCoverage PRIVATE cxx_range_for cxx_auto_type)
# 不使用C++ RTTI.
set_target_properties(MCDCCoverage PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)