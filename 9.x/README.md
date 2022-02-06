# OSの講義の9.xの問題を解く

これが正解だぜ(どや)みたいなノリでは書いてないです、ただの平均的な技術力を持っているB2が解いているだけ

---

## 9.1 Backup

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/140.html)の問題を解く

```
情報工学を学ぶものは、自分で運営する Web Service の一つや二つを持っているだろう。

自分自身のファイルのバックアップ戦略について Cloud と 自分の Web Service を含めて、提案せよ。

以下のことを考慮すること。

    最低、三重のコピーを維持する必要があるのは何故か。
    ファイル履歴はどこまで持つべきか。
    Apple の Time Capsule が Web Service に対して使用できないのは何故か。
    バックアップに要するコストは妥当か。
```

---

## 9.2

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/230.html)と[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os09/gpgsm.html)と[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/os09/openssl-smime.html)の問題を解く

---

## 9.3 Side Channel Attack

[このページ](https://ie.u-ryukyu.ac.jp/~kono/lecture/os/ex/problem/195.html)の問題を解く

```
Side Channel Attach とは何かを100文字程度で説明せよ。

例示した Sice Channel Attach に対する対策を示せ。
```

---

### Side Channel Attackとは

サイドチャネル攻撃とは、プログラムの欠陥を直接狙うのではなく、システムのプログラムを実行したときの処理時間やハードウェアの物理的な変化を観測して得られた情報から解析を行って暗号化されたデータを盗み取る攻撃である。Side Channel Attackの種類は情報の収集・解析方法によって多岐に分かれる。

---

### 対策

前述したと通り、Side Channel Attackには攻撃方法によって細かく種類が分かれるので、それぞれの攻撃方法によって対策の仕方が分かれる。今回はメッセージ長を用いるCompression Side Attackを取り上げて、対策を示す。Compression Side Attackは、圧縮が絡む処理で攻撃者が自分のデータを送信して秘密情報と一緒に圧縮して、圧縮の出力の長さから秘密情報の推測を行う攻撃手法である。


データ圧縮を行うとデータの冗長性を排除することである。LZ77という圧縮アルゴリズムを例に説明する。LZ77はデータに含まれる記号列が過去に出現したかどうかを調べ、出現していればその位置と長さの値で置き換えるアルゴリズムである。以下の文字列を圧縮する場合を考える。

「I thought what I’d do was, I’d pretend I was one of those deaf-mutes.」

この文字列には「I」、「'd」、「was」という文字列を複数含む。これらのような同じ文字列に相当するデータを1つだけ残して、そのコピーをどこに配置するかを示す指示を出力してLZ77はデータに圧縮を行う。


攻撃者のデータを「A」、推測したい暗号情報を「B」とし、AとBを一緒に圧縮したときにAとBに同じ部分があれば、データの圧縮が効くということになるので、圧縮の出力の長さからBを予測することができる。このようなBを予測する推測材料のことを圧縮オラクルと呼ぶ。

対策についてだが、TLS1.2の仕様[1]のSrction 6から引用して提示する。

```
Any protocol designed for use over TLS must be carefully designed to
deal with all possible attacks against it.  As a practical matter,
this means that the protocol designer must be aware of what security
properties TLS does and does not provide and cannot safely rely on
the latter.
Note in particular that type and length of a record are not protected
by encryption.  If this information is itself sensitive, application
designers may wish to take steps (padding, cover traffic) to minimize
information leakage.
```

Compression Side AttackはTLSプロトコルのレコードの型と長さが暗号化によって保護されない脆弱性を突いている攻撃だが、これはプロトコルの欠陥ではなく、プロトコルの限界である。仕様によれば、アプリケーションの設計者がパディングやトラフィック保護をすることで対策することを推奨されてる。

[1] The Transport Layer Security (TLS) Protocol, https://www.ipa.go.jp/security/rfc/RFC5246EN.html#06

---

### 9.3の感想

- 攻撃と対策はイタチごっこになるみたいな話をよく聞くがこのレベルまで議論を深堀りしたら、そりゃイタチごっこになるわー...って思いました
