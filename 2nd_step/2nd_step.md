# 抽象化
前回、システムコールを利用してファイルの中身を出力するプログラムを作成した。

```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return 1;

    char c;
    int size = read(fd, &c, sizeof(c));
    do {
        putchar(c);

        size = read(fd, &c, sizeof(c));
    }while (size > 0);
    return 0;
}
```

内容は

 1. ファイルを開きファイルディスクリプタを取得
 1. ファイルディスクリプタ経由でファイルから1バイト読み込む
 1. 読み込んだ1バイトを標準出力に書き込む

といった感じだった。

前回述べた通り、この頻度でシステムコールを発行するのはあまり良い手段とは言えない。

改善していく

このコードではファイルを示すのはファイルディスクリプタだ。

ファイルディスクリプタについては前回説明したのでわかるとは思うが、ただの整数では直感的ではない。

そこで、機能の追加も兼ねてファイルの抽象化を行う。

***システムプログラミングに於ける最も重要な概念、抽象化***

ファイル用の構造体を用意し、それを基軸に各種操作を実装する。
