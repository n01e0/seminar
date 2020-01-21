#define RED         "\e[31m"
#define GREEN       "\e[32m"
#define YELLOW      "\e[33m"
#define BLUE        "\e[34m"
#define PURPLE      "\e[35m"
#define CYAN        "\e[36m"
#define WHITE       "\e[37m"

#define on_BLACK    ";40"
#define on_RED      ";41"
#define on_GREEN    ";42"
#define on_YELLOW   ";43"
#define on_BLUE     ";44"
#define on_PURPLE   ";45"
#define on_CYAN     ";46"
#define on_WHITE    ";47"

#define BOLD        ";1m"
#define UNDERLINE   ";4m"
#define FLASH       ";5m"


#define NORMAL      "\e[m"

void eprint(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, RED"error: "NORMAL);
    fprintf(stderr, fmt, ap);
    fprintf(stderr, "%s\n", strerror(errno));
}

bool strmatch(const char *src, const char *dest) {
    return (strncmp(src, dest, strlen(src)) == 0 && strlen(src) == strlen(dest));
}
