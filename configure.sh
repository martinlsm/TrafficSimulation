#!/bin/bash
THIS_DIR=$(pwd)

# create symlinks for the agent to access cpp files
ln -s ${THIS_DIR}/src python_agent/src/cpp_src
ln -s ${THIS_DIR}/include python_agent/src/cpp_include
