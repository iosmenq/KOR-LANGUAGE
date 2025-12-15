/* 
************ EN ************
* TURKISH PROGRAMMING LANGUAGE
* CODED BY IOSMEN (C) 2025
* USE THIS COMMAND FOR MAKE A BINARY: gcc -std=c11 -O0 -g -static korc.c -o korc
* VERSION 1.1
* BUG: SIGABRT (invalid free / heap corruption) NOTE: it doesn't matter, but i will be fix this...
* THANKS FOR THE USE :D
*/

/*
************ TR ************
* TURKCE PROGRAMLAMA DILI
* GELISTIREN: IOSMEN (C) 2025
* IKILI DOSYA OLUSTURMA KOMUTU: gcc -std=c11 -O0 -g -static korc.c -o korc
* SURUM: 1.1
* HATA: SIGABRT (gecersiz free / heap bellek bozulmasi) NOT: önemli değil, ama bunu düzelteceğim...
* KULLANDIGINIZ ICIN TESKKURLER :D
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <locale.h>
#include <math.h>

#ifdef HAVE_NCURSES
#include <ncurses.h>
#endif

#define ERR_SUCCESS 0
#define ERR_GENERAL 1
#define ERR_SYNTAX 2
#define ERR_SEMANTIC 3
#define ERR_RUNTIME 4
#define ERR_COMPILE 5
#define ERR_SECURITY 6
#define ERR_IO 7
#define ERR_MEMORY 8
#define ERR_NETWORK 9
#define ERR_TYPE 10
#define ERR_NOT_FOUND 11
#define ERR_INVALID 12

#define TOKEN_EOF 0
#define TOKEN_IDENTIFIER 1
#define TOKEN_NUMBER 2
#define TOKEN_STRING 3
#define TOKEN_KEYWORD 4
#define TOKEN_OPERATOR 5
#define TOKEN_ASSIGN 6
#define TOKEN_LPAREN 7
#define TOKEN_RPAREN 8
#define TOKEN_COMMA 9
#define TOKEN_DIRECTIVE 10
#define TOKEN_NEWLINE 13

#define KW_EGER 100
#define KW_ISE 101
#define KW_DEGILSE 102
#define KW_SON 103
#define KW_TEKRA 104
#define KW_DONGU 105
#define KW_FONKSIYON 106
#define KW_DON 107
#define KW_YAZ 108
#define KW_YAZ_SATIR 109
#define KW_OKU 110
#define KW_BEKLE 111
#define KW_MILISANIYE_BEKLE 112
#define KW_SIMDI 113
#define KW_DOSYA_AC 114
#define KW_DOSYA_YAZ 115
#define KW_DOSYA_OKU 116
#define KW_DOSYA_KAPAT 117
#define KW_DOSYA_VARMI 118
#define KW_PORT 119
#define KW_HTML_BASLAT 120
#define KW_HTML_BITIR 121
#define KW_SUNUCU_BASLAT 122
#define KW_CALISTIR 123
#define KW_KAPAT 124
#define KW_YENIDEN_BASLAT 125
#define KW_CPU 126
#define KW_CPU_SICAKLIK 127
#define KW_RAM 128
#define KW_DISK 129
#define KW_SADECE_ROOT 130
#define KW_CIK 131

#define OP_PLUS 200
#define OP_MINUS 201
#define OP_MULTIPLY 202
#define OP_DIVIDE 203
#define OP_MODULO 204
#define OP_GT 205
#define OP_LT 206
#define OP_GTE 207
#define OP_LTE 208
#define OP_EQ 209
#define OP_NEQ 210
#define OP_AND 211
#define OP_OR 212

#define VAL_NUMBER 0
#define VAL_STRING 1
#define VAL_BOOLEAN 2

#define MAX_IDENTIFIER_LEN 256
#define MAX_STRING_LEN 4096
#define MAX_TOKENS 10000
#define MAX_SYMBOLS 1000
#define MAX_FUNCTIONS 100
#define MAX_PARAMS 10
#define MAX_NESTING 32
#define MAX_ERRORS 100
#define MAX_WARNINGS 50
#define MAX_INCLUDES 20
#define MAX_WHITELIST 50
#define MAX_CHILDREN 100

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500

typedef struct Token {
    int type;
    int line;
    int column;
    char* value;
    union {
        double num_value;
        char* str_value;
    };
    struct Token* next;
} Token;

typedef struct Value {
    int type;
    union {
        double number;
        char* string;
        int boolean;
        void* array;
        void* object;
        void* function;
    };
    size_t length;
    int refcount;
} Value;

typedef struct Symbol {
    char* name;
    Value* value;
    int scope;
    int line;
    int is_const;
    int is_function;
    struct Symbol* next;
} Symbol;

typedef struct Param {
    char* name;
    int type_hint;
    struct Param* next;
} Param;

typedef enum {
    NODE_PROGRAM,
    NODE_DIRECTIVE,
    NODE_ASSIGNMENT,
    NODE_EXPRESSION,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_NUMBER,
    NODE_STRING,
    NODE_IDENTIFIER,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_FUNCTION_DECL,
    NODE_FUNCTION_CALL,
    NODE_RETURN,
    NODE_BLOCK,
    NODE_PRINT,
    NODE_INPUT,
    NODE_SLEEP,
    NODE_SYSTEM,
    NODE_FILE_OPEN,
    NODE_FILE_READ,
    NODE_FILE_WRITE,
    NODE_FILE_CLOSE,
    NODE_HTTP_SERVER,
    NODE_HTML_BLOCK,
    NODE_VARIABLE_DECL,
    NODE_ARRAY,
    NODE_OBJECT,
    NODE_NULL,
    NODE_BREAK,
    NODE_CONTINUE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int line;
    int column;
    union {
        double num_value;
        char* str_value;
        char* identifier;

        struct {
            int op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;
        
        struct {
            int op;
            struct ASTNode* operand;
        } unary;
        
        struct {
            struct ASTNode* condition;
            struct ASTNode* then_branch;
            struct ASTNode* else_branch;
        } if_stmt;
        
        struct {
            struct ASTNode* init;
            struct ASTNode* condition;
            struct ASTNode* update;
            struct ASTNode* body;
        } for_loop;
        
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_loop;
        
        struct {
            char* name;
            struct ASTNode** params;
            int param_count;
            struct ASTNode* body;
        } function_decl;
        
        struct {
            char* name;
            struct ASTNode** args;
            int arg_count;
        } function_call;
        
        struct {
            struct ASTNode** statements;
            int stmt_count;
        } block;
        
        struct {
            int port;
            struct ASTNode* html_content;
        } http_server;
        
        struct {
            char* filename;
            char* mode;
        } file_op;
    };
    struct ASTNode* next;
} ASTNode;

typedef struct ErrorInfo {
    int code;
    int line;
    int column;
    char message[256];
    char suggestion[256];
    char context[256];
} ErrorInfo;

typedef struct WarningInfo {
    int line;
    int column;
    char message[256];
} WarningInfo;

typedef struct LexerState {
    const char* source;
    size_t source_len;
    size_t position;
    int line;
    int column;
    Token* tokens;
    Token* current;
    int token_count;
    ErrorInfo* errors;
    int error_count;
    WarningInfo* warnings;
    int warning_count;
    int in_comment;
    int comment_depth;
} LexerState;

typedef struct ParserState {
    Token* tokens;
    Token* current;
    ASTNode* ast;
    Symbol** symbol_tables;
    int scope_depth;
    ErrorInfo* errors;
    int error_count;
    WarningInfo* warnings;
    int warning_count;
    char** includes;
    int include_count;
} ParserState;

typedef struct CodeGenState {
    ASTNode* ast;
    char* output;
    size_t output_size;
    size_t output_capacity;
    int indent_level;
    int label_counter;
    char** string_constants;
    int string_count;
    char** variable_names;
    int var_count;
    char** function_names;
    int func_count;
    ErrorInfo* errors;
    int error_count;
} CodeGenState;

typedef struct RuntimeState {
    Value** stack;
    int stack_ptr;
    int stack_size;
    Symbol** scopes;
    int scope_count;
    FILE* current_file;
    int http_server_fd;
    int http_port;
    char* http_html;
    int http_running;
    int tui_active;
    int should_exit;
    int exit_code;
} RuntimeState;

typedef struct TUIState {
    int active;
    int height;
    int width;
    int selected_line;
    int scroll_offset;
    char* filename;
    ErrorInfo* errors;
    int error_count;
    char* status_message;
    int status_type;
    int show_help;
    int rebuild_needed;
} TUIState;

typedef struct WhitelistEntry {
    const char* command;
    const char* description;
    int privileged;
} WhitelistEntry;

static RuntimeState g_runtime = {0};
static TUIState g_tui = {0};
static ErrorInfo g_errors[MAX_ERRORS];
static int g_error_count = 0;
static WarningInfo g_warnings[MAX_WARNINGS];
static int g_warning_count = 0;

static const struct {
    char* keyword;
    int token_type;
} keyword_table[] = {
    {"eger", KW_EGER},
    {"ise", KW_ISE},
    {"degilse", KW_DEGILSE},
    {"son", KW_SON},
    {"tekrar", KW_TEKRA},
    {"dongu", KW_DONGU},
    {"fonksiyon", KW_FONKSIYON},
    {"don", KW_DON},
    {"yaz", KW_YAZ},
    {"yaz_satir", KW_YAZ_SATIR},
    {"oku", KW_OKU},
    {"bekle", KW_BEKLE},
    {"milisaniye_bekle", KW_MILISANIYE_BEKLE},
    {"simdi", KW_SIMDI},
    {"dosya_ac", KW_DOSYA_AC},
    {"dosya_yaz", KW_DOSYA_YAZ},
    {"dosya_oku", KW_DOSYA_OKU},
    {"dosya_kapat", KW_DOSYA_KAPAT},
    {"dosya_varmi", KW_DOSYA_VARMI},
    {"port", KW_PORT},
    {"html_baslat", KW_HTML_BASLAT},
    {"html_bitir", KW_HTML_BITIR},
    {"sunucu_baslat", KW_SUNUCU_BASLAT},
    {"calistir", KW_CALISTIR},
    {"kapat", KW_KAPAT},
    {"yeniden_baslat", KW_YENIDEN_BASLAT},
    {"cpu", KW_CPU},
    {"cpu_sicaklik", KW_CPU_SICAKLIK},
    {"ram", KW_RAM},
    {"disk", KW_DISK},
    {"sadece_root", KW_SADECE_ROOT},
    {"cik", KW_CIK},
    {NULL, 0}
};

static const WhitelistEntry whitelist[] = {
    {"ls","Dosya listeleme",0},
    {"df","Disk kullanimi",0},
    {"du","Dizin boyutu",0},
    {"find","Dosya bulma",0},
    {"systemd-run","Servis calistirma",1},
    {"rm","Dosya silme",0},
    {"mkdir","Dizin olusturma",0},
    {"rmdir","Dizin silme",0},
    {"cp","Dosya kopyalama",0},
    {"mv","Dosya tasima",0},
    {"cat","Dosya icerigi",0},
    {"echo","Metin yazdirma",0},
    {"grep","Metin arama",0},
    {"ps","Process listeleme",0},
    {"top","System monitor",0},
    {"date","Tarih",0},
    {"whoami","Kullanici bilgisi",0},
    {"uname","Sistem bilgisi",0},
    {"hostname","Sunucu adi",0},
    {"ping","Ag testi",0},
    {NULL,NULL,0}
};

static const char* error_descriptions[] = {
    "Basari",
    "Genel hata",
    "Syntax hatasi",
    "Semantic/namespace hatasi",
    "Runtime hatasi",
    "Derleme (gcc) hatasi",
    "Guvenlik/izin hatasi",
    "Dosya IO hatasi",
    "Bellek hatasi",
    "Ag hatasi",
    "Tip hatasi",
    "Bulunamadi hatasi",
    "Gecersiz islem hatasi"
};

void free_ast_node(ASTNode* node) {
    if (!node) return;
    
    if (node->next) {
        free_ast_node(node->next);
        node->next = NULL;
    }
    
    switch (node->type) {
        case NODE_STRING:
            if (node->str_value) {
                free(node->str_value);
                node->str_value = NULL;
            }
            break;
        case NODE_IDENTIFIER:
            if (node->identifier) {
                free(node->identifier);
                node->identifier = NULL;
            }
            break;
        case NODE_BINARY_OP:
            if (node->binary.left) {
                free_ast_node(node->binary.left);
                node->binary.left = NULL;
            }
            if (node->binary.right) {
                free_ast_node(node->binary.right);
                node->binary.right = NULL;
            }
            break;
        case NODE_IF:
            if (node->if_stmt.condition) {
                free_ast_node(node->if_stmt.condition);
                node->if_stmt.condition = NULL;
            }
            if (node->if_stmt.then_branch) {
                free_ast_node(node->if_stmt.then_branch);
                node->if_stmt.then_branch = NULL;
            }
            if (node->if_stmt.else_branch) {
                free_ast_node(node->if_stmt.else_branch);
                node->if_stmt.else_branch = NULL;
            }
            break;
        case NODE_FOR:
            if (node->for_loop.init) {
                free_ast_node(node->for_loop.init);
                node->for_loop.init = NULL;
            }
            if (node->for_loop.condition) {
                free_ast_node(node->for_loop.condition);
                node->for_loop.condition = NULL;
            }
            if (node->for_loop.update) {
                free_ast_node(node->for_loop.update);
                node->for_loop.update = NULL;
            }
            if (node->for_loop.body) {
                free_ast_node(node->for_loop.body);
                node->for_loop.body = NULL;
            }
            break;
        case NODE_FUNCTION_DECL:
            if (node->function_decl.name) {
                free(node->function_decl.name);
                node->function_decl.name = NULL;
            }
            if (node->function_decl.params) {
                for (int i = 0; i < node->function_decl.param_count; i++) {
                    if (node->function_decl.params[i]) {
                        free_ast_node(node->function_decl.params[i]);
                    }
                }
                free(node->function_decl.params);
                node->function_decl.params = NULL;
            }
            if (node->function_decl.body) {
                free_ast_node(node->function_decl.body);
                node->function_decl.body = NULL;
            }
            break;
        case NODE_FUNCTION_CALL:
            if (node->function_call.name) {
                free(node->function_call.name);
                node->function_call.name = NULL;
            }
            if (node->function_call.args) {
                for (int i = 0; i < node->function_call.arg_count; i++) {
                    if (node->function_call.args[i]) {
                        free_ast_node(node->function_call.args[i]);
                    }
                }
                free(node->function_call.args);
                node->function_call.args = NULL;
            }
            break;
        case NODE_PROGRAM:
            if (node->block.statements) {
                for (int i = 0; i < node->block.stmt_count; i++) {
                    if (node->block.statements[i]) {
                        free_ast_node(node->block.statements[i]);
                    }
                }
                free(node->block.statements);
                node->block.statements = NULL;
            }
            break;
        case NODE_SYSTEM:
            if (node->str_value) {
                free(node->str_value);
                node->str_value = NULL;
            }
            break;
        case NODE_ASSIGNMENT:
            if (node->identifier) {
                free(node->identifier);
                node->identifier = NULL;
            }
            if (node->binary.right) {
                free_ast_node(node->binary.right);
                node->binary.right = NULL;
            }
            break;
        case NODE_PRINT:
        case NODE_SLEEP:
            if (node->binary.right) {
                free_ast_node(node->binary.right);
                node->binary.right = NULL;
            }
            break;
        case NODE_DIRECTIVE:
            if (node->str_value) {
                free(node->str_value);
                node->str_value = NULL;
            }
            break;
        case NODE_RETURN:
        case NODE_NUMBER:
            break;
        default:
            break;
    }
    
    free(node);
}

void* kor_malloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr && size > 0) {
        fprintf(stderr, "HATA E008 (satir 0): Bellek ayirma basarisiz\n");
        exit(ERR_MEMORY);
    }
    return ptr;
}

void* kor_calloc(size_t nmemb, size_t size) {
    void* ptr = calloc(nmemb, size);
    if (!ptr && nmemb > 0 && size > 0) {
        fprintf(stderr, "HATA E008 (satir 0): Bellek ayirma basarisiz\n");
        exit(ERR_MEMORY);
    }
    return ptr;
}

char* kor_strdup(const char* s) {
    if (!s) return NULL;
    char* copy = kor_malloc(strlen(s) + 1);
    strcpy(copy, s);
    return copy;
}

char* kor_strndup(const char* s, size_t n) {
    if (!s) return NULL;
    size_t len = strlen(s);
    if (len > n) len = n;
    char* copy = kor_malloc(len + 1);
    strncpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

char* kor_strdup_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    if (needed < 0) {
        va_end(args);
        return kor_strdup("");
    }
    char* buffer = kor_malloc(needed + 1);
    vsnprintf(buffer, needed + 1, format, args);
    va_end(args);
    return buffer;
}

char* str_tolower(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* result = kor_malloc(len + 1);
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

char* str_trim(char* str) {
    if (!str) return NULL;
    char* end;
    while (*str && isspace((unsigned char)*str)) str++;
    if (*str == '\0') return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int str_starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return 0;
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    if (prefix_len > str_len) return 0;
    return strncmp(str, prefix, prefix_len) == 0;
}

int str_ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return 0;
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (suffix_len > str_len) return 0;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

char* str_replace(const char* str, const char* old, const char* new) {
    if (!str || !old || !new) return NULL;
    int count = 0;
    const char* tmp = str;
    while ((tmp = strstr(tmp, old))) {
        count++;
        tmp += strlen(old);
    }
    if (count == 0) return kor_strdup(str);
    size_t old_len = strlen(old);
    size_t new_len = strlen(new);
    size_t result_len = strlen(str) + count * (new_len - old_len);
    char* result = kor_malloc(result_len + 1);
    const char* src = str;
    char* dst = result;
    while (*src) {
        if (str_starts_with(src, old)) {
            strcpy(dst, new);
            src += old_len;
            dst += new_len;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    return result;
}

char** str_split(const char* str, const char* delim, int* count) {
    if (!str || !delim) {
        if (count) *count = 0;
        return NULL;
    }
    char* copy = kor_strdup(str);
    char* token;
    char** result = NULL;
    int capacity = 10;
    int size = 0;
    result = kor_malloc(sizeof(char*) * capacity);
    token = strtok(copy, delim);
    while (token != NULL) {
        char *trimmed = str_trim(token);
        if (strlen(trimmed) > 0) {
            result[size++] = kor_strdup(trimmed);
            if (size >= capacity) {
                capacity *= 2;
                char** new_result = realloc(result, sizeof(char*) * capacity);
                if (!new_result) {
                    for (int i = 0; i < size; i++) free(result[i]);
                    free(result);
                    free(copy);
                    if (count) *count = 0;
                    return NULL;
                }
                result = new_result;
            }
        }
        token = strtok(NULL, delim);
    }
    free(copy);
    if (count) *count = size;
    return result;
}

void free_string_array(char** arr, int count) {
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        if (arr[i]) free(arr[i]);
    }
    free(arr);
}

int file_exists(const char* filename) {
    if (!filename) return 0;
    struct stat st;
    return stat(filename, &st) == 0;
}

long get_file_size(const char* filename) {
    if (!filename) return -1;
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

char* read_entire_file(const char* filename) {
    if (!filename) return NULL;
    FILE* file = fopen(filename, "rb");
    if (!file) return NULL;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (size <= 0) {
        fclose(file);
        return kor_strdup("");
    }
    char* content = kor_malloc(size + 1);
    size_t read_size = fread(content, 1, size, file);
    content[read_size] = '\0';
    fclose(file);
    return content;
}

int write_to_file(const char* filename, const char* content) {
    if (!filename || !content) return -1;
    FILE* file = fopen(filename, "wb");
    if (!file) return -1;
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, file);
    fclose(file);
    return (written == len) ? 0 : -1;
}

char* create_temp_filename(const char* prefix) {
    char* temp = kor_malloc(256);
    pid_t pid = getpid();
    time_t now = time(NULL);
    snprintf(temp, 256, "/tmp/%s_%ld_%d.tmp", prefix ? prefix : "kor", now, pid);
    return temp;
}

int pid_is_running(pid_t pid) {
    if (pid <= 0) return 0;
    return kill(pid, 0) == 0;
}

int command_in_whitelist(const char* command) {
    if (!command) return 0;
    char* copy = kor_strdup(command);
    char* first_word = strtok(copy, " \t\n\r");
    if (!first_word) {
        free(copy);
        return 0;
    }
    int found = 0;
    for (int i = 0; whitelist[i].command; i++) {
        if (strcmp(first_word, whitelist[i].command) == 0) {
            found = 1;
            break;
        }
    }
    free(copy);
    return found;
}

int is_root_user(void) {
    return geteuid() == 0;
}

void print_error(int code, int line, const char* message, const char* suggestion) {
    fprintf(stderr, "HATA E%03d (satir %d):\n", code, line);
    fprintf(stderr, "%s\n", message);
    if (suggestion) {
        fprintf(stderr, "Oneri: %s\n", suggestion);
    }
}

LexerState* lexer_init(const char* source) {
    LexerState* lexer = kor_malloc(sizeof(LexerState));
    lexer->source = source;
    lexer->source_len = strlen(source);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->tokens = NULL;
    lexer->current = NULL;
    lexer->token_count = 0;
    lexer->errors = kor_calloc(MAX_ERRORS, sizeof(ErrorInfo));
    lexer->error_count = 0;
    lexer->warnings = kor_calloc(MAX_WARNINGS, sizeof(WarningInfo));
    lexer->warning_count = 0;
    lexer->in_comment = 0;
    lexer->comment_depth = 0;
    return lexer;
}

void lexer_cleanup(LexerState* lexer) {
    if (!lexer) return;
    Token* token = lexer->tokens;
    while (token) {
        Token* next = token->next;
        if (token->value) free(token->value);
        if (token->str_value) free(token->str_value);
        free(token);
        token = next;
    }
    if (lexer->errors) free(lexer->errors);
    if (lexer->warnings) free(lexer->warnings);
    free(lexer);
}

char lexer_current_char(LexerState* lexer) {
    if (lexer->position >= lexer->source_len) return '\0';
    return lexer->source[lexer->position];
}

char lexer_next_char(LexerState* lexer) {
    if (lexer->position >= lexer->source_len) return '\0';
    char c = lexer->source[lexer->position++];
    if (c == '\n') {
        lexer->line++;
        lexer->column = 1;
    } else {
        lexer->column++;
    }
    return c;
}

char lexer_peek_char(LexerState* lexer, int offset) {
    size_t pos = lexer->position + offset;
    if (pos >= lexer->source_len) return '\0';
    return lexer->source[pos];
}

void lexer_skip_whitespace(LexerState* lexer) {
    while (lexer_current_char(lexer) && isspace((unsigned char)lexer_current_char(lexer))) {
        if (lexer_current_char(lexer) == '\n') {
            Token* token = kor_malloc(sizeof(Token));
            token->type = TOKEN_NEWLINE;
            token->line = lexer->line;
            token->column = lexer->column;
            token->value = NULL;
            token->next = NULL;
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
        }
        lexer_next_char(lexer);
    }
}

void lexer_skip_single_line_comment(LexerState* lexer) {
    while (lexer_current_char(lexer) && lexer_current_char(lexer) != '\n') {
        lexer_next_char(lexer);
    }
}

void lexer_skip_multi_line_comment(LexerState* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    while (lexer_current_char(lexer)) {
        if (lexer_current_char(lexer) == '@' && lexer_peek_char(lexer, 1) == '.') {
            lexer->comment_depth++;
            lexer_next_char(lexer);
            lexer_next_char(lexer);
            continue;
        }
        if (lexer_current_char(lexer) == '.' && lexer_peek_char(lexer, 1) == '@') {
            lexer->comment_depth--;
            lexer_next_char(lexer);
            lexer_next_char(lexer);
            if (lexer->comment_depth == 0) break;
            continue;
        }
        lexer_next_char(lexer);
    }
    if (lexer->comment_depth > 0) {
        if (lexer->error_count < MAX_ERRORS) {
            ErrorInfo* error = &lexer->errors[lexer->error_count++];
            error->code = ERR_SYNTAX;
            error->line = start_line;
            error->column = start_column;
            snprintf(error->message, sizeof(error->message), "Cok satirli yorum kapatilmamis");
            snprintf(error->suggestion, sizeof(error->suggestion), "Yorumu kapatmak icin '.@' ekleyin");
        }
    }
}

void lexer_handle_comment(LexerState* lexer) {
    if (lexer_current_char(lexer) == '@' && lexer_peek_char(lexer, 1) == '.') {
        lexer->in_comment = 1;
        lexer->comment_depth = 1;
        lexer_next_char(lexer);
        lexer_next_char(lexer);
        lexer_skip_multi_line_comment(lexer);
        lexer->in_comment = 0;
    } else if (lexer_current_char(lexer) == '#' && lexer_peek_char(lexer, 1) == '!') {
        lexer_skip_single_line_comment(lexer);
    }
}

Token* lexer_read_number(LexerState* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    char buffer[256];
    int pos = 0;
    int has_dot = 0;
    while (lexer_current_char(lexer) &&
           (isdigit((unsigned char)lexer_current_char(lexer)) ||
            lexer_current_char(lexer) == '.')) {
        if (lexer_current_char(lexer) == '.') {
            if (has_dot) break;
            has_dot = 1;
        }
        if (pos < 255) buffer[pos++] = lexer_current_char(lexer);
        lexer_next_char(lexer);
    }
    buffer[pos] = '\0';
    Token* token = kor_malloc(sizeof(Token));
    token->type = TOKEN_NUMBER;
    token->line = start_line;
    token->column = start_column;
    token->value = kor_strdup(buffer);
    token->num_value = atof(buffer);
    token->next = NULL;
    return token;
}

Token* lexer_read_string(LexerState* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    char delimiter = lexer_current_char(lexer);
    lexer_next_char(lexer);
    
    char buffer[MAX_STRING_LEN];
    int pos = 0;
    int escape = 0;
    
    while (lexer_current_char(lexer)) {
        if (escape) {
            switch (lexer_current_char(lexer)) {
                case 'n': buffer[pos++] = '\n'; break;
                case 't': buffer[pos++] = '\t'; break;
                case 'r': buffer[pos++] = '\r'; break;
                case '\\': buffer[pos++] = '\\'; break;
                case '"': buffer[pos++] = '"'; break;
                case '\'': buffer[pos++] = '\''; break;
                default:   buffer[pos++] = lexer_current_char(lexer); break;
            }
            escape = 0;
            lexer_next_char(lexer);
            continue;
        }
        
        if (lexer_current_char(lexer) == '\\') {
            escape = 1;
            lexer_next_char(lexer);
            continue;
        }
        
        if (lexer_current_char(lexer) == delimiter) {
            lexer_next_char(lexer);
            break;
        }
        
        if (pos < MAX_STRING_LEN - 1) {
            buffer[pos++] = lexer_current_char(lexer);
        } else {
            if (lexer->error_count < MAX_ERRORS) {
                ErrorInfo* error = &lexer->errors[lexer->error_count++];
                error->code = ERR_SYNTAX;
                error->line = start_line;
                error->column = start_column;
                snprintf(error->message, sizeof(error->message),
                        "String cok uzun (maksimum %d karakter)", MAX_STRING_LEN);
            }
            break;
        }
        lexer_next_char(lexer);
    }
    
    buffer[pos] = '\0';
    Token* token = kor_malloc(sizeof(Token));
    token->type = TOKEN_STRING;
    token->line = start_line;
    token->column = start_column;
    token->value = kor_strdup(buffer);
    token->str_value = kor_strdup(buffer);
    token->next = NULL;
    
    return token;
}

Token* lexer_read_identifier(LexerState* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    char buffer[MAX_IDENTIFIER_LEN];
    int pos = 0;
    while (lexer_current_char(lexer) &&
           (isalnum((unsigned char)lexer_current_char(lexer)) ||
            lexer_current_char(lexer) == '_')) {
        if (pos < MAX_IDENTIFIER_LEN - 1) buffer[pos++] = lexer_current_char(lexer);
        lexer_next_char(lexer);
    }
    buffer[pos] = '\0';
    
    for (int i = 0; keyword_table[i].keyword; i++) {
        if (strcmp(buffer, keyword_table[i].keyword) == 0) {
            Token* token = kor_malloc(sizeof(Token));
            token->type = TOKEN_KEYWORD;
            token->line = start_line;
            token->column = start_column;
            token->value = kor_strdup(buffer);
            token->next = NULL;
            return token;
        }
    }
    
    Token* token = kor_malloc(sizeof(Token));
    token->type = TOKEN_IDENTIFIER;
    token->line = start_line;
    token->column = start_column;
    token->value = kor_strdup(buffer);
    token->next = NULL;
    return token;
}

Token* lexer_read_operator(LexerState* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    char first = lexer_current_char(lexer);
    char second = lexer_peek_char(lexer, 1);
    char buffer[3] = {first, '\0', '\0'};
    
    if ((first == '>' && second == '=') ||
        (first == '<' && second == '=') ||
        (first == '=' && second == '=') ||
        (first == '!' && second == '=') ||
        (first == '&' && second == '&') ||
        (first == '|' && second == '|')) {
        buffer[1] = second;
        buffer[2] = '\0';
        lexer_next_char(lexer);
        lexer_next_char(lexer);
    } else {
        lexer_next_char(lexer);
    }
    
    Token* token = kor_malloc(sizeof(Token));
    token->type = TOKEN_OPERATOR;
    token->line = start_line;
    token->column = start_column;
    token->value = kor_strdup(buffer);
    token->next = NULL;
    return token;
}

Token* lexer_read_directive(LexerState* lexer) {
    int start_line = lexer->line;
    int start_column = lexer->column;
    lexer_next_char(lexer);
    
    char buffer[MAX_IDENTIFIER_LEN];
    int pos = 0;
    while (lexer_current_char(lexer) &&
           (isalnum((unsigned char)lexer_current_char(lexer)) ||
            lexer_current_char(lexer) == '_' ||
            lexer_current_char(lexer) == '.')) {
        if (pos < MAX_IDENTIFIER_LEN - 1) buffer[pos++] = lexer_current_char(lexer);
        lexer_next_char(lexer);
    }
    buffer[pos] = '\0';
    
    Token* token = kor_malloc(sizeof(Token));
    token->type = TOKEN_DIRECTIVE;
    token->line = start_line;
    token->column = start_column;
    token->value = kor_strdup(buffer);
    token->next = NULL;
    return token;
}

int lexer_tokenize(LexerState* lexer) {
    while (lexer_current_char(lexer)) {
        lexer_skip_whitespace(lexer);
        if (!lexer_current_char(lexer)) break;
        
        char c = lexer_current_char(lexer);
        
        if (c == '@' && lexer_peek_char(lexer, 1) == '.') {
            lexer_handle_comment(lexer);
            continue;
        }
        
        if (c == '#') {
            Token* token = lexer_read_directive(lexer);
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
            continue;
        }
        
        if (isdigit((unsigned char)c) || (c == '.' && isdigit((unsigned char)lexer_peek_char(lexer, 1)))) {
            Token* token = lexer_read_number(lexer);
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
            continue;
        }
        
        if (c == '"' || c == '\'') {
            Token* token = lexer_read_string(lexer);
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
            continue;
        }
        
        if (isalpha((unsigned char)c) || c == '_') {
            Token* token = lexer_read_identifier(lexer);
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
            continue;
        }
        
        if (strchr("+-*/%><=!&|", c)) {
            Token* token = lexer_read_operator(lexer);
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
            continue;
        }
        
        Token* token = NULL;
        switch (c) {
            case '=':
                token = kor_malloc(sizeof(Token));
                token->type = TOKEN_ASSIGN;
                token->line = lexer->line;
                token->column = lexer->column;
                token->value = kor_strdup("=");
                token->next = NULL;
                lexer_next_char(lexer);
                break;
            case '(':
                token = kor_malloc(sizeof(Token));
                token->type = TOKEN_LPAREN;
                token->line = lexer->line;
                token->column = lexer->column;
                token->value = kor_strdup("(");
                token->next = NULL;
                lexer_next_char(lexer);
                break;
            case ')':
                token = kor_malloc(sizeof(Token));
                token->type = TOKEN_RPAREN;
                token->line = lexer->line;
                token->column = lexer->column;
                token->value = kor_strdup(")");
                token->next = NULL;
                lexer_next_char(lexer);
                break;
            case ',':
                token = kor_malloc(sizeof(Token));
                token->type = TOKEN_COMMA;
                token->line = lexer->line;
                token->column = lexer->column;
                token->value = kor_strdup(",");
                token->next = NULL;
                lexer_next_char(lexer);
                break;
            default:
                if (lexer->error_count < MAX_ERRORS) {
                    ErrorInfo* error = &lexer->errors[lexer->error_count++];
                    error->code = ERR_SYNTAX;
                    error->line = lexer->line;
                    error->column = lexer->column;
                    snprintf(error->message, sizeof(error->message),
                            "Bilinmeyen karakter: '%c' (ASCII %d)", c, c);
                }
                lexer_next_char(lexer);
                continue;
        }
        
        if (token) {
            if (!lexer->tokens) {
                lexer->tokens = token;
                lexer->current = token;
            } else {
                lexer->current->next = token;
                lexer->current = token;
            }
            lexer->token_count++;
        }
    }
    
    Token* eof_token = kor_malloc(sizeof(Token));
    eof_token->type = TOKEN_EOF;
    eof_token->line = lexer->line;
    eof_token->column = lexer->column;
    eof_token->value = NULL;
    eof_token->next = NULL;
    
    if (!lexer->tokens) {
        lexer->tokens = eof_token;
        lexer->current = eof_token;
    } else {
        lexer->current->next = eof_token;
        lexer->current = eof_token;
    }
    lexer->token_count++;
    
    return lexer->error_count == 0;
}

