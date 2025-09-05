#include "BasicMessageTypeSupportImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/StaticIncludes.h>
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    // 1. Initialize the DomainParticipantFactory
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

    // 2. Create the DomainParticipant
    DDS::DomainParticipant_var participant = dpf->create_participant(
        42, // Domain ID
        PARTICIPANT_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!participant) {
      std::cerr << "Failed to create participant" << std::endl;
      return 1;
    }

    // 3. Register the data type and create a Topic
    BasicMessage::Basic_MessageTypeSupport_var ts = new BasicMessage::Basic_MessageTypeSupportImpl;
    ts->register_type(participant, "");
    CORBA::String_var type_name = ts->get_type_name();
    DDS::Topic_var topic = participant->create_topic(
        "basic_message",
        type_name,
        TOPIC_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic) {
      std::cerr << "Failed to create topic" << std::endl;
      return 1;
    }

    // 4. Create a Publisher
    DDS::Publisher_var publisher = participant->create_publisher(
        PUBLISHER_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!publisher) {
      std::cerr << "Failed to create publisher" << std::endl;
      return 1;
    }

    // 5. Create a DataWriter
    DDS::DataWriter_var writer = publisher->create_datawriter(
        topic,
        DATAWRITER_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);
    BasicMessage::Basic_MessageDataWriter_var message_writer =
        BasicMessage::Basic_MessageDataWriter::_narrow(writer);

    if (!message_writer) {
      std::cerr << "Failed to create data writer" << std::endl;
      return 1;
    }

    // 6. Create a message and publish it
    BasicMessage::Basic_Message message;
    message.id = 1;
    message.message = "Hello World";
    
    std::cout << "Publishing message: " << message.message.in() << std::endl;
    
    DDS::ReturnCode_t ret = message_writer->write(message, DDS::HANDLE_NIL);
    if (ret != DDS::RETCODE_OK) {
      std::cerr << "Failed to write message" << std::endl;
      return 1;
    }

    // Give some time for the message to be sent
    ACE_OS::sleep(2);

    // 7. Clean up
    participant->delete_contained_entities();
    dpf->delete_participant(participant);
    TheServiceParticipant->shutdown();

    std::cout << "Publisher completed successfully" << std::endl;
    return 0;
  } catch (const CORBA::Exception& e) {
    std::cerr << "CORBA Exception: " << e << std::endl;
    return 1;
  }
}