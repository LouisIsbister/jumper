#!/bin/bash

echo -e "\n@ Testing\n"

jumper -add hook_a -dir /home/louis/uni-projects -descr "hello world!"
jumper -del hook_a -dir /home/louis/uni-projects -descr "hello world!"
jumper -mod hook_a -dir /home/louis/new-uni-projects -descr "new description!"
# jumper hook_a