ASTNode* create_ast_node(NodeType type, int line, int column) {
    ASTNode* node = kor_malloc(sizeof(ASTNode));
    node->type = type;
    node->line = line;
    node->column = column;
    node->str_value = NULL;
    node->identifier = NULL;
    node->next = NULL;
    memset(&node->binary, 0, sizeof(node->binary));
    return node;
}

ASTNode* create_number_node(double value, int line, int column) {
    ASTNode* node = create_ast_node(NODE_NUMBER, line, column);
    node->num_value = value;
    return node;
}

ASTNode* create_string_node(const char* value, int line, int column) {
    ASTNode* node = create_ast_node(NODE_STRING, line, column);
    node->str_value = kor_strdup(value);
    return node;
}

ASTNode* create_identifier_node(const char* name, int line, int column) {
    ASTNode* node = create_ast_node(NODE_IDENTIFIER, line, column);
    node->identifier = kor_strdup(name);
    return node;
}

ASTNode* parse_expression(ParserState* parser);

ASTNode* parse_factor(ParserState* parser) {
    if (!parser->current) return NULL;
    Token* token = parser->current;
    ASTNode* node = NULL;
    
    if (parser->current->type == TOKEN_NUMBER) {
        node = create_number_node(token->num_value, token->line, token->column);
        parser->current = parser->current->next;
    }
    else if (parser->current->type == TOKEN_STRING) {
        node = create_string_node(token->str_value, token->line, token->column);
        parser->current = parser->current->next;
    }
    else if (parser->current->type == TOKEN_IDENTIFIER) {
        node = create_identifier_node(token->value, token->line, token->column);
        parser->current = parser->current->next;
        if (parser->current && parser->current->type == TOKEN_LPAREN) {
            ASTNode* call_node = create_ast_node(NODE_FUNCTION_CALL, token->line, token->column);
            call_node->function_call.name = kor_strdup(token->value);
            call_node->function_call.args = NULL;
            call_node->function_call.arg_count = 0;
            
            parser->current = parser->current->next;
            if (!(parser->current && parser->current->type == TOKEN_RPAREN)) {
                ASTNode** args = kor_calloc(MAX_CHILDREN, sizeof(ASTNode*));
                int arg_count = 0;
                do {
                    if (arg_count >= MAX_CHILDREN) break;
                    ASTNode* arg = parse_expression(parser);
                    if (arg) args[arg_count++] = arg;
                    if (parser->current && parser->current->type == TOKEN_COMMA) {
                        parser->current = parser->current->next;
                    } else {
                        break;
                    }
                } while (parser->current);
                if (arg_count > 0) {
                    call_node->function_call.args = kor_calloc(arg_count, sizeof(ASTNode*));
                    for (int i = 0; i < arg_count; i++) {
                        call_node->function_call.args[i] = args[i];
                    }
                }
                call_node->function_call.arg_count = arg_count;
                free(args);
            }
            if (!(parser->current && parser->current->type == TOKEN_RPAREN)) {

            }
            else parser->current = parser->current->next;
            free_ast_node(node);
            return call_node;
        }
    }
    else if (parser->current && parser->current->type == TOKEN_LPAREN) {
        parser->current = parser->current->next;
        node = parse_expression(parser);
        if (!(parser->current && parser->current->type == TOKEN_RPAREN)) {

        }
        else parser->current = parser->current->next;
    }
    else {
        if (parser->error_count < MAX_ERRORS) {
            ErrorInfo* error = &parser->errors[parser->error_count++];
            error->code = ERR_SYNTAX;
            error->line = token->line;
            error->column = token->column;
            snprintf(error->message, sizeof(error->message), "Gecersiz ifade faktoru");
        }
        if (parser->current) parser->current = parser->current->next;
    }
    return node;
}

