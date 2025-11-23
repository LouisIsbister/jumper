
#include "jumper.h"
#include "commands.h"
#include <string.h>


static void init_context();
static void parse_args(int argc, char **argv);

static void exec(int argc, char **argv);
static ERR_CODE dispatch_execution();

static bool strncmp_ext(const char* s1, const char* s2, uint8_t n);
static void cleanup_jumper_context();



static jumper_context* _jctx;


int 
main(int argc, char **argv) {
        if (argc <= 1) {
                fprintf(stderr, "Incorrect usage of jumper. Please run 'jumper -help' for more information.\n");
                return 1;
        }

        init_context();

        if (argc == 2 && argv[1][0] != '-') { // early check for a jump command
                puts("Do jump!!\n");
                return 0;
        }

        parse_args(argc, argv);

        DEBUG_JUMPER_CONTEXT(_jctx);


        // exec(argc, argv);

        cleanup_jumper_context();
        return 0;
}


/**
 * @return the new program content structure
 */
static void
init_context() {
        _jctx = (jumper_context *)malloc(sizeof(jumper_context));
        if (_jctx == NULL) {
                fprintf(stderr, "Fatal error, failed to init jumper program context.\n");
                exit(1);
        }

        memset(_jctx->required_conf_access, '\0', sizeof(_jctx->required_conf_access));
        _jctx->required_conf_access[0] = 'r';
        _jctx->arg_count = 0;
}


static void
parse_args(int argc, char **argv) {
        argument** a = _jctx->args;
        
        uint8_t argv_ptr = 1;
        while (argv_ptr < argc) {
                *a = (argument*)malloc(sizeof(argument));
                (*a)->arg = argv[argv_ptr++];
                if (argv_ptr < argc && argv[argv_ptr][0] != '-') {
                        (*a)->value = argv[argv_ptr++];
                }
                a++; _jctx->arg_count++;
        }
}





static void 
exec(int argc, char **argv) {
        // ERR_CODE ret;

        // ret = parseCommand(pctx, argc, argv);
        // if (ret != ERR_SUCCESS) printErrCleanupAndExit(pctx, ret);

        // ret = openConfigFile(pctx);
        // if (ret != ERR_SUCCESS) printErrCleanupAndExit(pctx, ret);

        // ret = dispatchExecution(pctx);
        // if (ret != ERR_SUCCESS) printErrCleanupAndExit(pctx, ret);

        // cleanupProgramContext(pctx);
}




/**
 * @brief having parsed the command, take the appropriate action to execute the actions
 * 
 * @param pctx the program context structure
 * @return the appropriate return code
 */
static ERR_CODE 
dispatch_execution() {
        if (_jctx->arg_count == 0) {
                return ERR_INVALID_CMD;
        }

        // char* arg1 = _jctx->jumper_args.args[0];
        // if (strncmp_ext("-add", arg1, 4)) {

        // } else if (strncmp_ext("-mod", arg1, 4)) {

        // } else if (strncmp_ext("-del", arg1, 4)) {

        // } else if (strncmp_ext("-descr", arg1, 6)) {
                
        // } else if (strncmp_ext("-list", arg1, 5)) {
                
        // } else if (strncmp_ext("-help", arg1, 5)) {
                
        // }

        return ERR_SUCCESS;
        // char hookBuffer[MAX_HOOK_LENGTH];
        // char *hookName = pctx->hookName;
        // FILE *confFile = pctx->confFilePtr;

        // // try and execute commands that do not require the the exact hook
        // // to be retrieved from the .conf file!
        // switch (pctx->action) {
        //         case LIST:
        //                 list(pctx->confFilePtr);
        //                 return ERR_SUCCESS;
        //         case HELP:
        //                 help();
        //                 return ERR_SUCCESS;
        //         case ADD:
        //                 formatHook(hookBuffer, hookName, pctx->newHookDir, pctx->newHookDescr);
        //                 add(hookBuffer, pctx->confFilePtr);
        //                 return ERR_SUCCESS;
        //         case DESCR:
        //                 // descr(hookName, hookDescr);
        //                 return ERR_SUCCESS;
        //         case JUMP:
        //                 return ERR_SUCCESS;
        //         default: ;
        // }

        // // get the bytes in the .conf file before the hook,
        // // and store the hook itself in hookBuffer
        // uint16_t lineNumber = retrieveHook(hookName, hookBuffer, confFile);
        // if (lineNumber == 0) {
        //         return ERR_UNKNOWN_HOOK;
        // }

        // switch (pctx->action) {
        //         case MOD:
        //                 mod(lineNumber, hookName, pctx->newHookDir, pctx->newHookDescr, hookBuffer, confFile);
        //                 return ERR_SUCCESS;
        //         case DEL:
        //                 del(lineNumber, confFile);
        //                 return ERR_SUCCESS;
        //         default: 
        //                 return ERR_FAILURE;
        // }
}

