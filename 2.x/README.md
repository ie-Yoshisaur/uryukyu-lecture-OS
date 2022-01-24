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
      
      func TestFileWrite(t \*testing.T) {
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
- `$ find ~/workspace -name "\*.c" -or -name "\*.java" -or -name "\*.py" -or -name "\*.go" -exec wc -w {} \+`を実行してみたがgoファイルのワードカウントしか行わなかった
- `$ wc -w $(find ~/workspace -name "\*.c" -or -name "\*.java" -or -name "\*.py" -or -name "\*.go") | sort`を実行した
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
- `$ mdfind -onlyin ~/workspace "kMDItemDisplayName == \*.c || kMDItemDisplayName == \*.java || kMDItemDisplayName == \*.py || kMDItemDisplayName == \*.go" | xargs wc -w | sort`
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
        all_files = list(map(str, Path(path_to_execute_find).rglob('\*.\*')))
        regex_pattern = re.compile(r'.\*\.(c|java|py|go)$')
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

### .zshrcに設定を付け加える

- まず、このページのyomitanはグローバルなsshサーバという感じ、今はchatan、脳内変換して問題を解こうと思う
- あ、なんか.zshrc書かれているー
- amaneでvm作るか
- amaneにssh
  - ホスト名はamane.ie.u-ryukyu.ac.jp
  - chatanを踏み台にする
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

### for文と\*\*を組み合わせた例題を作成して、実行せよ、file commandを使用する

- \*\*ってなんだろう、file commandを指しているのかな
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

問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os02/lecture.html)にある

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

結構重め、正月らへんはこなした課題の数を優先したいのでまた今度!

---

## 2.7 MTBF

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

- [この資料](http://www.ecei.tohoku.ac.jp/hariyama/lecture/dependable/dependable01-2019.pdf)を根拠に問題を解く
- RAID1っていうのは保持するべきデータXを(x1, x1), (x2, x2), ..., (xn, xn)みたいに2つ1組のHDDに同じデータを持たせてるやつ
  - 組の両方のHDDが壊れない限りデータは失われない
- RAID1ってことは、1TBのデータを保存するために2TBの容量を使うことになる
  - 実質使える容量は20TBって事になる
- MTBFや故障率を計算するために式の構成要素を定義する
  - Nはサンプルの総数
  - tは時間(h)
  - S(t)をNのサンプルを動作させている時に、時刻tまでに正常に動作しているサンプル数
  - F(t)を時刻tまでに故障してしまったサンプル数
- 故障率はλとして定義する
  - λ\(t\) = F\(t\)/\(S\(t\)\*t\)
- MTBF=1/λとして定義する
- (1)の最初に求めたい値は予測される1年間の故障台数
- 式の定義と問題文からMTBF = 10^6 = (8760\*(80-F(t)))/F(t)
  - F(t) = 17520/25219 = 0.6947143027... = 0.69
  - 予測される1年間の故障台数は0.69台
- (1)のもう1つ求めたい値は1日のうちにRIAD1の2台目のHDDが壊れてデータが失われる確率
  - Downtimeは1日だから1日以内に1個壊れる確率に40(39)をかければいい
  - λは1時間以内に1個が故障する確率
  - λ = 10^-6
  - λは偶発故障期には一定らしいので、24時間以内に1個が壊れる確率をお求めたければλに24をかければいい
  - しかもRAID1の構成だからHDDが2つ壊れた時、壊れたHDDが保持していたデータが同一である確率を求める必要がある
    - 20/20^2
  - 1日のうちにRIAD1の2台目のHDDが壊れてデータが失われる確率は40\*24\*λ\*39\*24\*λ\*20/20^2 = 0.000000044928 = 0.0000044928% 

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

80TBのうち40TBをCephに使っている。多重度3かつCephに使われている1つのHDDが1TBであると考えると、(40/3)\*\(near-full ratio\) = \(40/3\)\*0.85 = 11.333...

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

## 2.8 df command

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

## 2.9 two factor authentication と password

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
