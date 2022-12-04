# OSの講義の11.xの問題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 11.1 VMの課題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/244.htmll)の問題を解く

```
学科サーバ 上の KVM に自分の仮想マシンを作る

二つの方法
KVMのcommand line interfaceである virsh の wrapper ie-virsh を使う
(1) 自分の手元のPCで、仮想マシンのimageを qcow2 で作り、それを upload する
(2) Server(amaneなど)で、Fredoraのdvd imageから boot する

Server上のKVMからの Fedoraのinstall

Level 1
VNC または ssh で仮想マシンへの login し動作を確認する

Level 2
web server を起動し、 外部から curl でアクセスできることを確認する。
Hugoでサイトを作成する

absible
VNCたは virsh console で IPv6を確認して、amane 上とかから password抜きで ssh できるようにする。
```

---

### (2) Server(amaneなど)で、Fredoraのdvd imageから boot する

- amaneにssh
- `$ mkdir /mnt/ie-virsh/e20/e205723` \(amane\)
- `$ qemu-img create -f qcow2 /mnt/ie-virsh/e20/e205723/e205723-os-11-1.qcow2  20G` \(amane\)
  -  `e205723-os-11-1.qcow2`というファイルが先程mkdirで作成したディレクトリに生成される
  - `e205723-os-11-1.qcow2`というファイル名の`os-11-1`の部分に注目
- `$ ie-virsh define os-11-1` \(amane\)
  - `os-11-1`という名前のVMを作成する
  - qcow2ファイルを作成するときに`$ qemu-img create -f qcow2 /mnt/ie-virsh/e20/e205723/e205723-xxxx.qcow2  20G`を実行していたら、`$ ie-virsh define xxxx`と実行しなければならない
- `$ ie-virsh start os-11-1` \(amane\)
  - VMを起動する
- `$ ie-virsh attach-disk os-11-1 /home/open/Fedora/Fedora-Server-dvd-x86_64-37_Beta-1.5.iso` \(amane\)
  - `/home/open/Fedora/Fedora-Server-dvd-x86_64-37_Beta-1.5.iso`というディスクイメージをos-11-1にマウントする
- `$ ie-virsh vncdisplay os-11-1` \(amane\)
  - VMのVNCディスプレイのポートを表示する
  - ```
     uid: ***** gid: **** name: e205723
     :xx(←これが重要、VMごとに違う)
     ```
    - ↑これOS初見なのにエグい実力の後輩が言ってた
- ローカルPCのターミナルを使う
- `$ ssh -L 10024:localhost:59xx -N amane` \(local pc\)
  - amaneの59xxポートをlocalhost\(自分の使っているPCのこと\)の10024にポートフォワーディングする
