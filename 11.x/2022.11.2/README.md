- OSの課題の「11\.1」が終わっていることが前提の課題である
  - 「1\.4」も終わっていることが望ましい
- `~/.ssh/config`を「11\.1」で作った自分のVMの情報\(ユーザー名、IPアドレス\)に合わせて以下のように編集する
  - HostNameとUserは各個人で違うので編集しよう
  - ```
    Host fedora
      HostName <Akatsukiで取得したIPアドレス>
      User ysano
      ProxyJump amane 
    ```
- `./inventories/hosts`の中身を以下のように自分のVMに合わせて記述する
  - ansible_ssh_userとuser_nameの中身は各個人で違う\(ysanoではなく別の値\) ので編集しよう
    - ansible_ssh_userとuser_nameは2つとも同じ値にすること
  - ansibleでのhugo + nginxのセットアップでは、hugoのサイトのレポジトリは「1\.4」で使った「`hugo-template`」を使う
  - ```
    [all]
    fedora
    
    [all:vars]
    ansible_ssh_user=ysano
    ansible_ssh_private_key_file=~/.ssh/id_rsa
    user_name=ysano
    gitlab_repository_url=https://gitlab.ie.u-ryukyu.ac.jp/ie-web/hugo-template/hugo-template.git
    ```
- 「11\.1」の課題で作成したVMが起動していることを確認したら次のコマンドを実行する
- `$ ansible-playbook -i inventories/hosts site.yml --ask-become-pass`
- `$ ssh -L xxxx:localhost:80 -N fedora`\(xxxxの例: 8080など\)を実行して、ローカルPCで「`http://localhost:xxxx`」にアクセスするとブログが見れる
