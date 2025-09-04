#!/usr/bin/env perl

use strict;
use warnings;

use Env qw(ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlDDS::Run_Test;

my $test = new PerlDDS::Run_Test();

$test->setup_discovery();

$test->process("subscriber", "subscriber", "");
$test->process("publisher", "publisher", "");

$test->start_process("subscriber");

# Let subscriber get ready
sleep 2;

$test->start_process("publisher");

# Give publisher time to send message
sleep 3;

my $PublisherResult = $test->finish(10);
my $SubscriberResult = $test->finish(10);

if ($PublisherResult != 0) {
    print STDERR "ERROR: publisher returned $PublisherResult\n";
    exit 1;
}

if ($SubscriberResult != 0) {
    print STDERR "ERROR: subscriber returned $SubscriberResult\n";
    exit 1;
}

exit 0;