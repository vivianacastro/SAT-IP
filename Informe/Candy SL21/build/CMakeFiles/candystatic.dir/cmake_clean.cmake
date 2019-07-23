file(REMOVE_RECURSE
  "libcandystatic.pdb"
  "libcandystatic.a"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/candystatic.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
