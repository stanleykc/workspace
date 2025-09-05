# OpenDDS Application Development Guide

## Overview

This guide provides a structured approach for creating new OpenDDS applications based on the OpenDDS DevGuide Getting Started chapter. OpenDDS is an open-source C++ implementation of the Object Management Group (OMG) Data Distribution Service (DDS) for Real-Time Systems specification.

## Core Concepts

### Data-Centric Publish-Subscribe (DCPS)

OpenDDS implements the DDS specification's Data-Centric Publish-Subscribe (DCPS) layer, which provides:
- **Publishers**: Send data to the DDS domain
- **Subscribers**: Receive data from the DDS domain
- **Topics**: Named data types that connect publishers and subscribers
- **Quality of Service (QoS)**: Policies controlling data distribution behavior

### Key Components

1. **Domain Participant**: Entry point to the DDS domain
2. **Data Writers**: Publish data samples
3. **Data Readers**: Subscribe to and receive data samples
4. **Topics**: Define the data type and name for communication
5. **IDL (Interface Definition Language)**: Define data structures

## Development Process

### Step 1: Define Data Types (IDL)

Create an IDL file defining your data structures:

```idl
// MessageTypes.idl
module YourModule {
  @topic
  struct YourDataType {
    string id;
    long count;
    string content;
    // Add your fields here
  };
};
```

Key considerations:
- Use `@topic` annotation for types that will be topics
- Keep structures simple and focused
- Consider versioning strategy for future changes

### Step 2: Generate Type Support Code

Use OpenDDS IDL compiler to generate C++ code:

```bash
opendds_idl YourTypes.idl
tao_idl -I$DDS_ROOT -I$TAO_ROOT/orbsvcs YourTypes.idl
```

This generates:
- Type support classes
- Serialization code
- DDS-specific interfaces

### Step 3: Create Publisher Application

Basic publisher structure:

```cpp
// Publisher.cpp
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include "YourTypesTypeSupportImpl.h"

int main(int argc, char* argv[]) {
    // 1. Initialize DDS
    DDS::DomainParticipantFactory_var dpf = 
        TheParticipantFactoryWithArgs(argc, argv);
    
    // 2. Create Domain Participant
    DDS::DomainParticipant_var participant = 
        dpf->create_participant(
            42,  // Domain ID
            PARTICIPANT_QOS_DEFAULT,
            0,   // No listener
            OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    // 3. Register Type
    YourModule::YourDataTypeTypeSupport_var ts = 
        new YourModule::YourDataTypeTypeSupportImpl;
    ts->register_type(participant, "");
    
    // 4. Create Topic
    DDS::Topic_var topic = 
        participant->create_topic(
            "YourTopicName",
            CORBA::String_var(ts->get_type_name()),
            TOPIC_QOS_DEFAULT,
            0,
            OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    // 5. Create Publisher
    DDS::Publisher_var publisher = 
        participant->create_publisher(
            PUBLISHER_QOS_DEFAULT,
            0,
            OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    // 6. Create DataWriter
    DDS::DataWriter_var writer = 
        publisher->create_datawriter(
            topic,
            DATAWRITER_QOS_DEFAULT,
            0,
            OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    // 7. Narrow to typed writer
    YourModule::YourDataTypeDataWriter_var typed_writer = 
        YourModule::YourDataTypeDataWriter::_narrow(writer);
    
    // 8. Write data
    YourModule::YourDataType sample;
    sample.id = "example";
    sample.count = 1;
    sample.content = "Hello OpenDDS";
    
    typed_writer->write(sample, DDS::HANDLE_NIL);
    
    // 9. Cleanup
    participant->delete_contained_entities();
    dpf->delete_participant(participant);
    TheServiceParticipant->shutdown();
    
    return 0;
}
```

### Step 4: Create Subscriber Application

Basic subscriber structure:

