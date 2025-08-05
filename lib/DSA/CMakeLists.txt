add_llvm_pass_plugin (DSA
  AddressTakenAnalysis.cpp
  DSTest.cpp
  StdLibPass.cpp
  AllocatorIdentification.cpp
  DataStructure.cpp
  Printer.cpp
  DSCallGraph.cpp
  DSGraph.cpp
  Local.cpp
  Steensgaard.cpp
  Registrar.cpp
)
set_property(TARGET DSA PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS DSA LIBRARY DESTINATION lib)
