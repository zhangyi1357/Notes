# 大模型使用 -- LLM Usage

- [大模型使用 -- LLM Usage](#大模型使用----llm-usage)
  - [通用聊天大模型推荐](#通用聊天大模型推荐)
  - [编程领域大模型推荐](#编程领域大模型推荐)

记录一些好用的大模型及其相关信息，只记录 SOTA (State of the Art) 或者性价比超高值得关注的大模型，尤其关注编程方向。

## 通用聊天大模型推荐

- [DeepSeek Chat V2](https://chat.deepseek.com)

  免费无限使用的 [聊天官网](https://chat.deepseek.com)，[低廉的 API 价格](https://platform.deepseek.com/api-docs/zh-cn/pricing/)，[模型开源](https://github.com/deepseek-ai/DeepSeek-Coder)，[各大榜单名列前茅的实力](https://deepseek.com/)，美中不足的是回复速度稍慢，没有联网功能，无法分析文件、图片等。仅就聊天而言还是不错，如果需要联网、文件分析等功能可以考虑 gpt-4o 以及 kimi 等模型。

  目前主要将其 API 用于网页翻译，翻译质量相当不错。

- [OpenAI gpt-4o](https://chatgpt.com/)

  SOTA 模型，几乎是目前最好的聊天模型，支持图片、文件等功能，可以联网查询，但是官网提供的免费聊天次数有限，充值订阅需要国外 VISA 信用卡，即使充值仍有 [聊天限额（80 条/3h）](https://help.openai.com/en/articles/7102672-how-can-i-access-gpt-4-gpt-4-turbo-gpt-4o-and-gpt-4o-mini#h_b04e3f8259)，而且价格较高（$20/月）。

  个人主力使用的聊天模型，SOTA 多模态大模型，官网的免费使用额度不少。

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

- [Claude-3.5-sonnet](https://www.anthropic.com/news/claude-3-5-sonnet)

  Reddit 以及知乎等平台多数人推荐，在 [Aider Chat LeaderBoard](https://aider.chat/docs/leaderboards/) 代码编辑和重构两个排行榜中分别排名第一和第二，[LiveCodeBench](https://livecodebench.github.io/leaderboard.html) 排行榜中排名第三，仅次于 o1 系列。

  目前（2024.09.15）在 [cursor](https://cursor.sh/) 中 Pro 用户每月可以有 500 次快速高级模型调用，其中可以使用 Claude-3.5-sonnet 模型。

  个人实际在 cursor 中已经使用 Claude-3.5-sonnet 模型很长时间，感受是指令遵循能力非常强，总是可以遵循用户指令完成任务，进行代码生成补全能力优秀，已经是主力使用的编程大模型。

- [OpenAI o1-family](https://openai.com/index/learning-to-reason-with-llms/)

  o1 系列通过引入思维链（Chain of Thought, CoT），用更长的推理时间换取更好的推理性能，在 [各项 Benchmark](https://openai.com/index/learning-to-reason-with-llms/) 中都显著优于 gpt-4o，其中 [o1-mini](https://openai.com/index/openai-o1-mini-advancing-cost-efficient-reasoning/) 在预训练期间针对 STEM（Science, Technology, Engineering, Mathematics，STEM）领域进行了优化，以更小的模型尺寸实现了智能和推理方面对 o1 模型的超越，代价则是在需要非 STEM 领域上的事实知识能力有所欠缺，仅与 gpt-4o-mini 等小模型持平。

  当前（2024.09.15）仅有 o1-preview 和 o1-mini 模型开放给用户使用，且使用限制相当严格，ChatGPT Plus 用户**每周**仅能调用 o1-preview 30 次，o1-mini 50 次，而 API 用户则需要达到 Tier 5 级别，即累计消费 1000 美元，才能在 API 中调用这两个模型，单次 o1 的 API 调用价格达到了 $0.6。[目前 cursor 中 Pro 用户可以使用 o1-mini 模型，且不限制调用次数](https://forum.cursor.com/t/openai-new-o1-models/16357/19)。

  [Livebench](https://livebench.ai/#) 排行榜的 Coding 分类包含两个测试，分别是 LCB_generation 和 coding_completion，o1 系列在 LCB_generation 测试中排名前列，o1-mini 第一，o1-preview 与 claude-3.5-sonnet 并列第二，在 coding_completion 测试中，o1 系列的表现则非常糟糕，o1-preview 与 gpt-4o-mini 持平，o1-mini 得分低于 gpt-4o-mini 约 25%。

  这是一个很有趣的现象，LCB_generation 和 coding_completion 可以分别理解为代码生成和代码补全，具体解释为

  * LCB Generation：评估模型解析编程竞赛问题并给出正确答案的能力。使用了 LiveCodeBench 提供的 50 个问题。
  * coding_completion：评估模型完成部分正确代码的能力。这个任务使用 LeetCode 的中等和困难问题，并从给定的代码中去掉最后 15% 的部分，要求 LLM 补全。

  上述 o1 系列在两个测试中表现出的差异性，一个简单的 [猜测](https://www.reddit.com/r/LocalLLaMA/comments/1ffovpa/comment/lmycggv/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button) 是：思维链上的多个步骤使得原始输入代码发生变化，导致最终输出时未能真正完成输入代码，而是显示了它认为能解决问题的代码的新版本，而在代码生成中，这不是问题，因为可以自由迭代其生成的代码，直到输出最终版本。

  以上现象可以为我们选择编程大模型提供一个参考，如果需要代码生成能力，尤其是针对 Leetcode 等编程竞赛题目或是生成一个完整文件，选择 o1 系列，如果需要从现有代码中补全代码，则选择 claude-3.5-sonnet。

编程大模型榜单：

- [EvalPlus](https://evalplus.github.io/leaderboard.html)
- [Aider Chat LeaderBoard](https://aider.chat/docs/leaderboards/)
- [LiveCodeBench](https://livecodebench.github.io/leaderboard.html)