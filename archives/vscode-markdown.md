# VSCode 中优雅地编写 Markdown

在 VSCode 中编写 Markdown 有几个无法拒绝的优势，首先是顺手方便，常写代码的同学打开 VSCode 各项功能和快捷键使用的都比较熟练，可以提高效率，其次可以方便地集成 Git 对文件进行版本管理，最后也是最重要的一点，可以无缝集成 Copilot 辅助编写内容，不管是写文档还是笔记都可以提供诸多便利。

首先会介绍几个专门用于 Markdown 的插件，然后会推荐几个 VSCode 上提高效率的插件，会对各个插件基本的使用和配置进行详细的介绍以方便使用，最后会说说如何将笔记上传到博客园和知乎。

- [VSCode 中优雅地编写 Markdown](#vscode-中优雅地编写-markdown)
  - [插件](#插件)
    - [Markdown All in One](#markdown-all-in-one)
    - [Markdownlint](#markdownlint)
    - [Pangu-Markdown](#pangu-markdown)
    - [Markdown Preview Enhanced](#markdown-preview-enhanced)
    - [Word Count CJK](#word-count-cjk)
    - [Local history](#local-history)
  - [设置项](#设置项)
  - [发布到博客园/知乎](#发布到博客园知乎)

## 插件

### [Markdown All in One](https://marketplace.visualstudio.com/items?itemName=yzhang.markdown-all-in-one)

![figs/vscode-markdown-image.png](figs/vscode-markdown-image.png)

该插件功能非常强大，提供了快捷键、目录、自动预览、图片快速上传等功能。

首先是快捷键功能，快捷键列表如下

| 快捷键              | 命令                     |
|-------------------|------------------------|
| Ctrl/Cmd + B     | 切换加粗                  |
| Ctrl/Cmd + I     | 切换斜体                  |
| Alt+S (Windows)  | 切换删除线                |
| Ctrl + Shift + ] | 切换标题（上级）           |
| Ctrl + Shift + [ | 切换标题（下级）           |
| Ctrl/Cmd + M     | 切换数学环境               |
| Alt + C          | 勾选/取消勾选任务列表项      |
| Ctrl/Cmd + Shift + V | 切换预览               |
| Ctrl/Cmd + K V   | 切换侧边预览               |

如果部分快捷键无法使用，大概率是快捷键冲突了，请检查自己的快捷键设置，其实常用的大概也就是粗体、斜体、切换标题、数学环境这几个。

还支持目录功能，按 `Ctrl + Shift + P` 呼出任务面板，然后搜索 `create table` 即可，搜索 `add section numbers` 还可以添加数字小标题如下：

![Alt text](figs/vscode-markdown-section-numbers.gif)

另外在指定内容上插入超链接的功能非常好用，只需要复制链接然后选中内容粘贴即可，如下：

![Alt text](figs/vscode-markdown-paste-link.gif)

### [Markdownlint](https://marketplace.visualstudio.com/items?itemName=DavidAnson.vscode-markdownlint)

![Alt text](figs/vscode-markdown-image-1.png)

该插件可以对 Markdown 文件的进行规范检测并执行自动修复，推荐使用。

### [Pangu-Markdown](https://marketplace.visualstudio.com/items?itemName=xlthu.Pangu-Markdown)

![Alt text](figs/vscode-markdown-image-2.png)

这个插件会自动为 Markdown 文件的中英文之间添加空格，中英文符号转换等。

### [Markdown Preview Enhanced](https://marketplace.visualstudio.com/items?itemName=shd101wyy.markdown-preview-enhanced)

![Alt text](figs/vscode-markdown-image-3.png)

### [Word Count CJK](https://marketplace.visualstudio.com/items?itemName=holmescn.vscode-wordcount-cjk)

![Alt text](figs/vscode-markdown-image-4.png)

中文字数统计，显示在底部状态栏左边。

### [Local history](https://marketplace.visualstudio.com/items?itemName=xyz.local-history)

![Alt text](figs/vscode-markdown-image-5.png)

本地文件历史记录，每次保存都会将正在编辑的文本文件保存到当前工作区下的 `.history/` 目录下，文本文件占不了多少存储，但是能有效防止丢内容。何况累积多了也可以自己删除其中的内容。

如果配合 Git 使用的话可以设置一个全局的 `.gitignore` 文件屏蔽 `.history/` 目录，这样就不需要在每个项目的 `.gitignore` 文件中屏蔽该目录了。

## 设置项

主要是设置默认粘贴图片文件的保存位置为 `figs` 目录，然后保存时自动格式化等。

```json
  "markdown.copyFiles.destination": {
      "*.md": "figs/${documentBaseName}-${fileName}"
  },
  "[markdown]": {
      "editor.defaultFormatter": "DavidAnson.vscode-markdownlint"
  },

  "pangu.auto_format_on_save": true,

  "markdown-preview-enhanced.codeBlockTheme": "github-dark.css",
  "markdown-preview-enhanced.previewTheme": "one-dark.css",
  "markdown-preview-enhanced.automaticallyShowPreviewOfMarkdownBeingEdited": true,
  "markdown-preview-enhanced.imageFolderPath": "/figs",

  "markdownlint.focusMode": false,
  "markdownlint.run": "onType",
```

## 发布到博客园/知乎

发布到知乎可以使用 [Zhihu On VSCode](https://zhuanlan.zhihu.com/p/106057556) 插件。

但是知乎的图片上传功能就是依托答辩，无法支持使用本地图片的 markdown 文档，我也不想搞任何的图床，所以想到了一个曲折的方式，首先发布到博客园，博客园官方有 [VSCode 插件](https://marketplace.visualstudio.com/items?itemName=cnblogs.vscode-cnb)，这个插件可以将所有本地图片上传到博客园自己的图床上，然后替换本地图片链接，这样就可以发布到知乎上了。
