# OSの講義の8.xの問題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 8.1 fragmentationを実際に見てみる

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/210.html)の問題を解く

---

## 8.2 Page テーブルのGUI demo

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/190.html)の問題を解く

```
[JavaScriptで書いた Page テーブルのGUI demo](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os08/vm/vm.html) を、もう少し見栄えのするように改良したい。

    わかりやすい表示 (線ぐらいでて欲しいよね?)
    白黒かよ!
    ページテーブルの中身の書き換え(当然...)
    アニメーション
    多段の場合
    仮想記憶が入る場合
    inverted page table

などの改良が考えられる。なんらかの改良をして見よう。
```

---

### GUIの改善点を見つける

- [JavaScriptで書いた Page テーブルのGUI demo](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os08/vm/vm.html)を見てみよう
- 改良できそうなことを列挙する
  - 矢印を表示していない
  - 白黒だけ
  - ページテーブルの中身の書き換えしていない
  - アニメーションがない
  - 多段の場合の表示がない
  - 仮想記憶が入る場合がない
- 矢印を表示していないところを改善することにした

### コードを編集して矢印を表示させる

- GUIのhtmlファイルをダウンロードする
- scriptタグ内にjavascriptのコードが記述されているので実装を見てみる

```
<script type="application/x-javascript">

var width = 30;
var scale = 2;
var address = 0;
var vmsize = 6;
var psize = 4096;
var pagetableString = "";
var pagetable = new Array(7);

var llow;
var lhigh;
var plow;
var phigh;

for(var i=0;i<vmsize;i++) {
    var v = Math.floor(Math.random()*vmsize);
    pagetable[i] = v;
    pagetableString += v.toString(16) +"\n";
}

function draw() {
    var canvas = document.getElementById("canvas");
    var ctx = canvas.getContext("2d");
    canvas.ontouchmove = TouchXY;

    memory(ctx,10,10);
    memory(ctx,250,10);
    pointer(ctx,80,50);
    pointer(ctx,170,50);
    page(ctx, 130,120);

}

function text(name,text,x,y)
{
    // ctx.fillText(text,x*scale,y*scale);
    var X=document.body.scrollLeft+x*scale;
    var Y=document.body.scrollTop+y*scale;
    document.all[name].style.left=X+20;
    document.all[name].style.top=Y-5;
    document.all[name].innerText=text;
}


function MouseXY()
{
    var x = event.x/scale;
    var y = event.y/1;
    XY(x,y);
}

function TouchXY()
{
    var canvas = document.getElementById("canvas");
    var x =  event.pageX - canvas.offsetLeft;
    var y =  event.pageY - canvas.offsetTop
    XY(x,y);
}

function XY(x,y)
{
    if (10<=x&&x<=110&&10<=y&&y<=310) {
    var canvas = document.getElementById("canvas");
    var ctx = canvas.getContext("2d");
    ctx.clearRect(10,10,10+60,410);
    memory(ctx,10,10);
    ctx.clearRect(250*scale,10,250*scale+60,410);
    memory(ctx,250,10);

    strokeRect(ctx,10,y,10+width,y+2);
        address = (y-10)*100;
        physical = translate(address);
    y = physical/100+10;
    strokeRect(ctx,250,y,250+width,y+2);
    update1();
    }
}

function update1() 
{
    text("Logical","logical address = 0x"+address.toString(16),10,150);
    text("Physical","physical address = 0x"+physical.toString(16),250,150);
    text("Pagetable",pagetableString,130,70);
    text("lhigh",lhigh.toString(16),90,32);
    text("llow",llow.toString(16),120,32);
    text("phigh",phigh.toString(16),90+90,32);
    text("plow",plow.toString(16),120+90,32);
}

function translate(adr)
{
    lhigh = Math.floor(adr/psize);
    llow = adr%psize;
    phigh = pagetable[lhigh];
    plow = llow;
    return phigh*psize+plow;
}

function strokeRect(ctx,x,y,x1,y1)
{
    return ctx.strokeRect(x*scale,y,(x1-x)*scale,(y1-y)*scale);
}

function pointer(ctx,x,y) {
    var dy = 10;
    strokeRect(ctx,x,y,x+width,y+dy);
    x += width;
    strokeRect(ctx,x,y,x+width,y+dy);
}

function memory(ctx,x,y) {
    var dy = 40;
    var dx = 40;

    for(var i=0;i<vmsize-1;i++) {
    strokeRect(ctx,x,y,x+width,y+dy);
    y += dy;
    }
}

function page(ctx,x,y) {
    var dy = 20;
    var dx = 40;

    for(var i=0;i<vmsize-1;i++) {
    strokeRect(ctx,x,y,x+width,y+dy);
    y += dy;
    }
    return box;
}


  </script>
```

