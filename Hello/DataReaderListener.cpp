#include <ace/Log_Msg.h>
#include <dds/DCPS/Service_Participant.h>
#include "DataReaderListener.h"
#include "Basic_MessageTypeSupportC.h"

DataReaderListener::DataReaderListener()
{
}

DataReaderListener::~DataReaderListener()
{
}

void DataReaderListener::on_requested_deadline_missed(
  DDS::DataReader_ptr,
  const DDS::RequestedDeadlineMissedStatus&)
{
}

void DataReaderListener::on_requested_incompatible_qos(
  DDS::DataReader_ptr,
  const DDS::RequestedIncompatibleQosStatus&)
{
}

void DataReaderListener::on_liveliness_changed(
  DDS::DataReader_ptr,
  const DDS::LivelinessChangedStatus&)
{
}

void DataReaderListener::on_subscription_matched(
  DDS::DataReader_ptr,
  const DDS::SubscriptionMatchedStatus& status)
{
  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT("INFO: Subscription matched. current_count = %d\n"),
             status.current_count));
}

void DataReaderListener::on_sample_rejected(
  DDS::DataReader_ptr,
  const DDS::SampleRejectedStatus&)
{
}

void DataReaderListener::on_data_available(DDS::DataReader_ptr reader)
{
  Hello::Basic_MessageDataReader_var message_dr =
    Hello::Basic_MessageDataReader::_narrow(reader);

  if (!message_dr) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" _narrow failed!\n")));
    return;
  }

  Hello::Basic_Message message;
  DDS::SampleInfo si;

  DDS::ReturnCode_t status = message_dr->take_next_sample(message, si);

  if (status == DDS::RETCODE_OK) {
    if (si.valid_data) {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("INFO: Received message: %C\n"),
                 message.message.in()));
    }
  } else if (status == DDS::RETCODE_NO_DATA) {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" no data available!\n")));
  } else {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" take_next_sample failed with error: %d\n"),
               status));
  }
}

void DataReaderListener::on_sample_lost(
  DDS::DataReader_ptr,
  const DDS::SampleLostStatus&)
{
}