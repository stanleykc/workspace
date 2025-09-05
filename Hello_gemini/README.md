# Hello Gemini OpenDDS Application

A simple OpenDDS publisher-subscriber application that demonstrates basic message passing.

## Overview

This application consists of:
- **Publisher**: Sends "Hello World" message on the "basic_message" topic
- **Subscriber**: Listens for messages on the "basic_message" topic and prints them to console
- **Basic_Message**: IDL-defined message type with an ID and message string

## Building

1. Source the OpenDDS environment:
   ```bash
   source /workspace/OpenDDS/setenv.sh
   ```

2. Build the application:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Running

1. In one terminal, run the subscriber:
   ```bash
   ./subscriber -DCPSConfigFile ../dds_tcp_conf.ini
   ```

2. In another terminal, run the publisher:
   ```bash
   ./publisher -DCPSConfigFile ../dds_tcp_conf.ini
   ```

The subscriber should display "Received message: Hello World" when the publisher sends the message.