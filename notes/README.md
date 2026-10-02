# pico-display-lib 知识库

本目录只放**这个库自己的**知识：面板配置与驱动的约定、实测基线、以及踩过的坑。
固件侧（Pico-USB-Display）的知识在固件仓的 `notes/`，协议定义在驱动仓，都不在这里重复。

## 文档索引

| 文档 | 一句话内容 |
| --- | --- |
| [panel-configs.md](panel-configs.md) | 面板配置三类属性的划分，以及"红蓝反/转 90°/边缘留边"分别该拧哪个旋钮 |
| [performance-baseline.md](performance-baseline.md) | 重构前的实测基线：USB 链路、解码、刷图、原始 fps、PIO 8080 带宽，以及瓶颈归属 |

## 维护约定

- 面板/总线的**实测数字**进 `performance-baseline.md`；**诊断经验**（症状 → 原因 → 旋钮）进 `panel-configs.md`。
- 只写已验证的结论，推测显式标"未验证"；引用绝对速率时必须带主机/端口条件。
- 单篇超过约 150 行做压缩审查，超过约 300 行考虑拆分。
- 通用约定（提交身份、铁律、测试分层）见仓库根的 `AGENTS.md` 与 `CLAUDE.md`。
