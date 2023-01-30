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

- 没頭して楽しかったけど時間溶かした
- ideのパッケージでデバッガ機能みたいなのを使っている人は一回はこういう作業をしたほうがいいのではないか?
- lldbとgdbどちらもログを取らなければいけないのが、時間を食う要因かな
  - [lldb.llvm.org](https://lldb.llvm.org/use/map.html)便利すぎる
- C言語で書いている処理はかなり高レベルのことをやっていて、システムコールレベル並の低レベルの処理を意識して書いてこなかったから、面白い勉強になった
- コンパイラ言語で生成した実行ファイルのデバッグする作業は初めてだった、面白い
- 3.1終わらせたら2.6が比較的簡単にできそう

---

## 3.2 システムコール

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/238.html)の問題を解く

まだ倒せていない、ただ作業メモ貼っとく
```
$ ssh amane
$ ie-virsh templates
  VMのイメージテンプレートを確認、Fedora36-Debugがあることを確認
$ ie-virsh define -g -t Fedora36-Debug os-3-2
  差分 image を作成する
$ ls -lh /ie-ryukyu/kvm/images/rental/e205723*
  差分の大きさを確認する
  -rw-r--r-- 1 root         root 193K Dec 17 12:16 /ie-ryukyu/kvm/images/rental/e205723-os-3-2.qcow2
  元のイメージと193Kの差分がある

UEFI (grub) メニューの上から二つめのkernelを選んで起動する
  「Fedora Linux (5.10.0+) 36 (Server Edition Prerelease)」を選ぶ
  UEFIメニューが開いてから5秒以内にメニューを操作しないと、自動で一番上のkernelが選ばれて起動される

ctrl+]でconsoleを抜ける

$ ie-virsh dumpxml os-3-2
  <qemu:commandline>
    <qemu:arg value='-S'/>
    <qemu:arg value='-gdb'/>
    <qemu:arg value='tcp::33181'/>
  </qemu:commandline>
33181を覚える

$ singularity shell --bind /home/open/Fedora/Fedora35/kernel:/usr/src/kernels/fedora /ie-ryukyu/singularity/fedora-debug/fedora-debug.sif

singularityに入った
$ cd /usr/src/kernels/fedora
$ gdb vmlinux

gdbに入った
target remote :33181

Remote debugging using :33181
0xffffffff81a740ee in native_safe_halt () at ./arch/x86/include/asm/irqflags.h:60
60              asm volatile("sti; hlt": : :"memory");

---

システムコール

x86_64に関して以下のことを調べる

selectの番号はいくつか?

/usr/src/kernels/fedora/arch/x86/entry/syscalls/syscall_64.tblにシステムコールテーブルが記述されている。
cat /usr/src/kernels/fedora/arch/x86/entry/syscalls/syscall_64.tbl | grep select

23      common  select                  sys_select
270     common  pselect6                sys_pselect6

x86_64のselectのシステムコール番号は23である。

---

ファイルディスクリプタ

fs/read_write.c を読む
  SYSCALL_DEFINE3(read, unsigned int, fd, char __user *, buf, size_t, count)
struct の名称とソースファイル名を示せ

(singularity内、 /usr/src/kernels/fedoraで)
$ find ./include/ -type f -exec grep -l SYSCALL_DEFINE3 {} \;
  ./include/linux/syscalls.h
  ...
```

---

## 3.3 system call trace

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/223.html)の問題を解く

```
Mac OS X または Linux で、いつかのtraceを実行し\(アプリケーションが呼び出すシステムコールの履歴を調べる\)、出力を示せ。
```

---

### Mac OSでシステムコールの履歴を調べる

`$ sudo dtruss ls`

