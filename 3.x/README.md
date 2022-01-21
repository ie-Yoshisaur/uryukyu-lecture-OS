# OSの講義の1.xの課題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 3.1 debuggerの使い方

---

### 問題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/209.html)の問題を解く

```
write を引数二つで指定した回数m * n だけ行うプログラムを C で作成せよ。(hint atol を用いる ) 出力は空白または0を使う。

    n 回出力する関数  f
    f をm 回呼び出す関数 g

これを main から呼び出す。
lldb と gdb 両方で以下の操作を行い、log を示せ

(1) 作成したプログラム 引数 n=100, m=200 を指定して、lldb/gdb を起動する。プログラムは -O0 -g で compile すること。

(2) 関数 f のwriteを呼び出しているところ break point を設定し、そこで止める。

(3) frame up を使って main 関数で、m と n に正しい値が格納されていることを示せ

(4) break point を学籍番号の下位二桁の回数だけ通過してから、break point で止まるようにせよ

     lldb     brea modiy -i xx
     gdb      ingore xx yy ?

(5) m の値が指定した値で停まるようにせよ
(6) write する直前停めた時に disass して assembler を見る。stepi を繰り返して、sycall の直前までいく

(7) 実際に write system call が呼ばれる直前の register の値を示せ

      lldb    regisgter read
      gdb     info register

(8) break point を解除し、 f が終了するように finish コマンドを使う

option
m と n の値を変更することが可能かどうかを調べて試してみよ。
f と g は、clang/gcc の最適化によって、どのような assembler になるかを調べよ。

最適化されたコードを assembler level で trace してみよう。
```

---

### 用語整理

意味がわからない単語多すぎピエンなので、整理する

- write
  - [Code Wiki](http://codewiki.wikidot.com/c:system-calls:write)を参照
  - write is a system call that is used to write data out of a buffer.
    - bufferはコンピューターが処理しきれないデータを一時的に保持しておくための記憶領域のこと
  - バッファからデータを書き出すときに使われるシステムコール
- atol
  - [tutorials point](https://www.tutorialspoint.com/c_standard_library/c_function_atol.htm)を参照
  - The C library function long int atol(const char \*str) converts the string argument str to a long integer (type long int).
- gdbとlldb
  - どっちもdebuggerである
  - [aaronbloomfield](https://aaronbloomfield.github.io/pdr/docs/gdb_vs_lldb.html)を参照
  - GDB is part of the GNU framework.
  - LLDB is part of the LLVM framework.
  - gdbとlldbの違いの一つにコマンドがある
    - [lldb.llvm.org](https://lldb.llvm.org/use/map.html)にGDBとLLDBのコマンドの対応表がある
- break point
  - [Linuxtopia](https://www.linuxtopia.org/online_books/an_introduction_to_gcc/gccintro_40.html#:~:text=A%20breakpoint%20stops%20the%20execution,locations%20with%20the%20break%20command.)を参照
  - A breakpoint stops the execution of a program and returns control to the debugger, where its variables and memory can be examined before continuing.
  - つまりbreak pointに遭遇すると、debuggerはプログラムの実行を停止してプログラムに介入して変数やメモリを調べさせるということ

---

### 環境構築

3.1というディレクトリを作成して、

---

### writeを引数二つで指定した回数m * nだけ行うプログラムをCで作成する

↓のような中身の3_1.cというファイルを作成した

```
#include <stdio.h>
#include <stdlib.h>

void f(int n){
    int f_i;
    for(f_i = 0; f_i < n; f_i++){
        printf("0");
    }
}

void g(int n, int m){
    int g_i;
    for(g_i = 0; g_i < m; g_i++){
        f(n);
    }
    printf("\n");
}
 
int main(int argc, char *argv[]){
    int n = atol(argv[1]);
    int m = atol(argv[2]);
    g(n, m);
}
```

---

### \(1\) 作成したプログラム 引数 n=100, m=200 を指定して、lldb/gdb を起動する



---