static bool
strncmp_ext(const char* s1, const char* s2, uint8_t n) {
        if (strlen(s1) > n || strlen(s2) > n) {
                return false;
        }
        return strncmp(s1, s2, n) == 0;
}


// ERR_CODE 
// openConfigFile() {
//         pctx->confFilePtr = fopen(pctx->confPath, pctx->requiredConfAccess);
//         if (pctx->confFilePtr == NULL) {
//                 return ERR_FAILURE;
//         }

//         return ERR_SUCCESS;
// }


static void 
cleanup_jumper_context() {
        if (_jctx == NULL)      
                return;
        free(_jctx);
}

// void 
// printErrCleanupAndExit(ProgramContext *pctx, ERR_CODE ec) {
//         printf("Err: '%s'\n", retrieveErrMsg(ec));
//         cleanupProgramContext(pctx);
//         exit(ec);
// }





// ERR_CODE tryBuild2ArgCommand(ProgramContext *pctx, char **argv);
// ERR_CODE tryBuild3ArgCommand(ProgramContext *pctx, char **argv);

// /**
//  * @brief given the command line arguments, parse their information into the program context
//  * 
//  * @param pctx the program context structure whose command info this method is trying to set
//  * @param argc the number of args passed
//  * @param argv the args themselvs
//  * @return the appropriate return code
//  */
// ERR_CODE 
// parseCommand(ProgramContext *pctx, int argc, char **argv) {
//         switch (argc) {
//                 case 2:
//                 return tryBuild2ArgCommand(pctx, argv); 
//                 case 3:
//                 return tryBuild3ArgCommand(pctx, argv);
//         }

//         // build the command for -add and -mod
//         if (strcmp("-add", argv[1]) == 0) {
//                 pctx->requiredConfAccess = "a";
//                 pctx->hookName = argv[2];
//                 pctx->action = ADD;
//         } else if (strcmp("-mod", argv[1]) == 0) {
//                 pctx->requiredConfAccess = "r+";
//                 pctx->hookName = argv[2];
//                 pctx->action = MOD;
//         } else {
//                 printf("Unknown flag: '%s'\n", argv[1]);
//                 return ERR_INVALID_CMD;
//         }

//         // consume the rest of the arguments passed after the add and mod flags

//         char **ptr = argv + 3; 
//         char *flag, *value;
//         while ((flag = *ptr) != NULL && (value = *(ptr+1)) != NULL) {
//                 if (strcmp("-dir", flag) == 0) {
//                 pctx->newHookDir = value;
//                 } else if (strcmp("-descr", flag) == 0) {
//                 pctx->newHookDescr = value;
//                 } else {
//                 return ERR_INVALID_CMD;
//                 }
//                 ptr += 2;
//         }
        
//         return ERR_SUCCESS;
// }


// /** @brief function that attempts to parse the commands -init -list -help and 'jumper <hook_name>'
//  *
//  * @param argv command line args 
//  * @param pctx program context structure
//  * @return flag for success/failure
//  */
// ERR_CODE 
// tryBuild2ArgCommand(ProgramContext *pctx, char **argv) {
//         if (strcmp("-list", argv[1]) == 0) {
//                 pctx->action = LIST;
//                 return ERR_SUCCESS;
//         } else if (strcmp("-help", argv[1]) == 0) {
//                 pctx->action = HELP;
//                 return ERR_SUCCESS;
//         } else {
//                 // the case when we are actually jumping!
//                 pctx->hookName = argv[1]; // the only time argv[1] is not a cmd
//                 pctx->action = JUMP;
//                 return ERR_SUCCESS;
//         }

//         printf("Unknown flag: '%s'\n", argv[1]);
//         return ERR_INVALID_CMD;
// }

// /** @brief function to parse the commands -del and -descr
//  *
//  * @param argv command line args 
//  * @param pctx program context structure
//  * @return flag for success/failure
//  */
// ERR_CODE 
// tryBuild3ArgCommand(ProgramContext *pctx, char **argv) {
//         pctx->hookName = argv[2]; // set the provided hook name
//         if (strcmp("-del", argv[1]) == 0) {
//                 pctx->requiredConfAccess = "r+"; // we need tor read and write to delete
//                 pctx->action = DEL;
//                 return ERR_SUCCESS;
//         } else if (strcmp("-descr", argv[1]) == 0) {
//                 pctx->action = DESCR;
//                 return ERR_SUCCESS;
//         }

//         printf("Unknown flag: '%s'\n", argv[1]);
//         return ERR_INVALID_CMD;
// }