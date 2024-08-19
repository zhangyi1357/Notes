# 大模型使用 -- LLM Usage

- [大模型使用 -- LLM Usage](#大模型使用----llm-usage)
  - [通用聊天大模型推荐](#通用聊天大模型推荐)
  - [编程领域大模型推荐](#编程领域大模型推荐)

记录一些好用的大模型及其相关信息，只记录 SOTA (State of the Art) 或者性价比超高值得关注的大模型，尤其关注编程方向。

## 通用聊天大模型推荐

- [DeepSeek Chat V2](https://chat.deepseek.com)

  免费无限使用的 [聊天官网](https://chat.deepseek.com)，[低廉的 API 价格](https://platform.deepseek.com/api-docs/zh-cn/pricing/)，[模型开源](https://github.com/deepseek-ai/DeepSeek-Coder)，[各大榜单名列前茅的实力](https://deepseek.com/)，美中不足的是回复速度稍慢，没有联网功能，无法分析文件、图片等。仅就聊天而言还是不错，如果需要联网、文件分析等功能可以考虑 gpt-4o 以及 kimi 等模型。

- [OpenAI gpt-4o](https://chatgpt.com/)

  SOTA 模型，几乎是目前最好的聊天模型，支持图片、文件等功能，可以联网查询，但是官网提供的免费聊天次数有限，充值订阅需要国外 VISA 信用卡，即使充值仍有 [聊天限额（80 条/3h）](https://help.openai.com/en/articles/7102672-how-can-i-access-gpt-4-gpt-4-turbo-gpt-4o-and-gpt-4o-mini#h_b04e3f8259)，而且价格较高（$20/月）。

- [OpenAI gpt-4o-mini](https://chatgpt.com/?model=gpt-4o-mini)

  快即是正义，更快的响应速度一方面能够提升体验，另一方面在编程等场景下方便快速迭代多轮对话，逐步改进。
  官网提供免费不限量的聊天。

- [MoonShoot Kimi](https://kimi.moonshot.cn/)

  中国国产大模型，支持联网查询，支持图片、文件等功能，200 万长上下文，尤其适合分析文档，联网查询等，且其联网搜索中文支持较好，引用网页内容较为准确。

大模型榜单：

- [Chatbot Arena LeaderBoard](https://huggingface.co/spaces/lmsys/chatbot-arena-leaderboard)
- [Livebench](https://livebench.ai/#)

## 编程领域大模型推荐

- [DeepSeek Coder V2](https://github.com/deepseek-ai/DeepSeek-Coder)

  免费无限使用的 [聊天官网](https://chat.deepseek.com/coder)，[低廉的 API 价格](https://platform.deepseek.com/api-docs/zh-cn/pricing/)，[模型开源](https://github.com/deepseek-ai/DeepSeek-Coder)，各大榜单名列前茅的实力 [^1](https://evalplus.github.io/leaderboard.html)[^2](https://aider.chat/docs/leaderboards/)，当之无愧的编程大模型首选。

- [Qwen/CodeQwen1.5-7B-Chat](https://github.com/QwenLM/CodeQwen1.5)

  Qwen 基本上也是国产开源大模型之光，在 [EvalPlus](https://evalplus.github.io/leaderboard.html) 等排行榜中也是名列前茅，在 7~9B 级别的模型中属于佼佼者，能够打败 DeepSeek-Coder-6.7B，7B 的参数量比较适合本地进行部署使用。

- Claude 3.5 Sonnet

  Reddit 以及知乎等平台多数人推荐，在 [Aider Chat LeaderBoard](https://aider.chat/docs/leaderboards/) 代码编辑和重构两个排行榜中分别排名第一和第二。

  TODO: 进一步介绍，添加引用链接等。

编程大模型榜单：

- [EvalPlus](https://evalplus.github.io/leaderboard.html)
- [Aider Chat LeaderBoard](https://aider.chat/docs/leaderboards/)