- 実装を見ると基本的には`XY()`関数を編集すればいいことがわかる
- 矢印を表示させるスクリプトを書けば終了なのだが、味気ないので[ChapGPT](https://chat.openai.com/chat)にコードを生成させてみることにした
- 以下のような命令をChatGPTに投げる
  - `javascriptで(fromX, fromY)から(toX, toY)までの矢印をctx上に描画するスクリプトを生成してください`
- ChatGPTは以下の関数を生成する
  - ちなみにまじで生成した関数に対して1ミリも調整や編集することなく課題を解くのに使うことができた
```
function drawArrow(ctx, fromX, fromY, toX, toY) {
  // 矢印を描画するために必要な変数を計算する
  var headLength = 10;
  var angle = Math.atan2(toY - fromY, toX - fromX);
  // 矢印を描画するために必要な座標を計算する
  var x1 = toX - headLength * Math.cos(angle - Math.PI / 6);
  var y1 = toY - headLength * Math.sin(angle - Math.PI / 6);
  var x2 = toX - headLength * Math.cos(angle + Math.PI / 6);
  var y2 = toY - headLength * Math.sin(angle + Math.PI / 6);
  // 矢印を描画する
  ctx.beginPath();
  ctx.moveTo(fromX, fromY);
  ctx.lineTo(toX, toY);
  ctx.lineTo(x1, y1);
  ctx.moveTo(toX, toY);
  ctx.lineTo(x2, y2);
  ctx.stroke();
}
```
- 関数をコピペする
- XY()関数を以下のように編集した
```
function XY(x,y)
{
    if (10<=x&&x<=110&&10<=y&&y<=310) {
        var canvas = document.getElementById("canvas");
        var ctx = canvas.getContext("2d");
        ctx.clearRect(0,0,159,canvas.height);
        ctx.clearRect(0,71,259,canvas.height);
        ctx.clearRect(321,71,canvas.width,canvas.height);
        ctx.clearRect(461,0,canvas.width,canvas.height);
        memory(ctx,10,10);
        memory(ctx,250,10);
        strokeRect(ctx,10,y,10+width,y+2);
        address = (y-10)*100;
        physical = translate(address);
        drawArrow(ctx,85,y+1,150,60);
        drawArrow(ctx,195,75,250,130+(lhigh*20));
        y = physical/100+10;
        drawArrow(ctx,330,130+(lhigh*20),375,75);
        drawArrow(ctx,465,60,495,y+1);
        strokeRect(ctx,250,y,250+width,y+2);
        update1();
    }
}
```
- 以上でコードの編集終了、ChatGPT怖すぎる
- amaneのホームディレクトリに`public_html`というディレクトリを作って\(すでにあるなら無視\)、その中に`8-2`というディレクトリを作り、`index.html`というファイルとしてhtmlファイルを置けば
- [このような感じ](https://ie.u-ryukyu.ac.jp/~e205723/8-2/)で改善したGUIを配信できる
  - URLはこんなかんじ→`https://ie.u-ryukyu.ac.jp/~e205723/8-2/`

---

## 8.3 Perfomance of Demand Paging

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/125.html)の問題を解く

```
(1) TLB miss してpage が実メモリにあった場合の平均page-falut処理時間が1μ sec, memory access 1.0n sec の時に、page fault rate が、1% の時の effective access time を求めよ。

(2) TLB miss してpage が仮想メモリにあった場合の平均page-falut処理時間が1m sec, memory access 1.0n sec の時に、page fault rate が、0.001% の時の effective access time を求めよ。

(3) 実メモリ上のpage fault rate が、0.001% の時の effective access time を求めよ。性能低下を 20%以下にするためには、page fault rate はいくらでなければならないか。

    effective access time = (1-p) x (memory access time) + p x (page fault time)

この結果を200文字程度に簡潔にまとめよ。
```

---

### \(1\)


`effective access time = (1-p) x (memory access time) + p x (page fault time)`

この式に当てはめる変数の中身は\(1\)の場合

`p`=0.01、`memory access time`=1.0n sec、`page fault time`=1μ sec = 1000.0n secである

式に代入すると

`effective access time = (1 - 0.01) x (1.0n sec) + 0.01 x (1000.0n sec) = 10.99n sec`

答えは`10.99n sec`

---

### \(2\)

\(1\)では、TLB miss したときにpage が実メモリにあったが、\(2\)ではpage が実メモリにある。

そのため、平均page-falut処理時間が\(1\)では1μ secだったのに対し、\(2\)では1m sec(1μ secの1000倍)である。

`effective access time`を求める式に、`p`=0.00001、`memory access time`=1.0n sec、`page fault time`= 1m sec = 1000000.0n sec`を代入すると

`effective access time = (1 - 0.00001) x (1.0n sec) + 0.00001 x (1000000.0n sec) = 10.99999n sec`

答えは`10.99999n sec`

---

### \(3\)

「実メモリ上のpage fault rate が、0.001% の時」と書かれているので、「TLB miss してpage が実メモリにある」と考える。

`effective access time`を求める式に、`p`=0.00001 、`memory access time`=1.0n sec、`page fault time`=1μ sec = 1000.0n secを代入すると

`effective access time = (1 - 0.00001) x (1.0n sec) + 0.00001 x (1000.0n sec) = 1.00999n sec`

`effective access time`は1.00999n secになる。

`memory access time`=1.0n sec、`page fault time`=1μ sec = 1000.0n secを固定して、性能低下\(率\)とpage fault rateの関係を考える。

性能低下率を`1 - (effective access time/page fault rate=0のeffective access time)`と定義する。

性能低下率が20%(=0.2)以下であることを数式で表現すると、

`0.2 >= 1 - {(1-p) x (memory access time) + p x (page fault time)}/memory access time`である

値を代入すると

`0.2 >= 1 - {(1-p) x (1.0n sec) + p x (1000.0n sec)}/1.0n sec = -999 x p`

となる。

`0.2 >= -999 x p`という不等式をこねくり回して`p <= 2/9990 = 0.0002002002 = 0.02002002%`に変形する。

結果、性能低下を20%以下に抑えるには、`page fault rate`は 0.02002002%以下でなければならないという結論になる。

有効数字とか全然意識せずに端数エグいことになっている計算ですが…ヨシザウルスの愛嬌に免じてということで…

---

## 8.4 mlock

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/028.html)の問題を解く

---

## 8.5 mmap によるコピー

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/194.html)の問題を解く

---