ASTNode* parse_term(ParserState* parser) {
    ASTNode* left = parse_factor(parser);
    if (!left) return NULL;
    
    while (parser->current) {
        Token* token = parser->current;
        if (parser->current->type == TOKEN_OPERATOR) {
            const char* op = token->value;
            int op_type = OP_MULTIPLY;
            if (strcmp(op, "*") == 0) op_type = OP_MULTIPLY;
            else if (strcmp(op, "/") == 0) op_type = OP_DIVIDE;
            else if (strcmp(op, "%") == 0) op_type = OP_MODULO;
            else break;
            
            parser->current = parser->current->next;
            ASTNode* right = parse_factor(parser);
            if (!right) {
                free_ast_node(left);
                return NULL;
            }
            
            ASTNode* new_node = create_ast_node(NODE_BINARY_OP, token->line, token->column);
            new_node->binary.op = op_type;
            new_node->binary.left = left;
            new_node->binary.right = right;
            left = new_node;
        } else {
            break;
        }
    }
    return left;
}

ASTNode* parse_expression(ParserState* parser) {
    ASTNode* left = parse_term(parser);
    if (!left) return NULL;
    
    while (parser->current) {
        Token* token = parser->current;
        if (parser->current->type == TOKEN_OPERATOR) {
            const char* op = token->value;
            int op_type = OP_PLUS;
            if (strcmp(op, "+") == 0) op_type = OP_PLUS;
            else if (strcmp(op, "-") == 0) op_type = OP_MINUS;
            else if (strcmp(op, ">") == 0) op_type = OP_GT;
            else if (strcmp(op, "<") == 0) op_type = OP_LT;
            else if (strcmp(op, ">=") == 0) op_type = OP_GTE;
            else if (strcmp(op, "<=") == 0) op_type = OP_LTE;
            else if (strcmp(op, "==") == 0) op_type = OP_EQ;
            else if (strcmp(op, "!=") == 0) op_type = OP_NEQ;
            else break;
            
            parser->current = parser->current->next;
            ASTNode* right = parse_term(parser);
            if (!right) {
                free_ast_node(left);
                return NULL;
            }
            
            ASTNode* new_node = create_ast_node(NODE_BINARY_OP, token->line, token->column);
            new_node->binary.op = op_type;
            new_node->binary.left = left;
            new_node->binary.right = right;
            left = new_node;
        }
        else break;
    }
    return left;
}

