# cppgo
Implementation of channel and other ideas inspired by Go

gcc 4.9 / clang 3.6: [![Build Status](https://travis-ci.org/makiolo/cppgo.svg?branch=master)](https://travis-ci.org/makiolo/cppgo)

MSVC 2015: [![Build status](https://ci.appveyor.com/api/projects/status/cd4vmk2emr53p9d8?svg=true)](https://ci.appveyor.com/project/makiolo/cppgo)

# download, build and test
- git clone --recursive https://github.com/makiolo/cppgo.git
- ./cmaki/run.sh

# bootstrap new project
- git clone https://github.com/makiolo/cppgo.git
- cd cppgo
- git submodule add https://github.com/makiolo/cmaki.git
- git submodule add https://github.com/makiolo/cmaki_generator.git
- ./cmaki/init.sh
