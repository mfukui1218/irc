#!/bin/bash


##### expected error reply #####
reply_error_400_unknown_error() {
	local nickname="$1"
	echo ":irc.localhost 400 $nickname :Unknown error^M$"
}

reply_error_401_no_such_nick_or_channel() {
	local nickname="$1" target="$2"
	echo ":irc.localhost 401 $nickname $target :No such nick/channel^M$"
}

reply_error_403_no_such_channel() {
	local nickname="$1" channel="$2"
	echo ":irc.localhost 403 $nickname $channel :No such channel^M$"
}

reply_error_411_no_recipient_given() {
	local nickname="$1"
	echo ":irc.localhost 411 $nickname :No recipient given^M$"
}

reply_error_412_no_text_to_send() {
	local nickname="$1"
	echo ":irc.localhost 412 $nickname :No text to send^M$"
}

reply_error_433_nickname_is_already_in_use() {
	local nickname="$1"
	echo ':irc.localhost 433  '"$nickname"' :Nickname is already in use^M$'
}

reply_error_431_no_nickname_given() {
	echo ':irc.localhost 431  :No nickname given^M$'
}

reply_error_432_erroneous_nickname() {
	local nickname="$1"
	echo ':irc.localhost 432  '"$nickname"' :Erroneous nickname^M$'
}

reply_error_461_not_enough_parameter() {
	local nickname="$1" command="$2"
	echo ":irc.localhost 461 $nickname $command :Not enough parameters^M$"
}

reply_error_451_have_not_registered() {
	local nickname="$1"
	echo ":irc.localhost 451 $nickname :You have not registered^M$"
}

reply_error_451_have_not_registered_with_command() {
	local command="$1"
	echo "451 $command :You have not registered$"
}

reply_error_462_may_not_reregister() {
	local nickname="$1"
	echo ":irc.localhost 462 $nickname :You may not reregister^M$"
}

reply_error_464_password_incorrect() {
	echo ':irc.localhost 464  :Password incorrect^M$'
}

reply_error_476_bad_channel_mask() {
	echo -n '';
}

##### other reply #####
__482_not_channel_operator() {
	local channel="$1"
	echo "482 $channel :You're not channel operator$";
}
