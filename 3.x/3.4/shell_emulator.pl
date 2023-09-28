#!/usr/bin/perl
use strict;
use warnings;

$| = 1;

sub process_command {
    my ($command) = @_;
    if ($command =~ /\|/) {
        my @pipe_cmds = split /\|/, $command;
        my ($rh, $wh);
        pipe($rh, $wh);
        if (fork() == 0) {
            close $rh;
            open(STDOUT, ">&", $wh);
            my @cmd = split ' ', $pipe_cmds[0];
            exec @cmd;
            exit;
        }
        close $wh;
        wait;

        if (fork() == 0) {
            open(STDIN, "<&", $rh);
            my @cmd = split ' ', $pipe_cmds[1];
            exec @cmd;
            exit;
        }
        close $rh;
        wait;
    } else {
        if (fork() == 0) {
            my @cmd = split ' ', $command;
            exec @cmd;
            exit;
        }
        wait;
    }
}

print "prompt> ";
while (<>) {
    chomp;
    my @commands = split /;/;
    for my $command (@commands) {
        if ($command =~ /^\s*\((.*)\)\s*$/) {
            my $block = $1;
            my @block_commands = split /&/, $block;
            for my $block_command (@block_commands) {
                process_command($block_command);
            }
        } else {
            process_command($command);
        }
    }
    print "prompt> ";
}
