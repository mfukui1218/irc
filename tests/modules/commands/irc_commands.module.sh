#!/bin/bash

# comment: include "../arg_utils.sh"

##### irc commands #####
pass_command() {
	local password="$1"
	echo "PASS $password"
}

nick_command() {
	local nickname="$1"
	echo "NICK $nickname"
}

user_command() {
	local user="$1" mode="$2" unused="$3" realname="$4"
	echo "USER $user $mode $unused :$realname"
}

simple_connect_command() { # command flow: pass -> nick -> user
	local password="$1" nickname="$2"
	pass_command "$password"
	nick_command "$nickname"
	user_command "u-$nickname" "localhost" "*" "r-$nickname"
}

join_command() {
	local channels="$1" keys="$2"
	echo "JOIN $channels"$([ -n "$keys" ] && echo -n " ")"$keys"
}

privmsg_command() {
	local target="$1" text_to_sent="$2"
	local msg="PRIVMSG"
	msg="$(append_text_with_space "$msg" "$target")"
	msg="$(append_text_with_space "$msg" "$(append_prefix ":" "$text_to_sent")")"
	echo "$msg"
}

channel_mode_command() {
	local msg="MODE"
	for arg in "$@"; do
	    msg="$(append_text_with_space "$msg" "$arg")" 
	done
	echo "$msg"
}
