add_library (DSA STATIC
  AddressTakenAnalysis.cpp
  CallTargets.cpp
  DSTest.cpp
  EquivClassGraphs.cpp
  StdLibPass.cpp
  AllocatorIdentification.cpp
  CompleteBottomUp.cpp
  DataStructure.cpp
  GraphChecker.cpp
  Printer.cpp
  TopDownClosure.cpp
  Basic.cpp
  DSCallGraph.cpp
  DataStructureStats.cpp
  TypeSafety.cpp
  BottomUpClosure.cpp
  DSGraph.cpp
  EntryPointAnalysis.cpp
  Local.cpp
  SanityCheck.cpp
  Steensgaard.cpp
)
set_property(TARGET DSA PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS DSA ARCHIVE DESTINATION lib)
