# Raspberry Pi Pico Display Template

🚧🚧 。。。工程正处于早期开发阶段。。。 🚧🚧

你可以借助这个工程，快速地在 Raspberry Pi Pico 上测试 TFT 模组，并在此基础上
移植其他 GUI 库。 为了减小体积，这个库只实现了最基本的接口，例如初始化、设置绘制窗口、图像填充。

### To-do

- [ ] Rotation support
- [ ] Debug support
- [ ] Fix I8080 GPIO bit-bang issue
- [ ] Refactor TFT model selection routine

## 特性

- 配置式开发。预设了多种 TFT 配置，开箱即用。
- 基于 Pico SDK 开发
- 分离设计，总线、屏幕、触摸隔离处理。

## 快速上手

### 1. 启用对应的屏幕型号配置文件

假设你有一个通用的 ST7789V 屏幕模块。 打开工程根目录的 `CMakeLists.txt` 文件，找到
型号选择部分，参考如下位置：
```cmake
# select the suitable config file
include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st7789v.cmake)
...
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_qd3503728.cmake)
...
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_ep4309n.cmake)
```

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
