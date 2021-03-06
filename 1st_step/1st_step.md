# システムコールを用いた簡易的なプログラムからOSの役割を実感する
## 今回やりたいこと
**日常の操作の裏に隠れたシステムの働きをすこしだけ理解する**

## 実例
例えばあるファイルの内容を見たい時、`cat`というコマンドを用いる[事がある](#concat)。

このコマンドがやっている事を再現しながら、意識しないと見えないシステムのうごきを見てみよう。

自分の力でやりたい、普段からシステムコールを用いたプログラミングに慣れているという方がいたら自力でやってもらって構わない。

でもこれはそういう人には多分簡単すぎて特に練習にもならないと思うし、

「このコードが反吐が出るほど嫌いで自分のPCをこんなコードで汚したくない」

という人以外は普通にコピペした方が無難だと思う。

以下にサンプルコードを提示する。

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

以降、このコードを元に説明をする。主にCの初心者を対象とした説明であり、なるべく正確性を欠かないように考慮はしているが、簡単の為省略しているような概念も多い。

なぜCか。モダンな高級言語は低レベルの概念の多くが抽象化されている。これらは普段のコーディングでは確かに扱いやすいが、システムプログラミングに於いては寧ろ理解の邪魔になってしまう事がある(これは主観的意見)なのでここではCを用いる。

当然、ほかの言語でやりたい方はやって構わない。

コードですべてわかる人は読み飛ばして[次へ進む]()なりしてもらって構わない。

## cの基本
```c
int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;
```

c言語ではプログラム実行時のコマンドライン引数(文字列)は`main`関数の引数とし扱える。

このプログラムではコマンドライン引数は`char*`型の配列`argv`に格納され、`argc`にはコマンドライン引数の数(実行時のファイル名含む)が入る。

例えば、`./a.out -hoge fuga`とシェルで入力して実行した場合、`argc`は**3**となり、`argv[0]`には`"./a.out"`が、`argv[1]`には`"-hoge"`、`argv[2]`には`"fuga"`が格納される。 

このプログラムの目的は**ファイルを開き、その内容を表示する**ことなので、実行時にファイル名を指定する必要がある。

よって`argc`が2より小さい時、ファイル名を指定されていない(ここでは、簡単の為指定するファイルは1つのみとする)という事になるので、求めている動作とは違う。ということでプログラムを終了する。

## ファイル
```c
    int fd = open(argv[1], O_RDONLY);
```

まず、`argv[1]`は実行時のコマンドライン引数。ここでは開きたいファイルのパスを表す。

`open()`はシステムコール関数で、引数にファイルのパスとファイルを開くときの**モード**(読み込み、書き込み等)を指定する。

ここでは読み込み限定の権限(`O_RDONLY`)でファイルを開いている。

また、この関数の戻り値は**ファイルディスクリプタ**である。
### ファイルディスクリプタとは
簡単に言うと、ファイルの操作用にOSが割り当てた**道**の識別番号である。

少し本当の事を言うと、`open(2)`で発行されるファイルディスクリプタとは、ファイルを開いた際に割り当てられるカーネル内のデータ構造へのインデックスである。

また、ここで重要なのは、ファイルディスクリプタが指すデータ構造は、ファイルの内容ではない。

ファイル自体の情報を持っているという点だ。

詳細はプログラムの解説をしたあとにまた。
## バッファへのデータの読み込み
```c
    char c;
    int size = read(fd, &c, sizeof(c));
```

最初の行では、char型の変数を宣言している。

この変数には1バイト(char型のサイズは1バイト)のデータが1つ入る。

次の行で行っている事を1つずつ分けると、

 - 整数型の変数`size`を定義
 - `size`に`read()`の戻り値を代入

している。

`read()`は第一引数にファイルディスクリプタ、第二引数にバッファのアドレス、第三引数に読み込むサイズを指定する。

名前の通り、ファイルディスクリプの指すファイルから指定サイズ分、バッファへ読み込む。

また、その戻り値は読み込むことが出来たサイズである。

つまり、データの終端に達した場合、戻り値は0となる。

ここでの引数は`fd, &c, sizeof(c)`

`fd`はさっきopenしたファイルのファイルディスクリプタ。

`&c`はさっき宣言したchar型の変数のアドレス(Cでは変数名に&を付ける事でその変数のアドレスが取得出来る)。

`sizeof(c)`は変数cの大きさ(バイト数)。

つまりこの行では変数cにfdの示すファイルから1バイト読みこんでいる。
## データの出力
```c
    do {
        putchar(c);

        size = read(fd, &c, sizeof(c));
    }while (size > 0);
```

ここでは最初の`putchar`で、ファイルディスクリプタから読み込んだデータを文字として出力している。

```c
size = read(fd, &c, sizeof(c));
```
では、データを読み進め、`size`の値を更新している。

ここで、先述の通りデータの終端に達すると`read()`の戻り値、即ち`size`の値は0になるので、ループを抜ける。

## 実行してみる
コンパイル、実行してみると、おそらく指定したファイルの中身が出力されるだろう。

## さてファイルディスクリプタ
とりあえずコードの説明は終わったのでファイルディスクリプタの説明に戻る。

`read()`の説明で疑問に思ったことは無いだろうか、

ファイルディスクリプタは変わっていないのに、ファイルから読み込めるデータは変わっているのだ。

察しの良い人はわかったかもしれないが、ファイルディスクリプタが指す情報には、読み込んでいるファイル内のデータの位置情報も含まれている。

`open()`システムコールが発行されると、カーネルは指定されたファイルの***オープンファイルオブジェクト***を作成する。

**ファイルディスクリプタとは、そのオープンファイルオブジェクトの識別子である。**
## オープンファイルオブジェクト
そもそも、プログラムがアクセス出来るのは「オープンされているファイル」だけである。

オープンファイルオブジェクトはそのファイルの状態を保持する

オープンファイルオブジェクトには、オープンされたファイルとプログラムとのやり取りや制御に関するデータ(即ち道)が含まれる。

一方、ファイルディスクリプタは、どのやり取り(道)かを示す。

ユーザー(プログラマー)は道を指定し、カーネルは指定された道が示すファイルオブジェクトに対し操作を行う。

ユーザーがファイルを操作する時、ファイルオブジェクトを意識する必要は無い。

実際にファイルオブジェクトに関わる作業を司るのはカーネル内の**ファイルシステム**である。

## ファイルシステム
フロッピーだろうが磁気テープだろうがハードディスクだろうがSSDだろうがCDだろうがみんなファイルとして開いたら同じように扱える。

***なぜか***

ファイルシステムのおかげ

そう。ファイルシステムのおかげ。

ストレージに保存された情報はリニアだ。

磁気テープとかCDとかを想像すればわかるだろう。

しかし、人が普段扱うファイルは木構造になっている。

それらのデータを人が使いやすい形に抽象化しているのがファイルシステムである。

とりあえずユーザーの目線に戻る。

## システムコール
このコードで使われているシステムコールは主に

 - `read`
 - `write`
 - `open`

である

これらについて改めて軽くおさらい。
### read
ファイルディスクリプタが指すファイルからバッファに指定バイト数読み込む。

 - 第一引数: ファイルディスクリプタ
 - 第二引数: バッファへのポインタ
 - 第三引数: 読み込むバイト数
 - 戻り値(成功): 読み込んだバイト数
 - 戻り値(失敗): -1
### write
ファイルディスクリプタが指すファイルにバッファから指定バイト書き出す。

 - 第一引数: ファイルディスクリプタ
 - 第二引数: バッファへのポインタ
 - 第三引数: 書き込むバイト数
 - 戻り値(成功): 書き込んだバイト数
 - 戻り値(失敗): -1
### open
 ファイルを開き、そのファイルのファイルディスクリプタを取得する。

 - 第一引数: 開きたいファイルのパス
 - 第二引数: フラグ(権限やモード)
 - 戻り値(成功): ファイルディスクリプタ
 - 戻り値(失敗): -1

ここでは簡単に説明を行ったが、以降はシェルで`man`コマンドを用いて自分で調べると良い。Cのライブラリ関数はだいたい載ってる。

システムコールは2ページ、その他はたいてい3ページにあると思って良い。

ex `man 2 read`

### そもそもシステムコール とは
詳解一章分使うので、簡潔に

OSが提供するAPIみたいなもの(厳密にはシステムコールはAPIでは無い)

一般的なオペレーティングシステムに於いて、ユーザーの権限は非常に制限されたものである。

そこで、様々な操作をOSカーネルを中継して行うこととなる。

そのインターフェースがシステムコールである。

このように、複数の手続きが発生する為、システムコールを発行するのは大きなオーバーヘッドが発生する。

冒頭のコードでは、ファイルに対して1バイト毎に`read`システムコールを発行している。

次は、これを改善してみる。


## 注釈・その他
### concat
catは本来、ファイルの表示の為に作られたものでは無い。

*cat*という名前は*concat*の*cat*であり、その名の通り**ファイルの内容を*****繋げる*****為にある。