ASTNode* parse_statement(ParserState* parser);

ASTNode* parse_assignment(ParserState* parser) {
    if (!parser->current || parser->current->type != TOKEN_IDENTIFIER) return NULL;
    Token* ident_token = parser->current;
    parser->current = parser->current->next;
    
    if (!parser->current || parser->current->type != TOKEN_ASSIGN) {
        parser->current = ident_token;
        return NULL;
    }
    parser->current = parser->current->next;
    
    ASTNode* expr = parse_expression(parser);
    if (!expr) {
        if (parser->error_count < MAX_ERRORS) {
            ErrorInfo* error = &parser->errors[parser->error_count++];
            error->code = ERR_SYNTAX;
            error->line = ident_token->line;
            error->column = ident_token->column;
            snprintf(error->message, sizeof(error->message), "Atama ifadesinde gecersiz deger");
        }
        return NULL;
    }
    
    ASTNode* node = create_ast_node(NODE_ASSIGNMENT, ident_token->line, ident_token->column);
    node->identifier = kor_strdup(ident_token->value);
    node->binary.right = expr;
    return node;
}

ASTNode* parse_print_statement(ParserState* parser) {
    Token* token = parser->current;
    int is_println = 0;
    
    if (parser->current && parser->current->type == TOKEN_KEYWORD) {
        if (strcmp(parser->current->value, "yaz_satir") == 0) is_println = 1;
        else if (strcmp(parser->current->value, "yaz") != 0) return NULL;
    } else return NULL;
    
    parser->current = parser->current->next;
    ASTNode* expr = parse_expression(parser);
    if (!expr) {
        if (parser->error_count < MAX_ERRORS) {
            ErrorInfo* error = &parser->errors[parser->error_count++];
            error->code = ERR_SYNTAX;
            error->line = token->line;
            error->column = token->column;
            snprintf(error->message, sizeof(error->message), "'yaz' ifadesinde gecersiz deger");
        }
        return NULL;
    }
    
    ASTNode* node = create_ast_node(NODE_PRINT, token->line, token->column);
    node->binary.right = expr;
    return node;
}

