add_llvm_pass_plugin (DSA
  DSA/AddressTakenAnalysis.cpp
  DSA/DSTest.cpp
  DSA/StdLibPass.cpp
  DSA/AllocatorIdentification.cpp
  DSA/DataStructure.cpp
  DSA/Printer.cpp
  DSA/DSCallGraph.cpp
  DSA/DSGraph.cpp
  DSA/Local.cpp
  DSA/Steensgaard.cpp
  Registrar.cpp
)
set_property(TARGET DSA PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS DSA LIBRARY DESTINATION lib)
