# OSの講義の2.xの課題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 2.1 Golangでプロジェクトを作る

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

### IntelliJを使うなら

僕はvim+tmuxで進めるのですが、IntelliJを使うなら環境を整える手順をまとめるとこんな感じになるって紹介しておきます

- [ここ](https://www.jetbrains.com/shop/eform/students)に行く
- 記入欄埋めて(メールアドレスは琉球大学のドメイン使うこと)、APPLY FOR FREE PRODUCTSをクリック
- 確認メールが飛ぶので、届いたメールの確認用URLをクリック
- 規約的なのを読むところに飛ぶ、ちゃんと読みたい人は読んでスクロールダウンしてACCEPTをクリック
- 新しいJetBrainのアカウントにログインする
- [IntelliJ IDEAをダウンロードするページ](https://www.jetbrains.com/idea/download/#section=mac)に行く
- Ulitmate版をダウンロードする
- インストーラをダウンロードしたら、dmgファイルをクリックしてIntelliJ IDEAを「アプリケーション」にドラッグアンドロップする
- IntelliJ IDEAを開く
- エディタの中でJetBrainのアカウントにログイン
- golangのpluginを入れる
  - おすすめplugin
    - IdeaVim
      - 名前からこのpluginが何を意味するのかわかってるよね、無論インストール必須
    - Go Linter
      - Linterってやつは潜在的にバグとなりうるかもしれないコードや規則から外れた書き方のコードのテェックをしてくれるやつ
    - Goctl
      - コードフォーマット(コード整形)をサポートしてくれる
    - Gopher
      - プログレスバーにGopherのアイコンが出てくるらしい、可愛いかも?

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
          fmt.Println(fileWrite2.Hello("Yoshisaur"))
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
