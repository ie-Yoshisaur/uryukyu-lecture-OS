# OSの講義の3.xの課題を解く

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
  - パラメータ\(`$ man 2 write`でわかる\)
    - fd
      - 書き込み先ファイルを指すファイルディスクリプタ
    - buf
      - 書き込むデータを格納しているバッファの先頭アドレス
    - count
      - 書き込むデータを格納しているバッファの先頭アドレス
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
  - つまりbreak pointに遭遇すると、debuggerはプログラムの実行を停止してプログラムに介入して変数やメモリを調べることができるということ

---

### 環境構築

3.1というディレクトリを作成して、ディレクトリの中身を[これ](https://github.com/e205723/uryukyu-lecture-OS/tree/main/3.x/3.1)と同じようにする

`$ docker-compose up -d` -> `$ docker-compose exec c_env bash`でdebuggerを使えるコンテナにログインできる

---

### writeを引数二つで指定した回数m * nだけ行うプログラムをCで作成する

↓のような中身の3_1.cというファイルを作成した

```
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void f(int n, int fd){
    int f_i;

    for(f_i = 0; f_i < n; f_i++){
        write(fd, "0", 1);
    }
}

void g(int n, int m, int fd){
    int g_i;

    for(g_i = 0; g_i < m; g_i++){
        f(n, fd);
    }
}
 
int main(int argc, char *argv[]){
    int n = atol(argv[1]);
    int m = atol(argv[2]);
    int fd = open("3_1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    g(n, m, fd);
    close(fd);   
}
```

---

### \(1\) 作成したプログラム 引数 n=100, m=200 を指定して、lldb/gdb を起動する

- バイナリファイルを生成するために`$ gcc -o 3_1 -O0 -g 3_1.c`を実行する
  - \-O0は最適化をオフにするオプション
  - \-gはデバッグ可能な状態にコンパイルするオプション
  - つまり、3_1.cを最適化しないで、その上、デバッグ可能な状態にコンパイルして3_1というシングルバイナリファイルを生成するというコマンドを実行している
- lldbを起動する
  - `$ lldb 3_1`を実行
- gdbを起動する
  - `$ gdb 3_1`を実行

---

### \(2\) 関数 f のwriteを呼び出しているところ break point を設定し、そこで止める

9行目で関数fのwriteが呼び出されている

- lldb
  - 関数 f のwriteを呼び出しているところに break point を設定
    - `breakpoint set --file 3_1.c --line 9`を実行
      - 出力:Breakpoint 1: where = 3_1\`f + 27 at 3_1.c:9:9, address = 0x00000000000011c4
  - プログラムを実行する
    - `run 100 200`を実行
    - ```
      Process 52 launched: '/workplace/3_1' (x86_64)
      Process 52 stopped
      * thread #1, name = '3_1', stop reason = breakpoint 1.1
          frame #0: 0x00005555555551c4 3_1`f(n=3, fd=3) at 3_1.c:9:9
         6        int f_i;
         7
         8        for(f_i = 0; f_i < n; f_i++){
      -> 9            write(fd, "0", 1);
         10       
         }
         11   }
         12`
      ```
- gdb
  - `break 3_1.c:9`
  - `run 100 200`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### \(3\) frame up を使って main 関数で、m と n に正しい値が格納されていることを示せ

