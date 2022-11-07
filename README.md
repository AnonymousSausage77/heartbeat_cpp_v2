# heartbeat_cpp_v2

RobotX 2022 heartbeat system developed for raspberry PI.

This system was built and tested on windows and deployed on PI and therefore is designed to be cross-platform. As such, the make-files and other config files are not included in this repository and must be added locally.

This was tested with GCC-10 compiler.

Unfortunately this means a few steps are in order to get this running.

## Setup

### Heartbeat_v2_main

In Heartbeat_v2_main settings:

  - add '../Heartbeat_v2_lib' to "Compiler" > "include paths" in Codelite
  - add 'Heartbeat_v2_lib;pthread' to "Linker" > "Libraries"
  - add '$(WorkspacePath)/build-$(WorkspaceConfiguration)/lib' to "Linker" > "Libraries Search Path"

  
Additionaly, this should be run with the c++ 17 copiler option enabled
