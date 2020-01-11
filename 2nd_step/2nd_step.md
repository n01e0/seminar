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

このコードではファイルを示すのはファイルディスクリプタだった。

しかし、`stdio.h`にはファイルの操作を抽象化した[`FILE`構造体](#FILE_struct)がある。

***システムプログラミングに於ける最も重要な概念、抽象化***

これを用いる事で、`read`や`write`等を含む操作がバッファリングされる。

バッファリングされる事で、なにが嬉しいかと言うと、操作が簡略化されるのはもちろんのこと、

システムコールの発行頻度が抑えられ、その分オーバーヘッドも小さくなる。

各位、サンプルコードやmanを参考にFILE構造体を用いて前回のコードを改善してみよう。

## FILE_struct
この構造体の定義が気になるようであれば、`stdio.h`を見てみると良い。

残念ながら多くの環境ではFILEは`stdio.h`では直接定義されていないので探すのは大変だが、cscopeやgtagsを用いると比較的簡単に探せる。

これらのツールはカーネルなど、大きなコードを読む際に非常に有用なツールである為、今後も使うことになるだろう。
