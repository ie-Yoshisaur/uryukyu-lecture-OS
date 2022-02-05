# OSの講義の11.xの問題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 11.1 VMの課題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/214.html)の問題を解く

---

## 11.2 Containerの課題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/215.html)の問題を解く

```
daru上で singularity を使う
GPU があるかどうかを確認する
vimを入れる
rust を入れてみる
daru 上で slum を使う
daru 上で podman を使う
podman でなにか動かす
podman の速度
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
- `$ mkdir 11_1`
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
  - squeueコマンドでジョブ情報を表示させたら何もジョブがない状態だった
    - slurmのジョブ使い方をシス管メンバーに教えてもらいます

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

### podmanでなにか動かす

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

### 11.2の感想

- Singularityで学科のGPU使えるの神
- ただ動的にコンテナ内のパッケージを自由に管理するDockerとは違って、sandboxオプションを使わないと動的に追加したパッケージをログアウトした状態で使えないのは初めて知った、あくまで検証環境として使うのが良さそう

---

## 11.3 Cloudの課題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/216.html)の問題を解く
