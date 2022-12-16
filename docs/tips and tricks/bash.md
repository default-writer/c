
# pre-commit

- Q: pre-commit message hook contains illegal characters (`\r`)
- A: remove `\r` characted from file

```sh
sed 's/\r$//' ./.hooks/pre-commit > ./.hooks/tmp && mv ./.hooks/tmp ./.hooks/pre-commit
```