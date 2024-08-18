# Github copilot 编程助手使用

- [Github copilot 编程助手使用](#github-copilot-编程助手使用)
  - [中文设置](#中文设置)
  - [快捷键设置和使用](#快捷键设置和使用)
  - [Copilot Chat 使用技巧](#copilot-chat-使用技巧)
    - [聊天参与者](#聊天参与者)
    - [斜杠命令](#斜杠命令)
    - [聊天变量](#聊天变量)
  - [提示工程](#提示工程)

## 中文设置

设置 `github.copilot.chat.localeOverride` 为 `zh-CN` 可以将 Github Copilot 的提示语言设置为中文。

```json
{
"github.copilot.chat.localeOverride": "zh-CN",
}
```

## 快捷键设置和使用

Github Copilot 提供了一些快捷键，可以在编辑器中快速使用。

- `Ctrl + I`：打开 Copilot Chat 内联聊天（编辑器中）
- `Ctrl + Alt + I`：聚焦在 Copilot Chat 视图（侧边栏的 Copilot Chat）
- `Ctrl + Shift + I`：打开 Copilot Chat 快速聊天（顶部命令位置的 Copilot Chat）
- `Alt + ]`：下一个 Copilot 提示（编辑器中）
- `Alt + [`：上一个 Copilot 提示（编辑器中）
- `Tab`：接受 Copilot 提示（编辑器中）
- `Ctrl + ➡`：接受 Copilot 提示的下一个词（编辑器中）

此外我还对一些其他快捷键进行了重新绑定并设定了一些方便使用的快捷键，

- `Alt + Backspace`：打开 Copilot 提示面板（新窗口中）
- `Ctrl + \`：触发 Copilot 提示（编辑器中）
- `Ctrl + Shift + →`：接受 Copilot 提示的一行（编辑器中）

`keybindings.json` 中配置如下：

```json
{
    {
        "key": "ctrl+enter",
        "command": "-github.copilot.generate",
        "when": "editorTextFocus && github.copilot.activated && !inInteractiveInput && !interactiveEditorFocused"
    },
    {
        "key": "alt+backspace",
        "command": "github.copilot.generate",
        "when": "editorTextFocus && github.copilot.activated && !inInteractiveInput && !interactiveEditorFocused"
    },
    {
        "key": "ctrl+oem_5",
        "command": "editor.action.inlineSuggest.trigger",
        "when": "config.github.copilot.inlineSuggest.enable && editorTextFocus && !editorHasSelection && !inlineSuggestionsVisible"
    },
    {
        "key": "shift+ctrl+right",
        "command": "editor.action.inlineSuggest.acceptNextLine",
        "when": "inlineSuggestionVisible && !editorReadonly"
    },
}
```

## Copilot Chat 使用技巧

参考链接：[在 IDE 中向 GitHub Copilot 提问](https://docs.github.com/zh/copilot/using-github-copilot/asking-github-copilot-questions-in-your-ide)

### 聊天参与者
使用聊天参与者将提示范围限定在特定域内。 要使用聊天参与者，请在聊天提示框中键入 @，然后再键入聊天参与者名称。 聊天参与者包括：

- `@workspace`：在工作区中具有关于代码的上下文。 当希望 Copilot 考虑项目的结构、代码的不同部分如何进行互动或者项目中的设计模式时，请使用 @workspace。
- `@vscode`：具有有关 Visual Studio Code 命令和功能的上下文。 当需要有关 Visual Studio Code 的帮助时，请使用 @vscode。
- `@terminal`：具有有关 Visual Studio Code 终端 shell 及其内容的上下文。 当需要帮助创建或调试终端命令时，请使用 @terminal。

要查看所有可用的聊天参与者，请在聊天提示框中键入 @。

### 斜杠命令
使用斜杠命令避免为常见场景编写复杂的提示。 要使用斜杠命令，请在聊天提示框中键入 /，然后键入命令。 斜杠命令包括：

- `/tests`：为所选代码生成单元测试
- `/fix`：为所选代码中的问题提供修复建议
- `/explain`：解释所选代码
- `/clear`：开始新聊天（`Ctrl + L` 快捷键可以达到同样效果）

要查看所有可用的斜杠命令，请在聊天提示框中键入 /。

### 聊天变量
使用聊天变量在提示中包含特定上下文。 要使用聊天变量，请在聊天提示框中键入 #，然后键入聊天变量。 聊天变量包括：

- `#file`：在聊天中包含特定文件作为上下文。
- `#git`：包含有关当前 Git 存储库的信息。
- `#terminalLastCommand`：包含 Visual Studio Code 活动终端中上次运行的命令。

要查看所有可用的聊天变量，请在聊天提示框中键入 #。

## 提示工程

refs：

[How to use GitHub Copilot: Prompts, tips, and use cases](https://github.blog/developer-skills/github/how-to-write-better-prompts-for-github-copilot/)
