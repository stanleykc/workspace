cmake_minimum_required(VERSION 3.10)
project(HelloGemini)

find_package(OpenDDS REQUIRED)

# Generate code from IDL
opendds_idl(BasicMessage.idl)

# Add executables for publisher and subscriber
add_executable(publisher publisher.cpp ${BasicMessage_IDL_GENERATED_FILES})
add_executable(subscriber subscriber.cpp DataReaderListenerImpl.cpp ${BasicMessage_IDL_GENERATED_FILES})

# Link against OpenDDS libraries
target_link_libraries(publisher OpenDDS::OpenDDS)
target_link_libraries(subscriber OpenDDS::OpenDDS)