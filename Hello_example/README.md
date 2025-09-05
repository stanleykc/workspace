# Hello_example - OpenDDS Hello World Application

A comprehensive OpenDDS "Hello World" example demonstrating Data Distribution Service (DDS) communication using the factory pattern and boilerplate abstraction layer for clean, maintainable code.

## Overview

This application demonstrates a basic OpenDDS publisher-subscriber communication where:
- **Publisher** sends a "Hello World" message with an ID
- **Subscriber** receives and displays the message
- Uses enterprise-grade patterns with abstraction layers for reusability
- Implements proper error handling and resource management

## Architecture

### Design Patterns
- **Factory Pattern**: Boilerplate helper functions create DDS entities
- **RAII**: Automatic resource cleanup through smart pointers
- **Listener Pattern**: Asynchronous message handling via DataReaderListener
- **Synchronization**: OpenDDS Model library for sync operations

### Key Components

#### Core Files
- `Basic_Message.idl` - Message type definition with DDS annotations
- `Publisher.cpp` - Message publishing application
- `Subscriber.cpp` - Message receiving application
- `DataReaderListenerImpl.cpp/h` - Asynchronous message handler
- `Boilerplate.cpp/h` - Factory functions for DDS entity creation

#### Configuration
- `HelloExample.mpc` - MPC build configuration with advanced features
- `.gitignore` - Excludes generated files from version control

#### Generated Files (Auto-generated, not edited)
- `Basic_Message*.cpp/h` - IDL-generated type support code
- `GNUmakefile*` - Generated build files
- `publisher`/`subscriber` - Compiled executables

### Message Structure
```idl
module Hello {
  @topic
  struct Basic_Message {
    @key long message_id;  // Unique identifier
    string text;           // Message content
  };
};
```

### Factory Pattern Implementation
The Boilerplate module provides clean factory functions:
```cpp
DDS::DomainParticipant_var participant = createParticipant(dpf);
DDS::Topic_var topic = createTopic(participant);
DDS::Publisher_var publisher = createPublisher(participant);
DDS::DataWriter_var writer = createDataWriter(publisher, topic);
```

## Prerequisites

- OpenDDS installed and configured
- ACE/TAO libraries available
- MPC (Make Project Creator) for build system
- GCC or compatible C++ compiler

## Building the Application

### 1. Set Up Environment
```bash
# Source OpenDDS environment (required before building/running)
source /workspace/OpenDDS/setenv.sh
```

### 2. Generate Build Files and Compile
```bash
# Navigate to project directory
cd Hello_example

# Generate Makefiles from MPC configuration
mwc.pl -type gnuace HelloExample.mpc

# Build the application
make
```

The build process will:
1. Generate type support code from `Basic_Message.idl`
2. Compile all source files including boilerplate helpers
3. Link against OpenDDS and ACE/TAO libraries
4. Create `publisher` and `subscriber` executables

## Running the Application

### Method 1: Manual Execution (Recommended for Learning)

#### Terminal 1: Start OpenDDS Info Repository
```bash
source /workspace/OpenDDS/setenv.sh
cd Hello_example
DCPSInfoRepo -NOBITS -o repo.ior &
```

#### Terminal 2: Start Subscriber
```bash
source /workspace/OpenDDS/setenv.sh
cd Hello_example
./subscriber
```

#### Terminal 3: Run Publisher
```bash
source /workspace/OpenDDS/setenv.sh
cd Hello_example
./publisher
```

### Method 2: Quick Test (All in one terminal)
```bash
source /workspace/OpenDDS/setenv.sh
cd Hello_example

# Start InfoRepo in background
DCPSInfoRepo -NOBITS -o repo.ior &
sleep 2

# Start subscriber in background  
./subscriber &
sleep 2

# Run publisher
./publisher

# Clean up background processes
killall DCPSInfoRepo
```

### Expected Output

**Subscriber Terminal:**
```
Received message: Hello World
Subscriber received 1 samples
```

**Publisher Terminal:**
```
(No output indicates successful execution)
```

## Troubleshooting

### Common Issues

#### 1. Shared Library Error
```
./publisher: error while loading shared libraries: libTAO_Valuetype.so.2.5.22: cannot open shared object file
```
**Solution**: Ensure OpenDDS environment is sourced:
```bash
source /workspace/OpenDDS/setenv.sh
```

#### 2. InfoRepo Connection Error
```
ERROR: InfoRepoDiscovery::get_dcps_info: unable to narrow DCPSInfo
```
**Solution**: Start DCPSInfoRepo before running applications:
```bash
DCPSInfoRepo -NOBITS -o repo.ior &
```

#### 3. Build Errors
**Solution**: Ensure MPC generated files are current:
```bash
mwc.pl -type gnuace HelloExample.mpc
make clean
make
```

### Environment Variables
The following should be set after sourcing `setenv.sh`:
- `DDS_ROOT` - OpenDDS installation directory
- `ACE_ROOT` - ACE library path
- `TAO_ROOT` - TAO library path  
- `LD_LIBRARY_PATH` - Includes OpenDDS and ACE libraries
- `PATH` - Includes OpenDDS and ACE binaries

## Technical Details

### DDS Configuration
- **Domain ID**: 42
- **Topic Name**: "basic_message"
- **QoS**: Default policies for all entities
- **Transport**: TCP transport with OpenDDS model integration

### Advanced Features Used
- **@topic Annotation**: IDL annotation for topic-specific optimizations
- **@key Annotation**: Defines message_id as the key field for instance management
- **OpenDDS Model Library**: WriterSync and ReaderSync for proper coordination
- **Safety Profile**: Disabled via `no_opendds_safety_profile` requirement

### Code Quality Features
- **Exception Handling**: Comprehensive CORBA and runtime exception handling
- **Resource Management**: Automatic cleanup via RAII and smart pointers
- **Error Reporting**: ACE-based logging with detailed error messages
- **Sample Counting**: Tracks received messages for verification

## File Structure
```
Hello_example/
├── README.md                          # This file
├── Basic_Message.idl                  # Message definition
├── Publisher.cpp                      # Publisher application
├── Subscriber.cpp                     # Subscriber application
├── DataReaderListenerImpl.cpp/.h      # Message event handler
├── Boilerplate.cpp/.h                 # Factory helper functions
├── HelloExample.mpc                   # Build configuration
├── .gitignore                         # Version control exclusions
├── Generated Files/
│   ├── Basic_Message*.cpp/h           # IDL-generated code
│   ├── GNUmakefile*                   # Generated build files
│   └── publisher, subscriber          # Compiled executables
└── Build Artifacts/
    ├── .depend.*                      # Dependency files
    └── .obj/                          # Object files directory
```

## Learning Objectives

This example demonstrates:
1. **OpenDDS Basics**: Core concepts of DDS publisher-subscriber communication
2. **Enterprise Patterns**: Factory pattern and abstraction layers for maintainable code
3. **IDL Usage**: Modern IDL with annotations for enhanced DDS features
4. **Build Systems**: MPC configuration for complex OpenDDS projects
5. **Error Handling**: Professional-grade exception handling and resource management
6. **Testing**: Manual verification of distributed communication

## Next Steps

After running this example, consider:
1. Modifying the message structure in `Basic_Message.idl`
2. Adding multiple subscribers or publishers
3. Experimenting with QoS policies
4. Implementing custom message types
5. Exploring OpenDDS advanced features like discovery configuration

## License

Distributed under the OpenDDS License.  
See: http://www.opendds.org/license.html