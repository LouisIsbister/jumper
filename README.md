# Jumper  
Simple program that allows you to hook directories you often visit to short and simple names, and navigate to them in one short command. I am fully aware this is neither original nor very difficult to make, I just wanted an execuse to write something small in C :)

### Commands:
``cd `jumper <hook_name>` ``
  - jump to the directory associated with the hook name

`jumper -add <hook_name> -dir <dir> [-descr "<description>"]`
  - create a new hooked directory

`jumper -mod <hook_name> (-dir <new_dir> | -descr "<new_description>")`
  - modify the contents of an existing hook. Both the directory and description are optional, but at least on must always be provided!

`jumper -del <hook_name>`
  - delete a hook by its name

`jumper -dir <hook_name>`
  - print the directory associated with a hook

`jumper -descr <hook_name>`
  - print the description associated with a hook

`jumper -list`
  - print all hooks along with their descriptions

`jumper -help`
  - print all this information to the terminal