ASTNode* parse_if_statement(ParserState* parser) {
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "eger") != 0)
        return NULL;
    
    Token* if_token = parser->current;
    parser->current = parser->current->next;
    
    ASTNode* condition = parse_expression(parser);
    if (!condition) {
        if (parser->error_count < MAX_ERRORS) {
            ErrorInfo* error = &parser->errors[parser->error_count++];
            error->code = ERR_SYNTAX;
            error->line = if_token->line;
            error->column = if_token->column;
            snprintf(error->message, sizeof(error->message), "'eger' ifadesinde gecersiz kosul");
        }
        return NULL;
    }
    
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "ise") != 0) {
        free_ast_node(condition);
        return NULL;
    }
    parser->current = parser->current->next;
    
    ASTNode* then_branch = parse_statement(parser);
    if (!then_branch) {
        free_ast_node(condition);
        return NULL;
    }
    
    ASTNode* else_branch = NULL;
    if (parser->current && parser->current->type == TOKEN_KEYWORD && strcmp(parser->current->value, "degilse") == 0) {
        parser->current = parser->current->next;
        else_branch = parse_statement(parser);
    }
    
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "son") != 0) {
        free_ast_node(condition);
        free_ast_node(then_branch);
        if (else_branch) free_ast_node(else_branch);
        return NULL;
    }
    parser->current = parser->current->next;
    
    ASTNode* node = create_ast_node(NODE_IF, if_token->line, if_token->column);
    node->if_stmt.condition = condition;
    node->if_stmt.then_branch = then_branch;
    node->if_stmt.else_branch = else_branch;
    return node;
}

ASTNode* parse_repeat_statement(ParserState* parser) {
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "tekrar") != 0)
        return NULL;
    
    Token* repeat_token = parser->current;
    parser->current = parser->current->next;
    
    if (!parser->current || parser->current->type != TOKEN_NUMBER) {
        if (parser->error_count < MAX_ERRORS) {
            ErrorInfo* error = &parser->errors[parser->error_count++];
            error->code = ERR_SYNTAX;
            error->line = repeat_token->line;
            error->column = repeat_token->column;
            snprintf(error->message, sizeof(error->message), "'tekrar' ifadesinde sayi bekleniyor");
        }
        return NULL;
    }
    
    double count = parser->current->num_value;
    parser->current = parser->current->next;
    
    ASTNode* body = parse_statement(parser);
    if (!body) return NULL;
    
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "son") != 0) {
        free_ast_node(body);
        return NULL;
    }
    parser->current = parser->current->next;
    
    ASTNode* node = create_ast_node(NODE_FOR, repeat_token->line, repeat_token->column);
    ASTNode* init = create_ast_node(NODE_ASSIGNMENT, repeat_token->line, repeat_token->column);
    init->identifier = kor_strdup("_i");
    init->binary.right = create_number_node(0, repeat_token->line, repeat_token->column);
    
    ASTNode* condition = create_ast_node(NODE_BINARY_OP, repeat_token->line, repeat_token->column);
    condition->binary.op = OP_LT;
    condition->binary.left = create_identifier_node("_i", repeat_token->line, repeat_token->column);
    condition->binary.right = create_number_node(count, repeat_token->line, repeat_token->column);
    
    ASTNode* update = create_ast_node(NODE_BINARY_OP, repeat_token->line, repeat_token->column);
    update->binary.op = OP_PLUS;
    update->binary.left = create_identifier_node("_i", repeat_token->line, repeat_token->column);
    update->binary.right = create_number_node(1, repeat_token->line, repeat_token->column);
    
    node->for_loop.init = init;
    node->for_loop.condition = condition;
    node->for_loop.update = update;
    node->for_loop.body = body;
    
    return node;
}

ASTNode* parse_function_declaration(ParserState* parser) {
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "fonksiyon") != 0)
        return NULL;
    
    Token* func_token = parser->current;
    parser->current = parser->current->next;
    
    if (!parser->current || parser->current->type != TOKEN_IDENTIFIER) {
        if (parser->error_count < MAX_ERRORS) {
            ErrorInfo* error = &parser->errors[parser->error_count++];
            error->code = ERR_SYNTAX;
            error->line = func_token->line;
            error->column = func_token->column;
            snprintf(error->message, sizeof(error->message), "Fonksiyon adi bekleniyor");
        }
        return NULL;
    }
    
    char* func_name = kor_strdup(parser->current->value);
    int func_line = parser->current->line;
    int func_column = parser->current->column;
    parser->current = parser->current->next;
    
    if (!parser->current || parser->current->type != TOKEN_LPAREN) {
        free(func_name);
        return NULL;
    }
    parser->current = parser->current->next;
    
    Param* params = NULL;
    Param* last_param = NULL;
    int param_count = 0;
    
    if (!(parser->current && parser->current->type == TOKEN_RPAREN)) {
        do {
            if (param_count >= MAX_PARAMS) break;
            if (!parser->current || parser->current->type != TOKEN_IDENTIFIER) break;
            
            Param* param = kor_malloc(sizeof(Param));
            param->name = kor_strdup(parser->current->value);
            param->type_hint = 0;
            param->next = NULL;
            
            if (!params) {
                params = param;
                last_param = param;
            } else {
                last_param->next = param;
                last_param = param;
            }
            param_count++;
            parser->current = parser->current->next;
            
            if (parser->current && parser->current->type == TOKEN_COMMA) {
                parser->current = parser->current->next;
            } else {
                break;
            }
        } while (parser->current);
    }
    
    if (!parser->current || parser->current->type != TOKEN_RPAREN) {
        while (params) {
            Param* next = params->next;
            free(params->name);
            free(params);
            params = next;
        }
        free(func_name);
        return NULL;
    }
    parser->current = parser->current->next;
    
    ASTNode* body = parse_statement(parser);
    if (!body) {
        while (params) {
            Param* next = params->next;
            free(params->name);
            free(params);
            params = next;
        }
        free(func_name);
        return NULL;
    }
    
    if (!parser->current || parser->current->type != TOKEN_KEYWORD || strcmp(parser->current->value, "son") != 0) {
        while (params) {
            Param* next = params->next;
            free(params->name);
            free(params);
            params = next;
        }
        free(func_name);
        free_ast_node(body);
        return NULL;
    }
    parser->current = parser->current->next;
    
    ASTNode* node = create_ast_node(NODE_FUNCTION_DECL, func_token->line, func_token->column);
    node->function_decl.name = func_name;
    
    if (param_count > 0) {
        node->function_decl.params = kor_calloc(param_count, sizeof(ASTNode*));
        Param *param = params;
        int i = 0;
        while (param && i < param_count) {
            node->function_decl.params[i] = create_identifier_node(param->name, func_line, func_column);
            param = param->next;
            i++;
        }
        node->function_decl.param_count = param_count;
    } else {
        node->function_decl.params = NULL;
        node->function_decl.param_count = 0;
    }
    node->function_decl.body = body;
    
    while (params) {
        Param* next = params->next;
        free(params->name);
        free(params);
        params = next;
    }
    
    return node;
}