- VNC Viewerを起動
  - [VNC Viewerをダウンロードする場所](https://www.realvnc.com/en/connect/download/viewer/)
- VNC Viewerを起動したら`localhost:10024`を入力すると、VNCディスプレイが見れる
- uefi shellが立ち上がるので速攻で`exit`を入力する
  - ↓こんな画面になる
  - [f:id:e205723:20221126221924p:plain]
- Boot Managerを十字キーでカーソル操作して選択
- `UEFI QEMU DVD-ROM QM00007`を選択
- `Install Fedora 37`を選択
- インストーラーが起動する
  - ここで`Attempting to reconnect to VNC Server...`と出たら、「Options > General > Picture qualityをMediumとかAutomatic以外にするといける」って優秀な後輩が言ってた、Yoshisaurの環境ではなかった
  - 言語設定をする(英語)
    - [f:id:e205723:20221126224815p:plain]
  - Rootユーザー、ユーザー作成、ディスクの設定をする
    - Rootユーザー
      - Rootユーザーにsshログインできるようにするのはあまりベストではないけどね
      - [f:id:e205723:20221126224531p:plain]
    - ユーザー作成
      - [f:id:e205723:20221126224619p:plain]
    - ディスクの設定
      - [f:id:e205723:20221126224656p:plain]
- Begin InstallationをクリックしてFedoraのインストールを開始する
  - 時間計測をした結果、5分7秒だった
- Rebootする
  - 再起動しなかったらamaneで`$ ie-virsh start os-11-1`を実行
- コンソールログインしたら`ysano`\(ユーザー作成のスクリーンショットを参考\)と入力
- パスワードを入力

---

### Level 1:「VNC または ssh で仮想マシンへの login し動作を確認する」

- 現段階でVNCを使い続けて作業をするとコピーペーストができないので、sshでログインできるようにする
- akatsukiでVMにipを付与してもらう
  - amaneで`$ ie-virsh domiflist os-11-1`を実行してMACアドレスを取得しよう
   - [akatsuki](https://akatsuki.ie.u-ryukyu.ac.jp)に行き、[IPアドレス申請方法](https://ie.u-ryukyu.ac.jp/syskan/service/ip-address/)を参考にVMにipを付与してもらう
- ローカルPCのssh configを書く
  - `$ vim ~/.ssh/config` \(local pc\)
  - 以下を追記
    - ```
       Host fedora
         HostName 10.0.4.125
         User ysano
         ProxyJump amane
     ```
- `$ ssh-copy-id fedora` \(local pc\)
  - 公開鍵認証方式でVMにsshログインできるようにする
- `$ ssh fedora "date; ls -ld" \(local pc\)
  - これが無事に実行できたらLevel 1は大丈夫だと感じる
  - ```
     Sat Nov 26 11:02:44 PM JST 2022
     drwx------. 3 ysano ysano 74 Nov 26 23:01 .
     ```

---

### Level 2: 「web server を起動し、 外部から curl でアクセスできることを確認する」

- [このサイト](https://developer.fedoraproject.org/start/sw/web-app/hugo.html)を参考に作業をする
- VM上で作業をする
- `$ sudo dnf install hugo` \(VM\)
  - hugoのインストール
- `$ hugo new site yoshisaur-blog`\(VM\)
- `$ cd !$`\(VM\)
- `$ git init`\(VM\)
- `$ git submodule add https://github.com/theNewDynamic/gohugo-theme-ananke themes/ananke`\(VM\)
- `$ echo "theme = 'ananke'" >> config.toml`\(VM\)
- `$ hugo server -D`\(VM\)
- ローカルPCで`$ ssh -L 8080:localhost:1313 -N fedora`\(local pc\)
  - fedoraのVMの1313番ポートで配信しているhugoのwebサイトをローカルpcの8080番ポートで見れるようにポートフォワーディング
- `localhost:8080`を見る
- `$ curl localhost:8080 > curl_result.txt`\(local pc\)
  - 最終的には`curl_result.txt`の中身をkono先生のメールに貼り付けて提出する

---

### absible

- 公開鍵認証方式でのsshログインはLevel 1で既に達成している
- `$ brew install ansible`
- `$ echo fedora ansible_ssh_user=ysano ansible_ssh_private_key_file=~/.ssh/id_rsa > hosts`
  - `ysano`の部分は各自のユーザー名
- `$ ansible fedora -i hosts -m ping`
  - ↓が帰ってきたらクリア\(でいいでしょ?\)
  - ```
      fedora | SUCCESS => {
          "ansible_facts": {
              "discovered_interpreter_python": "/usr/bin/python3"
          },
          "changed": false,
          "ping": "pong"
      }
     ```


---

## 11.3 Containerの課題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/215.html)の問題を解く

```
daru上で singularity を使う
GPU があるかどうかを確認する
vimを入れる
rust を入れてみる
daru 上で slum を使う
daru 上で podman を使う
podman でなにか動かす
```

---

### daru 上で singularity を使う

- ~/.ssh/configに以下の設定を追加する
- ```
    Host daru
      HostName daru.ie.u-ryukyu.ac.jp
      User e2057xx
      ProxyJump chatan
      ```
  - すでに設定されているだろうけど一応、chatanの設定は[この記事](https://yoshisaur.hatenablog.jp/entry/2021/12/26/213448)に書かれている
- `$ ssh daru`
- `$ mkdir 11_2`
- `$ cd !$`
- `$ singularity pull --docker-login ubuntu:20.04`
  - ubuntu:20.04のイメージを取得する
  - ubuntu_20.04.sifというイメージを格納しているファイルが生成される
- `$ singularity shell --nv ubuntu_20.04.sif`
  - ubuntu:20.04.sifからコンテナを立ててログインする
  - \-nコマンドがないとGPUが使えない

---

### GPU があるかどうかを確認する

- `$ nvidia-smi`
- ```
    Sat Feb  5 18:33:06 2022
    +-----------------------------------------------------------------------------+
    | NVIDIA-SMI 450.156.00   Driver Version: 450.156.00   CUDA Version: 11.0     |
    |-------------------------------+----------------------+----------------------+
    | GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
    | Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
    |                               |                      |               MIG M. |
    |===============================+======================+======================|
    |   0  Tesla V100S-PCI...  Off  | 00000000:3B:00.0 Off |                    0 |
    | N/A   37C    P0    36W / 250W |      0MiB / 32510MiB |      0%      Default |
    |                               |                      |                  N/A |
    +-------------------------------+----------------------+----------------------+
    
    +-----------------------------------------------------------------------------+
    | Processes:                                                                  |
    |  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
    |        ID   ID                                                   Usage      |
    |=============================================================================|
    |  No running processes found                                                 |
    +-----------------------------------------------------------------------------+
    ```
- Ctrl-dでコンテナからログアウトする

---

### Vimを入れる

- 中身が以下のようなtest.defというファイルを作る
  - Dockerfileのようなものである
  - ```
    BootStrap: docker
    From: ubuntu:20.04
    %post
        apt-get update
        apt-get -y upgrade
        apt-get -y install build-essential vim
    ```
- `$ singularity build --fakeroot test.sif test.def`
  - test.defファイルからtest.sifファイルを生成する
- `$ singularity shell test.sif`
  - test.sifからコンテナを生成してログイン
- `$ which vim`
- ```
    /usr/bin/vim
    ```
  - vimが入っていることを確認できた
- Ctrl-dでコンテナからログアウトする

---

### rustを入れる

- `$ singularity shell --fakeroot  --writable  test.sif`
  - test.sifからコンテナを作ってログインする
- `$ apt-get update`
- `$ apt-get install rustc`
- `$ mkdir rust; cd rust ; cargo new hello_world --bin`
  - rustというディレクトリが作られてhello_worldというパッケージが作られる
  - rustcパッケージがちゃんと入っていることを確認
- Ctrl-dでコンテナからログアウトする
  - 一旦ログアウトするとrustcパッケージが消え去り、cargoコマンドなどが使えなくなる
  - Singularityのコンテナは構築後に動的にパッケージが追加できない
- `$ singularity build --sandbox --fakeroot test/ test.def`
  - \-\-sandboxオプションは環境を動的に変更できるようにする(Dockerライクになる)
- `$ singularity shell --fakeroot --writable test`
- もう一度rustcをインストールする
  - `$ apt-get update`
  - `$ apt-get install rustc`
- Ctrl-dで抜ける
- `$ singularity shell --fakeroot --writable test`
  - 再度ログイン
- `$ which cargo`
  - /usr/bin/cargo
- コンテナからログアウトした後でも、動的に加えたパッケージが残っていることが確認できた

---

### daru上でslumを使う

- `$ singularity pull pytorch.sif docker://pytorch/pytorch:latest`
- `$ git clone https://github.com/pytorch/examples.git`
- 中身が以下のmnist.sbatchというファイルを作成する
- ```
    #!/bin/bash
    #SBATCH --job-name mnist 
    #SBATCH --output logs/%x-%j.log
    #SBATCH --error logs/%x-%j.err
    #SBATCH --nodes 1
    #SBATCH --gpus tesla:1
    date
    singularity exec --nv pytorch_latest.sif python examples/mnist/main.py
    date
    ```
- `$ sbatch mnist.sbatch`
  - slurmでJobを投げる
  - 課題ページでは、ジョブ情報を確認する命令がなかったが一応、squeueコマンドでジョブ情報を表示させたら何もジョブがない状態だった
    - GPUを使っても使わなくても同じ結果になりました

---

以下はオプション

---

### daru上でpodmanを使う

- `$ rm -rf ~/.local`
- `$ sudo podman run hello-world`
- ```
    Hello from Docker!
    This message shows that your installation appears to be working correctly.
    
    To generate this message, Docker took the following steps:
     1. The Docker client contacted the Docker daemon.
     2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
        (amd64)
     3. The Docker daemon created a new container from that image which runs the
        executable that produces the output you are currently reading.
     4. The Docker daemon streamed that output to the Docker client, which sent it
        to your terminal.
    
    To try something more ambitious, you can run an Ubuntu container with:
     $ docker run -it ubuntu bash
    
    Share images, automate workflows, and more with a free Docker ID:
     https://hub.docker.com/
    
    For more examples and ideas, visit:
     https://docs.docker.com/get-started/
  ```

---

### podmanでなにか動かす(オリジナリティが必要)

- イメージの作成or取得 -> イメージからコンテナを作成という流れ
- イメージは「Dockerfileから生成する」または「docker image pull <image>:<tag> のようなコマンドを実行する」のが考えられる。
- 以下のようなDockerfileを用意する
  - マルチステージビルドを使う
  - Stage1でGolangコンパイラがシングルバイナリファイルを生成して、Stage2でGoのファイル、コンパイル環境を持たないubuntu環境でStage1のシングルバイナリファイルを実行する
  - ```
    #Stage 1
    FROM golang AS build01
    RUN mkdir workspace
    COPY ./hello_world.go /workspace/
    WORKDIR /workspace
    RUN go build hello_world.go
    #Stage 2
    FROM ubuntu
    RUN mkdir workspace
    COPY --from=build01 workspace/hello_world /workspace
    WORKDIR /workspace
    ENTRYPOINT ["./hello_world"]
    ```
- 以下のようなhello_world.goを用意
- ```
    package main
    import "fmt"
    func main() {
        fmt.Println("Hello, World!")
    }
  ```
- `$ podman image build -f ./Dockerfile -t ubuntu:golang_hello .`
- `$ podman container run -it ubuntu:golang_hello`
- Hello, World!が出力された

---

### 11.3の感想

- Singularityで学科のGPU使えるの神
- ただ動的にコンテナ内のパッケージを自由に管理するDockerとは違って、sandboxオプションを使わないと動的に追加したパッケージをログアウトした状態で使えないのは初めて知った、あくまで検証環境として使うのが良さそう

---

## 11.4 Cloudの課題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/216.html)の問題を解く
