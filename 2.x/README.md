# OSの講義の2.xの課題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 2.1 Golangでプロジェクトを作るGitlLab編 (2022年度)

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/246.html)の問題を解く

```
Golangでプロジェクトを作る
GitlabのCI/CDを使う
```

Gitlabの設定に関しては11.xの記事で詳しく書いているので、スキップ!

---

### Golangをinstall

- `$ brew install go`

---

### 環境変数

- zshrcに以下の設定を追記する
- ```
  export GOPATH=$HOME/src/go
  ```
- 書き終わったら`$ source ~/.zshrc`をしておきましょう

---

### goを走らせる+gitlabに転送+testを書く

- 以下のような構成のファイル・ディレクトリを用意する
```
.
├── .gitlab-ci.yml
├── fileWrite
│   ├── fileWrite.go
│   └── fileWrite_test.go
├── go.mod
└── main.go
```
  - main.go
```
package main

import (
    "fmt"
    "gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-fileWrite/fileWrite"
)

func main() {
    fmt.Println(fileWrite.Hello("Yoshisaur"))
}
```
  - fileWrite/fileWrite.go
```
package fileWrite

import "fmt"

// Hello returns a greeting for the named person.
func Hello(name string) string {
    // Return a greeting that embeds the name in a message.
    message := fmt.Sprintf("Hello, %v. Welcome!", name)
    return message
}
```
  - fileWrite/fileWrite_test.go
```
package fileWrite

import "testing"

func TestFileWrite(t *testing.T) {
    result := Hello("Yoshisaur")
    want := "Hi, Yoshisaur. Welcome!"
    if result != want {
        t.Errorf("fileWrite.Hello() = %q want %q", result, want)
    }
}
```
  - .gitlab-ci.yml
```
image: golang:latest

stages:
  - test
  - build

test:
  stage: test
  script:
    - go fmt $(go list ./... | grep -v /vendor/)
    - go vet $(go list ./... | grep -v /vendor/)
    - go test -race $(go list ./... | grep -v /vendor/)
  tags:
    - container

build:
  stage: build
  script:
    - mkdir -p mybinaries
    - go build -o mybinaries ./...
  artifacts:
    paths:
      - mybinaries
  tags:
    - container
```
- `$ go mod init gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-fileWriteを実行してgo.modというファイルを生成する
- [学科gitlabのos/2022というグループ](https://gitlab.ie.u-ryukyu.ac.jp/os/2022)で「New project」→「Create blank project」→「e205723-fileWrite」という名前でCreate project
  - 公開設定はPublicにしておく
- 作った学科GItLabのプロジェクトに先ほどのファイル群とgo.modをpushする
- レポジトリのActionsというところでテストの結果がみれる
  - テストが失敗しているはず
- fileWrite/fileWrite.goを以下のように編集する
```
package fileWrite

import "fmt"

// Hello returns a greeting for the named person.
func Hello(name string) string {
    // Return a greeting that embeds the name in a message.
    message := fmt.Sprintf("Hi, %v. Welcome!", name)
    return message
}
```
- 変更したファイルをpushすると、テストが成功するようになる
- 失敗・成功バージョンのテストのコンソールは以下のようになった
  - [失敗バージョン](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/jobs/1892)
  - [成功バージョン](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/jobs/1894)

---

## 2.1 Golangでプロジェクトを作る GItHub編 (2021年度) (ACCEPT)

---

### 問題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/233.html)の問題を解く

```
Golangでプロジェクトを作る
Gitlab(今回はGithubを使う)のCI/CDを使う
```

---

### 解く方針

- [超軽量のチュートリアル](https://go.dev/doc/tutorial/getting-started)こなすと楽かも
- Gitlabじゃなく、Githubでもいいらしい
  - GitHubのCI/CDなら[このサイト](https://brunopaz.dev/blog/building-a-basic-ci-cd-pipeline-for-a-golang-application-using-github-actions/)がいいかもしれない(ちょっと古いかも)
- 色々アレンジしすぎてて原型留めてない、rejectされる可能性大になるけど頑張る
  - ダメだったらもう諦める、でもいい経験にはなるかな

---

### Golangをinstall

- `$ brew install go`

---

### 環境変数

- zshrcに以下の設定を追記する
- ```
  export GO111MODULE=on
  export GOROOT=/usr/local/opt/go/libexec
  export GOPATH=/Users/yoshisaur/src/go
  ```
- 書き終わったら`$ source ~/.zshrc`をしておきましょう

---

### goを走らせる+github(gitlab)に転送+testを書く

- 以下のような構成のファイル・ディレクトリを用意する
  - ```
    .
    ├── .github
    │   └── workflows
    │       └── go.yml
    ├── fileWrite
    │   ├── fileWrite.go
    │   └── fileWrite_test.go
    └── main.go
    ```
  - main.go
    - ```
      package main
      
      import (
          "fmt"
          "github.com/e205723/fileWrite/fileWrite"
      )
      
      func main() {
          fmt.Println(fileWrite.Hello("Yoshisaur"))
      }
      ```
  - fileWrite/fileWrite.go
    - ```
      package fileWrite
      
      import "fmt"
      
      // Hello returns a greeting for the named person.
      func Hello(name string) string {
          // Return a greeting that embeds the name in a message.
          message := fmt.Sprintf("Hello, %v. Welcome!", name)
          return message
      }
      ```
  - fileWrite/fileWrite_test.go
    - ```
      package fileWrite
      
      import "testing"
      
      func TestFileWrite(t *testing.T) {
          result := Hello("Yoshisaur")
          want := "Hi, Yoshisaur. Welcome!"
          if result != want {
              t.Errorf("fileWrite.Hello() = %q want %q", result, want)
          }
      }
      ```
  - .github/workflows/go.yml
    - ```
      name: Go
      
      on:
        push:
          branches: 
            - main
      
      jobs:
        test:
          runs-on: ubuntu-latest
          steps:
            - name: Set up Go
              uses: actions/setup-go@v2
              with:
                go-version: 1.17
      
            - name: Check out code
              uses: actions/checkout@v2
      
            - name: Test
              run: go test -v ./fileWrite
      ```
- `$ go mod init fileWrite`を実行してgo.modというファイルを生成する
- fileWriteという名前のレポジトリを作ってそこに先ほどのファイル群とgo.modをpushする
- レポジトリのActionsというところでテストの結果がみれる
  - テストが失敗しているはず
- fileWrite/fileWrite.goを以下のように編集する
  - ```
    package fileWrite
    
    import "fmt"
    
    // Hello returns a greeting for the named person.
    func Hello(name string) string {
        // Return a greeting that embeds the name in a message.
        message := fmt.Sprintf("Hi, %v. Welcome!", name)
        return message
    }
    ```
- 変更したファイルをpushすると、テストが成功するようになる
- 失敗・成功バージョンのテストのコンソールは以下のようになった
  - [失敗バージョン](https://github.com/e205723/fileWrite/runs/4668642744?check_suite_focus=true)
  - [成功バージョン](https://github.com/e205723/fileWrite/runs/4668649299?check_suite_focus=true)

---

### 2.1の感想

- 個人的にGolangとCI/CDどっちもやってみたかったので良かったという感じ
- 授業ページが古いままだったからかなり自分流にアレンジして課題をこなしてしまった
  - ACCEPTになってくれ...(手を合わせている)
- Golangのモジュールがアップロードされると、Go言語のモジュールをミラーリングするプロキシサーバにキャッシュが残ってしまってこの課題の手順の再現性を検証するのができなくなってしまった
  - なので先ほどの手順で完璧にこの結果を再現できるかはわからない、誰かこの記事を読みながらやった人は同じ結果になったか教えて欲しい

---

## 2.2 File 書き出し速度の測定

---

### 問題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/219.html)の問題を解く

```
File への書き出し部分を作成する(buffered/ unbuffered)

Buffered かどうかでファイルへの書き出し速度は変わると予測される。
Buffered の影響を測定するのに、もっとも適切なファイルの大きさとbufferの大きさは、どの範囲にするのが良いか?
マインドマップを描いて考察する。

FileWrite に時間の測定をする部分を追加する。OS Xや Linux、SSD や HDD などで測定する。

