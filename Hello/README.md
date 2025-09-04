# Hello OpenDDS Application

This is a simple OpenDDS application demonstrating publisher-subscriber communication using a Basic_Message type on the "basic_message" topic.

## Structure

- `Basic_Message.idl` - IDL definition of the message type
- `Publisher.cpp` - Publisher application that sends "Hello World" message
- `Subscriber.cpp` - Subscriber application that receives messages
- `DataReaderListener.h/cpp` - Listener for handling received messages
- `Hello.mpc` - MPC project file for building
- `build.sh` - Build script
- `run_test.pl` - Test script

## Building

1. Source the OpenDDS environment:
   ```bash
   source $DDS_ROOT/setenv.sh
   ```

2. Run the build script:
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

## Running

### Option 1: Use test script
```bash
perl run_test.pl
```

### Option 2: Manual execution
1. Start the subscriber first:
   ```bash
   ./subscriber
   ```

2. In another terminal, start the publisher:
   ```bash
   ./publisher
   ```

The subscriber will print the received "Hello World" message to the console.

## How it works

1. The publisher creates a DDS domain participant, topic, and data writer
2. It sends a Basic_Message with content "Hello World" on the "basic_message" topic
3. The subscriber creates a domain participant, topic, and data reader with a listener
4. When the subscriber receives the message, the listener's `on_data_available()` callback is invoked
5. The listener reads the message and prints it to the console