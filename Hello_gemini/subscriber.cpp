#include "BasicMessageTypeSupportImpl.h"
#include "DataReaderListenerImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/StaticIncludes.h>
#include <dds/DCPS/WaitSet.h>
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

    // 4. Create a Subscriber
    DDS::Subscriber_var subscriber = participant->create_subscriber(
        SUBSCRIBER_QOS_DEFAULT,
        0,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!subscriber) {
      std::cerr << "Failed to create subscriber" << std::endl;
      return 1;
    }

    // 5. Create a DataReaderListener
    DDS::DataReaderListener_var listener(new DataReaderListenerImpl);

    // 6. Create a DataReader
    DDS::DataReader_var reader = subscriber->create_datareader(
        topic,
        DATAREADER_QOS_DEFAULT,
        listener,
        OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!reader) {
      std::cerr << "Failed to create data reader" << std::endl;
      return 1;
    }

    std::cout << "Subscriber waiting for messages. Press Ctrl+C to exit..." << std::endl;

    // Wait for messages (keep the program running)
    DDS::WaitSet_var ws = new DDS::WaitSet;
    DDS::StatusCondition_var sc = reader->get_statuscondition();
    sc->set_enabled_statuses(DDS::DATA_AVAILABLE_STATUS);
    ws->attach_condition(sc);

    DDS::Duration_t timeout = { 30, 0 }; // 30 seconds timeout
    DDS::ConditionSeq conditions;

    while (true) {
      DDS::ReturnCode_t ret = ws->wait(conditions, timeout);
      if (ret == DDS::RETCODE_OK) {
        // Data is available, listener will handle it
        continue;
      } else if (ret == DDS::RETCODE_TIMEOUT) {
        std::cout << "Timeout waiting for data, continuing..." << std::endl;
        continue;
      } else {
        std::cerr << "Wait failed: " << ret << std::endl;
        break;
      }
    }

    // 7. Clean up
    ws->detach_condition(sc);
    participant->delete_contained_entities();
    dpf->delete_participant(participant);
    TheServiceParticipant->shutdown();

    return 0;
  } catch (const CORBA::Exception& e) {
    std::cerr << "CORBA Exception: " << e << std::endl;
    return 1;
  }
}