```
SYSCALL(args)            = return
c_env                   docker-compose.yml
access("/AppleInternal/XBS/.isChrooted\0", 0x0, 0x0)             = -1 Err#2
bsdthread_register(0x7FF817BDE020, 0x7FF817BDE00C, 0x2000)           = 1073742303 0
shm_open(0x7FF817AACF5D, 0x0, 0x17AAB7BA)                = 3 0
fstat64(0x3, 0x7FF7B79C31A0, 0x0)                = 0 0
mmap(0x0, 0x2000, 0x1, 0x40001, 0x3, 0x0)                = 0x10864D000 0
close(0x3)               = 0 0
ioctl(0x2, 0x4004667A, 0x7FF7B79C3254)           = 0 0
mprotect(0x108654000, 0x1000, 0x0)               = 0 0
mprotect(0x10865B000, 0x1000, 0x0)               = 0 0
mprotect(0x10865C000, 0x1000, 0x0)               = 0 0
mprotect(0x108663000, 0x1000, 0x0)               = 0 0
mprotect(0x10864F000, 0x90, 0x1)                 = 0 0
mprotect(0x10864F000, 0x90, 0x3)                 = 0 0
mprotect(0x10864F000, 0x90, 0x1)                 = 0 0
mprotect(0x108664000, 0x1000, 0x1)               = 0 0
mprotect(0x108665000, 0x90, 0x1)                 = 0 0
mprotect(0x108665000, 0x90, 0x3)                 = 0 0
mprotect(0x108665000, 0x90, 0x1)                 = 0 0
mprotect(0x10864F000, 0x90, 0x3)                 = 0 0
mprotect(0x10864F000, 0x90, 0x1)                 = 0 0
mprotect(0x108664000, 0x1000, 0x3)               = 0 0
mprotect(0x108664000, 0x1000, 0x1)               = 0 0
issetugid(0x0, 0x0, 0x0)                 = 0 0
getentropy(0x7FF7B79C3050, 0x20, 0x0)            = 0 0
getentropy(0x7FF7B79C30B0, 0x40, 0x0)            = 0 0
getpid(0x0, 0x0, 0x0)            = 86693 0
stat64("/AppleInternal\0", 0x7FF7B79C3770, 0x0)          = -1 Err#2
csops_audittoken(0x152A5, 0x7, 0x7FF7B79C32A0)           = 0 0
proc_info(0x2, 0x152A5, 0xD)             = 64 0
csops_audittoken(0x152A5, 0x7, 0x7FF7B79C3390)           = 0 0
sysctlbyname(kern.osvariant_status, 0x15, 0x7FF7B79C37C0, 0x7FF7B79C37B8, 0x0)                = 0 0
csops(0x152A5, 0x0, 0x7FF7B79C37F4)              = 0 0
mprotect(0x10854B000, 0x100000, 0x1)             = 0 0
open_nocancel("/usr/share/locale/ja_JP.UTF-8/LC_COLLATE\0", 0x0, 0x0) = 3 0
fcntl_nocancel(0x3, 0x3, 0x0)            = 0 0
getrlimit(0x1008, 0x7FF7B79C3CC0, 0x0)           = 0 0
fstat64(0x3, 0x7FF7B79C3C38, 0x0)                = 0 0
read_nocancel(0x3, "1.1A\n\0", 0x1000)           = 2086 0
close_nocancel(0x3)              = 0 0
open_nocancel("/usr/share/locale/ja_JP.UTF-8/LC_CTYPE\0", 0x0, 0x0)  = 3 0
fcntl_nocancel(0x3, 0x3, 0x0)            = 0 0
fstat64(0x3, 0x7FF7B79C3D78, 0x0)                = 0 0
fstat64(0x3, 0x7FF7B79C3B78, 0x0)                = 0 0
lseek(0x3, 0x0, 0x1)             = 0 0
lseek(0x3, 0x0, 0x0)             = 0 0
read_nocancel(0x3, "RuneMagAUTF-8\0", 0x1000)            = 4096 0
read_nocancel(0x3, "\0", 0x1000)                 = 4096 0
read_nocancel(0x3, "\0", 0x1000)                 = 4096 0
read_nocancel(0x3, "\0", 0x1000)                 = 4096 0
read_nocancel(0x3, "\0", 0x1000)                 = 4096 0
read_nocancel(0x3, "\0", 0x1000)                 = 4096 0
read_nocancel(0x3, "\0", 0x1000)                 = 4096 0
read_nocancel(0x3, "@\004\211\0", 0xF5D0)                = 62928 0
close_nocancel(0x3)              = 0 0
open_nocancel("/usr/share/locale/ja_JP.UTF-8/LC_MONETARY\0", 0x0, 0x0)                = 3 0
fstat64(0x3, 0x7FF7B79C3D88, 0x0)                = 0 0
read_nocancel(0x3, "JPY \n\302\245\n.\n,\n3;3\n\n-\n0\n0\n1\n0\n1\n0\n1\n4\n\0", 0x23)                = 35 0
close_nocancel(0x3)              = 0 0
open_nocancel("/usr/share/locale/ja_JP.UTF-8/LC_NUMERIC\0", 0x0, 0x0) = 3 0
fstat64(0x3, 0x7FF7B79C3D88, 0x0)                = 0 0
read_nocancel(0x3, ".\n,\n3;3\n\0", 0x8)                 = 8 0
close_nocancel(0x3)              = 0 0
open_nocancel("/usr/share/locale/ja_JP.UTF-8/LC_TIME\0", 0x0, 0x0)   = 3 0
fstat64(0x3, 0x7FF7B79C3D88, 0x0)                = 0 0
read_nocancel(0x3, " 1\n 2\n 3\n 4\n 5\n 6\n 7\n 8\n 9\n10\n11\n12\n1\346\234\210\n2\346\234\210\n3\346\234\210\n4\346\234\210\n5\346\234\210\n6\346\234\210\n7\346\234\210\n8\346\234\210\n9\346\234\210\n10\346\234\210\n11\346\234\210\n12\346\234\210\n\346\227\245\n\346\234\210\n\347\201\253\n\346\260\264\n\346\234\250\n\351\207\221\n\345\234\237\n\346\227\245\346\233\234\346\227\245\n\346\234\210\346\233\234\346\227\245\n\347\201\253\346\233\234\346\227\245\n\346\260\264\346\233\234\346\227\245\n\346\234\250\346\233\234\346\227\245\n\351\207\221\346\233\234\346\227\245\n\345\234\237\346\233\234\346\227\245\n%H\346\231\202%M\345\210\206%S\347\247\222\n%Y/%m/%d\n%a %b/%e %T %Y\nAM\nPM\n%Y\345\271\264 %B%e\346\227\245", 0x158)              = 344 0
close_nocancel(0x3)              = 0 0
open_nocancel("/usr/share/locale/ja_JP.UTF-8/LC_MESSAGES/LC_MESSAGES\0", 0x0, 0x0)            = 3 0
fstat64(0x3, 0x7FF7B79C3D88, 0x0)                = 0 0
read_nocancel(0x3, "^[yY\357\275\231\357\274\271\343\201\257\343\203\217].*\n^[nN\357\275\216\357\274\256\343\201\204\343\202\244].*\n@$\b\0", 0x28)               = 40 0
close_nocancel(0x3)              = 0 0
ioctl(0x1, 0x4004667A, 0x7FF7B79C42D4)           = 0 0
ioctl(0x1, 0x40087468, 0x7FF7B79C4950)           = 0 0
getuid(0x0, 0x0, 0x0)            = 0 0
fstatat64(0xFFFFFFFFFFFFFFFE, 0x600003098068, 0x7FF7B79C41E8)        = 0 0
open_nocancel(".\0", 0x1000000, 0x0)             = 3 0
fchdir(0x3, 0x0, 0x0)            = 0 0
open_nocancel(".\0", 0x1000000, 0x0)             = 4 0
open_nocancel(".\0", 0x1100004, 0x0)             = 5 0
getattrlistbulk(0x5, 0x7FF7B79C4238, 0x7F79D3008800)             = 2 0
getattrlistbulk(0x5, 0x7FF7B79C4238, 0x7F79D3008800)             = 0 0
close_nocancel(0x5)              = 0 0
fchdir(0x4, 0x0, 0x0)            = 0 0
close_nocancel(0x4)              = 0 0
fstat64(0x1, 0x7FF7B79C3A68, 0x0)                = 0 0
ioctl(0x1, 0x4004667A, 0x7FF7B79C3AB4)           = 0 0
write_nocancel(0x1, "c_env\t\t\tdocker-compose.yml\n\0", 0x1B)       = 27 0
fchdir(0x3, 0x0, 0x0)            = 0 0
close_nocancel(0x3)              = 0 0
```

