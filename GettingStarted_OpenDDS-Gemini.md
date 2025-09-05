# **Guide for Creating OpenDDS Applications**

This guide provides a step-by-step walkthrough for creating a new OpenDDS application. It uses the "Messenger" application as a representative example to illustrate the core concepts and implementation details of the OpenDDS framework. This guide is designed to be used with Claude Code to facilitate the development process.

## **Prerequisites**

Before you begin, ensure you have the following installed and configured:

* **OpenDDS**: The OpenDDS framework must be built and installed on your system.  
* **C++ Compiler**: A modern C++ compiler (e.g., GCC, Clang, MSVC).  
* **CMake**: For building the application.  
* **Perl**: Required for some OpenDDS scripts.

## **Step 1: Define Your Data Model with IDL**

The first step in creating an OpenDDS application is to define the data structure that will be communicated between the publisher and subscriber. This is done using the **Interface Definition Language (IDL)**. The IDL file defines the data types and topics for your application.

### **Messenger Example: Messenger.idl**

The Messenger application defines a simple Message structure in a file named Messenger.idl:

module Messenger {  
  @topic struct Message {  
    string from;  
    string subject;  
    @key long subject\_id;  
    string text;  
    long count;  
  };  
};

* @topic: This annotation marks the Message struct as a data type that can be used as a topic.  
* @key: This annotation on the subject\_id field indicates that it is a key field. Keys are used by OpenDDS to differentiate between different instances of the same topic.

### **Your Application's Data Model**

**Action**: Define the data model for your application in an .idl file. Replace the contents of the Messenger.idl example with your own data structures.

// TODO: Define your application's data model here in a .idl file.  
// For example:  
/\*  
module YourApp {  
  @topic struct YourData {  
    @key long id;  
    string data\_field\_1;  
    double data\_field\_2;  
    // ... other fields  
  };  
};  
\*/

## **Step 2: Generate Type Support Code**

Once you have defined your data model in IDL, you need to generate the C++ code that will handle the serialization, deserialization, and memory management of your data types. The opendds\_idl compiler is used for this purpose.

$DDS\_ROOT/bin/opendds\_idl Messenger.idl

This command will generate several files, including MessengerTypeSupport.idl, MessengerTypeSupportImpl.h, and MessengerTypeSupportImpl.cpp. These files contain the necessary type-specific DDS interfaces for your application.

## **Step 3: Implement the Publisher**

The **publisher** is the application component responsible for creating and sending data samples. The following is a breakdown of the key steps in implementing a publisher, using the Messenger example.

### **Publisher Code (publisher.cpp)**

// Include necessary headers  
\#include "MessengerTypeSupportImpl.h"  
\#include \<dds/DCPS/Marked\_Default\_Qos.h\>  
\#include \<dds/DCPS/Service\_Participant.h\>  
\#include \<dds/DCPS/StaticIncludes.h\>

int main(int argc, char\* argv\[\]) {  
  // 1\. Initialize the DomainParticipantFactory  
  DDS::DomainParticipantFactory\_var dpf \= TheParticipantFactoryWithArgs(argc, argv);

  // 2\. Create the DomainParticipant  
  DDS::DomainParticipant\_var participant \= dpf-\>create\_participant(  
      42, // Domain ID  
      PARTICIPANT\_QOS\_DEFAULT,  
      0,  
      OpenDDS::DCPS::DEFAULT\_STATUS\_MASK);

  // 3\. Register the data type and create a Topic  
  Messenger::MessageTypeSupport\_var ts \= new Messenger::MessageTypeSupportImpl;  
  ts-\>register\_type(participant, "");  
  CORBA::String\_var type\_name \= ts-\>get\_type\_name();  
  DDS::Topic\_var topic \= participant-\>create\_topic(  
      "Movie Discussion List",  
      type\_name,  
      TOPIC\_QOS\_DEFAULT,  
      0,  
      OpenDDS::DCPS::DEFAULT\_STATUS\_MASK);

  // 4\. Create a Publisher  
  DDS::Publisher\_var publisher \= participant-\>create\_publisher(  
      PUBLISHER\_QOS\_DEFAULT,  
      0,  
      OpenDDS::DCPS::DEFAULT\_STATUS\_MASK);

  // 5\. Create a DataWriter  
  DDS::DataWriter\_var writer \= publisher-\>create\_datawriter(  
      topic,  
      DATAWRITER\_QOS\_DEFAULT,  
      0,  
      OpenDDS::DCPS::DEFAULT\_STATUS\_MASK);  
  Messenger::MessageDataWriter\_var message\_writer \=  
      Messenger::MessageDataWriter::\_narrow(writer);

  // 6\. Create a message and publish it  
  Messenger::Message message;  
  message.from \= "Comic Book Guy";  
  message.subject \= "Review";  
  message.subject\_id \= 99;  
  message.text \= "Worst. Movie. Ever.";  
  message.count \= 0;  
  message\_writer-\>write(message, DDS::HANDLE\_NIL);

  // 7\. Clean up  
  participant-\>delete\_contained\_entities();  
  dpf-\>delete\_participant(participant);  
  TheServiceParticipant-\>shutdown();

  return 0;  
}