- lldb
  - 24行目にブレークポイントを設定する
    - `breakpoint set --file 3_1.c --line 24`を実行する
      - 出力:Breakpoint 2: where = 3_1`main + 63 at 3_1.c:24:14, address = 0x0000555555555269
  - プログラムを実行する
    - `run 100 200`を実行
      - Yを入力してEnter
  - nの値を確認する
    - `frame variable n`
      - `(int) n = 100`
  - mの値を確認する
    - `frame variable m`
      - `(int) m = 200`
- gdb
  - `break 3_1.c:24`
  - `run 100 200`
  - `info locals n`
  - `info locals m`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### \(4\) break point を学籍番号の下位二桁の回数だけ通過してから、break point で止まるようにせよ

- lldb
  - `breakpoint delete 2`を実行
    - 設定したbreakpoint\(2\)を削除する
  - `breakpoint modify -i 23 1`を実行
    - breakpoint\(1\)に対して学籍番号の下位二桁の回数\(自分は23\)だけ通過してから、break point で止まるようにする
  - `run 100 200`を実行してYを入力してEnter
  - `frame variable f_i`を実行
    - `(int) f_i = 23`と表示されているのでbreakpointが23回通過していることがわかる
- gdb
  - `delete 2`
  - `ignore 1 23`
  - `run 100 200`
  - `info locals f_i`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### \(5\) m の値が指定した値で停まるようにせよ

mは今回のプログラムでは定数のような扱いにしているので、代わりにmに関するfor文のイテレータが指定した値で停まるようにする

- lldb
  - `breakpoint disable 1`を実行
    - breakpoint\(1\)を無効にする
  - `breakpoint set --file 3_1.c --line 17 -c g_i=23`を実行
    - g_iが23のときに停止するようなbreakpointを設定する
  - `run 100 200`を実行してYを入力してEnter
  - `frame variable g_i`を実行
    - `(int) g_i = 23`という内容の出力が表示される、g_iの値が指定した値で停止していることを確認できた
- gdb
  - `disable 1`
  - `break 3_1.c:17 if g_i == 23`
  - `run 100 200`
  - `info locals g_i`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### \(6\) write する直前停めた時に disass して assembler を見る。stepi を繰り返して、sycall の直前までいく

disassはdisassemble、バイナリをアセンブラ言語に変換することを指す

stepiは、選択されているスレッドで、命令レベルのシングルステップを実行することを指す

- lldb
  - `breakpoint delete 3`を実行
  - `breakpoint enable 1`を実行
    - 無効にしたbreakpoint\(1\)を有効にする
  - `run 100 200`を実行し、Yを入力してEnter
    - ```
      There is a running process, kill it and restart?: [Y/n] y
      Process 870 exited with status = 9 (0x00000009)
      Process 876 launched: '/workplace/3_1' (x86_64)
      Process 876 stopped
      * thread #1, name = '3_1', stop reason = breakpoint 1.1
          frame #0: 0x00005555555551c4 3_1`f(n=100, fd=3) at 3_1.c:9:9
         6        int f_i;
         7
         8        for(f_i = 0; f_i < n; f_i++){
      -> 9            write(fd, "0", 1);
         10       
         }
         11   }
         12`
      ```
  - `disassemble --frame`を実行してdisassembleする
    - ```
      3_1`f:
          0x5555555551a9 <+0>:  endbr64
          0x5555555551ad <+4>:  pushq  %rbp
          0x5555555551ae <+5>:  movq   %rsp, %rbp
          0x5555555551b1 <+8>:  subq   $0x20, %rsp
          0x5555555551b5 <+12>: movl   %edi, -0x14(%rbp)
          0x5555555551b8 <+15>: movl   %esi, -0x18(%rbp)
          0x5555555551bb <+18>: movl   $0x0, -0x4(%rbp)
          0x5555555551c2 <+25>: jmp    0x5555555551e1            ; <+56> at 3_1.c:8:22
          0x5555555551c4 <+27>: movl   -0x18(%rbp), %eax
          0x5555555551c7 <+30>: movl   $0x1, %edx
      ->  0x5555555551cc <+35>: leaq   0xe31(%rip), %rcx
          0x5555555551d3 <+42>: movq   %rcx, %rsi
          0x5555555551d6 <+45>: movl   %eax, %edi
          0x5555555551d8 <+47>: callq  0x555555555080            ; ___lldb_unnamed_symbol47
          0x5555555551dd <+52>: addl   $0x1, -0x4(%rbp)
          0x5555555551e1 <+56>: movl   -0x4(%rbp), %eax
          0x5555555551e4 <+59>: cmpl   -0x14(%rbp), %eax
          0x5555555551e7 <+62>: jl     0x5555555551c4            ; <+27> at 3_1.c:9:9
          0x5555555551e9 <+64>: nop
          0x5555555551ea <+65>: nop
          0x5555555551eb <+66>: leave
          0x5555555551ec <+67>: retq`
      ```
  - `thread step-inst`を複数回実行して、シングルステップを実行していく
    - わかったこと
      - 0x00005555555551c4から0x00005555555551d8までは3_1.c:9.9の範囲のアセンブラ
      - 0x0000555555555080から0x0000555555555084までは___lldb_unnamed_symbol47の範囲のアセンブラ
      - 0x00007ffff7eab9a0から0x00007ffff7eab9bfまではwrite.c:27:1の範囲のアセンブラ
  - `thread step-inst`を5回(出席番号によって違うかもしれない)実行したあと、`disassemble --frame`を実行
    - ```
      3_1`f:
          0x5555555551a9 <+0>:  endbr64
          0x5555555551ad <+4>:  pushq  %rbp
          0x5555555551ae <+5>:  movq   %rsp, %rbp
          0x5555555551b1 <+8>:  subq   $0x20, %rsp
          0x5555555551b5 <+12>: movl   %edi, -0x14(%rbp)
          0x5555555551b8 <+15>: movl   %esi, -0x18(%rbp)
          0x5555555551bb <+18>: movl   $0x0, -0x4(%rbp)
          0x5555555551c2 <+25>: jmp    0x5555555551e1            ; <+56> at 3_1.c:8:22
          0x5555555551c4 <+27>: movl   -0x18(%rbp), %eax
          0x5555555551c7 <+30>: movl   $0x1, %edx
          0x5555555551cc <+35>: leaq   0xe31(%rip), %rcx
          0x5555555551d3 <+42>: movq   %rcx, %rsi
          0x5555555551d6 <+45>: movl   %eax, %edi
      ->  0x5555555551d8 <+47>: callq  0x555555555080            ; ___lldb_unnamed_symbol47
          0x5555555551dd <+52>: addl   $0x1, -0x4(%rbp)
          0x5555555551e1 <+56>: movl   -0x4(%rbp), %eax
          0x5555555551e4 <+59>: cmpl   -0x14(%rbp), %eax
          0x5555555551e7 <+62>: jl     0x5555555551c4            ; <+27> at 3_1.c:9:9
          0x5555555551e9 <+64>: nop
          0x5555555551ea <+65>: nop
          0x5555555551eb <+66>: leave
          0x5555555551ec <+67>: retq`
      ```
  - 結論から言うとcallqは違う、write関数を複数呼ばないとsyscallを見つけられない
  - もう8回(出席番号によって違うかもしれない)`thread step-inst`を実行する
  - そこで`disassemble --frame`を実行する
    - ```
      libc.so.6`__GI___libc_write:
          0x7ffff7eab9a0 <+0>:   endbr64
          0x7ffff7eab9a4 <+4>:   movl   %fs:0x18, %eax
          0x7ffff7eab9ac <+12>:  testl  %eax, %eax
          0x7ffff7eab9ae <+14>:  jne    0x7ffff7eab9c0            ; <+32> at write.c:25:1
          0x7ffff7eab9b0 <+16>:  movl   $0x1, %eax
      ->  0x7ffff7eab9b5 <+21>:  syscall
          0x7ffff7eab9b7 <+23>:  cmpq   $-0x1000, %rax            ; imm = 0xF000
          0x7ffff7eab9bd <+29>:  ja     0x7ffff7eaba10            ; <+112> at write.c:26:10
          0x7ffff7eab9bf <+31>:  retq
          0x7ffff7eab9c0 <+32>:  subq   $0x28, %rsp
          0x7ffff7eab9c4 <+36>:  movq   %rdx, 0x18(%rsp)
          0x7ffff7eab9c9 <+41>:  movq   %rsi, 0x10(%rsp)
          0x7ffff7eab9ce <+46>:  movl   %edi, 0x8(%rsp)
          0x7ffff7eab9d2 <+50>:  callq  0x7ffff7e288e0            ; __GI___pthread_enable_asynccancel at canc
      ellation.c:32:1
          0x7ffff7eab9d7 <+55>:  movq   0x18(%rsp), %rdx
          0x7ffff7eab9dc <+60>:  movq   0x10(%rsp), %rsi
          0x7ffff7eab9e1 <+65>:  movl   %eax, %r8d
          0x7ffff7eab9e4 <+68>:  movl   0x8(%rsp), %edi
          0x7ffff7eab9e8 <+72>:  movl   $0x1, %eax
          0x7ffff7eab9ed <+77>:  syscall
          0x7ffff7eab9ef <+79>:  cmpq   $-0x1000, %rax            ; imm = 0xF000
          0x7ffff7eab9f5 <+85>:  ja     0x7ffff7eaba28            ; <+136> [inlined] __libc_write at write.c:
      26:10
          0x7ffff7eab9f7 <+87>:  movl   %r8d, %edi
          0x7ffff7eab9fa <+90>:  movq   %rax, 0x8(%rsp)
          0x7ffff7eab9ff <+95>:  callq  0x7ffff7e28950            ; __GI___pthread_disable_asynccancel at can
      cellation.c:57:1
          0x7ffff7eaba04 <+100>: movq   0x8(%rsp), %rax
          0x7ffff7eaba09 <+105>: addq   $0x28, %rsp
          0x7ffff7eaba0d <+109>: retq
          0x7ffff7eaba0e <+110>: nop
          0x7ffff7eaba10 <+112>: movq   0x1003f9(%rip), %rdx
          0x7ffff7eaba17 <+119>: negl   %eax
          0x7ffff7eaba19 <+121>: movl   %eax, %fs:(%rdx)
          0x7ffff7eaba1c <+124>: movq   $-0x1, %rax
          0x7ffff7eaba23 <+131>: retq
          0x7ffff7eaba24 <+132>: nopl   (%rax)
          0x7ffff7eaba28 <+136>: movq   0x1003e1(%rip), %rdx
          0x7ffff7eaba2f <+143>: negl   %eax
          0x7ffff7eaba31 <+145>: movl   %eax, %fs:(%rdx)
          0x7ffff7eaba34 <+148>: movq   $-0x1, %rax
          0x7ffff7eaba3b <+155>: jmp    0x7ffff7eab9f7            ; <+87> [inlined] __libc_write + 37 at writ
      e.c:26:10`
      ```