実験データをグラフにする
PDF A4 1枚程度にまとめる。
LaTeX のソースと、PDFを repositoryにいれる。
書き出したデータファイル、中間ファイルは repository に中にあってはならない。
```

---

### Buffer is 何?

- Buffer\(バッファ\)とは、シンプルに書き込みを高速化する方法だと思って良い
- Golangで回答を実装しているのでGolangのbuffered I/Oで考える
- Golangのbufioは書き込み処理が行われるときに直接`write(2)`システムコールを呼び出しているのではなく、一旦ユーザー空間のメモリ領域に書き込む内容を保存して、保存している内容があるサイズを超えたら`write(2)`システムコールを呼ぶことになっている
- 上記のbufioの仕組みの流れで、書き込む内容を保存する領域を「バッファ」と読んでいる、バッファはユーザー空間にある
- しかし、なぜわざわざバッファに書き込む内容を溜め込んでから`write(2)`システムコールを呼ぶのか
  - これはユーザー空間のメモリ領域に書き込む速度とカーネル空間のページキャッシュに書き込む速度では、ユーザー空間のメモリ領域に書き込む速度が速いからである
- 例がないとイメージしづらいので「0」\(ASCIIなので1バイトの文字列\)を4096回\(=4KB\)追記し続けるGolangのプログラムの実装パターンを考える
  - 1. バッファなしで書き込む、`write(0x3, "0\0", 0x1)`が4096回呼ばれる。
  - 2. 16バイトのバッファありで書き込む、`write(0x3, "0000000000000000\0", 0x10)`が256回呼ばれる
    - 普通はバッファのサイズは4096バイトなのだが、システムコールの引数が非常に長くなって読みづらくなるのでバッファサイズを小さくしている
- どちらの実装パターンが速いだろうか
- ユーザー空間のメモリ領域に書き込む速度をx、 `write(2)`システムコールを呼び出してカーネル空間のページキャッシュに書き込む速度をyとすると
  - 実装パターン1の処理速度は`4096y`で、実装パターン2の処理速度は`4096x + 256y`
- xはyに比べると無視できるほど小さいので\(強引でごめんなさい\)、実装パターン2の方が処理が速いということになる
- 2.2の課題はバッファでどのくらいファイルの書き出しが速くなるかを調べる課題である、早速取り掛かっていく

---

### File への書き出し部分を作成する

2.1の問題と同じレポジトリ\(FileWrite\)と同じレポジトリを使う。

`e205723-filewrite/fileWrite/timeWrite.go`というファイルを作成する。

課題を解き終わった時点でのファイル構成はこのようになった。

```
.
├── experiment
│   ├── csv
│   │   ├── time_results1.csv
│   │   └── time_results2.csv
│   ├── gnuplot
│   │   ├── 1.plt
│   │   ├── 2.plt
│   │   └── run.sh
│   ├── latex
│   │   └── 2_2.tex
│   ├── pdf
│   │   └── 2_2.pdf
│   ├── png
│   │   ├── graph1.png
│   │   ├── graph2.png
│   │   └── mindmap.png
│   ├── svg
│   │   ├── graph1.svg
│   │   ├── graph2.svg
│   │   └── mindmap.svg
│   └── txt
│       └── file.txt
├── fileWrite
│   ├── fileWrite.go
│   ├── fileWrite_test.go
│   └── timeWrite.go
├── go.mod
├── main.go
└── memo.txt
```

`e205723-filewrite/fileWrite/timeWrite.go`の中身は以下の通りである。

```
package fileWrite

import (
    "io"
    "os"
    "time"
    "fmt"
    "bufio"
)

func WriteSingleByte(writer io.Writer) {
    singleByteData := []byte("0")
    writer.Write(singleByteData)
}

func TimeWriteWithoutBuffer(fileByteSize int) int64 {
    filePath := "experiment/txt/file.txt"
    writer, err := os.Create(filePath)
    if err != nil {
        fmt.Println(err)
    }
    defer writer.Close()
    start := time.Now()
    for i := 0; i < fileByteSize; i++ {
        WriteSingleByte(writer)
    }
    executionTime := time.Since(start).Nanoseconds()
    return executionTime
}

func TimeWriteWithBuffer(fileByteSize int, bufferByteSize int) int64 {
    filePath := "experiment/txt/file.txt"
    writer, err := os.Create(filePath)
    if err != nil {
        fmt.Println(err)
    }
    defer writer.Close()
    bufferedWriter := bufio.NewWriterSize(writer, bufferByteSize)
    defer bufferedWriter.Flush()
    start := time.Now()
    for i := 0; i < fileByteSize; i++ {
        WriteSingleByte(bufferedWriter)
    }
    executionTime := time.Since(start).Nanoseconds()
    return executionTime
}
```

`TimeWriteWithoutBuffer`関数はfileByteSizeバイト分の「0」をバッファなしでファイルに書き込む関数である。「0」はASCIIなので1バイト、つまりfileByteSize回0が書き込まれるということになる。

`TimeWriteWithBuffer`関数はfileByteSizeバイト分の「0」をバッファありでファイルに書き込む関数である。

---

### 実験に関する考察

Buffered の影響を測定するのに、もっとも適切なファイルの大きさとbufferの大きさは、どの範囲にするのが良いか?

結論から言うと、bufferの大きさは4096バイト以上、ファイルの大きさはbufferよりも大きいサイズが適切である

Linuxのファイルシステムのetx4はブロックサイズがデフォルトで4096バイトで、それ以下のサイズのバッファサイズで書き出しをしても無駄な切り上げが生じてしまう。ファイルの大きさはbufferのサイズより大きくするとwrite\(2\)システムコールが2回以上実行されるので、bufferの影響が見られる。

OS のファイルAPIにもとづいたMindMapは以下のmarkdownをもとに生成した。

```
# ファイルの書き出しに影響する要素とは
## 1. ファイルに書き出す流れ
### 1.1. プロセス
### 1.2. システムコールインターフェース
- 共通のシステムコールが呼び出される
### 1.3. VFSインターフェース
- 異なるファイルシステムを抽象化
  - ext4
  - FAT
  - isofs
  - NFS
### 1.4. ブロックデバイスインターフェース
- 異なるハードウェアを抽象化
  - ATA
  - Serial ATA
  - SCSI
  - Fibre channel
  - USB Mass Storage
## 2. ファイルの書き出しに影響するOSのファイルAPI
### 2.2. ファイルAPIの呼び出しの流れ(Linux v6.1)
  - 2.2.0. ソース
    - https://github.com/torvalds/linux/tree/v6.1
  - 2.2.1. write(2)
    - fs/read_write.c#L646
    - writeシステムコールがSYSCALL_DEFINEマクロで定義されている
    - 関数内でksys_write()が呼ばれている
  - 2.2.2. ksys_write()
    - fs/read_write.c#L626
    - writeシステムコールの実装がされている
    - 関数内でvsf_write()が呼ばれている
  - 2.2.3. vfs_write()
    - fs/read_write.c#L564
    - 各ファイルシステムが定義しているwrite操作を実行する
    - 関数内でnew_sync_write()が呼ばれている
  - 2.2.4. new_sync_write()
    - fs/read_write.c#L481
    - 関数内でwrite_iter=ext4_file_write_iter()が呼ばれている
  - 2.2.5. write_iter(ext4_file_write_iter)
    - fs/ext4/file.c#L686
    - 関数内でext4_buffered_write_iter()が呼ばれている
  - 2.2.6. ext4_buffered_write_iter()
    - fs/ext4/file.c#L270
    - 関数内でgeneric_perform_write()が呼ばれている
  - 2.2.7. generic_perform_write()
    - mm/filemap.c#L3716
    - buffered IOの実行がされている
    - 関数内でwrite_begin=ext4_write_begin()が呼ばれている=ext4のbuffered IOが開始される
### 2.3. ファイルの書き出しに影響する要素の考察
- ext4のブロックサイズは4096バイト
    - 出典: https://github.com/torvalds/linux/blob/master/Documentation/filesystems/fsverity.rst#ext4
    - バッファサイズは4096バイトのn倍(nは自然数)が適切
```

![mind map](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/raw/main/experiment/png/mindmap.png)

---

### 実験データの取得

main.goを以下のような内容に編集する

```
package main

import (
    "encoding/csv"
    "fmt"
    "os"
    "gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-fileWrite/fileWrite"
)

const (
    singleKiloBytes = 1024
)

var (
    fileSizeLimit1 = 8192
    bufferSizeArray1 = []int{1, 2, 4, 8, 16, 32, 64, 128, 256, 512}
    fileSizeLimit2 = 16384
    bufferSizeArray2 = []int{512, 1024, 2048, 4096, 8192, 16384}
)

func generateTimeResultRecords(fileSizeLimit int, bufferSizeArray []int) [][]string {
    // following 4 lines make [][]string slice
    timeResultRecords := make([][]string, fileSizeLimit)
    for i_fileSize := range timeResultRecords {
        timeResultRecords[i_fileSize] = make([]string, len(bufferSizeArray) + 1)
    }

    for i_fileSize := 0; i_fileSize * singleKiloBytes / 8 < fileSizeLimit; i_fileSize++ {
        fileSize := (i_fileSize + 1) * singleKiloBytes / 8
        timeResultRecords[i_fileSize][0] = fmt.Sprintf("%d", fileSize)
        for i_bufferSize, bufferSize := range bufferSizeArray {
            if bufferSize == 1 {
                // if bufferSize is 1 Byte, it can be considered writing without buffer
                timeResultRecords[i_fileSize][i_bufferSize + 1] = fmt.Sprintf("%d", fileWrite.TimeWriteWithoutBuffer(fileSize))
            } else {
                // if bufferSize is not 1 Byte, it can be considered writing with buffer
                timeResultRecords[i_fileSize][i_bufferSize + 1] = fmt.Sprintf("%d", fileWrite.TimeWriteWithBuffer(fileSize, bufferSize))
            }
        }
    }
    return timeResultRecords
}

