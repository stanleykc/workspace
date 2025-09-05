/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "DataReaderListenerImpl.h"
#include "Boilerplate.h"
#include <iostream>

using namespace examples::boilerplate;

DataReaderListenerImpl::DataReaderListenerImpl() : sample_count(0)
{
}

void
DataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
  Hello::Basic_MessageDataReader_var reader_i = narrow(reader);

  Hello::Basic_Message msg;
  DDS::SampleInfo info;

  DDS::ReturnCode_t error = reader_i->take_next_sample(msg, info);

  if (error == DDS::RETCODE_OK) {
    if (info.valid_data) {
      ++sample_count;
      std::cout << "Received message: " << msg.text.in() << std::endl;
    }
  } else {
    ACE_ERROR((LM_ERROR,
               ACE_TEXT("ERROR: %N:%l: on_data_available() -")
               ACE_TEXT(" take_next_sample failed!\n")));
  }
}