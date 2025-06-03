Create an executable that can create and jump to hook directories.
For example, my university directory might be located at /home/louis/documents/uni. And then each year has a subdirectory for 1st, 2nd, 3rd, and 4th year. I want to be able to just say `jumper 3rd-year` and jump straight the hooked directory.

Commands we would need:  

`jumper <hook_name>`
  - jump to the directory associated with the hook name

`jumper -add <hook_name> -dir <dir> [-descr " ... description ... "]`
  - add a new hooked directory

`jumper -mod <hook_name> -dir <new_dir> [-descr  ... description ... ]`
  - modeifiy an existing hookname

`jumper -del <hook_name>`
  - delete a hook

`jumper -descr <hook_name>`
  - print the description associated with a hook

`jumper -list`
  - print all hooks alone with their descriptions

`jumper -help`
  - print this information again


Have a conf file that stores said hooks in the form:
hook_name|target_dir|description

Add note that the hook name cannot have a | in it!