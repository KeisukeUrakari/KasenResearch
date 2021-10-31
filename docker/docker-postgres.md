### コンテナ起動

```
$ docker run --rm -d \
    -p 15432:5432 \
    -v postgres-tmp:/var/lib/postgresql/data \
    -e POSTGRES_HOST_AUTH_METHOD=trust \
    postgres:12-alpine
```

* コンテナの停止時には削除する (--rm)
    * ボリュームにデータを保存する設定 (後述) なので、コンテナは消えても問題ない
* ホスト側のポートは適当に 15432 にした (ご自由に)
* ボリューム postgres-tmp を /var/lib/postgresql/data にマウントする
    * ボリュームが存在しない場合は新規作成される
    * データはこのボリュームに保存されるため、コンテナを削除しても保存したデータは削除されない
    * 環境変数 PGDATA を指定すればコンテナ内のデータディレクトリを変更できるが、特に変更する必要はないと思う
* パスワードなしでアクセスできる設定 (POSTGRES_HOST_AUTH_METHOD=trust)
    * もちろんセキュリティ的にはよくない設定なので、外部公開するサーバなどでやってはいけない
* イメージは 12-alpine (Alpine Linux ベースのバージョン 12 系最新) を使った
    * 他のバージョン： https://hub.docker.com/r/library/postgres/tags/

### 接続
```
$ psql -h localhost -p 15432 -U postgres
```

* 初回起動時に特に環境変数を指定しなかった場合は postgres というユーザと postgres というデータベースが作成される
* 初回起動時に環境変数でパスワードを指定しなかった場合はパスワードなしで接続できるが、その場合は localhost 以外からの接続はできない

### 参考
https://qiita.com/hoto17296/items/070857971f95017b5e07
https://hub.docker.com/r/library/postgres/tags/
