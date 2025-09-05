#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/PublisherImpl.h>
#include "BasicMessageTypeSupportImpl.h"

int main(int argc, char* argv[])
{
    try {
        // Initialize DDS
        DDS::DomainParticipantFactory_var dpf = 
            TheParticipantFactoryWithArgs(argc, argv);
        
        // Create Domain Participant
        DDS::DomainParticipant_var participant = 
            dpf->create_participant(
                42,  // Domain ID
                PARTICIPANT_QOS_DEFAULT,
                0,   // No listener
                OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (!participant) {
            std::cerr << "Failed to create participant" << std::endl;
            return 1;
        }
        
        // Register Type
        BasicMessageModule::Basic_MessageTypeSupport_var ts = 
            new BasicMessageModule::Basic_MessageTypeSupportImpl;
        
        if (ts->register_type(participant, "") != DDS::RETCODE_OK) {
            std::cerr << "Failed to register type" << std::endl;
            return 1;
        }
        
        // Create Topic
        DDS::Topic_var topic = 
            participant->create_topic(
                "basic_message",
                CORBA::String_var(ts->get_type_name()),
                TOPIC_QOS_DEFAULT,
                0,
                OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (!topic) {
            std::cerr << "Failed to create topic" << std::endl;
            return 1;
        }
        
        // Create Publisher
        DDS::Publisher_var publisher = 
            participant->create_publisher(
                PUBLISHER_QOS_DEFAULT,
                0,
                OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (!publisher) {
            std::cerr << "Failed to create publisher" << std::endl;
            return 1;
        }
        
        // Create DataWriter
        DDS::DataWriter_var writer = 
            publisher->create_datawriter(
                topic,
                DATAWRITER_QOS_DEFAULT,
                0,
                OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (!writer) {
            std::cerr << "Failed to create datawriter" << std::endl;
            return 1;
        }
        
        // Narrow to typed writer
        BasicMessageModule::Basic_MessageDataWriter_var typed_writer = 
            BasicMessageModule::Basic_MessageDataWriter::_narrow(writer);
        
        if (!typed_writer) {
            std::cerr << "Failed to narrow datawriter" << std::endl;
            return 1;
        }
        
        // Wait for subscriber to connect
        std::cout << "Publisher waiting for subscriber..." << std::endl;
        ACE_OS::sleep(2);
        
        // Create and write data
        BasicMessageModule::Basic_Message sample;
        sample.message = "Hello World";
        
        std::cout << "Publishing: " << sample.message << std::endl;
        
        DDS::ReturnCode_t ret = typed_writer->write(sample, DDS::HANDLE_NIL);
        if (ret != DDS::RETCODE_OK) {
            std::cerr << "Write failed: " << ret << std::endl;
            return 1;
        }
        
        std::cout << "Message published successfully" << std::endl;
        
        // Wait a bit to ensure delivery
        ACE_OS::sleep(2);
        
        // Cleanup
        participant->delete_contained_entities();
        dpf->delete_participant(participant);
        TheServiceParticipant->shutdown();
        
        return 0;
    }
    catch (const CORBA::Exception& e) {
        std::cerr << "Exception: " << e << std::endl;
        return 1;
    }
}