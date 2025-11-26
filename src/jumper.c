
#include "jumper.h"
#include "commands.h"
#include <string.h>
#include <stdarg.h>


static void init_context();
static void cleanup_context();
static void cleanup_and_exit(uint8_t code);

static void parse_args(int argc, char **argv);
static jmp_flag_context_t* flag_ctxt_from(const char* flag_str);
static bool validate_cmd();

static errorc exec();


static jmp_context_t* _jctx;   

static const jmp_flag_context_t _flag_ctxts[] = {
        {ADD,   "a",  { ADD|DIR, ADD|DIR|DESCR, 0 }},
        {MOD,   "w+", { MOD|DIR, MOD|DESCR, MOD|DIR|DESCR, 0 }},
        {DEL,   "w+", { DEL, 0 }},
        {LIST,  "r",  { LIST, 0 }},
        {HELP,  NULL, { HELP, 0 }},
        {DIR,   NULL, { 0 }},
        {DESCR, NULL, { 0 }},
        {UNKNOWN, NULL, { 0 }}
};


int 
main(int argc, char **argv) {
        if (argc <= 1) {
                fprintf(stderr, "Incorrect usage of jumper. Please run 'jumper -help' for more information.\n");
                return 1;
        }

        if (argc == 2 && argv[1][0] != '-') { // early check for a jump command
                puts("Do jump!!\n");
                return 0;
        }

        init_context();

        parse_args(argc, argv);

        DEBUG_JUMPER_CONTEXT(_jctx);

        if (!validate_cmd()) {
                printf("\n [ERR] Invalid combination of arguments detected, please use the -help flag for further information.\n");
                cleanup_and_exit(0);
        }
        

        // exec(argc, argv);

        cleanup_context();
        return 0;
}


/**
 * @return the new program content structure
 */
static void
init_context() {
        _jctx = (jmp_context_t *)malloc(sizeof(jmp_context_t));
        if (_jctx == NULL) {
                fprintf(stderr, "Fatal error, failed to init jumper program context.\n");
                exit(1);
        }
        _jctx->arg_count = 0;
        _jctx->most_significant_flag = UNKNOWN;
}

static void 
cleanup_context() {
        if (_jctx == NULL)
                return;
        
        for (uint8_t i = 0; i < _jctx->arg_count; i++)
                free(_jctx->args[i]);

        free(_jctx);
}

static void
cleanup_and_exit(uint8_t code) {
        cleanup_context();
        exit(code);
}


static void
parse_args(int argc, char **argv) {
        jmp_arg_t** args = _jctx->args;
        
        uint8_t argv_ptr = 1;
        while (argv_ptr < argc) {
                jmp_arg_t* arg = (jmp_arg_t *)malloc(sizeof(jmp_arg_t));

                arg->flag = flag_ctxt_from(argv[argv_ptr++]);
                if (arg->flag == NULL || arg->flag->type == UNKNOWN) {
                        printf("\n [ERR] Parser failed to recognise flag: '%s'\n Exiting...\n\n", argv[argv_ptr-1]);
                        cleanup_and_exit(0);
                }

                if (arg->flag->type < _jctx->most_significant_flag)
                        _jctx->most_significant_flag = arg->flag->type;

                if (argv_ptr < argc && argv[argv_ptr][0] != '-')
                        arg->value = argv[argv_ptr++];

                args[_jctx->arg_count++] = arg;
        }
}


static jmp_flag_context_t*
flag_ctxt_from(const char* flag_str) {
        jmp_flag_t flag_type = flag_type_of(flag_str);
        uint8_t l = sizeof(_flag_ctxts) / sizeof(jmp_flag_context_t);

        for (uint8_t i = 0; i < l; i++) {
                jmp_flag_context_t* flag = &_flag_ctxts[i];
                if (flag->type == flag_type)
                        return flag;
        }
        return NULL;
}


static bool
validate_cmd() {
        uint8_t i;
        uint8_t flag_sum = 0;        
        for (i = 0; i < _jctx->arg_count; i++)
                flag_sum |= _jctx->args[i]->flag->type;

        const char* msf_str = flag_type_to_str(_jctx->most_significant_flag);
        jmp_flag_context_t* msf_ctxt = flag_ctxt_from(msf_str);

        uint8_t l = sizeof(msf_ctxt->combinations) / sizeof(msf_ctxt->combinations[0]);
        for (i = 0; i < l; i++) {
                if (msf_ctxt->combinations[i] == flag_sum)
                        return true;
        }

        return false;
}



static errorc 
exec() {

}

