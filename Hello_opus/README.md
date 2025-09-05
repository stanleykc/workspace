# Hello_opus OpenDDS Application

A simple OpenDDS publisher-subscriber application that demonstrates basic DDS communication.

## Structure

- `BasicMessage.idl` - IDL definition for the Basic_Message data type
- `Publisher.cpp` - Publisher application that sends "Hello World" message
- `Subscriber.cpp` - Subscriber application that receives and displays messages
- `Publisher.mpc` - MPC project file for building the publisher
- `Subscriber.mpc` - MPC project file for building the subscriber

## Building

1. Source the OpenDDS environment:
   ```bash
   source /workspace/OpenDDS/setenv.sh
   ```

2. Generate project files and build:
   ```bash
   cd Hello_opus
   
   # Generate makefiles from MPC files
   $ACE_ROOT/bin/mwc.pl -type gnuace Publisher.mpc
   $ACE_ROOT/bin/mwc.pl -type gnuace Subscriber.mpc
   
   # Build publisher
   make -f GNUmakefile.BasicMessage_Publisher
   
   # Build subscriber  
   make -f GNUmakefile.BasicMessage_Subscriber
   ```

## Running

1. Start DCPSInfoRepo in one terminal:
   ```bash
   $DDS_ROOT/bin/DCPSInfoRepo -o repo.ior
   ```

2. Start the subscriber in another terminal:
   ```bash
   cd Hello_opus
   ./subscriber -DCPSConfigFile ../rtps.ini
   ```

3. Start the publisher in a third terminal:
   ```bash
   cd Hello_opus
   ./publisher -DCPSConfigFile ../rtps.ini
   ```

The subscriber should display "Received message: Hello World" when the publisher sends the message.

## Communication Details

- **Topic**: "basic_message" 
- **Data Type**: BasicMessageModule::Basic_Message (contains a string message field)
- **Domain ID**: 42
- **Message**: "Hello World"

The publisher waits 2 seconds before sending the message to allow the subscriber to connect, then waits another 2 seconds before shutdown to ensure message delivery.