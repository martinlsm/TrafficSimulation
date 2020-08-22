#!/bin/bash
THIS_DIR=$(pwd)

# create symlinks for the agent to access cpp files
ln -sf ${THIS_DIR}/src env_api/src/cpp_src
ln -sf ${THIS_DIR}/include env_api/src/cpp_include
