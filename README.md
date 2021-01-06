# AS7341

## 1、介绍

这是一个在RT-Thread上，AS7341可见光谱传感器的驱动，是一款11通道可见光传感器，可测量8个波长的可见光适用于颜色检测、灯光色温检测等场景，并支持`Finsh/MSH`测试命令。
本驱动编程实现方便读取AS7341传感器数据。地址IO的控制和数据的滤波留给用户自行编程处理。

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| docs  | 文档目录 |
| docs/AS7341数据手册.pdf | 数据手册 |
| as7341.h  | 头文件 |
| as7341.c  | 源代码 |
| SConscript | RT-Thread 默认的构建脚本 |
| README.md | 软件包使用说明 |

### 1.2 许可证

AS7341 package 遵循 `Apache License 2.0` 许可，详见 `LICENSE` 文件。

### 1.3 依赖

依赖 `RT-Thread I2C` 设备驱动框架。

## 2、如何获取软件包

使用 AS7341 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    peripheral libraries and drivers  --->
        [*] AS7341: AS7341 visible light sensor, can measure 8 wavelengths of visible light
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 AS7341

本驱动实现基于RT-Thread的I2C设备驱动框架，在打开 AS7341 package 后，请务必打开至少1个I2C设备。

``` C
msh >as7341 probe i2c1
as7341 id: 0x24
as7341 probed, addr:0x39
msh >
msh >as7341 read spectral
F1(405-425nm): 1
F2(435-455nm): 3
F3(470-490nm): 4
F4(505-525nm): 5
F5(545-565nm): 7
F6(580-600nm): 6
F7(620-640nm): 7
F8(670-690nm): 4
Clear: 22
NIR: 2
msh >
```

* 更多文档位于 [`/docs`](/docs) 下，使用前 **务必查看**

## 4、注意事项



## 5、联系方式 & 感谢

* 维护：RiceChen
* 邮箱：980307037@qq.com
* 主页：https://github.com/RiceChen
