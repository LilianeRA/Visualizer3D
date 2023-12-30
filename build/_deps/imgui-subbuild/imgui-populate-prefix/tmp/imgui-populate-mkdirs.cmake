# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/GIT/Visualizer3D/build/_deps/imgui-src"
  "E:/GIT/Visualizer3D/build/_deps/imgui-build"
  "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix"
  "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix/tmp"
  "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
  "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix/src"
  "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/GIT/Visualizer3D/build/_deps/imgui-subbuild/imgui-populate-prefix/src/imgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
