
#include "jumper.h"
#include "commands.h"

#include <string.h>
#include <stdarg.h>
#include <assert.h>


static int handle_jump(const char *hook_name);

static void init_context();
static void cleanup_context();
static void cleanup_and_exit(uint8_t code);

static void parse_args(int argc, char **argv);
static jmp_flag_context_t *retrieve_flag_context(const char *str);

static bool validate_cmd();

static void exec();


/**
 * Jumper context strucuture, holds information regarding the user command
 * and target execution behaviour 
 */
static jmp_context_t *_jctx;   


/**
 * Defines the different flags the program can handle, each struct includes 
 * the flag itself, the name of the flag as passed by the user, the file handling
 * mode, and the different combinations of flags it can come with.
 */
static const jmp_flag_context_t _flag_ctxts[] = {
        { ADD,
                "-add", 
                "a",  
                { ADD|DIR, ADD|DIR|DESCR, 0 }
        },
        { MOD, 
                "-mod",
                "r", 
                { MOD|DIR, MOD|DESCR, MOD|DIR|DESCR, 0 }
        },
        { DEL,
                "-del",
                "r",
                { DEL, 0 }
        },
        { DIR,
                "-dir" ,
                "r",
                { DIR, 0 }
        },
        { DESCR,
                "-descr",
                "r",
                { DESCR, 0 }
        },
        { LIST,
                "-list",
                "r",
                { LIST, 0 }
        },
        { HELP,
                "-help",
                NULL,
                { HELP, 0 }
        }
};


int 
main(int argc, char **argv) {
        if (argc <= 1) {
                fprintf(stderr, "Incorrect usage of jumper. Please run 'jumper -help' for more information.\n");
                return 1;
        }

        if (argc == 2 && argv[1][0] != '-')
               return handle_jump(argv[1]);

        init_context();

        parse_args(argc, argv);

        if (!validate_cmd()) {
                printf(" [ERR] Invalid combination of arguments detected, please use the -help flag for further information.\n");
                DEBUG_JUMPER_CONTEXT(_jctx);
                cleanup_and_exit(0);
        }

        exec();

        cleanup_context();
        return 0;
}


/**
 * @brief if the only argument recv'ed was the name of a hook, then we can simply perform the desired jump. 
 * @param hook_name, identifier of the target hook
 * @return 
 */
static int
handle_jump(const char *hook_name) {
        FILE *fptr = fopen(CONF_FNAME, "r");
        if (fptr == NULL) {
                printf(" [ERR] Failed to open config file. Expected path: '%s'\n", CONF_FNAME);
                return 1;
        }

        errorc err = do_jump(hook_name, fptr);
        if (err != ERR_SUCCESS) {
                printf(" [ERR] Failure\n Message: %s\n", retrieve_err_msg(err));
                return 1;
        }

        return 0;
}



/**
  *@brief allocate a new jumper context structure and initialise the members
 */
static void
init_context() {
        _jctx = (jmp_context_t *) calloc(1, sizeof(jmp_context_t));
        if (_jctx == NULL) {
                printf("[ERR] Fatal error, failed to init program context.\n");
                exit(1);
        }
        assert(_jctx->arg_count == 0);
}



/**
  *@brief free all allocated memory of the jumper context struct
 */
static void 
cleanup_context() {
        if (_jctx == NULL)
                return;
        for (uint8_t i = 0; i < _jctx->arg_count; i++)
                free(_jctx->args[i]);
        free(_jctx);
}



/**
  *@brief cleanup the program and exit with the given code
  *@param code the desired exit code
 */
static void
cleanup_and_exit(uint8_t code) {
        cleanup_context();
        exit(code);
}



static void
parse_args(int argc, char **argv) {
        uint8_t argv_ptr = 1;
        while (argv_ptr < argc) {
                jmp_arg_t *arg = (jmp_arg_t *)malloc(sizeof(jmp_arg_t));

                arg->flag = retrieve_flag_context(argv[argv_ptr++]);
                if (arg->flag == NULL) {
                        printf("\n [ERR] Parser failed to recognise flag: '%s'\n Exiting...\n\n", argv[argv_ptr-1]);
                        cleanup_and_exit(0);
                }

                if (_jctx->msf == NULL || arg->flag->type < _jctx->msf->type)
                        _jctx->msf = arg->flag;         // update most significant flag

                if (argv_ptr < argc && argv[argv_ptr][0] != '-')
                        arg->value = argv[argv_ptr++];

                _jctx->args[_jctx->arg_count++] = arg;
        }
}


static jmp_flag_context_t*
retrieve_flag_context(const char *str) {
        uint8_t l = sizeof(_flag_ctxts) / sizeof(jmp_flag_context_t);
        for (uint8_t i = 0; i < l; i++) {
                jmp_flag_context_t *flag = &_flag_ctxts[i];

                size_t str_l = strlen(str);
                size_t flg_l = strlen(flag->name);
                if (str_l == flg_l && strncmp(flag->name, str, str_l) == 0)
                        return flag;
        }
        return NULL;
}


static bool
validate_cmd() {
        uint8_t i = 0, flag_sum = 0;        
        for (; i < _jctx->arg_count; i++)
                flag_sum |= _jctx->args[i]->flag->type;

        jmp_flag_context_t *ms_flag = _jctx->msf;
        uint8_t l = sizeof(ms_flag->combinations) / sizeof(ms_flag->combinations[0]);
        for (i = 0; i < l; i++) {
                if (ms_flag->combinations[i] == flag_sum)
                        return true;
        }
        return false;
}



static void 
exec() {
        if (_jctx->msf->type == HELP) {
                do_help();
                return;
        }
        
        FILE *conf_file = fopen(CONF_FNAME, _jctx->msf->file_mode);
        if (conf_file == NULL) {
                printf(" [ERR] Failed to open config file, expected path: '%s'\n", CONF_FNAME);
                return;
        }

        errorc ret;
        switch (_jctx->msf->type) {
                case ADD: 
                        ret = do_add(_jctx, conf_file);
                        break;
                case MOD:
                        ret = do_mod(_jctx, conf_file);
                        break;
                case DEL:
                        ret = do_del(_jctx, conf_file); 
                        break;
                case DIR:
                        ret = do_dir(_jctx, conf_file);
                        break;
                case DESCR:
                        ret = do_descr(_jctx, conf_file);
                        break;
                case LIST:
                        ret = do_list(conf_file);
                        break;
                default: 
                        printf("Invalid command. No primary action to perform, please use -help as a reference.\n");
        }

        // printf(" Response: %s\n", retrieve_err_msg(ret));

        fclose(conf_file);
}
