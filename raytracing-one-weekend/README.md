# raytracing-one-weekend
Ray Tracing in One Weekend をやります。

## DIARY.md
人間が記録を書くためのファイル。

日付の代わりにcommit hashを書く。
今現在書いている文字が入ったcommit hashはわからないので、HEADとかにして次のcommitで訂正しておく。

git hooksにより、raytracing-one-weekend/ 配下に変更があった場合はこれも変更しないとcommitが拒否されるぞ！

```
#!/bin/sh

if git diff --cached --quiet --exit-code  -- ./raytracing-one-weekend/; then
  :
else 
  DIARY_FILE="./raytracing-one-weekend/DIARY.md"
  if git diff --cached --quiet --exit-code  -- "$DIARY_FILE"; then
    echo "DIARY.mdに変更がない"
    exit 1
  else
    TARGET_LINE=$(grep -n "## HEAD" $DIARY_FILE | sed -n '1s/:.*//p')
    if [ -n "$TARGET_LINE" ]; then
      COMMIT_HASH=$(git rev-parse --short HEAD)
      sed -i "${TARGET_LINE}s/## HEAD/## $COMMIT_HASH/" $DIARY_FILE 
      git add $DIARY_FILE
    fi
    exit 0
  fi
fi
```
↑これを`.git/hooks/pre-commit`に書き込むのだ！


