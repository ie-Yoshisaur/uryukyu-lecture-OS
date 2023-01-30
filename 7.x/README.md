# OSの講義の7.xの問題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 7.1 Busy Waitの問題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/097.html)の問題を解く


```
Java FX を用いて、mouse の動きを real-time で記録し、再現するGUIアプリケーションを作成せよ。

(a) CPU を消費するができるだけ、細かくmouseの動きを再現する

(b) CPU を消費しないが、mouseの動きを再現は荒い

この二つの version を作成せよ。

application を動かしながら、top で cpu usage 測り、その動きを調べよ。top のオプションを使う。
```


---

### 環境構築

- [Javaのダウンロードサイト](https://www.java.com/ja/download/)からJavaをダウンロードし、インストールする
- [JavaFXのダウンロードサイト](https://gluonhq.com/products/javafx/)からSDKを選択してダウンロードする。
  - ファイルを展開して`/Library/Java/JavaVirtualMachines`に置く
    - `$ sudo mv /Users/yoshisaur/Downloads/javafx-sdk-19.0.2.1 /Library/Java/JavaVirtualMachines/`
- `$ javac --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media *.java`でコンパイルできて、
- `$ java --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media *.java`でファイルが実行できる

---

### aを実装する

- aをMouseRecorderA.javaというファイルで実装する
  - Timelineクラスを使ってアニメーションの割り込み間隔を1msくらいの細かさで設定してcpuの消費を上げている
- `$ javac --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media MouseRecorderA.java`でコンパイル
- `$ java --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media MouseRecorderA.java`で実行


```
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import javafx.util.Duration;

import java.util.ArrayList;
import java.util.List;

public class MouseRecorderA extends Application {
    private List<double[]> coordinates = new ArrayList<>();
    private boolean recording = false;
    private int timelineIndex = 0;
    private int frameIndex = 0;
    private Canvas canvas;
    private GraphicsContext gc;
    private double prevX = 0;
    private double prevY = 0;
    private double currentX = 0;
    private double currentY = 0;

    public static void main(String[] args) {
        launch(args);
    }

    Timeline timeline = new Timeline(
            new KeyFrame(Duration.millis(1), new EventHandler<ActionEvent>() {
                @Override
                public void handle(ActionEvent event) {
                    if (timelineIndex < coordinates.size()) {
                        gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
                        double[] point = coordinates.get(timelineIndex);
                        gc.setFill(Color.BLUE);
                        gc.fillOval(point[0], point[1], 5, 5);
                        timelineIndex++;
                    } else {
                        timeline.stop();
                    }
                }
            })
    );

    @Override
    public void start(Stage primaryStage) {
        canvas = new Canvas(800, 600);
        gc = canvas.getGraphicsContext2D();

        Button startButton = new Button("Start Recording");
        startButton.setOnAction(event -> {
            recording = true;
            coordinates.clear();
            timelineIndex = 0;
        });

        Button stopButton = new Button("Stop Recording");
        stopButton.setOnAction(event -> recording = false);

        Button replayButton = new Button("Replay");
        replayButton.setOnAction(event -> {
            timelineIndex = 0;
            timeline.setCycleCount(coordinates.size());
            timeline.play();
        });

        canvas.setOnMouseMoved(event -> {
            if (recording) {
                frameIndex = 0;
                currentX = event.getX();
                currentY = event.getY();
                if (prevX == 0 && prevY == 0) {
                    for (frameIndex = 0; frameIndex < (16); frameIndex++) {
                        coordinates.add(new double[]{currentX, currentY});
                    }
                } else {
                    for (frameIndex = 0; frameIndex < (16); frameIndex++) {
                        coordinates.add(new double[]{((currentX - prevX) * ((frameIndex + 1) / (16))) + prevX, ((currentY - prevY) * ((frameIndex + 1) / (16))) + prevY});
                    }
                }
                prevX = currentX;
                prevY = currentY;
            }
        });

        VBox root = new VBox();
        root.getChildren().addAll(canvas, startButton, stopButton, replayButton);

        Scene scene = new Scene(root, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
```

---

### bを実装する

- bをMouseRecorderB.javaというファイルで実装する
  - AnimationTimerクラスを使っている
  - 60fpsで動作する、aの実装よりも低いフレームレート
    - 参照: [stackover flow](https://stackoverflow.com/questions/28287398/what-is-the-preferred-way-of-getting-the-frame-rate-of-a-javafx-application#:~:text=Note%20the%20animation%20timer%20runs,as%20to%20the%20frame%20rate.)
- `$ javac --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media MouseRecorderB.java`でコンパイル
- `$ java --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media MouseRecorderB.java`で実行

```
import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.List;

public class MouseRecorderB extends Application {
    private List<double[]> coordinates = new ArrayList<>();
    private boolean recording = false;
    private int index = 0;
    private Canvas canvas;
    private GraphicsContext gc;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        canvas = new Canvas(800, 600);
        gc = canvas.getGraphicsContext2D();

        Button startButton = new Button("Start Recording");
        startButton.setOnAction(event -> {
            recording = true;
            coordinates.clear();
            index = 0;
        });

        Button stopButton = new Button("Stop Recording");
        stopButton.setOnAction(event -> recording = false);

        Button replayButton = new Button("Replay");
        replayButton.setOnAction(event -> {
            index = 0;
            AnimationTimer timer = new AnimationTimer() {
                @Override
                public void handle(long now) {
                    if (index < coordinates.size()) {
                        gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
                        double[] point = coordinates.get(index);
                        gc.setFill(Color.BLUE);
                        gc.fillOval(point[0], point[1], 5, 5);
                        index++;
                    } else {
                        stop();
                    }
                }
            };
            timer.start();
        });

        canvas.setOnMouseMoved(event -> {
            if (recording) {
                coordinates.add(new double[]{event.getX(), event.getY()});
            }
        });

        VBox root = new VBox();
        root.getChildren().addAll(canvas, startButton, stopButton, replayButton);

        Scene scene = new Scene(root, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
```

---

### topコマンドでcpu usageを測る + グラフにする

- 以下のようなbashスクリプトを用意した
  - 名前は`get_cpu_usage.sh`にした
  - 30秒間、1秒ごとにtopコマンドで表示される、user、sys、idleの項目の数値を出力する

```
#!/bin/bash

echo 'sec,user,sys,idle'

second=0

while [ $second -lt 30 ]
do
    echo -n "$second,"
    top -l 1 -n 5 -s 0 | grep "CPU usage" | sed 's/.* usage: \(.*\) user, \(.*\) sys, \(.*\) idle.*/\1,\2,\3/' | sed 's/%//g'
    sleep 1
    second=$((second+1))
done
```

- terminal以外のアプリケーションを終了しておく
- `$ bash get_cpu_usage.sh > cpu_usage_a.csv`や `$ bash get_cpu_usage.sh > cpu_usage_b.csv`を実行する
  - その前に`$ java --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media MouseRecorderA.java`や- `$ java --module-path /Library/Java/JavaVirtualMachines/javafx-sdk-19.0.2.1/lib --add-modules=javafx.controls,javafx.fxml,javafx.media MouseRecorderB.java`を実行して、Start Recordingボタンを押しておく
  - bashスクリプトが実行されたら、なるべく激しめにマウスをcanvas領域内で動かしてマウスの動きを記録する、15秒くらいしたらStop Recordingボタンを押して、Replayをする
- csvファイルをグラフ化するPythonスクリプトを書く、ファイル名を `cpu_usage_plot.py`にした
- `$ python3 cpu_usage_plot.py cpu_usage_a.csv`や`$ python3 cpu_usage_plot.py cpu_usage_b.csv`を実行してグラフ化する
```
import argparse
import pandas as pd
import matplotlib.pyplot as plt


def plot_bars(file_name):
    df = pd.read_csv(file_name)
    sec = df['sec']
    user = df['user']
    sys = df['sys']
    idle = df['idle']

    fig, ax = plt.subplots()
    ax.bar(sec, user, label='user')
    ax.bar(sec, sys, bottom=user, label='sys')
    ax.bar(sec, idle, bottom=user+sys, label='idle')

    ax.set_xlabel('sec')
    ax.set_ylabel('Usage %')
    ax.legend()
    plt.title(file_name)

    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', help='CSV file name')
    args = parser.parse_args()

    plot_bars(args.file_name)
```

- グラフ化したら以下のようになった
  - aのcpu使用状況
    - [f:id:e205723:20230130235229p:plain]
  - bのcpu使用状況
    - [f:id:e205723:20230130235250p:plain]
- aとbの実装ではさほどcpu使用率に差がなかった
  - フレームレートの頻度の差はcpu使用率の変化に影響がないということになる
  - 課題のキーワードである「busy wait」の観点からすると、aとbはどちらも`canvas.setOnMouseMoved`でイベントを待っているでcpuの使用率の推移に違いが見当たらなかったと考えることができる
  - ちなみに、マウスの座標を取るapiが`canvas.setOnMouseMoved`しかなかったので、これ以上の工夫はできなかった

---

## 7.2 getaddinfo

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/123.html)の問題を解く

```
Perl/C/Java のどれかを用いて、

getaddinfo (または、それに相当するAPI) を用いて、コンピュータが使用可能なアドレスをすべて取得するプログラムを作成せよ。

その結果を、ifconfig の出力と比較せよ。
```

### Javaでコンピュータが使用可能なアドレスをすべて取得するプログラムを作成する

Program7_2.javaというコードを作成した。

コードは[GitHub](https://github.com/e205723/uryukyu-lecture-OS/blob/main/7.x/7.2/Program7_2.java)にある。

```
import java.util.Enumeration;
import java.net.NetworkInterface;
import java.net.InetAddress;

public class Program7_2{
    public static void main(String[] args) {
        try {
            Enumeration interfaces = NetworkInterface.getNetworkInterfaces();
            while (interfaces.hasMoreElements()) {
                NetworkInterface intf = (NetworkInterface)interfaces.nextElement();
                System.out.println("The name of this network interface is " + intf.getName() + ".");
                System.out.println("  All the IP addresses of this network interface are: ");
                Enumeration addresses = intf.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress address = (InetAddress)addresses.nextElement();
                    System.out.println("    " + address.getHostAddress());
                }
            }
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}
```

JDK 1.4からネットワークインターフェースにアクセスするAPIが追加された。

ネットワークアドレスを表すJavaのクラスは以下の通りである

| クラス名 | 説明 |
| --- | --- |
| InetAdress | ・IPアドレスまたはそれを「解決」したホストネームを表す<br>・リモートアドレスを指定するために使う |
| InetSocketAddress extends SocketAddress | ソケットのアドレス、{IPアドレス, ポート}または{ホストネーム, ポート}というペアを表す |
| NetworkInterface | ・ローカルのネットワークインターフェースを表現する<br>・インターフェースの名前とインターフェースに結びついているIPアドレスのリストで表現される<br>・マルチキャストにおいて、ローカルキャストを識別するために使う |

getNetworkInterfacesメソッドは、そのホスト上の複数のNetworkInterfaceを収めたEnumerationを返す。

### ifconfigの出力と比較する

`$ ifconfig`を実行したのちinactiveなアドレスを取り省いた結果が以下の通りである。

```
lo0: flags=8049<UP,LOOPBACK,RUNNING,MULTICAST> mtu 16384
    options=1203<RXCSUM,TXCSUM,TXSTATUS,SW_TIMESTAMP>
    inet 127.0.0.1 netmask 0xff000000 
    inet6 ::1 prefixlen 128 
    inet6 fe80::1%lo0 prefixlen 64 scopeid 0x1 
    nd6 options=201<PERFORMNUD,DAD>
gif0: flags=8010<POINTOPOINT,MULTICAST> mtu 1280
stf0: flags=0<> mtu 1280
XHC0: flags=0<> mtu 0
XHC1: flags=0<> mtu 0
XHC20: flags=0<> mtu 0
en0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    options=400<CHANNEL_IO>
    ether dc:a9:04:88:a5:cb 
    inet6 fe80::1889:4afc:1e8b:355e%en0 prefixlen 64 secured scopeid 0x8 
    inet 192.168.43.25 netmask 0xffffff00 broadcast 192.168.43.255
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect
    status: active
en1: flags=8963<UP,BROADCAST,SMART,RUNNING,PROMISC,SIMPLEX,MULTICAST> mtu 1500
    options=460<TSO4,TSO6,CHANNEL_IO>
    ether 82:d0:e2:40:3c:01 
    media: autoselect <full-duplex>
    status: inactive
en3: flags=8963<UP,BROADCAST,SMART,RUNNING,PROMISC,SIMPLEX,MULTICAST> mtu 1500
    options=460<TSO4,TSO6,CHANNEL_IO>
    ether 82:d0:e2:40:3c:05 
    media: autoselect <full-duplex>
    status: inactive
en2: flags=8963<UP,BROADCAST,SMART,RUNNING,PROMISC,SIMPLEX,MULTICAST> mtu 1500
    options=460<TSO4,TSO6,CHANNEL_IO>
    ether 82:d0:e2:40:3c:00 
    media: autoselect <full-duplex>
    status: inactive
en4: flags=8963<UP,BROADCAST,SMART,RUNNING,PROMISC,SIMPLEX,MULTICAST> mtu 1500
    options=460<TSO4,TSO6,CHANNEL_IO>
    ether 82:d0:e2:40:3c:04 
    media: autoselect <full-duplex>
    status: inactive
bridge0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    options=63<RXCSUM,TXCSUM,TSO4,TSO6>
    ether 82:d0:e2:40:3c:01 
    Configuration:
        id 0:0:0:0:0:0 priority 0 hellotime 0 fwddelay 0
        maxage 0 holdcnt 0 proto stp maxaddr 100 timeout 1200
        root id 0:0:0:0:0:0 priority 0 ifcost 0 port 0
        ipfilter disabled flags 0x0
    member: en1 flags=3<LEARNING,DISCOVER>
            ifmaxaddr 0 port 9 priority 0 path cost 0
    member: en2 flags=3<LEARNING,DISCOVER>
            ifmaxaddr 0 port 11 priority 0 path cost 0
    member: en3 flags=3<LEARNING,DISCOVER>
            ifmaxaddr 0 port 10 priority 0 path cost 0
    member: en4 flags=3<LEARNING,DISCOVER>
            ifmaxaddr 0 port 12 priority 0 path cost 0
    nd6 options=201<PERFORMNUD,DAD>
    media: <unknown type>
    status: inactive
p2p0: flags=8843<UP,BROADCAST,RUNNING,SIMPLEX,MULTICAST> mtu 2304
    options=400<CHANNEL_IO>
    ether 0e:a9:04:88:a5:cb 
    media: autoselect
    status: inactive
awdl0: flags=8943<UP,BROADCAST,RUNNING,PROMISC,SIMPLEX,MULTICAST> mtu 1484
    options=400<CHANNEL_IO>
    ether 72:4a:a7:9a:13:14 
    inet6 fe80::704a:a7ff:fe9a:1314%awdl0 prefixlen 64 scopeid 0x10 
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect
    status: active
llw0: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    options=400<CHANNEL_IO>
    ether 72:4a:a7:9a:13:14 
    inet6 fe80::704a:a7ff:fe9a:1314%llw0 prefixlen 64 scopeid 0x11 
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect
    status: active
utun0: flags=8051<UP,POINTOPOINT,RUNNING,MULTICAST> mtu 1380
    inet6 fe80::c6e:6de5:847c:da6%utun0 prefixlen 64 scopeid 0x12 
    nd6 options=201<PERFORMNUD,DAD>
utun1: flags=8051<UP,POINTOPOINT,RUNNING,MULTICAST> mtu 2000
    inet6 fe80::5eb0:a477:8359:1b3e%utun1 prefixlen 64 scopeid 0x13 
    nd6 options=201<PERFORMNUD,DAD>
utun2: flags=8051<UP,POINTOPOINT,RUNNING,MULTICAST> mtu 1000
    inet6 fe80::ce81:b1c:bd2c:69e%utun2 prefixlen 64 scopeid 0x14 
    nd6 options=201<PERFORMNUD,DAD>
en6: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    options=6467<RXCSUM,TXCSUM,VLAN_MTU,TSO4,TSO6,CHANNEL_IO,PARTIAL_CSUM,ZEROINVERT_CSUM>
    ether 04:ab:18:3d:b9:47 
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect (none)
    status: inactive
en5: flags=8863<UP,BROADCAST,SMART,RUNNING,SIMPLEX,MULTICAST> mtu 1500
    ether ac:de:48:00:11:22 
    inet6 fe80::aede:48ff:fe00:1122%en5 prefixlen 64 scopeid 0x7 
    nd6 options=201<PERFORMNUD,DAD>
    media: autoselect
    status: active
```

Program7_2.javaを実行した結果は以下の通りである。

```
The name of this network interface is en5.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:aede:48ff:fe00:1122%en5
The name of this network interface is utun2.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:ce81:b1c:bd2c:69e%utun2
The name of this network interface is utun1.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:5eb0:a477:8359:1b3e%utun1
The name of this network interface is utun0.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:c6e:6de5:847c:da6%utun0
The name of this network interface is llw0.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:704a:a7ff:fe9a:1314%llw0
The name of this network interface is awdl0.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:704a:a7ff:fe9a:1314%awdl0
The name of this network interface is en0.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:1889:4afc:1e8b:355e%en0
    192.168.43.25
The name of this network interface is lo0.
  All the IP addresses of this network interface are: 
    fe80:0:0:0:0:0:0:1%lo0
    0:0:0:0:0:0:0:1%lo0
    127.0.0.1
```

**2つの出力の違い**

Javaのプログラムが出力が示したアドレスでは表示するネットワーク・インターフェースは、activeなものだけを限定して表示しているのに対し、ifconfigコマンドはinactiveなものも表示している

---

## 7.3 bloadcast と multicastの問題

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/124.html)の問題を解く

```
Perl/C/Java のどれかを用いて、

マルチキャストとブロードキャストを行うプログラムを作成せよ

送信可能な最大のパケットの大きさはいくつか。無線LANと有線LAN、マルチキャストとブロードキャストのそれぞれについて調べよ。
```

### マルチキャストアドレス・ブロードキャストアドレスとは

マルチキャストアドレスは複数のアドレスが参加しているグループで、そのグループに個々のアドレスが参加(join)したり去ったり(leave)できるので「動的グループ」として扱われる。一定のルーティングポリシーに従って、通信をグループの全メンバが受信する。

ブロードキャストアドレスは、1つのネットワークやサブネットワークのすべてのIPアドレスを集めたグループで、メンバが一定なので「静的グループ」として扱われる。通信はルーティングポリシーに従って、一定のアドレス範囲の全メンバへ送られる。

アドレスタイプの概要を以下の表[1]にまとめる

| タイプ | IPv4 | IPv6 | TCP | UDP | 送信先 | 受信者 |
| --- | --- | --- | --- | --- | --- | --- |
| マルチキャスト | オプション | Yes | No | Yes | 集合 | 集合の全員 |
| ブロードキャスト | Yes | No | No | Yes | 全員 | 全員 |

### UDPプロトコルとは

ネットワークの通信で利用するOSI参照モデルのレイヤ4に該当するトランスポート層のプロトコルの1つ

通信相手の存在を確認しないコネクションレス型である

### Javaでマルチキャストを行うプログラムを作成する

**マルチキャストの送信をするプログラム**

[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/7.x/7.3/Source.java)が今回作成したコード

```
import java.net.InetAddress;
import java.net.DatagramPacket;
import java.net.MulticastSocket;

public class Source {
    public static void main(String[] args) {
        try {
            int port = 8888;
            String dataToSend = "Hello, World!";
            byte[] data = dataToSend.getBytes("UTF-8");
            InetAddress address = InetAddress.getByName("239.1.14.126");
            DatagramPacket packet = new DatagramPacket(data, data.length, address, port);
            MulticastSocket socket = new MulticastSocket();
            socket.send(packet);
            socket.close();
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}
```

**マルチキャストの受信をするプログラム**

[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/7.x/7.3/Receiver.java)が今回作成したコード

マルチキャストを受信するには、MulticastSocketオブジェクトが必要になり、次の3つの操作が必要になる

- MulticastSocketを初期化する
- マルチキャストグループにjoinする
- データグラムを受信する

このコードはこれら3つの操作をしてマルチキャストを受信している

```
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.DatagramPacket;
import java.util.Arrays;

public class Receiver {
    public static void main(String[] args) {
        try {
            int port = 8888;
            MulticastSocket socket = new MulticastSocket(port);
            InetAddress multicastAddress = InetAddress.getByName("239.1.14.126");
            socket.joinGroup(multicastAddress);
            byte[] data = new byte[1024];
            DatagramPacket packet = new DatagramPacket(data, data.length);
            socket.receive(packet);
            System.out.println(new String(Arrays.copyOf(packet.getData(), packet.getLength()), "UTF-8"));
            socket.close();
        } catch (Exception e) {
            System.err.println(e);
        }
    }
}
```

**実行結果**

`$ java Receiver.java`を実行する
`$ java Source.java`を別のターミナルで実行する

出力結果は以下の通りになった

```
Hello, World!
```

---

### Javaでブロードキャストを行うプログラムを作成する

**ブロードキャストの送信をするプログラム**

[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/7.x/7.3/Source2.java)が今回作成したコード

```
import java.net.InetAddress;
import java.net.DatagramPacket;
import java.net.DatagramSocket;

public class Source2 {
    public static void main(String[] args) {
        try {
            int port = 8888;
            String dataToSend = "Hello, World!";
            byte[] data = dataToSend.getBytes("UTF-8");
            InetAddress address = InetAddress.getByName("255.255.255.255");
            DatagramPacket packet = new DatagramPacket(data, data.length, address, port);
            DatagramSocket socket = new DatagramSocket();
            socket.send(packet);
            socket.close();
    } catch (Exception e) {
            System.err.println(e);
    }
    }
}
```

**ブロードキャストの受信をするプログラム**

[こちら](https://github.com/e205723/uryukyu-lecture-OS/blob/main/7.x/7.3/Receiver2.java)が今回作成したコード

```
import java.net.InetAddress;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.util.Arrays;

public class Receiver2 {
    public static void main(String[] args) {
        try {
            int port = 8888;
            DatagramSocket socket = new DatagramSocket(port);
            byte[] data = new byte[1024];
            DatagramPacket packet = new DatagramPacket(data, data.length);
            socket.receive(packet);
            System.out.println(new String(Arrays.copyOf(packet.getData(), packet.getLength()), "UTF-8"));
            socket.close();
    } catch (Exception e) {
            System.err.println(e);
    }
    }
}
```

**実行結果**

`$ java Receiver2.java`を実行する
`$ java Source2.java`を別のターミナルで実行する

出力結果は以下の通りになった

```
Hello, World!
```

### 送信可能な最大のパケットの大きさを調査する

「UDPヘッダ + データ」の通信単位を「UDPデータグラム」と呼び、これは一般的にUDPパケットとも呼ばれる。

IPv4のプロトコルでは、65507バイトまでで、実際に8KBに制限している実装が多い。一方、IPv6では2の31乗-1バイトのUDPデータグラムが可能である。[2] しかし、それは65575オクテットより大きいリンクに接続できるIPv6ノードにとってのみに有効であり、通信に参加する全員がそのような大きなリンクに接続しているホストではないと意味がない。

UDPの制約として、データグラムが小分けにされたときの残りの断片を再送信できない。よってIPv4のUDPではこの問題を防ぐためにデータグラムのサイズを512バイトに制限することもよくある。512バイトよりも、大きなサイズの候補を探す場合、イーサネットの公称MTUである1500バイトからIPとUDPのヘッダを大きさである1472バイトが有効であると考えられる。

### 参考文献

[1] Unix Network Programming, 3rd edition, Vol.1, https://mathcs.clarku.edu/~jbreecher/cs280/UNIX%20Network%20Programming(Volume1,3rd).pdf

[2] RFC 2675, https://datatracker.ietf.org/doc/html/rfc2675

---

## 7.4 サーバとクライアント

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/048.html)の問題を解く

---

## 7.5 様々なAPI

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os07/lecture.html)の問題を解く

---

## 7.6 様々なAPI

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os07/lecture.html)の問題を解く

---