func saveTimeRecordAsCsv(csvFilePath string, timeResultRecords [][]string) {
    // the result of experiment is saved in the file "time_results.csv"
    csvFile, _ := os.Create(csvFilePath)
    defer csvFile.Close()
    csvWriter := csv.NewWriter(csvFile)
    defer csvWriter.Flush()
    for _, record := range timeResultRecords {
        csvWriter.Write(record)
    }
}

func main() {
    timeResultRecords1 := generateTimeResultRecords(fileSizeLimit1, bufferSizeArray1)
    csvFilePath1 := "./experiment/csv/time_results1.csv"
    saveTimeRecordAsCsv(csvFilePath1, timeResultRecords1)

    timeResultRecords2 := generateTimeResultRecords(fileSizeLimit2, bufferSizeArray2)
    csvFilePath2 := "./experiment/csv/time_results2.csv"
    saveTimeRecordAsCsv(csvFilePath2, timeResultRecords2)
}
```

作業ディレクトリが`e205723-fileWrite`だと想定して作業をする

- GitLabにGolangのコードを上げる
  - `$ git add main.go fileWrite/timeWrite.go`
  - `$ git commit -m "feat: time write"`
  - `$ git push"`
- `$ rsync -avPz ../e205723-filewrite amane:~`
- `$ ssh amane`
- `$ cd ~/e205723-fileWrite`
- `$ go run .`
- `$ exit`
- `$ rsync -avPz amane:~/e205723-fileWrite/experiment/csv/ experiment/csv/`

---

### 実験データをグラフにする

- `./experiment/gnuplot/1.plt`、`./experiment/gnuplot/2.plt`、`./experiment/gnuplot/run.sh`というファイルを作成する
- [ここ](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/tree/main/experiment/gnuplot)にあるファイルのように編集する
- `cd experiment/gnuplot`
- `$ bash run.sh`
- 実験データをグラフ化できる
  - ![graph1](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/raw/main/experiment/png/graph1.png)
  - ![graph2](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/raw/main/experiment/png/graph2.png)

---

### LaTeXでレポートを作成する

ここらへんは「マール描いて、フォイ」って感じで、できたレポートだけ貼っていく