- gdb
  - `delete 3`
  - `enable 1`
  - `run 100 200`
  - `stepi`を13回実行
  - `disassemble`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### \(7\) 実際に write system call が呼ばれる直前の register の値を示せ

- lldb
  - `register read`を実行して、スレッドの汎用レジスタを表示する
  - ```
    General Purpose Registers:
           rax = 0x0000000000000001
           rbx = 0x0000000000000000
           rcx = 0x0000555555556004
           rdx = 0x0000000000000001
           rdi = 0x0000000000000003
           rsi = 0x0000555555556004
           rbp = 0x00007fffffffe5f0
           rsp = 0x00007fffffffe5c8
            r8 = 0x1999999999999999
            r9 = 0x0000000000000000
           r10 = 0x00000000000001a4
           r11 = 0x0000000000000246
           r12 = 0x00007fffffffe778
           r13 = 0x000055555555522a  3_1`main at 3_1.c:21:33
           r14 = 0x0000000000000000
           r15 = 0x00007ffff7ffbc40
           rip = 0x00007ffff7eab9b5  libc.so.6`__GI___libc_write + 21 at write.c:26:10
        rflags = 0x0000000000000246
            cs = 0x0000000000000033
            fs = 0x0000000000000000
            gs = 0x0000000000000000
            ss = 0x000000000000002b
            ds = 0x0000000000000000
            es = 0x0000000000000000
    ```