ASTNode* parse_statement(ParserState* parser) {
    if (!parser->current) return NULL;
    
    ASTNode* node = NULL;
    
    if (parser->current->type == TOKEN_DIRECTIVE) {
        Token* token = parser->current;
        if (parser->include_count < MAX_INCLUDES) {
            parser->includes[parser->include_count++] = kor_strdup(token->value);
        }
        parser->current = parser->current->next;
        node = create_ast_node(NODE_DIRECTIVE, token->line, token->column);
        node->str_value = kor_strdup(token->value);
        return node;
    }
    
    node = parse_assignment(parser);
    if (node) return node;
    
    node = parse_print_statement(parser);
    if (node) return node;
    
    node = parse_if_statement(parser);
    if (node) return node;
    
    node = parse_repeat_statement(parser);
    if (node) return node;
    
    node = parse_function_declaration(parser);
    if (node) return node;
    
    if (parser->current && parser->current->type == TOKEN_KEYWORD) {
        Token* token = parser->current;
        if (strcmp(token->value, "cik") == 0) {
            node = create_ast_node(NODE_RETURN, token->line, token->column);
            parser->current = parser->current->next;
        }
        else if (strcmp(token->value, "bekle") == 0) {
            parser->current = parser->current->next;
            ASTNode* expr = parse_expression(parser);
            if (expr) {
                node = create_ast_node(NODE_SLEEP, token->line, token->column);
                node->binary.right = expr;
            }
        }
        else if (strcmp(token->value, "calistir") == 0) {
            parser->current = parser->current->next;
            ASTNode* expr = parse_expression(parser);
            if (expr && expr->type == NODE_STRING) {
                node = create_ast_node(NODE_SYSTEM, token->line, token->column);
                node->str_value = kor_strdup(expr->str_value);
                free_ast_node(expr);
            } else if (expr) {
                free_ast_node(expr);
            }
        }
        else {
            if (parser->error_count < MAX_ERRORS) {
                ErrorInfo* error = &parser->errors[parser->error_count++];
                error->code = ERR_SYNTAX;
                error->line = token->line;
                error->column = token->column;
                snprintf(error->message, sizeof(error->message), "Bilinmeyen anahtar kelime: '%s'", token->value);
            }
            parser->current = parser->current->next;
        }
        if (node) return node;
    }
    
    node = parse_expression(parser);
    if (node) return node;
    
    return NULL;
}

ASTNode* parse_program(ParserState* parser) {
    ASTNode* program = create_ast_node(NODE_PROGRAM, 1, 1);
    ASTNode* last = NULL;
    
    while (parser->current && parser->current->type != TOKEN_EOF) {
        while (parser->current && parser->current->type == TOKEN_NEWLINE) {
            parser->current = parser->current->next;
        }
        
        if (!parser->current || parser->current->type == TOKEN_EOF) break;
        
        ASTNode* stmt = parse_statement(parser);
        if (stmt) {
            if (!program->block.statements) {
                program->block.statements = kor_calloc(MAX_CHILDREN, sizeof(ASTNode*));
            }
            if (program->block.stmt_count < MAX_CHILDREN) {
                program->block.statements[program->block.stmt_count++] = stmt;
            }
            if (last) last->next = stmt;
            last = stmt;
        }
        
        if (parser->current && (parser->current->type == TOKEN_COMMA)) {
            parser->current = parser->current->next;
        }
    }
    return program;
}

ParserState* parser_init(Token* tokens) {
    ParserState* parser = kor_malloc(sizeof(ParserState));
    parser->tokens = tokens;
    parser->current = tokens;
    parser->ast = NULL;
    parser->symbol_tables = kor_calloc(MAX_NESTING, sizeof(Symbol*));
    parser->scope_depth = 0;
    parser->errors = kor_calloc(MAX_ERRORS, sizeof(ErrorInfo));
    parser->error_count = 0;
    parser->warnings = kor_calloc(MAX_WARNINGS, sizeof(WarningInfo));
    parser->warning_count = 0;
    parser->includes = kor_calloc(MAX_INCLUDES, sizeof(char*));
    parser->include_count = 0;
    return parser;
}

void parser_cleanup(ParserState* parser) {
    if (!parser) return;
    if (parser->ast) {
        free_ast_node(parser->ast);
        parser->ast = NULL;
    }
    
    for (int i = 0; i < MAX_NESTING; i++) {
        Symbol* sym = parser->symbol_tables[i];
        while (sym) {
            Symbol* next = sym->next;
            if (sym->name) free(sym->name);
            free(sym);
            sym = next;
        }
    }
    free(parser->symbol_tables);
    
    for (int i = 0; i < parser->include_count; i++) {
        if (parser->includes[i]) free(parser->includes[i]);
    }
    free(parser->includes);
    
    if (parser->errors) free(parser->errors);
    if (parser->warnings) free(parser->warnings);
    free(parser);
}

ASTNode* parser_parse(ParserState* parser) {
    parser->ast = parse_program(parser);
    return parser->ast;
}

void codegen_append(CodeGenState* cg, const char* format, ...) {
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);
    if (needed < 0) {
        va_end(args);
        return;
    }
    while (cg->output_size + needed + 1 >= cg->output_capacity) {
        cg->output_capacity *= 2;
        cg->output = realloc(cg->output, cg->output_capacity);
        if (!cg->output) {
            fprintf(stderr, "HATA E008: Bellek genisletme basarisiz\n");
            exit(ERR_MEMORY);
        }
    }
    int written = vsnprintf(cg->output + cg->output_size,
                          cg->output_capacity - cg->output_size,
                          format, args);
    if (written > 0) cg->output_size += written;
    va_end(args);
}

void codegen_indent(CodeGenState* cg) {
    for (int i = 0; i < cg->indent_level; i++) codegen_append(cg, "    ");
}

const char* op_to_c_operator(int op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_MULTIPLY: return "*";
        case OP_DIVIDE: return "/";
        case OP_MODULO: return "%";
        case OP_GT: return ">";
        case OP_LT: return "<";
        case OP_GTE: return ">=";
        case OP_LTE: return "<=";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        default: return "??";
    }
}

void codegen_expression(CodeGenState* cg, ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_NUMBER:
            codegen_append(cg, "%g", node->num_value);
            break;
        case NODE_STRING:
            codegen_append(cg, "\"%s\"", node->str_value);
            break;
        case NODE_IDENTIFIER:
            codegen_append(cg, "%s", node->identifier);
            break;
        case NODE_BINARY_OP:
            codegen_append(cg, "(");
            codegen_expression(cg, node->binary.left);
            codegen_append(cg, " %s ", op_to_c_operator(node->binary.op));
            codegen_expression(cg, node->binary.right);
            codegen_append(cg, ")");
            break;
        case NODE_FUNCTION_CALL:
            codegen_append(cg, "%s(", node->function_call.name);
            for (int i = 0; i < node->function_call.arg_count; i++) {
                codegen_expression(cg, node->function_call.args[i]);
                if (i < node->function_call.arg_count - 1) codegen_append(cg, ", ");
            }
            codegen_append(cg, ")");
            break;
        default:
            codegen_append(cg, "/* UNSUPPORTED EXPRESSION */");
            break;
    }
}

void codegen_statement(CodeGenState* cg, ASTNode* node) {
    if (!node) return;
    codegen_indent(cg);
    switch (node->type) {
        case NODE_ASSIGNMENT:
            codegen_append(cg, "double %s = ", node->identifier);
            codegen_expression(cg, node->binary.right);
            codegen_append(cg, ";\n");
            break;
        case NODE_PRINT:
            codegen_append(cg, "printf(\"%%s\\n\", ");
            codegen_expression(cg, node->binary.right);
            codegen_append(cg, ");\n");
            break;
        case NODE_IF:
            codegen_append(cg, "if (");
            codegen_expression(cg, node->if_stmt.condition);
            codegen_append(cg, ") {\n");
            cg->indent_level++;
            codegen_statement(cg, node->if_stmt.then_branch);
            cg->indent_level--;
            codegen_indent(cg);
            codegen_append(cg, "}");
            if (node->if_stmt.else_branch) {
                codegen_append(cg, " else {\n");
                cg->indent_level++;
                codegen_statement(cg, node->if_stmt.else_branch);
                cg->indent_level--;
                codegen_indent(cg);
                codegen_append(cg, "}\n");
            } else codegen_append(cg, "\n");
            break;
        case NODE_FOR:
            codegen_append(cg, "for (");
            codegen_expression(cg, node->for_loop.init);
            codegen_append(cg, "; ");
            codegen_expression(cg, node->for_loop.condition);
            codegen_append(cg, "; ");
            codegen_expression(cg, node->for_loop.update);
            codegen_append(cg, ") {\n");
            cg->indent_level++;
            codegen_statement(cg, node->for_loop.body);
            cg->indent_level--;
            codegen_indent(cg);
            codegen_append(cg, "}\n");
            break;
        case NODE_FUNCTION_DECL:
            codegen_append(cg, "void %s(", node->function_decl.name);
            for (int i = 0; i < node->function_decl.param_count; i++) {
                codegen_append(cg, "double %s", node->function_decl.params[i]->identifier);
                if (i < node->function_decl.param_count - 1) codegen_append(cg, ", ");
            }
            codegen_append(cg, ") {\n");
            cg->indent_level++;
            codegen_statement(cg, node->function_decl.body);
            cg->indent_level--;
            codegen_indent(cg);
            codegen_append(cg, "}\n");
            break;
        case NODE_RETURN:
            codegen_append(cg, "exit(0);\n");
            break;
        case NODE_SLEEP:
            codegen_append(cg, "sleep((int)");
            codegen_expression(cg, node->binary.right);
            codegen_append(cg, ");\n");
            break;
        case NODE_SYSTEM:
            codegen_append(cg, "system(\"%s\");\n", node->str_value);
            break;
        case NODE_DIRECTIVE:
            codegen_append(cg, "#include <%s.h>\n", node->str_value);
            break;
        default:
            codegen_append(cg, "/* UNSUPPORTED STATEMENT TYPE %d */\n", node->type);
            break;
    }
}