[実験レポート](https://gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-filewrite/-/blob/main/experiment/pdf/2_2.pdf)

関係ないけど、[md-to-pdf](https://github.com/e205723/md-to-pdf)を使えばマークダウンをそのままLaTeXにしてPDFにしてくれる

---

## 2.3 Mercurial での pull request

---

あれ、今ってMercurialって学科のやつ使えたっけ??

2.2やってないとダメそうなのでこれもパス。今度やった時更新します。

---

## 2.4 find / locate / Spotlight (ACCEPT)

---

### 問題

- この[ページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/235.html)の問題を解く

```
find で、これまで書いた c と java と python と go のソースコードをすべて調べて、それぞれの word count を表示せよ。

exec の ; の代わりに + を使うと速い。

mdfind を使って、同じことをやってみよ。
```

グハハハ、もう私はfindコマンドについての[ブログ](https://yoshisaur.net/study_linux_unix/find/)は書いているんだよね〜〜〜!!

---

### find

- スクリプトは必ず~/workplaceというディレクトリに書いているのでworkplaceの中のファイルを調べるという方針でいい
- `$ find ~/workspace -name "*.c" -or -name "*.java" -or -name "*.py" -or -name "*.go" -exec wc -w {} \+`を実行してみたがgoファイルのワードカウントしか行わなかった
- `$ wc -w $(find ~/workspace -name "*.c" -or -name "*.java" -or -name "*.py" -or -name "*.go") | sort`を実行した
  - 第三者に見せられる範囲で得られた結果をまとめてみた
  - ```
      12 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/main.go
      12 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/main.go
      12 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/main.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite/fileWrite_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite_test.go
      31 /Users/yoshisaur/workspace/lectures/software-development/KonoChat/api/scripts/app.py
      37 /Users/yoshisaur/workspace/lectures/advanced_information_technology/script/ReadPDF.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo/api/scripts/app.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo2/api/scripts/app.py
      38 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite.go
      67 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/test/java/jp/ac/uryukyu/ie/e205702_e205723/NeuralNetworkTest.java
      68 /Users/yoshisaur/workspace/lectures/software-development/python_scripts/tweet/twitter.py
      87 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/Main.java
     113 /Users/yoshisaur/workspace/lectures/software-development/python_scripts/tweet/createDataset.py
     118 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Neuron.java
     147 /Users/yoshisaur/workspace/hobby/Home-made-NN/wine.py
     165 /Users/yoshisaur/workspace/hobby/Home-made-NN/iris.py
     167 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Weight.java
     229 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Layer.java
     257 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Neural_Network.java
     431 /Users/yoshisaur/workspace/hobby/Home-made-NN/Neural_Network.py
     505 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/DrawNeuralNetwork.java
     704 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/DataProcess.java
    3440 total
    ```

---

### mdfindを使う

- `$ man mdfind`でコマンドの使い方を見た
- ついでにネット上でmdfindの使い方について調べた
  - [mdfindの使い方](https://ss64.com/osx/mdfind.html)
- `$ mdfind -onlyin ~/workspace "kMDItemDisplayName == *.c || kMDItemDisplayName == *.java || kMDItemDisplayName == *.py || kMDItemDisplayName == *.go" | xargs wc -w | sort`
  - ```
      12 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/main.go
      12 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/main.go
      12 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/main.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite/fileWrite_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite_test.go
      31 /Users/yoshisaur/workspace/lectures/software-development/KonoChat/api/scripts/app.py
      37 /Users/yoshisaur/workspace/lectures/advanced_information_technology/script/ReadPDF.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo/api/scripts/app.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo2/api/scripts/app.py
      38 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite.go
      67 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/test/java/jp/ac/uryukyu/ie/e205702_e205723/NeuralNetworkTest.java
      68 /Users/yoshisaur/workspace/lectures/software-development/python_scripts/tweet/twitter.py
      87 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/Main.java
     113 /Users/yoshisaur/workspace/lectures/software-development/python_scripts/tweet/createDataset.py
     118 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Neuron.java
     147 /Users/yoshisaur/workspace/hobby/Home-made-NN/wine.py
     165 /Users/yoshisaur/workspace/hobby/Home-made-NN/iris.py
     167 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Weight.java
     229 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Layer.java
     257 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Neural_Network.java
     431 /Users/yoshisaur/workspace/hobby/Home-made-NN/Neural_Network.py
     505 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/DrawNeuralNetwork.java
     704 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/DataProcess.java
    3440 total
    ```
  - めっちゃ速い

---

### script言語で find を行う

自分の普段使いの script 言語で同じことをやってみよ。

- Pythonを使って書いてみる
- `$ find.py`というスクリプトを作成して以下のように編集する
  - ```
    import argparse
    import re
    from pathlib import Path
    
    def find(path_to_execute_find):
        all_files = list(map(str, Path(path_to_execute_find).rglob('*.*')))
        regex_pattern = re.compile(r'.*\.(c|java|py|go)$')
        filtered_files = list(filter(regex_pattern.match, all_files))
    
        for file in filtered_files:
            print(file)
    
    if __name__ == '__main__':
        parser = argparse.ArgumentParser()
        parser.add_argument("path_to_execute_find")
        args = parser.parse_args()
        path_to_execute_find = args.path_to_execute_find
        find(path_to_execute_find)
    ```
- `$ python3 find.py <path>`みたいに使う
- `$ python3 find.py ~/workspace | xargs wc -w | sort`を実行したら以下のようになった
  - ```
      12 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/main.go
      12 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/main.go
      12 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/main.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite/fileWrite_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite_test.go
      31 /Users/yoshisaur/workspace/lectures/software-development/KonoChat/api/scripts/app.py
      34 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.2/find.py
      37 /Users/yoshisaur/workspace/lectures/advanced_information_technology/script/ReadPDF.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo/api/scripts/app.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo2/api/scripts/app.py
      38 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite.go
      67 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/test/java/jp/ac/uryukyu/ie/e205702_e205723/NeuralNetworkTest.java
      68 /Users/yoshisaur/workspace/lectures/software-development/python_scripts/tweet/twitter.py
      87 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/Main.java
     113 /Users/yoshisaur/workspace/lectures/software-development/python_scripts/tweet/createDataset.py
     118 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Neuron.java
     147 /Users/yoshisaur/workspace/hobby/Home-made-NN/wine.py
     165 /Users/yoshisaur/workspace/hobby/Home-made-NN/iris.py
     167 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Weight.java
     229 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Layer.java
     257 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/Neural_Network.java
     431 /Users/yoshisaur/workspace/hobby/Home-made-NN/Neural_Network.py
     505 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/DrawNeuralNetwork.java
     704 /Users/yoshisaur/workspace/hobby/Neural_Network_GUI/app/src/main/java/jp/ac/uryukyu/ie/e205702_e205723/DataProcess.java
    3474 total
    ```

---

### option

locate command を使えるようにして、同じことをlocate を使って実現せよ。

- `$ sudo launchctl load -w /System/Library/LaunchDaemons/com.apple.locate.plist`
  - ローカルPCのパスワードを入力する
- `$ cd /usr/libexec/`
  - 後々実行するコマンドのエラー(retrieving current directory)を回避する
- `$ sudo /usr/libexec/locate.updatedb`
  - ローカルPCのパスワードを入力する
- 色々試したけど、自分のユーザディレクトリ以下のファイル・ディレクトリが読み込めない
  - 原因は[このstackoverflow風のサイト](https://apple.stackexchange.com/questions/239410/locate-isnt-finding-a-file-that-exists)に書かれている
  - ```
    The locate database in built by the script /usr/libexec/locate.updatedb. The script is run by the user nobody in this line
    ```
- nobodyっていうユーザがデータベースを立てているので、nobodyが扱えない権限のディレクトリ・ファイルはデータベースに登録されないみたい
  - `$ ls -l /var/db/locate.database`
    - ```
      -r--r--r--  1 nobody  wheel  12618853 Dec 31 07:42 /var/db/locate.database
      ```
  - `$ ls -l ~/..`の結果が`drwxr-x---+ 38 yoshisaur  staff  1216 Dec 31 07:58 yoshisaur`
    - nobody(=other)は僕のホームディレクトリを覗くことができないっていう解釈で合ってる??
- ホームディレクトリの権限を変えればいいのかって感じになる
- だけど、個人用PCとは言えど、なんか権限あたりよく知らないまま触るのよくなさそうなので、ここら辺でyak shaving終わり

---

### 2.4の感想

- 最初は比較的得意分野じゃん!って飛びついてこの課題に取り掛かったけど、色々沼にハマった
- mdfind速い、これから少しはこれ使おう
- localも実行した感じ速い、linuxでも動くらしいのでlinuxディストリビューションのOSいじるときはそれを使ってみるのもアリかなって感じになった
- 体感だけど、local = mdfind > findの順番で速い、ただ正規表現とかオプションの使い勝手はfindの方が良さそう
- 色々掘りすぎて時間かけすぎたよん

---

## 2.5 zsh の使い方 (ACCEPT)

---

### 問題

[このページ](https://xn--kie-bb4b.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/202.html)の問題を解く

```
ssh で remote login した directory あるいは、terminal の複数の画面で最後にいた directory を覚えておくように zshrc に設定を付け加えて、正しく実行されることを確認せよ。
```

---

### .zshrcに設定を付け加える

- まず、このページのyomitanはグローバルなsshサーバという感じ、今はchatan、脳内変換して問題を解こうと思う
- chatanにsshする
- あ、chatanのホームディレクトリに.zshrcがすでに書かれている...
- amaneでvm作るか
- amaneにssh
  - ホスト名はamane.ie.u-ryukyu.ac.jp
  - chatanを踏み台にする
  - ```
    Host amane
      HostName amane.ie.u-ryukyu.ac.jp
      User e2057xx
      Port xxxx
      ProxyJump chatan
    ```
- `$ ie-virsh define hoge -t Ubuntu-20`
- `$ ie-virsh domiflist hoge`
  - Macアドレスを確認する
- [Akatsuki](http://akatsuki.ie.u-ryukyu.ac.jp/sign_in)へgo、ipを申請する
  - [参考](https://ie.u-ryukyu.ac.jp/syskan/service/ip-address/)
- `$ ie-virsh start hoge`
- amaneからログアウト
- `$ ssh ie-user@<VMのIPアドレス>`
  - パスワードはシス管に聞く
  - chatanを踏み台にしないとsshできないようになった
    - \~/.ssh/configに↓のような設定を書くといいかも
```
Host vm
  HostName <ipアドレス>
  User e2057xx
  ProxyJump chatan
```
- ユーザを作って、sudoersに入れて、ie-userは消す(粒度粗くてす見ません)
- ~/.ssh/config書く、鍵通す(ssh-copy-id、もうやったので粒度粗めに)、作ったvmにssh
- `$ sudo apt install zsh`
- `$ sudo chsh -s zsh`
- 一回ログアウト、再度ssh
- .zshrcないぞって言われて色々な選択肢を提示されので作る選択肢を選ぶ
- `$ mkdir hoge`から`$ cd !$`、そのままログアウト、再度ssh
- `$ pwd`
  - `/home/<user>`
  - つまりログインする時にいるディレクトリがホームディレクトリで固定されているということ
- この[講義ページ](https://ie.u-ryukyu.ac.jp/~kono/howto/xterm.html)を頼りに.zshrcに書き込む内容を考える
- 以下の記述を.zshrcに追加する
  - ```
    export dirfile="$HOME/.who.$host.$tty"
    export dirhfile="$HOME/.who.$host"
    
    if [[ ! -f $dirfile ]]; then
        if [[ ! -f $dirhfile ]]; then
            dir=$HOME
        else
            dir=`cat $dirhfile`
        fi
    else
        dir=`cat $dirfile`
        if [[ -d $dir ]]; then
            cd $dir
        else
            dir=$HOME
        fi
    fi
    
    function pushd {
        builtin pushd "$@"
        echo $PWD > $dirfile
        echo $PWD > $dirhfile
    }
    
    function popd {
        builtin popd "$@"
        echo $PWD > $dirfile
        echo $PWD > $dirhfile
    }
    
    function cd {
        builtin cd "$@"
        echo $PWD > $dirfile
        echo $PWD > $dirhfile
    }
    ```
- `$ source .zshrc`
- `$ cd hoge`からログアウト、再度ログインからの`$ pwd`
- `/home/<user>/hoge`になっている
  - 成功

---

### for文と**を組み合わせた例題を作成して、実行せよ、file commandを使用する

- **ってなんだろう、file commandを指しているのかな
  - vm内に以下のような構成のディレクトリとファイルを生成した
    - ```
      .
      ├── hoge0
      │   ├── foo0
      │   └── foo1
      ├── hoge1
      │   ├── foo2
      │   ├── foo3
      │   └── foo4
      ├── hoge2
      │   └── foo5
      ├── hoge3
      │   ├── foo6
      │   ├── foo7
      │   ├── foo8
      │   └── foo9
      └── hoge4
          ├── foo10
          ├── foo11
          └── foo12
      ```
  - ディレクトリ内のファイルをfor文とfile commandを使用して調べる
  - 以下のようなスクリプトを用意した
    - example.sh
      - ```
        echo 'ssh to:'
        read server
        
        directories=($(ssh ${server} "ls ~"))
        
        for ((i=0;i<${#directories[@]};i++))
        do
        ssh ${server} "find ~/${directories[i]} -type f -exec file {} \;"
        done
        ```
  - 実行するとユーザ入力を促すので、sshしたいサーバのホスト名とかを打つといいと思う
  - `$ echo vm | zsh example.sh`
    - ```
      sh to:
      /home/yoshisaur/.who..: ASCII text
      /home/yoshisaur/.who.: ASCII text
      /home/yoshisaur/.ssh/authorized_keys: OpenSSH RSA public key
      /home/yoshisaur/hoge3/foo6: empty
      /home/yoshisaur/hoge3/foo7: empty
      /home/yoshisaur/hoge3/foo8: empty
      /home/yoshisaur/hoge3/foo9: empty
      /home/yoshisaur/.zcompdump: ASCII text
      /home/yoshisaur/hoge2/foo5: empty
      /home/yoshisaur/.sudo_as_admin_successful: empty
      /home/yoshisaur/hoge4/foo10: empty
      /home/yoshisaur/hoge4/foo12: empty
      /home/yoshisaur/hoge4/foo11: empty
      /home/yoshisaur/hoge1/foo4: empty
      /home/yoshisaur/hoge1/foo3: empty
      /home/yoshisaur/hoge1/foo2: empty
      /home/yoshisaur/hoge0/foo0: empty
      /home/yoshisaur/hoge0/foo1: empty
      /home/yoshisaur/.bashrc: ASCII text
      /home/yoshisaur/.zshrc: ASCII text
      /home/yoshisaur/.cache/motd.legal-displayed: empty
      /home/yoshisaur/.bash_history: data
      /home/yoshisaur/.bash_logout: ASCII text
      /home/yoshisaur/.zsh_history: ASCII text
      /home/yoshisaur/.profile: ASCII text
      /home/yoshisaur/hoge0/foo0: empty
      /home/yoshisaur/hoge0/foo1: empty
      /home/yoshisaur/hoge1/foo4: empty
      /home/yoshisaur/hoge1/foo3: empty
      /home/yoshisaur/hoge1/foo2: empty
      /home/yoshisaur/hoge2/foo5: empty
      /home/yoshisaur/hoge3/foo6: empty
      /home/yoshisaur/hoge3/foo7: empty
      /home/yoshisaur/hoge3/foo8: empty
      /home/yoshisaur/hoge3/foo9: empty
      ```
---

### 2.5の感想

- シェルスクリプトを書きまくった...
  - 意外とこんなことできるんだっていう発見が多い
- lsの結果から動的に配列を生成できるのは意外と便利だなって感じがある

---

## 2.6 Cのファイルシステムに関して調べる 

---

問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/192.html)にある

```
Cで

ls -la に相当するプログラムを書く

ls -laR に相当するプログラムを書く

DIR と struct stat はどこで定義されている?

FILE 構造体

debugger の操作

基本的な動作

ポインタの復習

ポインタの演算

dirp
```

---

### Cでls \-la に相当するプログラムを書く

`$ ls -la`を実行してみる

結果

```
total 8
drwxr-xr-x   3 yoshisaur  staff   96 Aug 19 17:44 .
drwxr-xr-x  11 yoshisaur  staff  352 Aug 18 14:09 ..
```

これと同じ結果を出力するCのコードを記述する。

opendirとfstatを使うよう指示されているので、マニュアルをmanコマンドで調べる。

`$ man 3 <hoge>`でライブラリコール(C言語の関数など)のマニュアルを見ることができる。

補足: `$ man 2 <hoge>`はシステムコールのマニュアルを見ることができる。

`$ man 3 opendir`の実行結果で「SYNOPSIS」の項目から`#include <dirent.h>`を入力しなければならないことがわかる。
`$ man 2 fstat`の実行結果で「SYNOPSIS」の項目から`#include <sys/stat.h>`を入力しなければならないことがわかる。
このようにmanコマンドを駆使してls \-laに相当するプログラムを実行する。

`ls-la.c`というls \-laに相当するプログラムを作成した。

GitHub上のソースコードは[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/2.x/2.6/ls-la/ls-la.c)

```
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define ZERO_ARG 1
#define ONE_ARG 2
#define PERMISSION_CHARS_LENGTH 11
#define MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH 6
#define MAXIMUM_USER_NAME_CHARS_LENGTH 256
#define MAXIMUM_GROUP_NAME_CHARS_LENGTH 256
#define MAXIMUM_BLOCK_SIZE_CHARS_LENGTH 20
#define DATE_CHARS_LENGTH 13
#define MAXIMUM_FILE_NAME_CHARS_LENGTH 256

struct DirectoryInfo {
    int num_of_files;
    int index_of_file;
    int total_size;
    int max_num_of_hard_links_chars_length;
    int max_user_name_chars_length;
    int max_group_name_chars_length;
    int max_block_size_chars_length;
};

struct FileInfo {
    char permissions[PERMISSION_CHARS_LENGTH];
    char num_of_hard_links[MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH];
    char user_name[MAXIMUM_USER_NAME_CHARS_LENGTH];
    char group_name[MAXIMUM_GROUP_NAME_CHARS_LENGTH];
    char block_size[MAXIMUM_BLOCK_SIZE_CHARS_LENGTH];
    char last_modified_date[DATE_CHARS_LENGTH];
    char file_name[MAXIMUM_FILE_NAME_CHARS_LENGTH];
};

void print_space(int num_of_space) {
    // print spaces to adjust the indentation of the output

    for (int i_space=0; i_space<num_of_space; i_space++) {
        printf(" ");
    }
}

void print_file_info_array(struct DirectoryInfo directory_info, struct FileInfo *file_info_array) {
    int num_of_space;
    for (int i_file=0; i_file<directory_info.num_of_files; i_file++) {
        printf("%s ", file_info_array[i_file].permissions);
        // the number of spaces before print num_of_hard_links
        num_of_space = directory_info.max_num_of_hard_links_chars_length - strlen(file_info_array[i_file].num_of_hard_links) + 1;
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].num_of_hard_links);
        // the number of spaces before print user_name
        num_of_space = directory_info.max_user_name_chars_length - strlen(file_info_array[i_file].user_name);
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].user_name);
        // the number of spaces before print group_name
        num_of_space = directory_info.max_group_name_chars_length - strlen(file_info_array[i_file].group_name) + 1;
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].group_name);
        // the number of spaces before print block_size
        num_of_space = directory_info.max_block_size_chars_length - strlen(file_info_array[i_file].block_size) + 1;
        print_space(num_of_space);
        printf("%s ", file_info_array[i_file].block_size);
        printf("%s ", file_info_array[i_file].last_modified_date);
        printf("%s\n", file_info_array[i_file].file_name);
    }
}

int main (int argc, char *argv[]) {
    for (int i_dir=0; i_dir<argc; i_dir++) {
        struct dirent *dirent;
        DIR *dir;
        if (argc == ZERO_ARG) {
            char *current_directory = ".";
            dir = opendir(current_directory);
        } else if (i_dir == 0) {
            continue;
        } else {
            dir = opendir(argv[i_dir]);
        }

        if (dir == NULL) {
            printf("opendir ERROR: cannot open directory '%s'\n", argv[1]);
            return 1;
        }
        struct DirectoryInfo directory_info = {0, 0, 0, 0, 0, 0, 0};
        struct FileInfo *file_info_array = malloc(0); // free memory at <1>

        // { a while loop to get the files in the directory
        while ((dirent = readdir(dir)) != NULL) {

            // { read file
            int file;
            char *p_file_name = dirent->d_name;
            int length_of_path = sizeof(argv[i_dir]) + sizeof(p_file_name) + 1;
            char *path_of_file[length_of_path];
            if (argc == ZERO_ARG) {
                sprintf(*path_of_file, "%s/%s", ".", p_file_name);
            } else {
                sprintf(*path_of_file, "%s/%s", argv[i_dir], p_file_name);
            }
            if((file=open(*path_of_file, O_RDONLY)) < -1) {
                printf("open ERROR: null file descriptor\n");
                close(file);
                closedir(dir);
                return 1;
            }
            struct stat fileStat;
            errno = 0;
            if(fstat(file, &fileStat) < 0){
                printf("fstat ERROR: [%s]\n", strerror(errno));
                close(file);
                closedir(dir);
                return 1;
            }
            // } read file

            // { get permissions
            char permissions[PERMISSION_CHARS_LENGTH];
            sprintf(
                permissions,
                "%s%s%s%s%s%s%s%s%s%s",
                (S_ISDIR(fileStat.st_mode)) ? "d" : "-",
                (fileStat.st_mode & S_IRUSR) ? "r" : "-",
                (fileStat.st_mode & S_IWUSR) ? "w" : "-",
                (fileStat.st_mode & S_IXUSR) ? "x" : "-",
                (fileStat.st_mode & S_IRGRP) ? "r" : "-",
                (fileStat.st_mode & S_IWGRP) ? "w" : "-",
                (fileStat.st_mode & S_IXGRP) ? "x" : "-",
                (fileStat.st_mode & S_IROTH) ? "r" : "-",
                (fileStat.st_mode & S_IWOTH) ? "w" : "-",
                (fileStat.st_mode & S_IXOTH) ? "x" : "-"
            );
            // } get permissions

            // { get number of hard links
            char num_of_hard_links[MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH];
            sprintf(num_of_hard_links, "%d", fileStat.st_nlink);
            int num_of_hard_links_chars_length = strlen(num_of_hard_links);
            if (num_of_hard_links_chars_length > directory_info.max_num_of_hard_links_chars_length) {
                directory_info.max_num_of_hard_links_chars_length = num_of_hard_links_chars_length;
            }
            // } get number of hard links

            // { get user name
            struct passwd *pws;
            if((pws=getpwuid(fileStat.st_uid)) == NULL) {
                printf("getpwuid ERROR: cannot get user name\n");
                close(file);
                closedir(dir);
                return 1;
            }
            char user_name[MAXIMUM_USER_NAME_CHARS_LENGTH];
            sprintf(user_name, "%s", pws->pw_name);
            int user_name_chars_length = strlen(user_name);
            if (user_name_chars_length > directory_info.max_user_name_chars_length) {
                directory_info.max_user_name_chars_length = user_name_chars_length;
            }
            // } get user name

            // { get group name
            struct group *grp;
            if ((grp = getgrgid(fileStat.st_gid)) == NULL){
                printf("getgrgid ERROR: cannot get group name\n");
                close(file);
                closedir(dir);
                return 1;
            }
            char group_name[MAXIMUM_GROUP_NAME_CHARS_LENGTH];
            sprintf(group_name, "%s", grp->gr_name);
            int group_name_chars_length = strlen(group_name);
            if (group_name_chars_length > directory_info.max_group_name_chars_length) {
                directory_info.max_group_name_chars_length = group_name_chars_length;
            }
            // } get group name

            // { get block size
            char block_size[MAXIMUM_BLOCK_SIZE_CHARS_LENGTH];
            sprintf(block_size, "%lld", fileStat.st_size);
            int block_size_chars_length = strlen(block_size);
            if (block_size_chars_length > directory_info.max_block_size_chars_length) {
                directory_info.max_block_size_chars_length = block_size_chars_length;
            }
            // } get block size

            // { get last modified date
            char last_modified_date[DATE_CHARS_LENGTH];
            strftime(last_modified_date, DATE_CHARS_LENGTH, "%b %d %H:%M", localtime(&fileStat.st_mtime));
            // } get last modified date

            // { get file name
            char file_name[MAXIMUM_FILE_NAME_CHARS_LENGTH];
            sprintf(file_name, "%s", p_file_name);
            // } get file name

            // { update total size and number of files
            directory_info.total_size += fileStat.st_blocks;
            directory_info.num_of_files += 1;
            // } update total size and number of files

            // { load file information to file_info
            struct FileInfo file_info;
            memcpy(file_info.permissions, permissions, PERMISSION_CHARS_LENGTH);
            memcpy(file_info.num_of_hard_links, num_of_hard_links, MAXIMUM_NUM_OF_HARD_LINKS_CHARS_LENGTH);
            memcpy(file_info.user_name, user_name, MAXIMUM_USER_NAME_CHARS_LENGTH);
            memcpy(file_info.group_name, group_name, MAXIMUM_GROUP_NAME_CHARS_LENGTH);
            memcpy(file_info.block_size, block_size, MAXIMUM_BLOCK_SIZE_CHARS_LENGTH);
            memcpy(file_info.last_modified_date, last_modified_date, DATE_CHARS_LENGTH);
            memcpy(file_info.file_name, file_name, MAXIMUM_FILE_NAME_CHARS_LENGTH);
            file_info_array = realloc(file_info_array, sizeof(file_info) * directory_info.num_of_files);
            file_info_array[directory_info.index_of_file] = file_info;
            // } load file information to file_info

            // { update index of file
            directory_info.index_of_file += 1;
            // } update index of file

            close(file);
        }
        // } a while loop to get the files in the directory

        closedir(dir);

        // { print files with long information
        if (argc > ONE_ARG) {
            printf("%s:\n", argv[i_dir]);
        }
        printf("total %d\n", directory_info.total_size);
        print_file_info_array(directory_info, file_info_array);
        if (i_dir != argc - 1) {
            printf("\n");
        }
        // } print files with long information

        free(file_info_array); // free memory <1>
    }
    return 0;
}
```

`CMakeLists.txt`というファイルを作成した。

GitHub上のソースコードは[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/2.x/2.6/ls-la/CMakeLists.txt)

```
cmake_minimum_required(VERSION 3.24.1)
project(ls-la C)
add_executable(ls-la ls-la.c)
```

プログラムをcmakeで作成するときのcommand sequence + 実行結果

- `$ brew install cmake`
- `$ mkdir build`
- `$ cd build`
- `$ cmake ..`
- `$ cmake --build .`

`$ ./ls-la . ..`の実行結果
```
.:
total 160
drwxr-xr-x   7 yoshisaur  staff    224 Aug 23 22:30 .
drwxr-xr-x   6 yoshisaur  staff    192 Aug 23 22:30 ..
drwxr-xr-x  14 yoshisaur  staff    448 Aug 23 22:30 CMakeFiles
-rw-r--r--   1 yoshisaur  staff   5477 Aug 23 22:30 Makefile
-rw-r--r--   1 yoshisaur  staff   1642 Aug 22 20:15 cmake_install.cmake
-rw-r--r--   1 yoshisaur  staff  13114 Aug 22 20:15 CMakeCache.txt
-rwxr-xr-x   1 yoshisaur  staff  50688 Aug 23 22:30 ls-la

..:
total 88
drwxr-xr-x  6 yoshisaur  staff    192 Aug 23 22:30 .
drwxr-xr-x  4 yoshisaur  staff    128 Aug 21 23:34 ..
-rw-r--r--  1 yoshisaur  staff  28672 Aug 23 22:30 .ls-la.c.swp
-rw-r--r--  1 yoshisaur  staff     86 Aug 21 23:20 CMakeLists.txt
drwxr-xr-x  7 yoshisaur  staff    224 Aug 23 22:30 build
-rw-r--r--  1 yoshisaur  staff   9848 Aug 23 22:30 ls-la.c
```

`$ ls -la . ..`の実行結果
```
.:
total 160
drwxr-xr-x   7 yoshisaur  staff    224 Aug 23 22:30 .
drwxr-xr-x   6 yoshisaur  staff    192 Aug 23 22:30 ..
-rw-r--r--   1 yoshisaur  staff  13114 Aug 22 20:15 CMakeCache.txt
drwxr-xr-x  14 yoshisaur  staff    448 Aug 23 22:30 CMakeFiles
-rw-r--r--   1 yoshisaur  staff   5477 Aug 23 22:30 Makefile
-rw-r--r--   1 yoshisaur  staff   1642 Aug 22 20:15 cmake_install.cmake
-rwxr-xr-x   1 yoshisaur  staff  50688 Aug 23 22:30 ls-la

..:
total 88
drwxr-xr-x  6 yoshisaur  staff    192 Aug 23 22:30 .
drwxr-xr-x  4 yoshisaur  staff    128 Aug 21 23:34 ..
-rw-r--r--  1 yoshisaur  staff  28672 Aug 23 22:30 .ls-la.c.swp
-rw-r--r--  1 yoshisaur  staff     86 Aug 21 23:20 CMakeLists.txt
drwxr-xr-x  7 yoshisaur  staff    224 Aug 23 22:30 build
-rw-r--r--  1 yoshisaur  staff   9848 Aug 23 22:30 ls-la.c
```

totalの表示も出力したり、インデントを合わせるようにした。また、出力のソートは合わせるようには実装しなかった。

---

### Cでls \-laR に相当するプログラムを書く

`$ ls -laR`を実行してみる。

`$ make mkdir -p 1/2/3/4/5/6/7/8/9/A/B`と`$ touch 1/2/3/4/5/6/7/8/9/A/B/hoge.txt`を実行したあとに`$ ls -laR 1`を実行した。

結果

```
total 0
drwxr-xr-x  3 yoshisaur  staff   96 Aug 25 18:27 .
drwxr-xr-x  8 yoshisaur  staff  256 Aug 25 18:43 ..
drwxr-xr-x  3 yoshisaur  staff   96 Aug 25 18:27 2

1/2:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 3

1/2/3:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 4

1/2/3/4:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 5

1/2/3/4/5:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 6

1/2/3/4/5/6:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 7

1/2/3/4/5/6/7:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 8

1/2/3/4/5/6/7/8:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:07 9

1/2/3/4/5/6/7/8/9:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:07 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 A

1/2/3/4/5/6/7/8/9/A:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:07 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 B

1/2/3/4/5/6/7/8/9/A/B:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 ..
-rw-r--r--  1 yoshisaur  staff   0 Aug 25 18:30 hoge.txt

```

これと同じ結果を出力するCのコードを記述する。

`ls-laR.c`というls \-laRに相当するプログラムを作成した。

GitHub上のソースコードは[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/2.x/2.6/ls-laR/ls-laR.c)

ビルドはls-la.cと同じようにbuildディレクトリを作成してその中でビルドした。

`$ ./ls-laR .1`の実行結果

```
2
total 0
drwxr-xr-x  3 yoshisaur  staff   96 Aug 25 18:27 .
drwxr-xr-x  8 yoshisaur  staff  256 Aug 25 18:43 ..
drwxr-xr-x  3 yoshisaur  staff   96 Aug 25 18:27 2

1/2:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 3

1/2/3:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 4

1/2/3/4:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 5

1/2/3/4/5:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 6

1/2/3/4/5/6:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 7

1/2/3/4/5/6/7:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 8

1/2/3/4/5/6/7/8:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:07 9

1/2/3/4/5/6/7/8/9:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:07 .
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 18:27 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 A

1/2/3/4/5/6/7/8/9/A:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:07 ..
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 B

1/2/3/4/5/6/7/8/9/A/B:
total 0
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 //
drwxr-xr-x  3 yoshisaur  staff  96 Aug 25 19:08 ..
-rw-r--r--  1 yoshisaur  staff   0 Aug 25 18:30 hoge.txt
```

1/2/3/4/5/6/7/8/9/A/Bのファイル一覧の表示が\.のかわりに//になっているが、概ね同じ結果になった。

*stackも10個しか格納しないようになっているが\([286行目を参照](https://github.com/e205723/uryukyu-lecture-OS/blob/3d034153af22a38ae261fcb49456da79662600cf/2.x/2.6/ls-laR/ls-laR.c#L286)\)、必要に応じて拡張するようにしているので\([97行目を参照](https://github.com/e205723/uryukyu-lecture-OS/blob/3d034153af22a38ae261fcb49456da79662600cf/2.x/2.6/ls-laR/ls-laR.c#L97)\)、11個目のBのディレクトリまで再帰的にls \-laを実行できていることが確認できた。

---

### DIR と struct stat はどこで定義されている?

`$ clang -E ls-laR.c`を実行

実行結果はかなり長いので割愛

実行結果をDIRで検索を書けた結果、以下のような実行結果の断片を発見した。

```
typedef struct {
 int __dd_fd;
 long __dd_loc;
 long __dd_size;
 char *__dd_buf;
 int __dd_len;
 long __dd_seek;
 __attribute__((__unused__)) long __padding;
 int __dd_flags;
 __darwin_pthread_mutex_t __dd_lock;
 struct _telldir *__dd_td;
} DIR;
# 105 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/dirent.h" 
```

`$ less /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/dirent.h
`を実行し、DIRを探すと上記と同じDIRの定義を発見した。

`$ clang -E ls-laR.c`の実行結果からDIRと同様にstruct statで検索をすると以下のような実行結果の断片を発見した。

```
struct stat { dev_t st_dev; mode_t st_mode; nlink_t st_nlink; __darwin_ino64_t st_ino; uid_t st_uid; gi
d_t st_gid; dev_t st_rdev; struct timespec st_atimespec; struct timespec st_mtimespec; struct timespec
st_ctimespec; struct timespec st_birthtimespec; off_t st_size; blkcnt_t st_blocks; blksize_t st_blksize
; __uint32_t st_flags; __uint32_t st_gen; __int32_t st_lspare; __int64_t st_qspare[2]; };
# 221 "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/stat.h"
```

`$ less /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/stat.h
`を実行し、struct statを探すと上記と同じstruct statの定義を発見した。

---

### FILE 構造体

まだやっていない

---

### debugger の操作

まだやっていない

#### 基本的な動作

まだやっていない

#### ポインタの復習

まだやっていない

#### ポインタの演算

まだやっていない

#### dirp

まだやっていない

---

## 2.7 MTBF (ACCEPT)

問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/236.html)にある

```
(1) 学科のSakuraのサーバは RAID1 構成になっている。容量は 40TB。使える容量は20TB。

40TBが 1TB の HDD x 40、 MTBF 100万時間を仮定して、1年間の故障台数を予測せよ。

Sakuaa は故障予測及び即日入れ換えのサービスが付いてる。(Downtime 1日) 2台目のHDDが壊れてデータが失われる確率はどれくらいか。

(2) 学科のSサーバは Ceph で構成されていて、多重度3になっている。容量は 80TB。半分の40TBをCephに使っている。

Ceph に付いて調べて簡単に説明し、RAID1 と比較せよ。

実際に使える容量は圧縮があるので自明にはわからない。しかし、使える容量を推定してみよ。(option)

Ceaph は多重度3なので二台目が壊れても大丈夫だが...

(3) 三重のバックアップが必要かどうかを議論せよ

(4) 学科のストレージ、学科のさくらクラウド、自分のノートPCを使うとして、現実的なバックアップ戦略を自分の視点から考察せよ。
```

---

### (1) RAIDとMTBFとsakura
- [Understanding Your Product Through Reliability Modeling
](https://www.quanterion.com/understanding-your-product-through-reliability-modeling/)を参考にして問題解く。
- 40TBが 1TB の HDD x 40、 MTBF 100万時間を仮定して、1年間の故障台数を予測せよ。
  - MTBF = 1,000,000 hours
  - HDDの台数 = 40台
  - `(24*365/1000000) * 40`が答え
    - 0.3504台壊れる
    - 5年間同じHDDで学科システムを運用するとその5倍の台数(1.752台)は普通に壊れるので、RAID1でバックアップ戦略とらないと普通にデータ損失する
- HDD故障時に入れ換えのDowntimeが1日で、RAID 1でバックアップを取っている場合、 2台目のHDDが壊れてデータが失われる確率はどれくらいか。
  - HDD故障の発生数がポアソン分布に従うと仮定し、故障率λをλ=1/MTBFとし、単位時間内にHDD1台が無事に稼働し続ける確率Rは以下のように求められる
    - `R＝ e^(-λt)`
  - 問題文から「`λ = 1 / 1000000、t = 24`」である
    - よって、1日でHDD1台が無事に稼働し続ける確率Rは、`R＝ e^(-λt) = e^(-(1/1000000)*24)`
  - 1 - R は「単位時間のうち、HDDが1台壊れる確率」である。
  - 「1日で40台あるHDDのうち1台が壊れる可能性」 = 「稼働台数」 * 「1日でHDDが1台壊れる確率」
    - `(1 - R) * 40 (= (1 - (e^(-(1/1000000) * 24))) * 40)`
  - 「40台のうち1台が壊れたとき、同じ日に39台あるうちのもう一台が壊れる確率」
    - `(1 - R) * 39 (= (1 - (e^(-(1/1000000) * 24))) * 39)`
  - 「2台同じ日に壊れたとき、2台ともRAID1の構成の中で同じデータを保存している確率」
    - `20 / (20^2)`
  - 答えは「1日で40台あるHDDのうち1台が壊れる可能性」*「40台のうち1台が壊れたとき、同じ日に39台あるうちのもう一台が壊れる確率」*「2台同じ日に壊れたとき、2台ともRAID1の構成の中で同じデータを保存している確率」なので、
    - `((1 - (e^(-(1/1000000) * 24))) * 40) * ((1 - (e^(-(1/1000000) * 24))) * 39) * (20 / (20^2))` = `4.49269217e-8` ≒ `0.000000045` = `0.0000045%`

---

### (2) Ceph

- Cephを調べる
  - [LouwrentiusというブログのCephに関する記事](https://louwrentius.com/understanding-ceph-open-source-scalable-storage.html)を参考にする
    - 初めて知った英語や用語
      - software\-defined
        - \(サーバやストレージ、ネットワークなどの物理的なITインフラが\)ソフトウェアによって制御された
      - Commercial off\-the\-shelf\(COTS\)
        - 、既製品で販売やリースが可能となっているソフトウェア製品やハードウェア製品、または一般向けにライセンス提供されるものを採用すること
      - Host Bus Adapter \(HBA\)
        - コンピュータと他のネットワーク機器やストレージ機器を接続するハードウェア
      - JBOD\(Just a Bunch of Disks\)
        - RAIDを構成していないハードディスクの集まり
      - CRUSH
        - Cephが採用しているオブジェクトの識別子をそれが保存されているデバイスにマッピングするアルゴリズム
      - Erasure Coding
        - クラスタ上の有効容量を増加させる手法
        - データを複製する代わりに、パリティ情報を使用して、 ディスク障害が発生した場合にデータを再構築
      - The Object Storage Daemon \(OSD\)
        - ファイルを保存するディスクごとに起動されているデーモンプロセス
        - 対象となるディスクのOSDにファイルの読み書きを依頼する

---

- Cephとは
  - 分散ストレージ機能を提供するソフトウェアのこと
  - 優れた拡張性と高い信頼性が特徴
- Cephの構成要素
  -  Ceph Monitor (ceph-mon)
     - モニターマップ、マネージャーマップ、OSDマップ、MDSマップ、CRUSHマップを含むクラスターの状態マップを保持する
     - Paxosというアルゴリズムで奇数個のモニタが用意される
  -  Ceph Metadata Server (ceph-mds)
     - Ceph File Systemの代わりにメタデータを保存する
  -  Ceph OSD (object storage daemon (ceph-osd))
     - データのレプリケーション、リカバリーなどの処理をするためのデータを保持する
     - 1つのHDDにつき1つのOSDを構成する
     - 他のOSDの状態を確認してモニターやマネージャーに監視状況を提供する
     - OSDの所在はCRUSHアルゴリズムで管理される
- 仕組み
  - Cephではマシンを「ノード」として扱う
  - ノードには複数のOSDがマシンの中のHDDの数分存在している
    - PaxosによってクラスタのOSDの中から1つだけceph-monに置き換わることもある
  - Cephでは複数のノード(=マシン)を使ってデータを分散して保持している

---

CephとRAID1との比較

[この記事](https://www.itprotoday.com/storage/erasure-coding-vs-raid-which-right-and-when)を参考にした

CephのErasure Codingに着目して、CephとRAID1を比較していく。

CephとRAID1はどちらもHDDの故障によるデータの損失を防ぐ点では同じである。
CephのErasure Codingはデータを断片に分割したあとに、エンコードして、分散して冗長化することでデータの損失を防いでいる。RAID1はペアの2個目のHDDに1個目のHDDのデータをコピーしている。

故障したHDDのディスクを再度ビルドするときにかかる時間は、Erasure Codingを使うCephのほうが速い。

パフォーマンス面でCephのほうがRAID1より優れているといえる。

---

オプション: 使える容量を推定する

[cephのドキュメント](https://docs.ceph.com/en/latest/rados/troubleshooting/troubleshooting-osd/)の「NO FREE DRIVE SPACE」を参照した

Cephにはfull ratio\(デフォルト95%\)とnear-full ratio\(デフォルト 85%\)という値が存在し、いずれかのOSDの使用率がfull ratioに達すると、受け付けを停止する。full-ratioの値を上げることができるが、容量が足りなくなってOSDが停止した場合、データを損失する可能性がある。OSDのfull ratioに達さないnear-full ratio以下の使用率に留めるのが懸命である。

80TBのうち40TBをCephに使っている。多重度3かつCephに使われている1つのHDDが1TBであると考えると、(40/3)*\(near-full ratio\) = \(40/3\)*0.85 = 11.333...

よって、使える容量は11.333...TBである

---

### (3) 三重のバックアップの必要性

三重のバックアップのバックアップは必要である。

CephやRAID1の件で、適当な手段でバックアップを取ればデータの損失する確率は低い。しかし、それらの手段はバックアップの作成、リカバリ、取得、保持などの段階、またはその手段においてのリスクを低くするわけではない。データのバックアップを取る上でHDDの故障以外のデータ損失のリスクを少なくするために、三重のバックアップを取る必要がある。

例を使用して説明をする。

---

京都大学のスーパーコンピュータシステムのLustreファイルシステムのファイル消失の重大障害

[参考](https://www.iimc.kyoto-u.ac.jp/services/comp/pdf/file_loss_insident_20211228.pdf)

日本ヒューレット・パッカード合同会社が開発したスパコン用ストレージをバックアップするプログラムの不具合により、京都大学のスーパーコンピュータシステムの大容量ストレージ(/LARGE0)の一部(約77TB)が意図せず削除された。

バックアップのスクリプトにはfindコマンドが使われていて、findコマンドの消去処理に渡す変数名を可読性を高めるために変更を加えたところ、bashの「シェルスクリプトの実行中に適時シェルスクリプトが読み込まれる挙動」によって副作用を起こして、未定義の変数を含むfindコマンドが実行してしまった。

これは推測の範囲ではあるがシェルスクリプトに`set -u`\(errors if an variable is referenced before being se\)を追加していなかったのが原因であると思われる。なにより、この障害は、HDDの故障によって発生したものではない。ソフトウェアによる不備が原因となっている。

---

シス管: サーバ切り替え時のIPアドレスの変更によるCeph MonitorとOSDの接続の切断

[参考: Seeker's eye Ceph 22 Sep 2020](https://seeker-s-eye.blogspot.com/2020/09/ceph.html)

サーバ切り替え時にIPアドレスが変わったが、Ceph Monitorの移行がされなくて、Ceph MonitorとOSDの接続が切れてOSDの中身が見れなくなった。この出来事もHDDの故障によって発生したわけではない。

---

### (4)  学科のストレージ、学科のさくらクラウド、自分のノートPCを使った現実的なバックアップ戦略

自分の場合は、ラップトップPCのデータにのみしか大切なデータが存在しないので自分のラップトップPCのデータの損失を防ぐためのバックアッププランを考える

- amaneでVMを2つ(AとB)用意する
  - A
    - 公開鍵認証方式によるログインのみを受け付ける
    - 秘密鍵以外のバックアップする必要があるファイルを保持させる
  - B
    - 公開鍵を登録せずにパスワード認証方式によるログインのみを受け付ける
    - パスフレーズを設定した秘密鍵\(Permission: 400\)のみを保持させる
      - des3で秘密鍵を暗号化する
      - `$ openssl genrsa -out private.key -des3 2048`で秘密鍵を暗号化することができる
  - AとBに割り当てられるIPアドレスはAkatsukiにログインすればわかる
    - IPアドレスのデータを紛失することは学科のパスワードを忘れない限りない
  - amaneまたはAkatsukiが何らかの理由で運用ができなくなったときに備えて別のバックアップを用意する必要がある
- 学科のさくらクラウド
  - umeのことだと思われるがアクセス権限がシステム管理チームにしかないはず
  - umeが運用しているmattermostにファイルをアップロードするなどがある
    - ただ、秘密鍵などの機密性が高い情報を含むファイルはアップロードできない
  - 学科のさくらクラウド使ったいいバックアップ戦略が思い浮かばないので、自分はSSDでファイル、暗号化した秘密鍵を保存するという戦略を取りたい
- 自分のノートPC
  - Time Machineに対応したNASデバイスにバックアップを作成する

---

### 2.7の感想

MTBFの計算やそれを使った確率の計算をするのは結構楽しかった。
Cephの概要を勉強するとき結構苦戦した。CephめんどくさそうだけどCRUSHやらPaxosやらオリジナリティがあって面白いアルゴリズムと出会えたのでよきかな。

学科のさくらクラウドを使ったいいバックアップ戦略が思いつかない。難しい。

---

## 2.8 df command (ACCEPT)

問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/134.html)にある

```
df . を使うと、自分がどのファイルシステム上にいるかを調べることができる。

以下の場所がどこにあるかを df command を使って調べよ。

    Note PC 上の home direcotry
    urasoe 上の home direcotry
    kvm 上の disk image の置き場所
```
---

### Note PC 上の home direcotry

- `$ cd ~`
- `$ df .`

```
Filesystem   512-blocks      Used  Available Capacity iused      ifree %iused  Mounted on
/dev/disk1s5 1953595632 220164864 1672872912    12% 1014435 8364364560    0%   /System/Volumes/Data
```

- `/dev/disk1s5`というファイルシステムにNote PCのhome directoryが存在する

---

### urasoe上のhome directory

urasoeにはsshできないのでchatanのhome directoryの場所を調べる

- `$ ssh chatan`
- `$ cd ~`
- `$ df .`

セキュリティに悪そう?よくわからないが、詳しく書きすぎてもって感じなのでちょっと結果を加工させている

```
Filesystem                                                     1K-blocks       Used  Available Use% Mounted on
<サーバbのIPアドレス>:<ポート番号xxxx>,<サーバcのIPアドレス>:<ポート番号xxxx>,<サーバdのIPアドレス>:<ポート番号xxxx>:/ie-home/ 18432344064 9649704960 8782639104  53% /home
+chatan+e205723
```

\<ポート番号xxxx\>はcephのポート番号かな、予想

---

### kvm 上の disk image の置き場所

vmにsshしてdfコマンドを実行してみる

- `$ ssh vm`
- `$ cd ~`
- `$ df .`

```
Filesystem                  1K-blocks    Used Available Use% Mounted on
/dev/mapper/ubuntu--vg-root   8641944 2554012   5625904  32% /
```

kvm上のdisk imageは`/dev/mapper/ubuntu--vg-root`に置かれている

---

### 2.8の感想

作業自体は簡単?少し自信ない

---

## 2.9 two factor authentication と password (ACCEPT)

問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/217.html)にある

```
gitlab での two factor authentication
スマホに authenticator などの two factor authentication tool を入れて、学科のgitlabで two factor 認証を設定する
(IIJ Slim がおすすめか?)

gitlab の2020 の group に登録する

password 生成スクリプト
自分用の password 生成スクリプトを作り、出力を10個示せ。パスワードには十分な強度をもたせよ。
```

---

### gitlab での two factor authentication

gitlab使えないのでgithubでtwo factor 認証を設定する、というか認証済みである

---

### password 生成スクリプト

以下のようなPythonスクリプトを書いた

```
import argparse
import secrets
import string

def generate_password(password_length):
    valid_chars_for_password = string.ascii_letters + string.digits + string.punctuation
    password = ''.join(secrets.choice(valid_chars_for_password) for _ in range(password_length))
    print(password)

if __name__=='__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("password_length")
    args = parser.parse_args()
    password_length = int(args.password_length)
    generate_password(password_length)
```

10回パスワードを生成するために書いたシェルスクリプト

```
#!/bin/bash
for i in {1..10}
do
   python3 generate_password.py 12
done
```

↓がシェルスクリプトを実行した結果

```
:XH?xD`$(Ijg
EhjcF|T!xO>i
Ya;Y~Z[FN,#0
j&C/r2~Zwv0'
6*{HJH~~:f~y
3X\cqR<irG{U
aW4P;6l9J\kO
K[=ts"F-|RBx
gC&3G60&Z7[r
bY[gatk`AMUW
```

↓が[PasswordMonster](https://www.passwordmonster.com/)で出力したパスワードが強力であるかどうか調べた結果

| 回目 | 解析にかかる時間 |
| --- | --- |
| 1 | 10億年 |
| 2 | 100万年 |
| 3 | 450億年 |
| 4 | 8570億年 |
| 5 | 190億年 |
| 6 | 180億年 |
| 7 | 50億年 |
| 8 | 110億年 |
| 9 | 20億年 |
| 10 | 200万年 |

十分な強度のパスワードが生成できていることがわかる

---

### 2.9の感想

2段階認証大事だよね、救済コードとかも大事に保管しておこう
パスワードは普段はLinux/UNIXのコマンドを使って生成している、名前とか誕生日は論外だね

---
