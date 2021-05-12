#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_NAME_LENGTH 16
#define MAX_REG_LENGTH 64

#define BOLD  "\x1B[1m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

struct Bitfield {
    char field[2];
    char name[MAX_NAME_LENGTH];
};

void print_help(char* cmd) {
    printf("%s <regname> <hexvalue>\n", cmd);
}

char* get_line(FILE* f) {
    char* res = NULL;
    int len = 0, size = 0, cur;

    while ((cur = fgetc(f)) != EOF) {
        if (len >= size) {
            void* tmp;
            if (!(tmp = realloc(res, size += 10))) {
                fprintf(stderr, "Error: realloc failed\n");
                free(res);
                return NULL;
            }
            res = tmp;
        }
        if ((res[len++] = cur) == '\n') break;
    }
    if (len) res[len - 1] = '\0';
    return res;
}

int parse_field(struct Bitfield* bits, const char* line) {
    int from, to;
    char* p;

    from = strtol(line, &p, 10);
    if (p == line) {
        fprintf(stderr, "Error: syntax error: %s\n", line);
        return 0;
    }
    if (*p == '-') {
        char* q;
        to = strtol(p + 1, &q, 10);
        if (q == p + 1)  {
            fprintf(stderr, "Error: syntax error: %s\n", line);
            return 0;
        }
        p = q;
    } else {
        to = from;
    }
    if (*p != ':') {
        fprintf(stderr, "Error: syntax error: %s\n", line);
        return 0;
    }
    p++;
    for (; *p == ' '; p++);
    strncpy(bits->name, p, MAX_NAME_LENGTH - 1);
    bits->name[MAX_NAME_LENGTH - 1] = '\0';

    if (from > to) {
        int tmp;
        tmp = from;
        from = to;
        to = tmp; 
    }
    bits->field[0] = from;
    bits->field[1] = to;
    return 1;
}

int load_bits(struct Bitfield* bits, unsigned int* n, const char* file) {
    FILE* f;
    char *cur, ok = 1, *name;

    if (!(name = malloc(strlen(TEMPLATE_DIR) + strlen(file) + 2))) return 0;
    sprintf(name, TEMPLATE_DIR"/%s", file);
    printf("opening %s\n", name);
    if (!(f = fopen(name, "r"))) {
        fprintf(stderr, "Error: can't open %s\n", file);
        free(name);
        return 0;
    }
    free(name);

    *n = 0;
    while (ok && (*n < MAX_REG_LENGTH) && (cur = get_line(f))) {
        ok = parse_field(bits + *n, cur);
        if (ok) (*n)++;
        free(cur);
    }
    return ok;
}

int load_reg(uint64_t* reg, const char* s) {
    char* p;
    *reg = strtoull(s, &p, 16);
    return s != p;
}

void print_bits(char* field, uint64_t reg) {
    uint64_t i;
    for (i = field[1]; i >= field[0]; i--) {
        if (reg & ((uint64_t)1 << i)) {
            printf(RED "1");
        } else {
            printf(BLU "0");
        }
        if (!i) break;
    }
}

void print_field(struct Bitfield* f, uint64_t reg, uint64_t* reg2) {
    char prefix[MAX_NAME_LENGTH + 16];
    uint64_t i;

    sprintf(prefix, "%s [%d-%d]:", f->name, f->field[1], f->field[0]);
    fputs(prefix, stdout);
    for (i = strlen(prefix); i < MAX_NAME_LENGTH + 10; i++) putc('_', stdout);
    fputs(BOLD, stdout);
    print_bits(f->field, reg);
    fputs(RESET, stdout);
    if (reg2) {
        for (i = f->field[0]; i <= f->field[1]; i++) {
            if (           (reg & ((uint64_t)1 << i))
                    != ((*reg2) & ((uint64_t)1 << i))) {
                fputs(" -> ", stdout);
                fputs(BOLD, stdout);
                print_bits(f->field, *reg2);
                fputs(RESET, stdout);
                break;
            }
        }
    }
    fputs("\n", stdout);
}

int main(int argc, char** argv) {
    struct Bitfield bits[MAX_REG_LENGTH] = {0};
    unsigned int numFields, i;
    uint64_t reg, reg2;

    if (argc < 3) {
        print_help(argv[0]);
        return 1;
    }
    if (!load_reg(&reg, argv[2])) {
        fprintf(stderr, "Error: couldn't parse reg value\n");
        return 1;
    }
    if (argc >= 4 && !load_reg(&reg2, argv[3])) {
        fprintf(stderr, "Error: couldn't parse reg value\n");
        return 1;
    }
    if (!load_bits(bits, &numFields, argv[1])) {
        fprintf(stderr, "Error: parsing %s failed\n", argv[1]);
        return 1;
    }
    for (i = 0; i < numFields; i++) {
        print_field(bits + i, reg, argc >= 4 ? &reg2 : NULL);
    }
    return 0;
}