- gdb
  - `info registers`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### \(8\) break point を解除し、 f が終了するように finish コマンドを使う

- lldb
  - `breakpoint delete 1`を実行してbreakpoint\(1\)を削除する
  - `finish`を実行して、現在のフレームから出る
- gdb
  - `delete 1`
  - `finish`
  - lldbで対応するコマンドを入力したときの結果と内容が同じであることを確認

---

### option

mとnの値を変更することが可能かどうかを調べて試してみよ。

- lldb
  - `breakpoint set --file 3_1.c --line 24`を実行
  - `run 100 200`を実行
  - `expr n = 2`を実行
  - `expr m = 3`を実行
  - `breakpoint delete 4`を実行
  - `continue`を実行
  - `Ctrl-d`でlldbを終了させる
  - 生成される3_1.txtには0が2\*3=6個あると考えられる
  - 3_1.txtの中身を確認
    - ```
      000000
      ```
  - mとnの値を変更することは可能であるとわかった
- gdb
  - `break 3_1.c:24`
  - `run 100 200`
  - `expr n = 2`
  - `expr m = 3`
  - `delete 4`
  - `continue`
  - `Ctrl-d`

---

fとgは、clang/gccの最適化によって、どのようなassemblerになるかを調べよ。

- lldb
  - `disassemble --name f`を実行
    - ```
      3_1`f:
      0x5555555551a9 <+0>:  endbr64
      0x5555555551ad <+4>:  pushq  %rbp
      0x5555555551ae <+5>:  movq   %rsp, %rbp
      0x5555555551b1 <+8>:  subq   $0x20, %rsp
      0x5555555551b5 <+12>: movl   %edi, -0x14(%rbp)
      0x5555555551b8 <+15>: movl   %esi, -0x18(%rbp)
      0x5555555551bb <+18>: movl   $0x0, -0x4(%rbp)
      0x5555555551c2 <+25>: jmp    0x11e1                    ; <+56> at 3_1.c:8:22
      0x5555555551c4 <+27>: movl   -0x18(%rbp), %eax
      0x5555555551c7 <+30>: movl   $0x1, %edx
      0x5555555551cc <+35>: leaq   0xe31(%rip), %rcx
      0x5555555551d3 <+42>: movq   %rcx, %rsi
      0x5555555551d6 <+45>: movl   %eax, %edi
      0x5555555551d8 <+47>: callq  0x1080                    ; ___lldb_unnamed_symbol47
      0x5555555551dd <+52>: addl   $0x1, -0x4(%rbp)
      0x5555555551e1 <+56>: movl   -0x4(%rbp), %eax
      0x5555555551e4 <+59>: cmpl   -0x14(%rbp), %eax
      0x5555555551e7 <+62>: jl     0x11c4                    ; <+27> at 3_1.c:9:9
      0x5555555551e9 <+64>: nop
      0x5555555551ea <+65>: nop
      0x5555555551eb <+66>: leave
      0x5555555551ec <+67>: retq`
      ```
  - `disassemble --name g`を実行
    - ```
      3_1`g:
      0x5555555551ed <+0>:  endbr64
      0x5555555551f1 <+4>:  pushq  %rbp
      0x5555555551f2 <+5>:  movq   %rsp, %rbp
      0x5555555551f5 <+8>:  subq   $0x20, %rsp
      0x5555555551f9 <+12>: movl   %edi, -0x14(%rbp)
      0x5555555551fc <+15>: movl   %esi, -0x18(%rbp)
      0x5555555551ff <+18>: movl   %edx, -0x1c(%rbp)
      0x555555555202 <+21>: movl   $0x0, -0x4(%rbp)
      0x555555555209 <+28>: jmp    0x121e                    ; <+49> at 3_1.c:16:22
      0x55555555520b <+30>: movl   -0x1c(%rbp), %edx
      0x55555555520e <+33>: movl   -0x14(%rbp), %eax
      0x555555555211 <+36>: movl   %edx, %esi
      0x555555555213 <+38>: movl   %eax, %edi
      0x555555555215 <+40>: callq  0x11a9                    ; f at 3_1.c:5:22
      0x55555555521a <+45>: addl   $0x1, -0x4(%rbp)
      0x55555555521e <+49>: movl   -0x4(%rbp), %eax
      0x555555555221 <+52>: cmpl   -0x18(%rbp), %eax
      0x555555555224 <+55>: jl     0x120b                    ; <+30> at 3_1.c:17:9
      0x555555555226 <+57>: nop
      0x555555555227 <+58>: nop
      0x555555555228 <+59>: leave
      0x555555555229 <+60>: retq`
      ```
  - `Ctrl-d`でlldbを終了する
  - `$ gcc -o 3_1 -O1 -g 3_1.c`を実行
  - `$ lldb 3_1`を実行
  - `disassemble --name f`を実行
    - ```
      3_1`f:
      3_1[0x11a9] <+0>:  endbr64
      3_1[0x11ad] <+4>:  testl  %edi, %edi
      3_1[0x11af] <+6>:  jle    0x11ee                    ; <+69> at 3_1.c:11:1
      3_1[0x11b1] <+8>:  pushq  %r13
      3_1[0x11b3] <+10>: pushq  %r12
      3_1[0x11b5] <+12>: pushq  %rbp
      3_1[0x11b6] <+13>: pushq  %rbx
      3_1[0x11b7] <+14>: subq   $0x8, %rsp
      3_1[0x11bb] <+18>: movl   %edi, %ebp
      3_1[0x11bd] <+20>: movl   %esi, %r12d
      3_1[0x11c0] <+23>: movl   $0x0, %ebx
      3_1[0x11c5] <+28>: leaq   0xe38(%rip), %r13
      3_1[0x11cc] <+35>: movl   $0x1, %edx
      3_1[0x11d1] <+40>: movq   %r13, %rsi
      3_1[0x11d4] <+43>: movl   %r12d, %edi
      3_1[0x11d7] <+46>: callq  0x1080                    ; ___lldb_unnamed_symbol47
      3_1[0x11dc] <+51>: addl   $0x1, %ebx
      3_1[0x11df] <+54>: cmpl   %ebx, %ebp
      3_1[0x11e1] <+56>: jne    0x11cc                    ; <+35> at 3_1.c:9:9
      3_1[0x11e3] <+58>: addq   $0x8, %rsp
      3_1[0x11e7] <+62>: popq   %rbx
      3_1[0x11e8] <+63>: popq   %rbp
      3_1[0x11e9] <+64>: popq   %r12
      3_1[0x11eb] <+66>: popq   %r13
      3_1[0x11ed] <+68>: retq
      3_1[0x11ee] <+69>: retq`
      ```
  - `disassemble --name g`を実行
    - ```
      3_1`g:
      3_1[0x11ef] <+0>:  endbr64
      3_1[0x11f3] <+4>:  testl  %esi, %esi
      3_1[0x11f5] <+6>:  jle    0x122b                    ; <+60> at 3_1.c:19:1
      3_1[0x11f7] <+8>:  pushq  %r13
      3_1[0x11f9] <+10>: pushq  %r12
      3_1[0x11fb] <+12>: pushq  %rbp
      3_1[0x11fc] <+13>: pushq  %rbx
      3_1[0x11fd] <+14>: subq   $0x8, %rsp
      3_1[0x1201] <+18>: movl   %edi, %r12d
      3_1[0x1204] <+21>: movl   %esi, %ebp
      3_1[0x1206] <+23>: movl   %edx, %r13d
      3_1[0x1209] <+26>: movl   $0x0, %ebx
      3_1[0x120e] <+31>: movl   %r13d, %esi
      3_1[0x1211] <+34>: movl   %r12d, %edi
      3_1[0x1214] <+37>: callq  0x11a9                    ; f at 3_1.c:5:22
      3_1[0x1219] <+42>: addl   $0x1, %ebx
      3_1[0x121c] <+45>: cmpl   %ebx, %ebp
      3_1[0x121e] <+47>: jne    0x120e                    ; <+31> at 3_1.c:17:9
      3_1[0x1220] <+49>: addq   $0x8, %rsp
      3_1[0x1224] <+53>: popq   %rbx
      3_1[0x1225] <+54>: popq   %rbp
      3_1[0x1226] <+55>: popq   %r12
      3_1[0x1228] <+57>: popq   %r13
      3_1[0x122a] <+59>: retq
      3_1[0x122b] <+60>: retq`
      ```

