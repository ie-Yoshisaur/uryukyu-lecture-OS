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

---

## 2.2 File 書き出し速度の測定

---

今回は飛ばしました今度やった時更新します

---

## 2.3 Mercurial での pull request

---

あれ、今ってMercurialって学科のやつ使えたっけ??

2.2やってないとダメそうなのでこれもパス。今度やった時更新します。

---

## 2.4 find / locate / Spotlight

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
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite2/fileWrite2_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite_test.go
      31 /Users/yoshisaur/workspace/lectures/software-development/KonoChat/api/scripts/app.py
      37 /Users/yoshisaur/workspace/lectures/advanced_information_technology/script/ReadPDF.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo/api/scripts/app.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo2/api/scripts/app.py
      38 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite2/fileWrite2.go
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
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite2/fileWrite2_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite_test.go
      31 /Users/yoshisaur/workspace/lectures/software-development/KonoChat/api/scripts/app.py
      37 /Users/yoshisaur/workspace/lectures/advanced_information_technology/script/ReadPDF.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo/api/scripts/app.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo2/api/scripts/app.py
      38 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite2/fileWrite2.go
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
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite2/fileWrite2_test.go
      30 /Users/yoshisaur/workspace/lectures/operating-system/worklog/golang/fileWrite/fileWrite_test.go
      31 /Users/yoshisaur/workspace/lectures/software-development/KonoChat/api/scripts/app.py
      34 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.2/find.py
      37 /Users/yoshisaur/workspace/lectures/advanced_information_technology/script/ReadPDF.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo/api/scripts/app.py
      37 /Users/yoshisaur/workspace/study/vue_sample_todo2/api/scripts/app.py
      38 /Users/yoshisaur/workspace/lectures/operating-system/uryukyu-lecture-OS/2.x/2.1/fileWrite/fileWrite.go
      38 /Users/yoshisaur/workspace/lectures/operating-system/worklog/cp/golang/fileWrite2/fileWrite2.go
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

## 2.5 zsh の使い方

---

### 問題

[このページ](https://xn--kie-bb4b.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/202.html)の問題を解く

```
ssh で remote login した directory あるいは、terminal の複数の画面で最後にいた directory を覚えておくように zshrc に設定を付け加えて、正しく実行されることを確認せよ。
```

---

### 解く方針

- 確か前にいたディレクトリを指す環境変数は$OLDPWDなはず
- ログイン時に最初に実行するファイルがzshの場合、.zshrcなはず

---

### .zshrcに設定を付け加える

- まず、このページのyomitanはグローバルなsshサーバという感じ、今はchatan、脳内変換して問題を解く
- `$ ssh chatan`
