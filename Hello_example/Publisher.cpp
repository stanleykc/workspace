/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "Boilerplate.h"
#include <dds/DCPS/Service_Participant.h>
#include <model/Sync.h>
#include <stdexcept>

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

    DDS::Publisher_var publisher = createPublisher(participant);

    DDS::DataWriter_var writer = createDataWriter(publisher, topic);

    Hello::Basic_MessageDataWriter_var msg_writer = narrow(writer);

    {
      OpenDDS::Model::WriterSync ws(writer);

      Hello::Basic_Message message;
      message.message_id = 1;
      message.text = "Hello World";

      DDS::ReturnCode_t error = msg_writer->write(message, DDS::HANDLE_NIL);
      if (error != DDS::RETCODE_OK) {
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("ERROR: %N:%l: main() -")
                   ACE_TEXT(" write returned %d!\n"), error));
      }
    }

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