#!/bin/bash

echo "\n\n @ Testing"

jumper -add hook_a -dir /home/louis/uni-projects -descr "hello world!"
jumper hook_a
