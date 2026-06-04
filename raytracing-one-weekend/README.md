# raytracing-one-weekend
Ray Tracing in One Weekend をやります。

## DIARY.md
人間が記録を書くためのファイル。

日付の代わりにcommit hashを書く。
今現在書いている文字が入ったcommit hashはわからないので、HEADとかにして次のcommitで訂正しておく。

git hooksにより、raytracing-one-weekend/ 配下に変更があった場合はこれも変更しないとcommitが拒否されるぞ！

```
#!/bin/sh
if git diff --quiet --exit-code  -- ./raytracing-one-weekend/; then
  if git diff --quiet --exit-code  -- ./raytracing-one-weekend/DIARY.md; then
    echo "DIARY.mdに変更がない"
    exit 1
  else
    echo 'ok'
    exit 0
  fi
fi
```
↑これを`.git/hooks/pre-commit`に書き込むのだ！