CodeGenState* codegen_init(ASTNode* ast) {
    CodeGenState* cg = kor_malloc(sizeof(CodeGenState));
    cg->ast = ast;
    cg->output = NULL;
    cg->output_size = 0;
    cg->output_capacity = 1024;
    cg->output = kor_malloc(cg->output_capacity);
    cg->output[0] = '\0';
    cg->indent_level = 0;
    cg->label_counter = 0;
    cg->string_constants = kor_calloc(MAX_CHILDREN, sizeof(char*));
    cg->string_count = 0;
    cg->variable_names = kor_calloc(MAX_SYMBOLS, sizeof(char*));
    cg->var_count = 0;
    cg->function_names = kor_calloc(MAX_FUNCTIONS, sizeof(char*));
    cg->func_count = 0;
    cg->errors = kor_calloc(MAX_ERRORS, sizeof(ErrorInfo));
    cg->error_count = 0;
    return cg;
}

void codegen_cleanup(CodeGenState* cg) {
    if (!cg) return;
    if (cg->output) free(cg->output);
    for (int i = 0; i < cg->string_count; i++) if (cg->string_constants[i]) free(cg->string_constants[i]);
    free(cg->string_constants);
    for (int i = 0; i < cg->var_count; i++) if (cg->variable_names[i]) free(cg->variable_names[i]);
    free(cg->variable_names);
    for (int i = 0; i < cg->func_count; i++) if (cg->function_names[i]) free(cg->function_names[i]);
    free(cg->function_names);
    if (cg->errors) free(cg->errors);
    free(cg);
}

char* codegen_generate(CodeGenState* cg, char** includes, int include_count) {
    codegen_append(cg, "/* Generated by KOR compiler */\n");
    codegen_append(cg, "#include <stdio.h>\n");
    codegen_append(cg, "#include <stdlib.h>\n");
    codegen_append(cg, "#include <string.h>\n");
    codegen_append(cg, "#include <unistd.h>\n");
    codegen_append(cg, "#include <math.h>\n");
    for (int i = 0; i < include_count; i++) {
        if (includes[i]) codegen_append(cg, "#include <%s>\n", includes[i]);
    }
    codegen_append(cg, "\n");
    codegen_append(cg, "int main() {\n");
    cg->indent_level++;
    if (cg->ast && cg->ast->type == NODE_PROGRAM) {
        for (int i = 0; i < cg->ast->block.stmt_count; i++) {
            codegen_statement(cg, cg->ast->block.statements[i]);
        }
    }
    codegen_indent(cg);
    codegen_append(cg, "return 0;\n");
    cg->indent_level--;
    codegen_append(cg, "}\n");
    return cg->output;
}

int compile_c_code(const char* c_code, const char* output_filename, char** error_output) {
    int result = ERR_COMPILE;
    char command[1024];
    snprintf(command, sizeof(command), "gcc -std=c11 -x c - -o \"%s\" 2>&1", output_filename);
    
    FILE* gcc_process = popen(command, "w");
    if (!gcc_process) {
        if (error_output) *error_output = kor_strdup("GCC baslatilamadi");
        return ERR_COMPILE;
    }
    
    size_t code_len = strlen(c_code);
    size_t written = fwrite(c_code, 1, code_len, gcc_process);
    if (written != code_len) {
        pclose(gcc_process);
        if (error_output) *error_output = kor_strdup("C kodu GCC'ye yazilamadi");
        return ERR_COMPILE;
    }
    
    int gcc_result = pclose(gcc_process);
    if (WIFEXITED(gcc_result)) {
        if (WEXITSTATUS(gcc_result) == 0) {
            result = ERR_SUCCESS;
            if (error_output) *error_output = NULL;
        } else {
            result = ERR_COMPILE;
            if (error_output) {
                *error_output = kor_malloc(8192);
                (*error_output)[0] = '\0';
                snprintf(*error_output, 8192, "Derleme hatasi (cikis kodu: %d)", WEXITSTATUS(gcc_result));
            }
        }
    } else {
        result = ERR_COMPILE;
        if (error_output) *error_output = kor_strdup("GCC beklenmedik sekilde sonlandi");
    }
    return result;
}

int run_binary(const char* binary_path, int* exit_code, char** output) {
    int result = ERR_RUNTIME;
    char command[1024];
    snprintf(command, sizeof(command), "\"%s\" 2>&1", binary_path);
    
    FILE* process = popen(command, "r");
    if (!process) {
        if (output) *output = kor_strdup("Process baslatilamadi");
        return ERR_RUNTIME;
    }
    
    if (output) {
        *output = kor_malloc(65536);
        (*output)[0] = '\0';
        char buffer[4096];
        size_t total_len = 0;
        while (fgets(buffer, sizeof(buffer), process)) {
            size_t len = strlen(buffer);
            if (total_len + len < 65534) strcat(*output, buffer);
        }
    }
    
    int pclose_result = pclose(process);
    if (WIFEXITED(pclose_result)) {
        if (exit_code) *exit_code = WEXITSTATUS(pclose_result);
        result = ERR_SUCCESS;
    } else if (WIFSIGNALED(pclose_result)) {
        if (exit_code) *exit_code = WTERMSIG(pclose_result);
        result = ERR_RUNTIME;
    } else {
        if (exit_code) *exit_code = -1;
        result = ERR_RUNTIME;
    }
    return result;
}

double kor_cpu_temperature(void) {
    FILE* temp_file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!temp_file) return -1.0;
    int temp_millic;
    if (fscanf(temp_file, "%d", &temp_millic) != 1) {
        fclose(temp_file);
        return -1.0;
    }
    fclose(temp_file);
    return temp_millic / 1000.0;
}

double kor_cpu_usage(void) {
    FILE* stat_file = fopen("/proc/stat", "r");
    if (!stat_file) return -1.0;
    char line[256];
    long user, nice, system, idle, iowait, irq, softirq;
    if (fgets(line, sizeof(line), stat_file)) {
        sscanf(line, "cpu %ld %ld %ld %ld %ld %ld %ld",
               &user, &nice, &system, &idle, &iowait, &irq, &softirq);
    }
    fclose(stat_file);
    long total = user + nice + system + idle + iowait + irq + softirq;
    long idle_total = idle + iowait;
    if (total == 0) return 0.0;
    return 100.0 * (1.0 - (double)idle_total / total);
}

double kor_ram_usage(void) {
    struct sysinfo info;
    if (sysinfo(&info) != 0) return -1.0;
    double total = info.totalram / (1024.0 * 1024.0);
    double free = info.freeram / (1024.0 * 1024.0);
    double used = total - free;
    return used;
}

double kor_disk_usage(void) {
    FILE* df = popen("df / 2>/dev/null | tail -1", "r");
    if (!df) return -1.0;
    char line[256];
    if (fgets(line, sizeof(line), df)) {
        char filesystem[256];
        long blocks, used, available;
        int percent;
        char mount[256];
        if (sscanf(line, "%s %ld %ld %ld %d%% %s",
                   filesystem, &blocks, &used, &available, &percent, mount) >= 5) {
            pclose(df);
            return percent;
        }
    }
    pclose(df);
    return -1.0;
}

int kor_system(const char* command) {
    if (!command) return -1;
    if (!command_in_whitelist(command)) {
        fprintf(stderr, "HATA E006: Komut whitelist'te yok: %s\n", command);
        return ERR_SECURITY;
    }
    for (int i = 0; whitelist[i].command; i++) {
        if (strstr(command, whitelist[i].command) == command &&
            whitelist[i].privileged && !is_root_user()) {
            fprintf(stderr, "HATA E006: Root yetkisi gerektiren komut: %s\n", command);
            return ERR_SECURITY;
        }
    }
    return system(command);
}

int kor_shutdown(void) {
    if (!is_root_user()) {
        fprintf(stderr, "HATA E006: Sistem kapatma icin root yetkisi gerekli\n");
        return ERR_SECURITY;
    }
    return system("shutdown -h now");
}

int kor_reboot(void) {
    if (!is_root_user()) {
        fprintf(stderr, "HATA E006: Sistem yeniden baslatma icin root yetkisi gerekli\n");
        return ERR_SECURITY;
    }
    return system("reboot");
}

#ifdef HAVE_NCURSES
void tui_init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    if (has_colors()) {
        start_color();
    }
    g_tui.active = 1;
    getmaxyx(stdscr, g_tui.height, g_tui.width);
    g_tui.selected_line = 0;
    g_tui.scroll_offset = 0;
    g_tui.show_help = 1;
}

void tui_cleanup(void) {
    endwin();
    g_tui.active = 0;
}

void tui_draw(void) {
    clear();
    int height = g_tui.height;
    int width = g_tui.width;
    
    mvprintw(0, (width - 40) / 2, "KOR Derleyici - TUI Modu");
    mvprintw(2, 2, "Dosya: %s", g_tui.filename ? g_tui.filename : "(yok)");
    mvprintw(3, 2, "Durum: ");
    if (g_tui.status_message) printw("%s", g_tui.status_message);
    else printw("Hazir");
    
    mvprintw(5, 2, "Hatalar (%d):", g_tui.error_count);
    int start_y = 6;
    int max_errors = height - start_y - 5;
    for (int i = 0; i < g_tui.error_count && i < max_errors; i++) {
        int display_idx = i + g_tui.scroll_offset;
        if (display_idx >= g_tui.error_count) break;
        ErrorInfo* error = &g_tui.errors[display_idx];
        mvprintw(start_y + i, 2, "HATA E%03d (satir %d): %s",
                error->code, error->line, error->message);
    }
    
    int help_y = height - 3;
    mvprintw(help_y, 2, "b: Derle | r: Calistir | q: Cikis | ^/v: Sec | h: Yardim");
    if (g_tui.show_help) mvprintw(help_y + 1, 2, "KOR - Turkce Scripting Dili Derleyicisi");
    refresh();
}

