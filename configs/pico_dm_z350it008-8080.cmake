# PWD 的面板配置：**Z350IT008 模组 + GT911 触摸，屏幕驱动 ILI9488**（用户看屏上丝印确认 ✓），8080 16-bit
#
# 这份**以库里那份能工作的 `configs/pico_dm_qd3503728.cmake` 为底**（同一颗 ILI9488 ✓），
# 只改"本模块接线不同"的地方，逐条写明为什么 ✓ —— 不自己发明参数 ✗。
#
#   | 项 | qd3503728（库里能工作那份） | 本仓 | 为什么 |
#   | --- | --- | --- | --- |
#   | `TFT_PIN_CS` | `18` | **`16`** | 本接线表里 **18 是触摸 CTP_RST** ✗，且 CS 在模块上**硬件下拉** ✓；库的 i80 每次写都要动 CS ⇒ 只能给空闲脚占位（本表里只有 GP16/17 空着 ✓） |
#   | `TFT_PIN_BLK` | `28` | `28` ✓ | 实测确认：改到别的脚上背光就灭 ✓ |
#   | 触摸 | FT6236（SCL 27 / SDA 26 / RST 18） | **GT911**（同引脚 + IRQ 21 ✓） | 用户给的接线 ✓；触摸**还没启用**（见文末 ✗） |
#   | 调试串口 | TX 16 / RX 17 | **关掉** ✗ | 16 被 CS 占位拿走 ✓；固件只走 USB stdio ✓ |
#
# **留档（别再犯 ✗）**：我先照 PUD 那份**别的模块**（YT350S006 ✗，用户后来确认记错了）的 SPI 配置抄了 480×320 / rotation 0 +
# ST7796 初始化变体 ✗ ⇒ 屏幕全白 ✗。用户确认这块模组的驱动就是 **ILI9488** ✓，换成 ILI9488 后
# 立刻点亮 ✓。教训：**同名模块的配置不一定适用于手上这块板子** ✗，模块名与控制器要分开问 ✓。

set(OVERCLOCK_ENABLED 0)   # 先不超频：面板已经点亮了 ✓，一次只动一个变量（见下面的总线时钟说明 ✓）

# 库自身的调试串口关掉 ✓（`DEBUG_UART_INTERFACE 0` ⇒ config.h 里那些宏变成空操作 ✓）：
# 它默认要的 16/17 在本接线表里被 CS 占位拿走了 ✗。
set(DEBUG_UART_INTERFACE 0)

set(TFT_BUS_TYPE 1)        # 1 = I8080

# /* I8080 interface pins —— 按本模块接线表（用户给的 ✓） */
set(TFT_PIN_DB_BASE  0)    # D0..D15 ⇒ GP0..GP15
set(TFT_PIN_DB_COUNT 16)
set(TFT_PIN_WR  19)
set(TFT_PIN_RS  20)
set(TFT_PIN_RES 22)
# CS 硬件下拉 ⇒ 模块常驻选中 ✓；库里没有"CS 不接"的开关 ✗（`i80_set_rs_cs()` 每次写都要动 CS），
# 所以给一个**合法且空闲**的脚 16 占位 ✓。**绝不能给 18** ✗ —— 那是触摸 CTP_RST，
# 拿它当 CS 会在每次写像素时乱动触摸复位 ✓（库里同系列那份配置正是这么写的 ✗，要引以为戒 ✓）。
set(TFT_PIN_CS  16)
set(TFT_PIN_BLK 28)        # 背光（实测：改到别的脚上背光就灭 ✓）

set(TFT_BLK_ACTIVE_HIGH 1)

set(TFT_HOR_RES   320)     # 面板**原生竖屏** …
set(TFT_VER_RES   480)
set(TFT_ROTATION  1)       # … 靠 rotation 1 转成逻辑 480×320 ✓（照 qd3503728 ✓）

# 16 位并口**不换字节**（SPI/8 位那版才是 1 ✗）
set(TFT_COLOR_16_SWAP 0)

# 总线时钟：库里 i80 的分频是**编译期**算的（`pico_i80.c`: PERI_CLK_KHZ / 2 / 请求值 ✓），
# 所以它必须和 CPU 频率一起定 ✓：qd3503728 那套是「50 MHz 总线 + 240 MHz CPU（profile 1）」✓。
# 本仓现在不超频（PERI = 125 MHz）⇒ 请求 25 MHz 得到分频 2.5、实际 25 MHz ✓；
# 若请求 50 MHz 会落到分频 1.25、PIO 占用率约 80% ✗（余量太小）。
# **要提到 50 MHz 就得同时开 `OVERCLOCK_PROFILE 1`（240 MHz）** ✓，那时再一起改 ✓。
set(TFT_BUS_CLK_KHZ 25000)
set(DISP_OVER_PIO 1)
# A/B 对照（临时 ✗）：Pico W 上 WiFi 起来后，i80 的 DMA 写会卡在 `dma_channel_is_busy` ✗。
# 先把 i80 改成 **CPU 写**（不申请 DMA ✓）来判定"是不是 DMA 这条路径与 cyw43 冲突" ✓。
# 若这样就通 ⇒ 病因收窄到 DMA ✓，随后再决定是"库改成动态申请"还是"面板总线改走别的 PIO/SM" ✓。
set(PIO_USE_DMA 0)

set(TFT_DRV_USE_ILI9488 1)   # 用户确认：这块模组的屏幕驱动就是 ILI9488 ✓
set(TFT_MODEL_ZT350IT008 1)  # 面板型号（丝印 ✓）⇒ 用它的**专属初始化序列** ✓
                             # 默认那套是 QD3503728 的，伽马/帧率/VCOM 都不同 ⇒ 用错色彩有问题 ✗

# 触摸 GT911：引脚按用户接线 ✓（SCL 27 / SDA 26 / RST 18 / IRQ 21）。**先不启用** ✗ ——
# 面板先通、再上触摸，一次只动一个变量 ✓。
set(INDEV_BUS_TYPE 0)
set(INDEV_DRV_NOT_USED    1)
# set(INDEV_DRV_USE_GT911   1)
# set(INDEV_POLLING_PERIOD_MS 33)
# set(INDEV_PIN_SCL   27)
# set(INDEV_PIN_SDA   26)
# set(INDEV_PIN_RST   18)
# set(INDEV_PIN_IRQ   21)
