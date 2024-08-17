# Github

## 如何清理 Github 仓库，缩小仓库空间占用

参考文章 [how-reduce-git-repository-size-sasan-soroush](https://www.linkedin.com/pulse/how-reduce-git-repository-size-sasan-soroush)，成功将本仓库体积从 77M 缩小到 3.7M。

使用 [bfg](https://rtyley.github.io/bfg-repo-cleaner/?trk=article-ssr-frontend-pulse_little-text-block) 清理工具，大致步骤：

1. 安装 bfg 工具

    使用相应的包管理器安装 bfg，在 ArchLinux 下，
    ```bash
    sudo pacman -S bfg
    ```

2. 运行命令查看文件占用体积

    ```bash
    git rev-list --objects --all | git cat-file --batch-check='%(objecttype) %(objectname) %(objectsize) %(rest)' | sed -n 's/^blob //p' | sort --numeric-sort --key=2 | cut -c 1-12,41- | $(command -v gnumfmt || echo numfmt) --field=2 --to=iec-i --suffix=B --padding=7 --round=nearest
    ```

3. 使用 bfg 删除大文件

    bfg 有三种删除模式，分别超过指定文件大小、指定文件夹、指定文件。
    ```bash
    bfg --strip-blobs-bigger-than 50M
    bfg --delete-files id_{dsa,rsa}
    bfg --delete-folders somefolder
    ```
    每次运行完 bfg 命令后需要按照 bfg 命令的提示运行如下 git 命令对内容进行清理：
    ```bash
    git reflog expire --expire=now --all && git gc --prune=now --aggressive
    ```
