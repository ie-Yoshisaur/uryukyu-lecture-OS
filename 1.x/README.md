Sの講義の1.xの課題を解く

---

## 1.1 英語の教科書の使い方

---

### 問題

解く問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/198.html)の以下のもの。

```
英語の教科書の項目について

   (1) 日本語Wikipedia
   (2) 英語版Wikipedia
   (3) 教科書の内容

が異なっているものを探し出し、考察せよ 面白いものを持ってくること。

考察は、日本語のものと英語のもの教科書をそれぞれ必要な分だけ引用して比較する。

面白いものとは差が大きいだけでなく、項目の内容の面白さ、新しさに依存する
```

---

### 解く方針

使用している教科書は最近の2019年11月に新版である第5版が発行されている。

旧版(2014年3月出版)と新版の中身の差をリストアップしていくと自然と面白い話題が見つかりそう。

[教科書のAmazonのページ](https://www.amazon.co.jp/dp/B085B1W656)の説明を読むと新版に追加した項目について以下のような記述があった。

```
NEW TO THE FIFTH EDITION

- Includes the details on Windows 7, 8 and 10
- Describes an Instructional Operating System (PintOS), FEDORA and Android
```

OSにはFEDORAをVirtualBoxでインストールする課題があるので、先程の新版に追加された記述と合わせて考えて「FEDORA」について考察するという方針で良さそう。

---

### 実際に読む

日本語/英語のWikipedia、教科書のFEDORAに関する説明を斜め読みしてみた結果、[日本語のWikipedia](https://ja.wikipedia.org/wiki/Fedora)と教科書では「リポジトリ」について共通して解説されていて、[英語のWikipedia](https://en.wikipedia.org/wiki/Fedora_Linux)と教科書では「セキュリティ」について共通して解説されていたので、FEDORAのリポジトリとセキュリティについての記述について、内容の差を考察することにする。日本語のWikipedia、英語のWikipedia、教科書で共通して書かれている内容を見つけることは難しかったので断念した。

---

- リポジトリ
  - 日本語のWikipedia
    - 初めから利用できる公式のリポジトリの他に、RPM FusionやLivna等のコミュニティによって運営されているリポジトリ、アドビやDropboxなどのサードパーティー製リポジトリもある
    - 紹介されているリポジトリの種類
      - Fedora Core
        - 中核のパッケージはFedora Coreで提供している
      - Fedora Extras
        - 追加のパッケージはFedora Extrasで問供している
        - 従来は非公式なリポジトリで提供されていたパッケージがFedora Extrasで提供されていた
        - 当初はFedora Coreしかリポジトリ登録されていなかったが、Fedora Core 3以降はFedora Extrasも利用可能となった
        - Fedora Core 4以降はYumにデフォルトで登録され、インストールすればすぐ利用できるようになっていた
        - Fedora 7でCoreと統合され、同時に名称が現在のFedoraに変更された
      - RPM Fusion
        - Core統合後もFedoraの「フリーソフトウェア精神」に反する、あるいはアメリカ国内法に違反する恐れがあるために収録が見送られた以下を含むコミュニティベースのリポジトリが統合されたもの
          - Dribble
          - Freshrpms
          - Livna
        - オープンソースの「free」とそれ以外の「nonfree」に分かれてメンテナンスされている
          - nonfree
            - 主にGPUドライバなどのプロプライエタリソフトウェア、またはMP3や動画再生関連のライブラリなどが収録されている
  - 教科書の内容(著作権の関係で和訳+要約する)
    - リポジトリのデジタルアセットは、整形されたXMLで保存される
    - 特定のコンテンツは、MIMEオブジェクトのインスタンスとして伝達される
    - コンテンツは使用ポリシーに則り、APIによって管理されている
    - FEDORAのリポジトリは、アクセスのための強力なオーセンティケーションを持ち、かつ、リポジトリの開発と発展の両方をサポートする十分な柔軟性を提供しなければならない

---

- セキュリティ
  - 英語のWikipedia
    - Fedora uses Security-Enhanced Linux by default, which implements a variety of security policies, including mandatory access controls, which Fedora adopted early on.
      - Fedoraは標準でSecurity-Enhanced Linuxを採用しており、Security-Enhanced Linuxは、当初Fedoraが採用していたmandatory access controlsを含む様々なセキュリティポリシーを実装している
    - Fedora provides a hardening wrapper, and does hardening for all of its packages by using compiler features such as position-independent executable (PIE).
      - Fedoraは堅牢化用ラッパーを使用しており、PIE(position-independent executable)などのコンパイラ機能を使用することでFedoraの全てのパッケージを堅牢化している
  - 教科書の内容(著作権の関係で和訳+要約する)
    - FEDORAで提供されている主なセキュリティ機能
      - ファイアウォール
      - SELinuxの機能であるMAC
        - 開かれているファイル、ソケット通信などを、アプリケーションのプロセスやユーザが監視したり追跡したりすることを防ぐ
      - ファイルおよびディスクの完全な暗号化
      - 仮想化とサンドボックス化
      - カーネルメモリへアクセスの制限
      - No eXecute (NX)
      - 強力なパスワード体制
      - コンパイル時のバッファのチェック
      - VMのリモート管理
      - システムログデーモンの暗号化
      - ブートプロセスへのマルウェアの侵入の防止
      - システムコールとcookieのフィルタリング

---

### Wikipediaと教科書の内容の差とそれについての考察

FEDORAついて

※日本語Wikipediaと英語Wikipediaと教科書で3つ同時に扱っているFEDORAに関する小項目がなかったのでリポジトリ(日本語Wikipediaと教科書)とセキュリティ(英語Wikipediaと教科書)というように2つに分けて考察する

- リポジトリ
  - 日本語Wikipediaの記事は構成として、対象の項目に関する歴史などを解説していた
  - 教科書では、目的(使用ポリシー)を実装するための手段(well-formed XML、MIMEオブジェクト、API)について解説されていた
- セキュリティ
  - 英語Wikipediaと教科書では解説するセキュリティポリシーの数と質が単純に違った
    - 教科書のほうが知識の量と密度が上だった
- 知識の結びつき方
  - 日本語/英語のWikipediaの記事では、FEDORAのポリシーに関する背景(リポジトリを長期間存続させるためのセキュリティポリシー)を説明していないため、独立した知識(リポジトリとセキュリティ)同士を結びつけることが難しかった
  - 教科書ではリポジトリの説明が終わったあとに、「リポジトリの存続のために必要であるもの」という立ち位置で以下のようなセキュリティの説明がされていたので、知識が結びつきやすかった
    - ```
      Supporting the attributes above requires a strong security regime for long duration survival of the repository. It is for this reason, FEDORA security model is highly regarded in the professional circles. Next, we explore FEDORA's security features.
      
      BHATT, PRAMOD CHANDRA P. AN INTRODUCTION TO OPERATING SYSTEMS. PHI LEARNING. 
      ```
- Wikipediaと教科書の使い分け
  - Wikipediaは対象の歴史を知るには適しているが、独立している知識を体系的に結びついた知識へと昇華させるツールとしては適していない
  - 教科書は知識の体系化を意識して書かれているので、項目を順に読むだけでも勉強になる

---

### 1.1の感想

---

まず、日本語/英語Wikipediaと教科書が共通で扱っている項目を探し出すのが大変だった

「教科書のわからないことを解決するためにWikipediaに逃げても意味ないよ」ってことを伝えたい説

---


kono先生の「面白い」や「新しい」と思うものを学部2年の拙い知識で見つける方法を考え出すのが大変だった

新版の追加項目から攻めるのが良いと思った、他にいい方法あれば教えてほしい

---

## 1.2 

---

### 問題

解く問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/232.html)にある。

ざっとまとめると、

```
学科のサーバにloginして、shellをzshに変更する。
shellをzshに変更するときはakatukiを使う。
~/.ssh/configでsshの設定をする。
学科のサーバに側にpassword入力抜きで二重暗号鍵でsshできるようにする。
    ssh amane.ie.u-ryukyu.ac.jp "date ; ls -ld ~"
でpassword を入力することなく、date とlsが実行できることを確認する。
sshのhostnameがtabで補完される様子を示せ。
```

---

### bashよりzshが優れている理由

- zshは置換機能がとにかく優れている
- 色々

---

### 解く方針
- ~/.ssh/configを編集する
- amaneにパスを通す
- Akatukiにログインしてログインシェルを変更する

---

### ~/.ssh/configを編集する

`$ vim ~/.ssh/config`でsshの設定を書く

```
Host amane
  HostName amane.ie.u-ryukyu.ac.jp
  User e2057xx
  Port xxxx
```

こんな感じ

`Host xxxx`はホスト名わざわざ打たなくてもxxxxを代わりに打たなくてもいいようにするやつ

`Port xxxx`でxxxx番のポートでsshするようにする

---

### amaneにパスを通す

`$ ssh-keygen`

で公開鍵と秘密鍵を生成する

~/.ssh/id_rsa.pubと~/.ssh/id_rsaができる

`$ ssh-copyid amane`で公開鍵をamaneに登録する

今まではamaneにsshするときにパスワードが必要だったが、公開鍵をamaneに登録した(「パスを通す」というらしい)ので、sshするときに公開鍵認証方式を使ってパスワードなしでログインすることができるようになった

「え、パスワード打ったほうが安全じゃね」って思う人もいるがそうでもないらしい

パスワード認証方式と公開鍵認証方式についてまとめる

- パスワード認証方式
  - ユーザ名とパスワードを使って認証を行いログインする
  - ユーザ名とパスワードがバレたら、誰でもログインし放題
- 公開鍵認証方式
  - クライアント側で秘密鍵を使って署名を作成してサーバ側が公開鍵を使って検証する
  - 秘密鍵が流出したら、誰でもログインし放題だが流出自体が困難

なんかそういう感じ

---

### Akatukiにログインしてログインシェルを変更する

[Akatuki](http://akatsuki.ie.u-ryukyu.ac.jp/sign_in)にログインする

ログインシェルをbashからzshに変更する

---

### コマンド実行

`$ ssh amane "date ; ls -ld ~"`

結果がこうなればいいはず

```
Fri 05 Nov 2021 04:19:03 AM JST
drwxr-xr-x 8 e2057xx student 75 Nov  4 00:35 /home/student/e20/e2057xx
```

`$ ssh amane "date ; ls -ld ~ ; echo $SHELL"`とやると現在のシェルも見れる

```
Fri 05 Nov 2021 04:23:13 AM JST
drwxr-xr-x 8 e2057xx student 75 Nov  4 00:35 /home/student/e20/e2057xx
/bin/zsh
```

---

### 1.2の感想

「B1の頃シス管やってて良かった」に尽きる

先輩と繋がれたし、Vimにも出会えたし、hugoで[ブログ](https://yoshisaur.net/)とかできて、そのブログをしていたおかげで今の技術系バイトにも出会えたし、シス管のお陰で今があるって感じがある

---

## 1.3 OSの管理する資源とAPI

---

### 問題

解く問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/212.html)にある

```
 遠隔会議ツールZoom を考える。Zoom に関係するコンピュータの

資源と、構成するソフトウェアの要素を複数名前を上げて、それらの関係をわかりやすく図示せよ。
構成要素同士を結ぶ矢印に対応する具体的なAPI名を図に記述し、それに対応するAPI DocumentのURLを示せ。

ここで Web API をあげてはならない。
```

---

### 解く方針

- APIの定義を理解する
- Web会議アプリケーションでユーザが行う操作をリスト化する
  - 作成するリストをリスト1とする
- リスト1の操作を実現するハードウェアの機能をリスト化する
  - 作成するリストをリスト2とする
- リスト1とリスト2の要素間で行われる標準化を担うAPIをリスト化する
  - 作成するリストをリスト3とする
- リスト3を図にする

---

- APIの定義を理解する
  - 一連のクラスや関数のインターフェース
  - httpでjson投げたりするみたいなやつではない
    - それはWeb APIとか言われている

---

- Web会議アプリケーションでユーザが行う操作をリスト化する

- リスト1
  - テキスト入力
  - 音声入力
  - 映像入力

---

- リスト1の操作を実現するハードウェアをリスト化する

- リスト2
  - キーボード
    - テキスト入力に使う
  - マイク
    - 音声入力に使う
  - カメラ
    - 映像入力に使う

---

- リスト1とリスト2の要素間で行われる標準化を担うAPIをリスト化する

- リスト3
  - キーボード
    - usb_kbd
    - [ドキュメント](https://www.kernel.org/doc/html/v4.12/input/input.html#usbkbd)
    - [ソースコード](https://github.com/torvalds/linux/blob/master/drivers/hid/usbhid/usbkbd.c)
    - [参考にしたサイト](https://stackoverflow.com/questions/39911846/source-code-of-keyboard-driver-of-linux)
  - マイク
    - ALSA
    - [ドキュメント](https://www.alsa-project.org/alsa-doc/alsa-lib/)
    - [ソースコード](https://github.com/alsa-project/alsa-lib)
    - [参考にしたサイト](https://unix.stackexchange.com/questions/529646/how-to-find-out-the-internal-microphones-driver-on-a-linux-pc)
  - カメラ
    - V4L2
    - [ドキュメント](https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/v4l2.html)
    - [ソースコード](https://github.com/torvalds/linux/tree/master/drivers/media/v4l2-core)
    - [V4L2について調べてたときに見たサイト](https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/hist-v4l2.html#:~:text=In%20August%201998%20Bill%20Dirks,replacement%20for%20the%20V4L%20API.)
  - システムコールのためのAPI
    - open()
    - [ドキュメント](https://man7.org/linux/man-pages/man2/open.2.html)
    - [ソースコード](https://github.com/torvalds/linux/blob/16f73eb02d7e1765ccab3d2018e0bd98eb93d973/fs/open.c)
    - [参考にしたサイト](https://0xax.gitbooks.io/linux-insides/content/SysCall/linux-syscall-5.html)

---

- リスト3を図にする

[apiの図](https://github.com/e205723/uryukyu-lecture-OS/blob/main/1.x/1.3/api.sv://raw.githubusercontent.com/e205723/uryukyu-lecture-OS/main/1.x/1.3/api.svg)

---

## 1.4 

---

### 問題

解く問題は[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/220.html)にある

```
Fedora35 を自分でインストールした場合のVM imageの大きさ
Ansible で、Wordpressの設定を行い、動作を確認する
```

---

### 作業ログ

---

### VirtualBoxのインストール
- [VirtualBoxのダウンロードページ](https://www.virtualbox.org/wiki/Downloads)から「OS X hosts」を選択してインストーラをダウンロードしてVirturalBoxをインストールする
### Fedora 35のダウンロード
- この[ページ](https://getfedora.org/en/server/download/)からFedora 35をダウンロードする
- GUIでポチポチ操作するのも億劫なので`$ wget https://download.fedoraproject.org/pub/fedora/linux/releases/35/Server/x86_64/iso/Fedora-Server-dvd-x86_64-35-1.2.iso`を実行した
  - wgetコマンドについての[自分のブログの記事](https://yoshisaur.net/study_linux_unix/wget/)があるので、ぜひ見てください
- ターミナルのログ
  - ```
    --2021-12-26 03:36:08--  https://download.fedoraproject.org/pub/fedora/linux/releases/35/Server/x86_64/iso/Fedora-Server-dvd-x86_64-35-1.2.iso
    Resolving download.fedoraproject.org (download.fedoraproject.org)... 2406:da1a:fcb:2f01:f381:af1a:f922:c519, 2406:da18:39f:a01:35a2:d9e9:8164:a209, 2406:da18:39f:a01:8c16:c226:1274:7098, ...
    Connecting to download.fedoraproject.org (download.fedoraproject.org)|2406:da1a:fcb:2f01:f381:af1a:f922:c519|:443... connected.
    HTTP request sent, awaiting response... 302 Found
    Location: https://ftp.yz.yamagata-u.ac.jp/pub/linux/fedora-projects/fedora/linux/releases/35/Server/x86_64/iso/Fedora-Server-dvd-x86_64-35-1.2.iso [following]
    --2021-12-26 03:36:09--  https://ftp.yz.yamagata-u.ac.jp/pub/linux/fedora-projects/fedora/linux/releases/35/Server/x86_64/iso/Fedora-Server-dvd-x86_64-35-1.2.iso
    Resolving ftp.yz.yamagata-u.ac.jp (ftp.yz.yamagata-u.ac.jp)... 2001:df0:25e:e100::3, 2001:df0:25e:e100::2, 133.24.248.19, ...
    Connecting to ftp.yz.yamagata-u.ac.jp (ftp.yz.yamagata-u.ac.jp)|2001:df0:25e:e100::3|:443... connected.
    HTTP request sent, awaiting response... 200 OK
    Length: 2232418304 (2.1G) [application/x-iso9660-image]
    Saving to: ‘Fedora-Server-dvd-x86_64-35-1.2.iso’
    
    Fedora-Server-dvd-x86_64-35-1.2.iso                         100%[==========================================================================================================================================>]   2.08G  4.17MB/s    in 8m 38s  
    
    2021-12-26 03:44:47 (4.11 MB/s) - ‘Fedora-Server-dvd-x86_64-35-1.2.iso’ saved [2232418304/2232418304]
    ```
### Fedora OSをVirtualboxにインストールする
- [このページ](https://itsfoss.com/install-fedora-in-virtualbox/)を参考にした
- 作成するVMの設定
  - Name
    - Fedora
  - Type
    - Linux
  - Version
    - Fedora(64-bit)
  - Memory size
    - 1024MB
  - Hard disk
    - Create a virtual hard disk now
  - File size
    - 8.0GB
  - Hard disk file type
    - VDI(VirtualBox Disk Image)
  - Storage on physical hard disk
    - Fixed size
  - StorageのController IDE
    - 先ほどダウンロードしたisoファイルのパス
- 注意(別に必要ない人もいる)
  - Fedora OSをインストールする前にMacBookのSystem PreferencesのSecurity & PrivacyでAllowボタンをクリックしないとバグを踏む可能性がある
- Startボタンをクリック、しばらくするとインストーラが立ち上がる
- インストーラの設定
  - 言語
    - English
  - Local Standard DIsks
    - ATA VBOX HARDDISK
  - ROOT ACCOUNT
    - Enable root account
      - パスワードを設定
      - Allow root SSH login with passwordにもテェックを入れた
  - CREATE USER
    - ユーザを作る
    - Make this user administratorにテェックを入れる
- Begin InstallationをクリックしてFedoraのインストールを開始する
  - 時間計測をした結果、5分7秒だった
  - インストールが完了したらウィンドウを閉じる
- ディスクを削除する
  - Controller: IDEに設定されているディスクをクリックして、Remove disk from virtual driveをクリック
    - そうしないと再度インストーラを起動してしまう
- 再起動する
  - →マークのStartをクリック
    - ▼をクリックしてHeadless Startを選択して起動するのもアリ
### ssh
- 歯車マークのSetting -> Network -> Advanced -> Port Forwardingに行く
- ポートを割り当てる
  - Guest port 22
  - Host port 4022
- そもそもなぜPort Forwardingの設定をしなければいけないのか
  - ローカルマシンに立てたVMなのでネットワーク的には問題なさそうだが、NATモードで作成されるVMはhostマシンから独立したプライベートネットワークに属する
  - VMのGlobal IPからsshすればいいという発想になるが、hostマシンのグローバルIPと同じものなので外からsshしようとしてもダメ
  - なので、Port Forwardingの指定したポートへの通信が届いたら、それを別のポートに転送する機能を使う必要がある
- `$ ssh localhost -p 4022 -l root ls -al`を実行して、パスワードを入力する
  - ```
    total 24
    dr-xr-x---.  2 root root 114 Dec 26 17:48 .
    dr-xr-xr-x. 17 root root 224 Dec 26 17:44 ..
    -rw-------.  1 root root 797 Dec 26 17:48 anaconda-ks.cfg
    -rw-r--r--.  1 root root  18 Jul 23 20:53 .bash_logout
    -rw-r--r--.  1 root root 141 Jul 23 20:53 .bash_profile
    -rw-r--r--.  1 root root 429 Jul 23 20:53 .bashrc
    -rw-r--r--.  1 root root 100 Jul 23 20:53 .cshrc
    -rw-r--r--.  1 root root 129 Jul 23 20:53 .tcshrc
    ```
### Fedora側にpassword入力抜きでsshできるようにする
- `$ ssh-keygen`を実行する事になっているが、前回のレポートで実行しているのでパス
- 公開鍵をfedera側に置く
  - [学科のサーバの設定](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/232.html)を参照して、ssh-copyidと同じことをする
  - `$ ssh localhost -p 4022 -l root "mkdir ~/.ssh; chmod 700 ~/.ssh "`
  - `$ cat ~/.ssh/id_rsa.pub | ssh localhost -p 4022 -l root "cat >> ~/.ssh/authorized_keys"`
- `$ ssh localhost -p 4022 -l root ls -al`を実行する
  - パスワードを入力することなくVMにsshできた
### Ansibleのinstall
- `$ brew install ansible`を実行する
### Fedora 側に python をinstallする
- `$ sudo dnf install python`
### Ansibleのping
- `$ echo vm ansible_user=root ansible_port=4022 ansible_host=localhost > hosts`を実行する
- `$ ansible vm -i hosts -m ping`を実行する
- 以下のようなメッセージが表示される
  - ```
    vm | SUCCESS => {
        "ansible_facts": {
            "discovered_interpreter_python": "/usr/bin/python3"
        },
        "changed": false,
        "ping": "pong"
    }
    ```
### Ansibleのplaybook
- `$ hg clone http://www.cr.ie.u-ryukyu.ac.jp/hg/Members/anatofuz/wordpress_ansible/`を実行する
- ~/.ssh/configを編集する
  - 以下を追加する
    - ```
      Host test-fedora
        HostName localhost
        Port 4022
        User root
        IdentityFile ~/.ssh/id_rsa
      ```
- `$ cd wordpress_ansible`を実行する
- hostsを編集する
  - user_nameをrootに変える
- `$ ansible-playbook -i hosts main.yml --ask-become-pass`を実行する
- VirtualBoxのPort Forwardingの設定を追加する
  - Guest Port
    - 80
  - Host Port
    - 8080
- ブラウザでhttp://localhost:8080にアクセスする
  - 入力項目を埋めてwordpressのインストールを完了させる
  - `$ curl http://localhost:8080/`を実行する
    - 出てくる文字列が長いので割愛します

---

### 1.4の感想

VM面白い。アナグラさんのmercurialレポジトリに感謝。NATモードで作成されるVMはhostマシンとは独立したプライベートネットワークを持つのも、この課題のはまりどころであり面白味でもあるような感じ。


