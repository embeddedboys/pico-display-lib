# AGENTS.md

本仓库的工作规则，供 AI agent（以及人）在改动前先读一遍。

本仓库是显示/板级库（面板驱动、总线、解码器、输入），被 `Pico-USB-Display` 固件以子模块
方式使用。**面板配置来自固件仓的 `configs/`**，不要在本仓库里长第二套。

---

## 铁律

1. **未经明确指令，不要 `git commit`，更不要 `git push`。**
2. **仓库内不得出现内网/个人信息**：本机绝对路径、内网 IP、口令、板子序列号。
3. **vendored 解码器（`rgb565_qoi` / `rgb565_rle` 等）必须与上游逐字节一致**：要改行为先改
   上游再把文件整体拷回，编译期差异走它们的钩子宏。
4. **触摸坐标只在本库变换一次**（`indev.c` 里按 `indev_dir_for_resolution()` 做），各触摸驱动
   只返回控制器原始值 —— 往驱动里塞 `set_dir()` 会让旋转不跟着走。
5. **总线时钟是编译期常量**：`drivers/bus/pio_i80.c` 与 `pio_spi_tx.c` 用
   `DEFAULT_PIO_CLK_KHZ / 2 / TFT_BUS_CLK_KHZ` 算分频，而 `DEFAULT_PIO_CLK_KHZ` 又跟着
   `PERI_CLK_KHZ` —— **提高 clk_sys 会连带把面板总线时钟按同比例抬高**。
   这是与超频库（pico-turbo）集成时的已知阻塞点，改之前先读
   [pico-turbo 的实测记录](https://github.com/IotaHydrae/pico-turbo/blob/main/docs/measurements.md)，
   不要默认"面板跟得上"。
6. **不要在会被中断/回调调用的路径里做阻塞等待**（总线等待、`sleep`）—— 固件侧对应规则是
   "面板寄存器写不在 USB 中断里做"。

## 提交与身份

- `user.name` = `Wooden Chair`，`user.email` = `hua.zheng@embeddedboys.com`；`git commit -s`
- 内核风格提交信息；一个逻辑改动一个提交
- 本仓库是固件的子模块：**先在这里提交，再到固件仓更新指针**，并在固件仓说明跟着动了什么

## 构建与验证

本库没有独立入口，验证方式是构建固件：

```bash
# 在 Pico-USB-Display 里
./build.sh lunch     # 选板子 + 面板配置
./build.sh           # 配置 + 构建
./build.sh flash     # 烧录
```

- **不需要内核驱动也能验证显示路径**：固件仓的 `tools/`（pyusb 直连）与 `tests/`
  跑一遍给出对/错结论。
- 改面板驱动/总线/输入后，至少要跑一遍固件仓的 `tests/rotation_test.py`（几何 + 旋转）
  与 `tests/touch_test.py`（触摸），并在真机上确认画面（顶栏像素错位的 bug 就是分辨率与
  旋转没成对处理造成的）。

## 代码约定

- 内核风格：tab + 8 宽缩进（`.clang-format` 取自内核，`UseTab: ForIndentation`）。
- 新增源文件要加进对应的 `CMakeLists.txt`。
- 只写已验证的结论；推测显式标"未验证"。
