#include "DataReaderListenerImpl.h"
#include <iostream>

void DataReaderListenerImpl::on_requested_deadline_missed(
  DDS::DataReader_ptr,
  const DDS::RequestedDeadlineMissedStatus&)
{
}

void DataReaderListenerImpl::on_requested_incompatible_qos(
  DDS::DataReader_ptr,
  const DDS::RequestedIncompatibleQosStatus&)
{
}

void DataReaderListenerImpl::on_liveliness_changed(
  DDS::DataReader_ptr,
  const DDS::LivelinessChangedStatus&)
{
}

void DataReaderListenerImpl::on_subscription_matched(
  DDS::DataReader_ptr,
  const DDS::SubscriptionMatchedStatus&)
{
}

void DataReaderListenerImpl::on_sample_rejected(
  DDS::DataReader_ptr,
  const DDS::SampleRejectedStatus&)
{
}

void DataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader) {
  BasicMessage::Basic_MessageDataReader_var reader_i =
      BasicMessage::Basic_MessageDataReader::_narrow(reader);
  
  if (!reader_i) {
    std::cerr << "Failed to narrow data reader" << std::endl;
    return;
  }

  BasicMessage::Basic_Message message;
  DDS::SampleInfo info;
  DDS::ReturnCode_t error = reader_i->take_next_sample(message, info);

  if (error == DDS::RETCODE_OK) {
    if (info.valid_data) {
      std::cout << "Received message: " << message.message.in() << std::endl;
    }
  } else if (error != DDS::RETCODE_NO_DATA) {
    std::cerr << "Error reading data: " << error << std::endl;
  }
}

void DataReaderListenerImpl::on_sample_lost(
  DDS::DataReader_ptr,
  const DDS::SampleLostStatus&)
{
}