clang/gccの最適化したfとgのassemblerの特徴

最適化していない場合と最適化した場合のfとgを比べてみて、instructionに違いがあると気づいた

共通部分や相違点を調べてみた

fの場合

- 共通部分
  - endbr64
  - pushq
  - movq
  - subq
  - movl
  - leaq
  - callq
  - addl
  - cmpl
  - retq
- 最適化なしにしかない
  - jmp
  - nop
  - leave
- 最適化ありにしかない
  - testl
  - jle
  - jne
  - addq
  - popq

gの場合

- 共通部分
  - endbr64
  - pushq
  - subq
  - movl
  - cmpl
  - callq
  - addl
  - retq
- 最適化なしにしかない
  - movq
  - jmp
  - movl
  - jl
  - nop
  - leave
- 最適化ありにしかない
  - testl
  - jle
  - jne
  - addq
  - popq

fとgの最適化なし/ありのAssemblerを見比べたら

最適化ありの関数のAssemblerには
最適化ありには、movq, jmp, movl, jl, nop, leaveが含まれないで、
最適化なしにはない、testl, jle, jne, addq, popqを含むことがわかった

- testl
  - eaxが0か調べる
- jle
  - cmpの左のオペランドが右のオペランド以下だった場合、指定された場所にジャンプする
