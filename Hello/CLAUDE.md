# Hello OpenDDS Application - Claude Instructions

## Environment Setup
- source /workspace/OpenDDS/setenv.sh

## Build Commands
- ./build.sh
- make clean (to clean build artifacts)

## Test Commands
- perl run_test.pl (runs automated test)
- ./subscriber & ./publisher (manual testing)

## Generated Files (should be ignored by .gitignore)
- .depend.*
- GNUmakefile*
- Basic_MessageC.*
- Basic_MessageS.*
- Basic_MessageTypeSupportC.*
- Basic_MessageTypeSupportS.*
- Basic_MessageTypeSupportImpl.*
- *.o files
- subscriber and publisher executables

## Key Files
- Basic_Message.idl: Message type definition
- Publisher.cpp: Publisher application
- Subscriber.cpp: Subscriber application  
- DataReaderListener.h/cpp: Message handling
- Hello.mpc: MPC project configuration

## Important Instructions
- Always source OpenDDS environment before building
- Use existing build.sh script for compilation
- Test with run_test.pl script
- Generated files should not be committed to git