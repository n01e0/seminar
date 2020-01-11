#include <stdio.h>

int main(int argc, const char **argv) {
    if (argc != 2) {
        // ファイルが指定されていなければこのプログラムの使い方を表示して終了する
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r"); // 読み込み限定のモードでファイルを開き、FILE構造体へのアドレスを取得
    if (fp == NULL) {
        perror(argv[1]);
        return 1;
    }

    char buf[256];
    int size = fread(buf, sizeof(char), sizeof(buf), fp);

    do {
        for (int i = 0; i < size; i++)
            putchar(buf[i]);

        size = fread(buf, sizeof(char), sizeof(buf), fp);
    }while (size > 0);
    return 0;
}
