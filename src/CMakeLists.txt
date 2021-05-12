add_library(ConditionCoverage MODULE
    ConditionCoverage.cpp
)
# 使用c++11
target_compile_features(ConditionCoverage PRIVATE cxx_range_for cxx_auto_type)
# 不使用C++ RTTI.
set_target_properties(ConditionCoverage PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)