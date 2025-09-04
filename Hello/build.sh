#!/bin/bash

# Build script for Hello OpenDDS application

# Source OpenDDS environment
source $DDS_ROOT/setenv.sh

# Generate type support code
opendds_idl -Wb,export_macro=Hello_Export -Wb,export_include=Hello_Export.h Basic_Message.idl

# Generate Makefiles from MPC
mwc.pl -type gnuace Hello.mpc

# Build the application
make

echo "Build complete."