- jne
  - ゼロフラグ(ZF)がクリア(0)の場合、指定された場所にジャンプ
- addq
  - addq a bならばbにaを足してbに結果を代入するということ
- popq
  - スタックからデータをポップする

自分の力だけでは決定的/本質的な違いがわからない...、今度kono先生にヒントを貰って解いてみたいです

---

最適化されたコードをassembler levelでtraceしてみよう。

今度kono先生にヒントを貰って解いてみたいです、時間に余裕があればやってみたいけど、取り組んでたら1,2日余裕で溶けてしまう...

---

### 3.1の感想

- 没頭して楽しかったけど時間溶かしたwww
- ideのパッケージでデバッガ機能みたいなのを使っている人は一回はこういう作業をしたほうがいいのではないか?
- lldbとgdbどちらもログを取らなければいけないのが、時間を食う要因かな
  - [lldb.llvm.org](https://lldb.llvm.org/use/map.html)便利すぎる
- C言語で書いている処理はかなり高レベルのことをやっていて、システムコールレベル並の低レベルの処理を意識して書いてこなかったから、面白い勉強になった
- コンパイラ言語で生成した実行ファイルのデバッグする作業は初めてだった、面白い
- 面白いけど、期末らへんにやる課題じゃないかなwww
- 3.1終わらせたら2.6が比較的簡単にできそう

---
