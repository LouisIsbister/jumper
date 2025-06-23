# Jumper  
As much as I enjoy doing things from the command line, one thing I thoroughly dislike is typing out really long and laborious directoy paths to get to my frequently visited directories, especially my university directory! As such, this is a simple program to that allows you to hook directories you often visit and navigate to them immediately by just executing `jumper <hook_name>`. I am fully aware this is neither orignal nor very difficult to make, I just wanted an execuse to write something small in C :)

### Commands:
`jumper <hook_name>`
  - jump to the directory associated with the hook name

`jumper -add <hook_name> -dir <dir> [-descr "<description>"]`
  - create a new hooked directory

`jumper -mod <hook_name> [-dir <new_dir>] [-descr "<new_description>"]`
  - modify the contents of an existing hook. Both the directory and description are optional, providing neither will simply do nothing!

`jumper -del <hook_name>`
  - delete a hook by its name

`jumper -descr <hook_name>`
  - print the description associated with a hook

`jumper -list`
  - print all hooks along with their descriptions

`jumper -help`
  - print all this information to the terminal