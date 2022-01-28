# OSの講義の4.xの問題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

4.2から4.5はkono先生からメールで各自ランダムな数字を与えられるので、その数字に該当する教科書の問題(Exercisesにある)を解くといったもの

こんな風にメールで問題が与えられる、一人一人が違う問題を与えられる

該当する番号の問題がなかったら好きなの解いていい\(って質問チャンネルで書いてた\)、ただ簡単すぎるのを選ぶのは...ね?

```
Your OS assignments for Paperback are
Chapter 11 ex 11.15
Chapter 6 ex 6.17
Chapter 12 ex 12.5
Chapter 21 ex 21.15
```

で、[この講義ページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os04/lecture.html)の通りに、4.2から4.5までの課題としてこれらの問題を解く

---

## 4.1 シミュレータ

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/205.html)の問題を解く

んー難しい

また今度!

---

## 4.2 教科書の問題 Chapter 7 ex 7.7

メールでは11.15と書かれていたがChapter 11に問題がなかった

7章\(Interprocess Communication\)から問題を選ぶ

7.7が面白そう

教科書の7.7の問題を解く、問題文は以下の通り、

```
    In this example the character names bear similarity with the Gauls. Gauls have a chief evaluator of art. His name is Evalutrix and his job is to evaluate every piece of art which is brought in the Gaul land. He has advisors whose names are Moodix, Chancix, and Gamblix. When a piece of art is brought up to Evalutrix he contacts his advisors indicating the likely base price. The three advisors use the following systems to indicate their evaluations.  


    a. Chancix always tosses a coin. If it is heads he increases the evaluation by 5 per cent. If it is tails, he reduces the evaluation by 5 per cent.

    b. Gamblix shuffles a pack of cards and draws a card from it. The face value of the card determines the percentage change he would suggest. If the card is a one from the black suit, he increases the evaluation. If it is from the red suit, then he decreases the evaluation.

    c. Moodix always takes a look at the real-time clock on his system. He adds up the digits in hours, and minutes and subtracts 15 from it. Whatever the result is, that percentage change in value he communicates to Evalutrix.


Evalutrix averages all the values including his own base price and declares the result as the value of the art. Simulate Evalutrix as the parent process, and his advisors as child processes with the base price being given as the inline argument.
```

---

### 和訳

```
この例では、登場人物名がガリア人のと似ている。ガリア人には芸術品の最高評価者が存在し、その名をEvalutrixという。彼には、Moodix、Chancix、Gamblixという名のアドバイサーがいる。美術品がEvalutrixに持ち込まれると、彼はアドバイサー3人に連絡して、その芸術品の基本価格を提示する。3人のアドバイサーは、以下のような方法で評価をする。

a. Chancixはコインを投げて、表が出れば5%上げる、裏が出れば5%下げる。

b. Gamblixはカードのパックをシャッフルして、そこからカードを1枚引き黒のスートならば評価を上げて、赤のスートならば評価を下げる。カードの番号が提示する変化の割合(%)を決定する。

c. Moodixは、時計を見る。時計の「時」と「分」の数字を足し合わせて、15を引く。その値の変化の割合(%)をEvalutrixに伝える。

Evalutrixは、自分の基本価格とa, b, cの提示した値の平均をとって、その結果を芸術品の価値として、宣言する。

Evalutrixを親プロセスと、アドバイサー達を基本価格をインライン引数とする子プロセスとしてシミュレートしなさい。
```

---

### 解く

システムコール(fork)を使いプロセスを生成して、パイプ(pipe)によるプロセス間通信をする

上記の操作をC言語で実装してEvalutrixを親プロセス、アドバイサー達を子プロセスとして扱い、芸術品の価値を決定するスクリプトを作成する。

---

### 答え\(日本語\)

---

### 答え\(英語\)

---

## 4.3 教科書の問題 Chapter 6 ex 6.8

メールでは6.17と書いてたが問題が存在しなかったので、6章の中の面白そうな問題を選んだ

教科書の6.8の問題を解く、問題文は以下の通り、

```
Dining philosophers problem:

    This is a very famous and often quoted problem in OS research. It reflects considerable degree of concurrency in operation. It also requires synchronisation of actions. We will assume that the philosophers are all Japanese and love eating the Japanese rice (Gohan in Japanese). Five philosophers sit around a circular table. Each philosopher spends his life alternatively thinking and eating. In the centre of the table is a bowl of rice and there is a plate in front of each philosopher. Clearly, philosophers would need two chopsticks to have a helping of rice. The philosophers have five chopsticks which are placed between their plates. So each plate has a chopstick to its right and to its left. The philosophers only use the chopsticks to their immediate right and left.

    How would the philosophers need to schedule their thinking and eating?

(Hint: Think of each chopstick as a semaphore.)
```

---

### 和訳

```
dining philosophers問題:

    OSの研究において非常に有名でよく引用される問題である。哲学者はみんな日本のご飯を食べるのが大好きな日本人で、全員で5人が円卓を囲んでいると想定する。哲学者はそれぞれ全員、考えることと食べることを交互に繰り返して暮らしている。テーブルの中心にはご飯が盛られたボウルがあり、哲学者一人一人の前に皿が置かれている。ご飯を食べるには箸が2本(1膳)必要である。哲学者達は合わせて5本の箸を持ち、それぞれの皿の間に置いている。つまり、皿の右側と左側に1本の箸があるということを意味する。哲学者はすぐ右と左の箸しか使えない。

    哲学者たちは、どのように思考と食事のスケジュールを立てる必要があるか?

(ヒント: それぞれの箸をsemaphoreと考える)
```

---

### 答え\(日本語\)

---

### 答え\(英語\)

---

## 4.4 教科書の問題 Chapter 12 ex 12.5

教科書の12.5の問題を解く、問題文は以下の通り

```
What is a basic tool? Give three examples of tools you use often.
```

---

### 和訳

```
basic toolとは何か? あなたがよく使うツールの例を3つ挙げよ。
```

---

### 答え\(日本語\)

---

### 答え\(英語\)

---

## 4.5 教科書の問題 Chapter 9 ex 9.13

メールでは21.15と書かれていたがChapter 21に問題がなかった

9章\(Real-Time Operating Systems and Microkernels\)から問題を選ぶ

教科書の9.13の問題(実は#4.2でといた教科書の7.7と繋がっている)を解く、問題文は以下の通り、

```
    Gauls whom we came across in the exercises on interprocess communication are back again here. GetAFix, the druid who makes the magic potion, uses herb concentrates for his magic potion. However in the preparation of the potion,4 some enzymes are used as catalysts. The concentration of enzymes affects the rate of reaction. Additionally, the volume of the herb concentrate and its temperature need to be regulated. Note that the total time for brewing is also critical. What kind of schedule would you propose to GetAFix? Will any one of the three scheduling methods, namely rate-monotonic, earliest deadline first or earliest laxity first, by itself be adequate?
```

---

### 和訳

```
    プロセス間通信の演習で出会ったガリア人が、ここでも登場する。魔法薬を作るドルイドである、GetAFixはハーブの濃縮液を使っている。しかし、魔法薬の調合には触媒としていくつかの酵素が使われている。酵素の濃度は、反応速度に影響を与える。さらに、ハーブの濃縮液の量や温度も調節する必要がある。また、醸造にかかる総時間も重要である。GetAFixにどのようなスケジュールを提案したら良いだろうか。rate-monotonic、earliest deadline first、earliest laxity firstの3つのスケジューリングを行う方法のうち、どれか1つだけでも十分になるだろうか?
```

---

### 答え\(日本語\)

---

### 答え\(英語\)

---