---

### Linuxでシステムコールの履歴を調べる

vmを使う、[2.xのブログ](https://yoshisaur.hatenablog.jp/entry/2021/12/31/030235)の2.5でvmの作り方が書いてある

vmへssh

- `$ sudo apt-get install auditd`
- `$ sudo autrace /bin/ls`
  - Trace complete. You can locate the records with 'ausearch -i -p 48711'
- `$ sudo ausearch -i -p 48711`
  - 結果は結構長いので一部をカットします
  - ```
    ----                                                                                          [731/825]
    type=PROCTITLE msg=audit(01/23/2022 12:00:51.408:555) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:51.408:555) : arch=x86_64 syscall=close success=yes exit=0 a0=0
    x4 a1=0x0 a2=0x0 a3=0x7f419dc38a10 items=0 ppid=48709 pid=48711 auid=ie-user uid=root gid=root euid=roo
    t suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=1210 comm=autrace exe=/sbin/autrace
    key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:51.412:556) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:51.412:556) : arch=x86_64 syscall=fstat success=yes exit=0 a0=0
    x1 a1=0x7ffcc0017d70 a2=0x7ffcc0017d70 a3=0x7f419dc38a10 items=0 ppid=48709 pid=48711 auid=ie-user uid=
    root gid=root euid=root suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=1210 comm=autr
    ace exe=/sbin/autrace key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:51.412:557) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:51.412:557) : arch=x86_64 syscall=write success=yes exit=28 a0=
    0x1 a1=0x5596f379d4c0 a2=0x1c a3=0x5596f32a61b2 items=0 ppid=48709 pid=48711 auid=ie-user uid=root gid=
    root euid=root suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=1210 comm=autrace exe=/
    sbin/autrace key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:51.412:558) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:51.412:558) : arch=x86_64 syscall=read success=yes exit=1 a0=0x
    3 a1=0x7ffcc0018530 a2=0x1 a3=0x5596f32a61b2 items=0 ppid=48709 pid=48711 auid=ie-user uid=root gid=roo
    t euid=root suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=1210 comm=autrace exe=/sbi
    n/autrace key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:52.412:559) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:52.412:559) : arch=x86_64 syscall=close success=yes exit=0 a0=0
    x3 a1=0x7ffcc0018530 a2=0x1 a3=0x5596f32a61b2 items=0 ppid=48709 pid=48711 auid=ie-user uid=root gid=ro
    ot euid=root suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=1210 comm=autrace exe=/sb
    in/autrace key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:52.412:560) : proctitle=autrace /bin/ls
    type=PATH msg=audit(01/23/2022 12:00:52.412:560) : item=1 name=/lib64/ld-linux-x86-64.so.2 inode=393254
     dev=fd:00 mode=file,755 ouid=root ogid=root rdev=00:00 nametype=NORMAL cap_fp=none cap_fi=none cap_fe=
    0 cap_fver=0 cap_frootid=0
    type=PATH msg=audit(01/23/2022 12:00:52.412:560) : item=0 name=/bin/ls inode=393433 dev=fd:00 mode=file
    ,755 ouid=root ogid=root rdev=00:00 nametype=NORMAL cap_fp=none cap_fi=none cap_fe=0 cap_fver=0 cap_fro
    otid=0
    type=CWD msg=audit(01/23/2022 12:00:52.412:560) : cwd=/home/ie-user
    type=EXECVE msg=audit(01/23/2022 12:00:52.412:560) : argc=1 a0=/bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:52.412:560) : arch=x86_64 syscall=execve success=yes exit=0 a0=
    0x7ffcc00188b7 a1=0x7ffcc0018670 a2=0x7ffcc0018680 a3=0x5596f32a61b2 items=2 ppid=48709 pid=48711 auid=
    ie-user uid=root gid=root euid=root suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=12
    10 comm=ls exe=/bin/ls key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:52.416:561) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:52.416:561) : arch=x86_64 syscall=brk success=yes exit=94653812
    789248 a0=0x0 a1=0x7f02a1bbce13 a2=0x53 a3=0x7f02a1bbce13 items=0 ppid=48709 pid=48711 auid=ie-user uid
    =root gid=root euid=root suid=root fsuid=root egid=root sgid=root fsgid=root tty=pts0 ses=1210 comm=ls
    exe=/bin/ls key=(null)
    ----
    type=PROCTITLE msg=audit(01/23/2022 12:00:52.416:562) : proctitle=autrace /bin/ls
    type=SYSCALL msg=audit(01/23/2022 12:00:52.416:562) : arch=x86_64 syscall=arch_prctl success=no exit=EI
    NVAL(Invalid argument) a0=0x3001 a1=0x7ffc4590c2e0 a2=0x7f02a1bb3230 a3=0x7f02a1bbb8b8 items=0 ppid=487
    09 pid=48711 auid=ie-user uid=root gid=root euid=root suid=root fsuid=root egid=root sgid=root fsgid=ro
    ot tty=pts0 ses=1210 comm=ls exe=/bin/ls key=(null)
    ----  
    ```

---

### 3.3の感想

- アプリケーションが呼ぶシステムコールの履歴を調べるコマンドは3.xの問題では重宝するので積極的に使っていきたい
- Linuxは場合はコマンドをインストールする必要があるので、難易度は少し上がる
  - [この記事](https://linuxhint.com/auditd_linux_tutorial/)を参考にしよう

---

## 3.4

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/237.html)の問題を解く

2021年度はリンクが壊れていて解いてなかったけど、いつか解こうかな

---

## 3.5 プロセスのファイルディスクリプタ

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/073.html)の問題を解く

