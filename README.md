# Raspberry Pi Pico Display Template

🚧🚧 。。。工程正处于早期开发阶段。。。 🚧🚧

你可以借助这个工程，快速地在 Raspberry Pi Pico 上测试 TFT 模组，并在此基础上
移植其他 GUI 库。 为了减小体积，这个库只实现了最基本的接口，例如初始化、设置绘制窗口、图像填充。

### To-do

- [x] Rotation support
- [ ] Debug support
- [ ] Fix I8080 GPIO bit-bang issue
- [x] Refactor TFT model selection routine (legacy config flags map to validated, controller-specific build variants)

## 特性

- 配置式开发。预设了多种 TFT 配置，开箱即用。
- 基于 Pico SDK 开发
- 分离设计，总线、屏幕、触摸隔离处理。
- 运行期可改朝向：`tft_set_rotation()` 只写一次 MADCTL 并交换逻辑几何（`xres/yres`），
  缓冲区按像素数分配、90 度旋转不改像素总数，所以不需要重算缓冲；触摸方向由调用方跟着调
  `indev_set_dir(indev_dir_for_rotation(rot))`。

## 快速上手

### 1. 选择屏幕型号

面板配置由 CMake 变量 `PUD_CONFIG` 选择，取值是 `configs/` 下某个文件名去掉 `.cmake`：

```bash
cmake -S . -B build -DPICO_BOARD=pico2 -DPUD_CONFIG=generic-st7789v
```

名字写错或者没给，configure 阶段就会指名报错——不会静默地用上别的屏。可用名字就是
`configs/` 目录下的那些。

> 本仓库以前是在根 `CMakeLists.txt` 里取消注释一行 `include(configs/...)` 来选屏。那会
> 弄脏工作树、也容易被误提交，已经删掉，改由 `PUD_CONFIG` 驱动。

### 2. 按需要修改配置文件中的设置

打开 `configs` 目录下的 `generic-st7789v.cmake` 文件。 这里以修改分辨率为例：

```cmake
set(TFT_HOR_RES   240)
set(TFT_VER_RES   280)
```

选择 TFT 的总线类型：
```cmake
set(TFT_BUS_TYPE 0)
#     | Bus type | Description
#  0  |   SPI    | Serial Peripheral Interface
#  1  |  I8080   | Intel 8080 interface
#  2  |   I2C    | Inter-Integrated Circuit (Not implemented yet)
```

也可以修改屏幕的引脚定义：
```cmake
set(TFT_PIN_SCL   18)
set(TFT_PIN_SDA   19)
set(TFT_PIN_RES   15)
set(TFT_PIN_DC    14)
set(TFT_PIN_CS    13)
set(TFT_PIN_BLK   12)
```

这里还有一些其他配置的说明：
```cmake
set(TFT_BUS_CLK_KHZ 75000)  # 总线时钟频率
set(TFT_BLK_ACTIVE_HIGH 1)  # 背光控制信号是否为高电平有效
```

## 开发

### 添加一个暂未支持的 TFT 型号
