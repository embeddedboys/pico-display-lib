# CLAUDE.md

供 AI agent（以及人）在本仓库改动前先读一遍的规则。这里只写**必须遵守的约束**，
细节知识看 `README.md`。

## 提交与身份

- `user.name` = `Wooden Chair`，`user.email` = `hua.zheng@embeddedboys.com`
- **提交一律带 `Signed-off-by`**：用 `git commit -s`
- 提交信息用**内核风格**：`模块: 组件: 简述`（如 `drivers: display: ...`），
  正文写清改了什么、为什么、实测效果；一个逻辑改动一个提交
- **未经明确指令不要 `git commit`；永远不要在没有明确许可时 `git push`**
- **不要改写已推送的历史**：历史里存在 `Zheng Hua`、`Roy Moe` 两个旧名字，
  邮箱与上面相同，属于已发布历史，保持原样
- 默认分支 `main`

## 仓库结构

```
CMakeLists.txt              — 静态库 pico-display-lib（Pico SDK + FreeRTOS）
configs/*.cmake             — 每个屏幕型号一份预置配置
drivers/
  bus/                      — SPI / i80(PIO) / I8080 GPIO 总线
  display/                  — tft.c + tft_<model>.c 面板驱动
  input/                    — 触摸驱动 + indev.c（坐标变换的唯一出处）
  backlight/  clk/  uart/
include/                    — 对外头文件（tft.h / indev.h / config.h / debug.h ...）
lib/FreeRTOS-Kernel         — 子模块
```

- **配置式开发**：在根 `CMakeLists.txt` 里取消注释一行
  `include(${CMAKE_CURRENT_LIST_DIR}/configs/<型号>.cmake)` 就选定了屏幕，
  各驱动的 `config.cmake` 再由顶层 `include` 进来。
- 对外只暴露最基本的接口（初始化、设置绘制窗口、填充等），方便在上面移植 GUI 库。

## 代码约定

- **TFT 像素格式只有一个出处**：驱动 init 里的 `0x3A`（COLMOD）。`tft_video_sync()`
  与异步刷新路径都原样把缓冲区送出去，**不要再给驱动加"顺手转 RGB666"的
  `video_sync`** —— ILI9488/ILI9486 里那两份 init 写 `0x55` 却按 3 字节发送的已经删掉，
  ILI9481 保留是因为它 init 就是 `0x66`。
- **触摸坐标只有一处变换**：`ft6236`/`gt911`/`cst816d`/`tsc2007` 只返回**控制器原始值**，
  轴序/反向/偏移/钳位全在 `indev.c` 的 `indev_dir_for_rotation(TFT_ROTATION)` 里做
  （触摸和显示共用同一个 `TFT_ROTATION`）。不要把 `set_dir()` 常量塞回驱动：
  旋转会不跟着走，而且调用两次会把轴序转回去。
- **寄存器读写不每次分配堆**：`tft_priv` 里有固定的 `reg_buf` 与 `ops`
  （`tft_probe()` 里 `priv->buf = priv->reg_buf`、`priv->tftops = &priv->ops`），
  寄存器写不要再 malloc。`txbuf` 按 `need_tx_buf` 堆分配是另一回事（传输缓冲）。
- 改动对外 API（`include/tft.h`、`include/indev.h`）要同步所有 `tft_*.c` / 触摸驱动，
  以及引用本库的 `Pico-USB-Display`（本库以子模块形式被它引用）。
