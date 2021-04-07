add_library(traverseIf MODULE
    traverseIf.cpp
)
# 使用c++11
target_compile_features(traverseIf PRIVATE cxx_range_for cxx_auto_type)
# 不使用C++ RTTI.
set_target_properties(traverseIf PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)
#
set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_executable(traverseIf traverseIf.cpp)

target_link_libraries(traverseIf
  ${llvm_libs}

  PRIVATE
  clangAST 
	clangASTMatchers 
	clangAnalysis 
	clangBasic 
	clangDriver 
	clangEdit 
	clangFrontend 
	clangFrontendTool 
	clangLex 
	clangParse 
	clangSema 
	clangEdit 
	clangRewrite 
	clangRewriteFrontend 
	clangStaticAnalyzerFrontend 
	clangStaticAnalyzerCheckers 
	clangStaticAnalyzerCore 
	clangCrossTU 
	clangIndex 
	clangSerialization 
	clangToolingCore 
	clangTooling 
	clangFormat 
  )