```
Linux の /proc (proc file system) を使って、特定のプロセスがどのファイルを開いているかを調べてみよう。root でないと見れないプロセスもある。

この機能を使って、WWW serverと Database Server ( postgress や mysql  ) がどのようなファイルにアクセスしているかを調べよ。
```

---

### /proc file system is 何

[tldp.org](https://tldp.org/LDP/Linux-Filesystem-Hierarchy/html/proc.html)を参照する

/procは、「記憶装置にある本当のファイル」を含まない、システムメモリや、マウントされたデバイス、ハードウェア構成などの「カーネルの状態を示す情報を擬似的にファイル」として含むファイルシステムである

---

### /procを使った特定のプロセスがどのファイルを開いているか調べる方法

特定のプロセスがどのファイルを開いているか調べるために`/proc/<process id>/fd/`を見る必要がある

tldp.orgより、
```
/proc/PID/fd

    Directory, which contains all descriptors.
```

descriptorsとは何か、それは操作対象のファイルを識別するために割り当てられる番号のこと

プログラムがファイルを指定して開く操作をすると、カーネル内のファイルテーブルと呼ばれる項目が作成される。そのテーブルで対象のファイルのストレージ上の位置などの情報が記憶される。
descriptorsはそのテーブルないのファイルの識別番号として扱われる。

`$ ls -l /proc/<process id>/fd/`と入力すると一番右にdescriptorsが指すファイルのパスが表示される

これを使って特定のプロセスがどのファイルを開いているのか調べる

---

### WWW serverがどのようなファイルにアクセスしているかを調べる

dockerを使ってnginx(www サーバ)のコンテナを立てる

- `$ docker run -it --privileged --rm -d -p 8080:80 --name web nginx`でwebというコンテナを立てる
  - http://localhost:8080/ をブラウザで開くと文字列が表示される
  - 以下のような文字列が表示されていたらコンテナがwwwサーバとして機能しているいうことになる
    - ```
      Welcome to nginx!
      If you see this page, the nginx web server is successfully installed and working. Further configuration is required.
      
      For online documentation and support please refer to nginx.org.
      Commercial support is available at nginx.com.
      
      Thank you for using nginx.
      ```
- `$ docker exec -it web /bin/bash`でコンテナにログインする
- `$ ls -l /proc | grep nginx`でnginxのプロセスを表示する
  - ```
    dr-xr-xr-x  9 nginx nginx               0 Jan 23 13:52 31
    dr-xr-xr-x  9 nginx nginx               0 Jan 23 13:52 32
    dr-xr-xr-x  9 nginx nginx               0 Jan 23 13:52 33
    dr-xr-xr-x  9 nginx nginx               0 Jan 23 13:52 34
    ```
  - 31から34がnginxのプロセス
- `$ ls -l /proc/3[1-4]/fd/`
  - 結果は長いので一部を抜粋する
  - ```
    /proc/31/fd/:
    total 0
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 0 -> /dev/pts/0
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 1 -> /dev/pts/0
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 10 -> 'anon_inode:[eventpoll]'
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 11 -> 'anon_inode:[eventfd]'
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 12 -> 'anon_inode:[eventfd]'
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 13 -> 'socket:[144708]'
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 14 -> 'socket:[144710]'
    l-wx------ 1 nginx nginx 64 Jan 23 13:52 2 -> /dev/pts/0
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 3 -> /dev/pts/0
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 4 -> 'socket:[144706]'
    l-wx------ 1 nginx nginx 64 Jan 23 13:52 5 -> /dev/pts/0
    l-wx------ 1 nginx nginx 64 Jan 23 13:52 6 -> /dev/pts/0
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 7 -> 'socket:[144702]'
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 8 -> 'socket:[144703]'
    lrwx------ 1 nginx nginx 64 Jan 23 13:52 9 -> 'socket:[144705]'
    ```
- プロセスが開いているファイルをググって調べる
  - `/dev/pts/0`
    - デバイスファイルと呼ばれる
      - xterm, screen, sshなどのプログラムによってエミュレートされたターミナルデバイスを指す
    - ptsはpseudo terminal slaveの頭文字を取っている
  - `'anon_inode:[eventpoll]'`
    - anon_inode
      - inodeはファイル名とファイルの中身以外のデータを持つデータ構造のことを指す
      - anon_inodeはディレクトリのエントリがついていないanonymous inodeを指す
    - eventpoll
      - [linux/fs/eventpoll.cのコード](https://github.com/torvalds/linux/blob/master/fs/eventpoll.c)の172行から229行を参照
      - pollとはselectと同様にdescriptorsのいずれか1つがI/Oを実行可能な状態になるのを待つシステムコール
      - ソースコードの構造体はpoll対象となるdescriptorsを保持するもの
        - rbとか書かいているので赤黒木でkey集合を保存している??そこはわからない
          - これ以上はyak shavingしそうなのでストップ
      - epollとかは、O\(1\)でdescriptorsを監視する
        - 一方、select\(2\)やpoll\(2\)などのposixと呼ばれる規約に則ったシステムコールはO\(n\)で監視する
  - `'socket:[<num>]'`
    - num
      - linuxのソケット通信はファイル操作を使う
        - numがそのinodeを表す

- プロセスが開いているファイルを調べてわかったこと
  - WebサーバはディスクやネットワークなどのI/Oを多重化させることが必要だから、epollといったシステムコールを使っている
  - \(感想に近いけど...\)webサーバを高速化させるには、I/Oを多重化させるシステムコールの知識が必要不可欠な感じがする

---

### Database Serverがどのようなファイルにアクセスしているか調べる

dockerを使ってmysql(databaseサーバ)のコンテナを立てる

- `$ docker run -it --privileged --rm -d --name=database mysql/mysql-server`でdatabaseというコンテナを立てる
- `$ docker exec -it database /bin/bash`でコンテナにログインする
- `$ ls -l /proc | grep mysql`でmysqlのプロセスを表示する
  - ```
    dr-xr-xr-x  9 mysql mysql               0 Jan 24 03:02 1
    ```
- `$ ls -l /proc/1/fd/`
  - ```
    total 0
    lrwx------ 1 root root 64 Jan 24 03:02 0 -> /dev/pts/0
    lrwx------ 1 root root 64 Jan 24 03:02 1 -> /dev/pts/0
    lrwx------ 1 root root 64 Jan 24 03:02 10 -> /var/lib/mysql/#ib_16384_0.dblwr
    lrwx------ 1 root root 64 Jan 24 03:02 11 -> /var/lib/mysql/#ib_16384_1.dblwr
    lrwx------ 1 root root 64 Jan 24 03:02 12 -> /var/lib/mysql/undo_001
    lrwx------ 1 root root 64 Jan 24 03:02 13 -> '/tmp/ibFopFzg (deleted)'
    lrwx------ 1 root root 64 Jan 24 03:06 14 -> /var/lib/mysql/undo_002
    lrwx------ 1 root root 64 Jan 24 03:02 15 -> /var/lib/mysql/ibtmp1
    lrwx------ 1 root root 64 Jan 24 03:02 16 -> /var/lib/mysql/mysql.ibd
    lrwx------ 1 root root 64 Jan 24 03:02 17 -> 'anon_inode:[eventpoll]'
    lr-x------ 1 root root 64 Jan 24 03:02 18 -> 'pipe:[147675]'
    l-wx------ 1 root root 64 Jan 24 03:06 19 -> 'pipe:[147675]'
    lrwx------ 1 root root 64 Jan 24 03:02 2 -> /dev/pts/0
    lrwx------ 1 root root 64 Jan 24 03:06 20 -> 'anon_inode:[eventfd]'
    lrwx------ 1 root root 64 Jan 24 03:06 21 -> 'socket:[147677]'
    lrwx------ 1 root root 64 Jan 24 03:06 22 -> 'socket:[147678]'
    lrwx------ 1 root root 64 Jan 24 03:06 23 -> 'socket:[147680]'
    l-wx------ 1 root root 64 Jan 24 03:06 24 -> /var/lib/mysql/binlog.000002
    lrwx------ 1 root root 64 Jan 24 03:06 25 -> 'socket:[147681]'
    lrwx------ 1 root root 64 Jan 24 03:06 26 -> /var/lib/mysql/#innodb_temp/temp_1.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 27 -> /var/lib/mysql/#innodb_temp/temp_2.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 28 -> /var/lib/mysql/#innodb_temp/temp_3.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 29 -> /var/lib/mysql/#innodb_temp/temp_4.ibt
    lrwx------ 1 root root 64 Jan 24 03:02 3 -> /var/lib/mysql/binlog.index
    lrwx------ 1 root root 64 Jan 24 03:06 30 -> /var/lib/mysql/#innodb_temp/temp_5.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 31 -> /var/lib/mysql/#innodb_temp/temp_6.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 32 -> /var/lib/mysql/#innodb_temp/temp_7.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 33 -> /var/lib/mysql/#innodb_temp/temp_8.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 34 -> /var/lib/mysql/#innodb_temp/temp_9.ibt
    lrwx------ 1 root root 64 Jan 24 03:06 35 -> /var/lib/mysql/#innodb_temp/temp_10.ibt
    lrwx------ 1 root root 64 Jan 24 03:02 4 -> /var/lib/mysql/ib_logfile0
    lrwx------ 1 root root 64 Jan 24 03:02 5 -> '/tmp/ibCdxxti (deleted)'
    lrwx------ 1 root root 64 Jan 24 03:02 6 -> '/tmp/ibZunhQi (deleted)'
    lrwx------ 1 root root 64 Jan 24 03:02 7 -> '/tmp/ib3iQ3Qi (deleted)'
    lrwx------ 1 root root 64 Jan 24 03:02 8 -> /var/lib/mysql/ibdata1
    lrwx------ 1 root root 64 Jan 24 03:02 9 -> /var/lib/mysql/ib_logfile1
    ```
- プロセスが開いているファイルを調べる
  - `/dev/pts/0`
    - Webサーバのプロセスが開いているファイルを調べるときに調査済み
  - `'anon_inode:[eventpoll]'`
    - Webサーバのプロセスが開いているファイルを調べるときに調査済み
  - `'socket:[<num>]'`
    - Webサーバのプロセスが開いているファイルを調べるときに調査済み
  - `'pipe:[<num>]'`
    - pipeは一方方向のデータを送信する経路のこと
      - numがそのinodeを表す
  - `/tmp/ib<random str>(deleted)`
    - MySQLのデータベースエンジンであるInnoDBの一時ファイル
  - `/var/lib/mysql/#ib.dblwr`
    - Double Write Bufferは.dblwrファイルに保存される
      - InnoDBはテーブルスペースへ更新を行う前に、OSが更新の途中でクラッシュしてもいいように、一旦Double Write Bufferに同じデータを書き込む
  - `/var/lib/mysql/ib_logfile<num>`
    - InnoDBのログが保存されているファイル
  - `/var/lib/mysql/ibtmp1`
    - InnoDBがパフォーマンス向上のために一時的にデータを格納するファイル
  - `/var/lib/mysql/#innodb_temp/temp_<num>.ibt`
    - 一時テーブルスペースを格納するファイル
  - `/var/lib/mysql/ibdata1`
    - InnoDBのデータ構造のためのシステムテーブルスペース
    - 管理する情報
      - Table Data Pages
      - Table Index Pages
      - Data Dictionary
      - MVCC Control Data
      - Double Write Buffer
      - Insert Buffer
  - `/var/lib/mysql/mysql.ibd`
    - MySQL\(InnoDB\)で使用されるテーブルとインデックスを格納するファイル
  - `/var/lib/mysql/undo_<num>`
    - undoログを含むundoテーブルスペースを格納するファイル
  - `/var/lib/mysql/binlog.<num>`
    - バイナリログファイル

---

3.5の感想

- 面白い、特にwwwサーバのプロセスが開いているファイルを調べるとき
  - Webサーバの高速化の話でepollがC10K問題に絡んでくるとか知れた

---

