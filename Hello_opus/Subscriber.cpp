#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/WaitSet.h>
#include "BasicMessageTypeSupportImpl.h"

class DataReaderListenerImpl : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener>
{
public:
    void on_data_available(DDS::DataReader_ptr reader)
    {
        BasicMessageModule::Basic_MessageDataReader_var typed_reader = 
            BasicMessageModule::Basic_MessageDataReader::_narrow(reader);
        
        BasicMessageModule::Basic_Message sample;
        DDS::SampleInfo info;
        
        DDS::ReturnCode_t status = typed_reader->take_next_sample(sample, info);
        
        if (status == DDS::RETCODE_OK && info.valid_data) {
            std::cout << "Received message: " << sample.message << std::endl;
        }
    }
    
    void on_requested_deadline_missed(DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus&) {}
    void on_requested_incompatible_qos(DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus&) {}
    void on_sample_rejected(DDS::DataReader_ptr, const DDS::SampleRejectedStatus&) {}
    void on_liveliness_changed(DDS::DataReader_ptr, const DDS::LivelinessChangedStatus&) {}
    void on_subscription_matched(DDS::DataReader_ptr, const DDS::SubscriptionMatchedStatus&) {}
    void on_sample_lost(DDS::DataReader_ptr, const DDS::SampleLostStatus&) {}
};

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
        
        // Create Subscriber
        DDS::Subscriber_var subscriber = 
            participant->create_subscriber(
                SUBSCRIBER_QOS_DEFAULT,
                0,
                OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (!subscriber) {
            std::cerr << "Failed to create subscriber" << std::endl;
            return 1;
        }
        
        // Create DataReader with listener
        DDS::DataReaderListener_var listener = new DataReaderListenerImpl;
        
        DDS::DataReader_var reader = 
            subscriber->create_datareader(
                topic,
                DATAREADER_QOS_DEFAULT,
                listener,
                OpenDDS::DCPS::DEFAULT_STATUS_MASK);
        
        if (!reader) {
            std::cerr << "Failed to create datareader" << std::endl;
            return 1;
        }
        
        std::cout << "Subscriber waiting for messages on 'basic_message' topic..." << std::endl;
        
        // Wait for data
        ACE_OS::sleep(10);  // Wait for 10 seconds
        
        std::cout << "Subscriber shutting down..." << std::endl;
        
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