### **Your Publisher Implementation**

**Action**: Implement your publisher application. You will need to:

* Initialize the DomainParticipant.  
* Register your custom data type.  
* Create a Topic, Publisher, and DataWriter.  
* Instantiate your data structure and populate it with data.  
* Write the data to the DDS network.

// TODO: Implement your publisher logic here.  
// \- Instantiate your custom data type.  
// \- Populate the data fields.  
// \- Use the DataWriter to send the data.

## **Step 4: Implement the Subscriber**

The **subscriber** is the application component that receives data from the network. A key part of the subscriber is the **DataReaderListener**, which is an object that is called by OpenDDS when new data is available.

### **DataReaderListener Implementation (DataReaderListenerImpl.h and DataReaderListenerImpl.cpp)**

The listener implementation defines the on\_data\_available method, which is where you process the incoming data.

// DataReaderListenerImpl.h  
class DataReaderListenerImpl : public DDS::DataReaderListener {  
public:  
  virtual void on\_data\_available(DDS::DataReader\_ptr reader);  
};

// DataReaderListenerImpl.cpp  
void DataReaderListenerImpl::on\_data\_available(DDS::DataReader\_ptr reader) {  
  Messenger::MessageDataReader\_var reader\_i \=  
      Messenger::MessageDataReader::\_narrow(reader);  
  Messenger::Message message;  
  DDS::SampleInfo info;  
  DDS::ReturnCode\_t error \= reader\_i-\>take\_next\_sample(message, info);

  if (error \== DDS::RETCODE\_OK) {  
    // Process the received message  
    std::cout \<\< "Message: " \<\< message.text \<\< std::endl;  
  }  
}

### **Your Subscriber Implementation**

**Action**: Implement your subscriber application and DataReaderListener.

* Initialize the DomainParticipant.  
* Register your custom data type.  
* Create a Topic, Subscriber, and DataReader.  
* Attach a DataReaderListener to the DataReader.  
* Implement the on\_data\_available method in your listener to process the incoming data.

// TODO: Implement your subscriber logic here.  
// \- Create a DataReaderListener for your custom data type.  
// \- In the on\_data\_available method, take the data from the reader.  
// \- Process the received data according to your application's requirements.

## **Step 5: Configuration**

OpenDDS uses configuration files (.ini) to specify the transport protocol and other settings.

### **Example Configuration (dds\_tcp\_conf.ini)**

\[common\]  
DCPSGlobalTransportConfig=$file  
DCPSDefaultDiscovery=DEFAULT\_RTPS

\[transport/the\_rtps\_transport\]  
transport\_type=rtps\_udp

This example configures the application to use the RTPS (Real-Time Publish-Subscribe) protocol over UDP. Other options include TCP.

## **Step 6: Building and Running the Application**

You can use CMake to build your OpenDDS application.

### **CMakeLists.txt**

cmake\_minimum\_required(VERSION 3.10)  
project(MessengerExample)

find\_package(OpenDDS REQUIRED)

\# Generate code from IDL  
opendds\_idl(Messenger.idl)

\# Add executables for publisher and subscriber  
add\_executable(publisher publisher.cpp ${Messenger\_IDL\_GENERATED\_FILES})  
add\_executable(subscriber subscriber.cpp DataReaderListenerImpl.cpp ${Messenger\_IDL\_GENERATED\_FILES})

\# Link against OpenDDS libraries  
target\_link\_libraries(publisher OpenDDS::OpenDDS)  
target\_link\_libraries(subscriber OpenDDS::OpenDDS)

### **Build and Run**

1. **Build the application**:  
   mkdir build  
   cd build  
   cmake ..  
   make

2. **Run the DCPSInfoRepo** (if not using RTPS discovery):  
   $DDS\_ROOT/bin/DCPSInfoRepo \-o repo.ior

3. **Run the subscriber**:  
   ./subscriber \-DCPSConfigFile dds\_tcp\_conf.ini

4. **Run the publisher**:  
   ./publisher \-DCPSConfigFile dds\_tcp\_conf.ini

You should see the subscriber print the message sent by the publisher.

## **Conclusion**

This guide has provided a comprehensive overview of how to create a simple OpenDDS application using the Messenger example as a reference. By following these steps, you can define your own data models, implement publishers and subscribers, and configure the communication between them. For more advanced topics such as Quality of Service (QoS) policies, security, and different transport configurations, please refer to the official [OpenDDS Developer's Guide](https://opendds.readthedocs.io/en/latest-release/devguide/index.html).