#!/bin/bash


##### expected correct reply #####

simple_connect_msg() {
	local nickname="$1"
	welcome_msg
	reply_001_welcome_to_server "$nickname"
}

reply_001_welcome_to_server() {
	local nickname="$1"
	echo ":irc.localhost 001 $nickname :Welcome to the server!^M$"
}

##### none numeric reply #####
welcome_msg() {
	echo "Welcome to IRC server!$"
}

join_reply() {
	local nickname="$1" host="${2:-localhost}" channel="$3"
	none_numeric_reply "$nickname" "$host" "JOIN :$channel^M$"
}

privmsg_reply() {
	local nickname="$1" host="${2:-localhost}" target="$3" msg="$4"
	none_numeric_reply "$nickname" "$host" "PRIVMSG $target :$msg^M$"
}

mode_reply() {
	local nickname="$1" host="${2:-localhost}" channel="$3" modes="$4"
	none_numeric_reply "$nickname" "$host" "MODE $channel $modes $"
}

none_numeric_reply() {
	local nickname="$1" host="${2:-localhost}" msg="$3"
	echo ":$nickname!u-$nickname@$host $msg"
}