```cpp
// Subscriber.cpp
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/WaitSet.h>
#include "YourTypesTypeSupportImpl.h"

class DataReaderListenerImpl : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener> {
public:
    void on_data_available(DDS::DataReader_ptr reader) {
        YourModule::YourDataTypeDataReader_var typed_reader = 
            YourModule::YourDataTypeDataReader::_narrow(reader);
        
        YourModule::YourDataType sample;
        DDS::SampleInfo info;
        
        DDS::ReturnCode_t status = typed_reader->take_next_sample(sample, info);
        
        if (status == DDS::RETCODE_OK && info.valid_data) {
            // Process the sample
            std::cout << "Received: " << sample.content << std::endl;
        }
    }
    
    // Implement other listener methods as needed
    void on_requested_deadline_missed(DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus&) {}
    void on_requested_incompatible_qos(DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus&) {}
    void on_sample_rejected(DDS::DataReader_ptr, const DDS::SampleRejectedStatus&) {}
    void on_liveliness_changed(DDS::DataReader_ptr, const DDS::LivelinessChangedStatus&) {}
    void on_subscription_matched(DDS::DataReader_ptr, const DDS::SubscriptionMatchedStatus&) {}
    void on_sample_lost(DDS::DataReader_ptr, const DDS::SampleLostStatus&) {}
};

int main(int argc, char* argv[]) {
    // 1-4. Same initialization as publisher
    // ... (DomainParticipant, Type registration, Topic creation)
    
    // 5. Create Subscriber
    DDS::Subscriber_var subscriber = 
        participant->create_subscriber(
            SUBSCRIBER_QOS_DEFAULT,
            0,
            OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    // 6. Create DataReader with listener
    DDS::DataReaderListener_var listener = new DataReaderListenerImpl;
    
    DDS::DataReader_var reader = 
        subscriber->create_datareader(
            topic,
            DATAREADER_QOS_DEFAULT,
            listener,
            OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    
    // 7. Wait for data (or use WaitSet)
    // Simple wait
    ACE_OS::sleep(30);  // Wait for 30 seconds
    
    // 8. Cleanup
    // ... (same as publisher)
    
    return 0;
}
```

### Step 5: Configuration File

Create an OpenDDS configuration file (`opendds.ini`):

```ini
[common]
DCPSDebugLevel=0
DCPSInfoRepo=file://repo.ior

[transport/tcp]
transport_type=tcp
local_address=localhost:12345

[domain/42]
DefaultTransport=tcp

[repository]
RepositoryIor=repo.ior
```

### Step 6: Build Configuration

Create a CMakeLists.txt or MPC file:

```cmake
# CMakeLists.txt example
cmake_minimum_required(VERSION 3.8)
project(YourOpenDDSApp CXX)

find_package(OpenDDS REQUIRED)

# IDL Processing
OPENDDS_TARGET_SOURCES(YourOpenDDSApp PUBLIC YourTypes.idl)

# Publisher executable
add_executable(publisher 
    Publisher.cpp
)
target_link_libraries(publisher 
    OpenDDS::OpenDDS
)

# Subscriber executable
add_executable(subscriber 
    Subscriber.cpp
)
target_link_libraries(subscriber 
    OpenDDS::OpenDDS
)
```

## Application Structure Template

Recommended directory structure:
```
your-opendds-app/
├── idl/
│   └── YourTypes.idl
├── src/
│   ├── Publisher.cpp
│   ├── Subscriber.cpp
│   └── common/
│       └── QosProfiles.cpp
├── config/
│   └── opendds.ini
├── CMakeLists.txt
└── README.md
```

## Quality of Service (QoS) Policies

Key QoS policies to consider:

1. **Reliability**: RELIABLE vs BEST_EFFORT
2. **Durability**: VOLATILE, TRANSIENT_LOCAL, TRANSIENT, PERSISTENT
3. **History**: KEEP_ALL vs KEEP_LAST
4. **Deadline**: Maximum time between samples
5. **Lifespan**: How long data remains valid
6. **Partition**: Logical grouping of endpoints

Example QoS configuration:
```cpp
DDS::DataWriterQos writer_qos;
publisher->get_default_datawriter_qos(writer_qos);

writer_qos.reliability.kind = DDS::RELIABLE_RELIABILITY_QOS;
writer_qos.reliability.max_blocking_time.sec = 1;
writer_qos.reliability.max_blocking_time.nanosec = 0;

writer_qos.durability.kind = DDS::TRANSIENT_LOCAL_DURABILITY_QOS;

writer_qos.history.kind = DDS::KEEP_LAST_HISTORY_QOS;
writer_qos.history.depth = 10;
```