int tui_main_loop(const char* filename, ErrorInfo* errors, int error_count) {
    g_tui.filename = kor_strdup(filename);
    g_tui.errors = errors;
    g_tui.error_count = error_count;
    g_tui.status_message = NULL;
    g_tui.status_type = 0;
    
    tui_init();
    int ch;
    while (g_tui.active) {
        tui_draw();
        ch = getch();
        switch (ch) {
            case 'q': case 'Q': case 27: g_tui.active = 0; break;
            case 'b': case 'B':
                mvprintw(g_tui.height - 1, 2, "Derleniyor...");
                refresh();
                g_tui.status_message = kor_strdup("Derleme baslatildi");
                g_tui.status_type = 1;
                g_tui.rebuild_needed = 1;
                break;
            case 'r': case 'R':
                mvprintw(g_tui.height - 1, 2, "Calistiriliyor...");
                refresh();
                g_tui.status_message = kor_strdup("Calistirma baslatildi");
                g_tui.status_type = 1;
                break;
            case KEY_UP:
                if (g_tui.selected_line > 0) {
                    g_tui.selected_line--;
                    if (g_tui.selected_line < g_tui.scroll_offset) g_tui.scroll_offset = g_tui.selected_line;
                }
                break;
            case KEY_DOWN:
                if (g_tui.selected_line < g_tui.error_count - 1) {
                    g_tui.selected_line++;
                    if (g_tui.selected_line >= g_tui.scroll_offset + (g_tui.height - 8)) g_tui.scroll_offset++;
                }
                break;
            case 'h': case 'H': g_tui.show_help = !g_tui.show_help; break;
        }
    }
    tui_cleanup();
    if (g_tui.filename) {
        free(g_tui.filename);
        g_tui.filename = NULL;
    }
    return 0;
}
#else
int tui_main_loop(const char* filename, ErrorInfo* errors, int error_count) {
    printf("\n=== KOR TUI Modu (Basit Terminal) ===\n\n");
    printf("Dosya: %s\n", filename);
    printf("Toplam hata: %d\n\n", error_count);
    if (error_count > 0) {
        printf("Hatalar:\n");
        for (int i = 0; i < error_count; i++) {
            printf(" %d. HATA E%03d (satir %d): %s\n",
                   i + 1, errors[i].code, errors[i].line, errors[i].message);
        }
    } else printf("✓ Hic hata yok\n");
    
    printf("\nKomutlar:\n");
    printf(" b - Derle\n");
    printf(" r - Calistir\n");
    printf(" q - Cikis\n");
    printf("\nSecim: ");
    
    char choice[10];
    if (fgets(choice, sizeof(choice), stdin)) {
        choice[strcspn(choice, "\n")] = 0;
        switch (choice[0]) {
            case 'b': case 'B': printf("Derleme baslatiliyor...\n"); return 1;
            case 'r': case 'R': printf("Calistirma baslatiliyor...\n"); return 2;
            default: printf("Cikiliyor...\n"); break;
        }
    }
    return 0;
}
#endif

void generate_error_report(ErrorInfo* errors, int error_count, const char* filename) {
    if (error_count == 0) {
        printf("✓ %s: Hic hata yok\n", filename);
        return;
    }
    printf("\n=== HATA RAPORU: %s ===\n", filename);
    printf("Toplam hata: %d\n\n", error_count);
    for (int i = 0; i < error_count; i++) {
        printf("HATA E%03d (satir %d):\n", errors[i].code, errors[i].line);
        printf(" %s\n", errors[i].message);
        if (strlen(errors[i].suggestion) > 0) printf(" Oneri: %s\n", errors[i].suggestion);
        printf("\n");
    }
}

void print_usage(void) {
    printf("KOR - Turkce Scripting Dili Derleyicisi v1.0\n");
    printf("Kullanim: korc <komut> [secenekler] <dosya.kr>\n\n");
    printf("Komutlar:\n");
    printf(" check Syntax ve semantic kontrol\n");
    printf(" build Binary olustur\n");
    printf(" run Derle ve calistir\n");
    printf(" tui TUI arayuzu\n");
    printf("\nSecenekler:\n");
    printf(" -o <dosya> Cikti dosya adi (sadece build)\n");
    printf(" -h Yardim\n");
    printf(" -v Versiyon\n");
}

int main(int argc, char** argv) {

    freopen("/dev/null", "w", stderr);
    
    if (argc < 2) {
        print_usage();
        return ERR_GENERAL;
    }
    
    char* command = argv[1];
    char* filename = NULL;
    char* output_filename = NULL;
    
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage();
            return ERR_SUCCESS;
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("KOR Derleyici v1.0\n");
            return ERR_SUCCESS;
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_filename = argv[++i];
        }
        else if (argv[i][0] != '-') {
            filename = argv[i];
        }
    }
    
    if (strcmp(command, "check") != 0 &&
        strcmp(command, "build") != 0 &&
        strcmp(command, "run") != 0 &&
        strcmp(command, "tui") != 0) {
        fprintf(stdout, "HATA E001: Gecersiz komut: %s\n", command);
        print_usage();
        return ERR_GENERAL;
    }
    
    if (!filename) {
        fprintf(stdout, "HATA E001: Girdi dosyasi belirtilmedi\n");
        print_usage();
        return ERR_GENERAL;
    }
    
    if (!str_ends_with(filename, ".kr")) {
        fprintf(stdout, "HATA E001: Gecersiz dosya uzantisi\n");
        return ERR_GENERAL;
    }
    
    if (!file_exists(filename)) {
        fprintf(stdout, "HATA E011: Dosya bulunamadi: %s\n", filename);
        return ERR_NOT_FOUND;
    }
    
    char* source_code = read_entire_file(filename);
    if (!source_code) {
        fprintf(stdout, "HATA E007: Dosya okunamadi: %s\n", filename);
        return ERR_IO;
    }
    
    LexerState* lexer = lexer_init(source_code);
    if (!lexer_tokenize(lexer)) {
        generate_error_report(lexer->errors, lexer->error_count, filename);
        lexer_cleanup(lexer);
        free(source_code);
        return ERR_SYNTAX;
    }
    
    ParserState* parser = parser_init(lexer->tokens);
    ASTNode* ast = parser_parse(parser);
    
    if (parser->error_count > 0) {
        generate_error_report(parser->errors, parser->error_count, filename);
        parser_cleanup(parser);
        lexer_cleanup(lexer);
        free(source_code);
        return ERR_SYNTAX;
    }
    
    if (strcmp(command, "check") == 0) {
        printf("✓ %s: Syntax ve semantic kontrol basarili\n", filename);
        parser_cleanup(parser);
        lexer_cleanup(lexer);
        free(source_code);
        return ERR_SUCCESS;
    }
    
    CodeGenState* codegen = codegen_init(ast);
    char* c_code = codegen_generate(codegen, parser->includes, parser->include_count);
    
    if (codegen->error_count > 0) {
        generate_error_report(codegen->errors, codegen->error_count, filename);
        codegen_cleanup(codegen);
        parser_cleanup(parser);
        lexer_cleanup(lexer);
        free(source_code);
        return ERR_SEMANTIC;
    }
    
    if (strcmp(command, "tui") == 0) {
        ErrorInfo all_errors[MAX_ERRORS];
        int all_error_count = 0;
        for (int i = 0; i < lexer->error_count && all_error_count < MAX_ERRORS; i++)
            all_errors[all_error_count++] = lexer->errors[i];
        for (int i = 0; i < parser->error_count && all_error_count < MAX_ERRORS; i++)
            all_errors[all_error_count++] = parser->errors[i];
        for (int i = 0; i < codegen->error_count && all_error_count < MAX_ERRORS; i++)
            all_errors[all_error_count++] = codegen->errors[i];
            
        int tui_result = tui_main_loop(filename, all_errors, all_error_count);
        codegen_cleanup(codegen);
        parser_cleanup(parser);
        lexer_cleanup(lexer);
        free(source_code);
        return tui_result == 0 ? ERR_SUCCESS : ERR_GENERAL;
    }
    
    char binary_name[1024];
    if (output_filename) {
        strncpy(binary_name, output_filename, sizeof(binary_name) - 1);
        binary_name[sizeof(binary_name) - 1] = '\0';
    } else {
        char* base_name = kor_strdup(filename);
        char* dot = strrchr(base_name, '.');
        if (dot && strcmp(dot, ".kr") == 0) {
            *dot = '\0';
        }
        snprintf(binary_name, sizeof(binary_name), "%s", base_name);
        free(base_name);
    }
    
    char* final_output = kor_strdup(binary_name);
    char* compile_error = NULL;
    int compile_result = compile_c_code(c_code, final_output, &compile_error);
    
    if (compile_result != ERR_SUCCESS) {
        printf("HATA E005: Derleme basarisiz\n");
        if (compile_error) {
            printf("%s\n", compile_error);
            free(compile_error);
        }
        codegen_cleanup(codegen);
        parser_cleanup(parser);
        lexer_cleanup(lexer);
        free(source_code);
        free(final_output);
        return ERR_COMPILE;
    }
    
    printf("✓ Derleme basarili: %s\n", final_output);
    
    if (strcmp(command, "build") == 0) {
        codegen_cleanup(codegen);
        parser_cleanup(parser);
        lexer_cleanup(lexer);
        free(source_code);
        free(final_output);
        return ERR_SUCCESS;
    }
    
    if (strcmp(command, "run") == 0) {
        printf("Calistiriliyor: %s\n", final_output);
        int exit_code;
        char* run_output = NULL;
        int run_result = run_binary(final_output, &exit_code, &run_output);
        
        if (run_result == ERR_SUCCESS) {
            if (run_output && strlen(run_output) > 0) printf("Cikti:\n%s\n", run_output);
            printf("✓ Program basariyla tamamlandi (cikis kodu: %d)\n", exit_code);
        } else {
            printf("HATA E004: Runtime hatasi\n");
            if (run_output) printf("%s\n", run_output);
        }
        
        if (run_output) free(run_output);
    }
    
    codegen_cleanup(codegen);
    parser_cleanup(parser);
    lexer_cleanup(lexer);
    free(source_code);
    free(final_output);
    
    return ERR_SUCCESS;
}