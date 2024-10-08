# Settings

各种软件的配置。

## VS Code

TODO：VS Code 的配置文件，Settings，Keybindings，Snippets，Extensions。

### AI 生成 commit message

[使用 gitlens 插件结合 OpenAI api 实现 AI 生成 commit message](https://linux.do/t/topic/181361)

settings.json 配置：

```json
{
  "gitlens.experimental.generateCommitMessagePrompt": "You are an expert software engineer. Review the provided context and diffs which are about to be committed to a git repo. Review the diffs carefully. Generate a commit message for those changes. The commit message MUST use the imperative tense. The commit message should be structured as follows: <type>: <title> The commit message can come with an optional description after the title with a blank line. Remember don't make the title too long. Use these for <type>: fix, feat, build, chore, ci, docs, style, refactor, perf, test Reply with JUST the commit message, without quotes, comments, questions, etc!",
  "gitlens.ai.experimental.openai.url": "https://api.keya.pw/v1/chat/completions",
  "gitlens.ai.experimental.model": "openai:gpt-4o-mini",
}
```

## [Clangd](https://clangd.llvm.org/)

Clangd 配置文件，`~/.config/clangd/config.yaml`：

```yaml
# ~/.config/clangd/config.yaml
If:                               # Apply this config conditionally
  PathMatch: [.*\.h, .*\.cpp, .*\.cpp, .*\.cc]  # to all headers...
CompileFlags:                        # Tweak the parse settings
  Add: [-xc++, -std=c++20, -I/usr/include/python3.10]  # treat all files as C++, enable more warnings
Diagnostics:
  ClangTidy:
    Add:
      - bugprone-*
      - modernize-*
      - performance-*
      - cpp-core-guidelines-*
      - clang-analyzer-*
      - cert-*
      - concurrency-*
      - google-*
      - readability-*
    Remove:
      - bugprone-unchecked-optional-access
      - bugprone-lambda-function-name
      - modernize-use-trailing-return-type
      - modernize-use-using
      - readability-identifier-length
      - readability-magic-numbers
      - readability-function-cognitive-complexity
      - readability-redundant-access-specifiers
      - readability-implicit-bool-conversion
      - readability-braces-around-statements
      - google-readability-braces-around-statements
  UnusedIncludes: Strict

---

If:
  PathMatch:
    - /usr/include/.*
Diagnostics:
  Suppress: "*"
```

VS Code 配置 Clangd 命令行启动参数：
```json
{
    "clangd.arguments": [
        "--background-index",
        "-j=8",
        "--clang-tidy",
        "--all-scopes-completion",
        "--completion-style=detailed",
        "--header-insertion=iwyu",
        "--pch-storage=memory",
        "--suggest-missing-includes"
    ],
}
```

## Git

基本配置项，分别为：用户名，邮箱，编辑器，凭证存储。

```bash
git config --global user.name "zhangyi"
git config --global user.email "zhangyi_1357@sjtu.edu.cn"
git config --global core.editor "vim"
git config --global credential.helper store
git config --global core.excludesfile ~/.gitignore_global
```

全局忽略文件 `~/.gitignore_global` 内容：

```gitignore
# ~/.gitignore_global
.vscode/
.history/
.cache/

build/
debug.sh
```