/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "DataReaderListenerImpl.h"
#include "Boilerplate.h"
#include <dds/DCPS/Service_Participant.h>
#include <model/Sync.h>
#include <stdexcept>
#include <iostream>

#include "dds/DCPS/StaticIncludes.h"

using namespace examples::boilerplate;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try {
    DDS::DomainParticipantFactory_var dpf =
      TheParticipantFactoryWithArgs(argc, argv);

    DDS::DomainParticipant_var participant = createParticipant(dpf);

    DDS::Topic_var topic = createTopic(participant);

    DDS::Subscriber_var subscriber = createSubscriber(participant);

    DataReaderListenerImpl* listener_impl = new DataReaderListenerImpl;
    DDS::DataReaderListener_var listener(listener_impl);

    DDS::DataReader_var reader = createDataReader(subscriber,
                                                  topic,
                                                  listener);

    {
      OpenDDS::Model::ReaderSync rs(reader);
    }

    std::cout << "Subscriber received " << listener_impl->sample_count
              << " samples" << std::endl;

    cleanup(participant, dpf);

  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  } catch (std::runtime_error& err) {
    ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: main() - %C\n"),
                      err.what()), -1);
  } catch (std::string& msg) {
    ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("ERROR: main() - %C\n"),
                      msg.c_str()), -1);
  }

  return 0;
}