## Error Handling Best Practices

1. Always check return codes:
```cpp
DDS::ReturnCode_t ret = writer->write(sample, DDS::HANDLE_NIL);
if (ret != DDS::RETCODE_OK) {
    std::cerr << "Write failed: " << ret << std::endl;
}
```

2. Implement comprehensive listener callbacks
3. Use WaitSets for synchronous operations
4. Handle network disconnections gracefully
5. Implement proper cleanup in destructors

## Discovery Configuration

OpenDDS supports multiple discovery mechanisms:

1. **Centralized (DCPSInfoRepo)**:
```bash
$DDS_ROOT/bin/DCPSInfoRepo -o repo.ior
```

2. **RTPS Discovery** (Peer-to-peer):
```cpp
// In configuration
[common]
DCPSGlobalTransportConfig=$file
DCPSDefaultDiscovery=DEFAULT_RTPS

[transport/the_rtps_transport]
transport_type=rtps_udp
```

3. **Static Discovery** (No discovery traffic):
```ini
[common]
DCPSDefaultDiscovery=DEFAULT_STATIC

[endpoint/reader]
domain=42
participant=1
entity=2
type=YourModule::YourDataType
topic=YourTopicName
```

## Testing Strategy

1. **Unit Tests**: Test IDL types and business logic
2. **Integration Tests**: Test publisher-subscriber communication
3. **Performance Tests**: Measure throughput and latency
4. **Fault Injection**: Test error handling and recovery

## Common Patterns

### Request-Reply Pattern
```cpp
// Use two topics: Request and Reply
// Correlate using a request_id field
```

### Last Value Cache Pattern
```cpp
// Set QoS for latest value:
reader_qos.history.kind = DDS::KEEP_LAST_HISTORY_QOS;
reader_qos.history.depth = 1;
reader_qos.durability.kind = DDS::TRANSIENT_LOCAL_DURABILITY_QOS;
```

### Content Filtering
```cpp
DDS::ContentFilteredTopic_var filtered_topic = 
    participant->create_contentfilteredtopic(
        "FilteredTopic",
        topic,
        "count > 10",  // Filter expression
        DDS::StringSeq());
```

## Debugging Tips

1. Enable debug output:
```ini
[common]
DCPSDebugLevel=5
DCPSTransportDebugLevel=5
```

2. Use OpenDDS Monitor GUI for visualization
3. Enable logging to files
4. Use Wireshark with RTPS dissector
5. Check discovery with `DCPSInfoRepo -v`

## Migration from Messenger Example

When adapting from the Messenger example:

1. Replace `Message` struct with your data types
2. Update topic names and domain IDs
3. Modify QoS policies for your requirements
4. Extend DataReaderListener for your logic
5. Add error handling and reconnection logic
6. Implement proper shutdown sequences

## Performance Optimization

1. **Batch multiple samples**: Reduce overhead
2. **Use appropriate QoS**: Don't over-specify reliability
3. **Tune transport settings**: Buffer sizes, thread pools
4. **Consider multicast**: For one-to-many scenarios
5. **Profile your application**: Identify bottlenecks

## Security Considerations

OpenDDS supports DDS Security specification:
- Authentication
- Access control
- Encryption
- Data integrity

Configure in governance and permissions XML files.

## Next Steps

1. Start with simple publisher-subscriber pair
2. Add QoS policies incrementally
3. Implement error handling
4. Add discovery configuration
5. Optimize for your use case
6. Add security if required

## References

- OpenDDS Developer's Guide: https://opendds.readthedocs.io/
- DDS Specification: https://www.omg.org/spec/DDS/
- OpenDDS GitHub: https://github.com/OpenDDS/OpenDDS
- Example Applications: $DDS_ROOT/DevGuideExamples/

---

*This guide is designed for use with Claude Code to assist in OpenDDS application development. Provide specific requirements for your distributed application to receive tailored implementation guidance.*