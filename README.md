# zWatch
## 硬件设计

### 原理图

![image-20210615175712941](https://gitee.com/LucasXm/img/raw/master/img/image-20210615175712941.png)

### PCB

![image-20210615175756511](https://gitee.com/LucasXm/img/raw/master/img/image-20210615175756511.png)

![image-20210615175815945](https://gitee.com/LucasXm/img/raw/master/img/image-20210615175815945.png)

### 位图

![image-20210615175916706](https://gitee.com/LucasXm/img/raw/master/img/image-20210615175916706.png)

### BOM

| Comment        | Description    | Designator                                         | Footprint                         | LibRef                  | Quantity |
| -------------- | -------------- | -------------------------------------------------- | --------------------------------- | ----------------------- | -------- |
| 10uf           | 无极性贴片电容 | C2, C4, C5                                         | 0402-c                            | C                       | 3        |
| 22pf           | 无极性贴片电容 | C3                                                 | 0402-c                            | C                       | 1        |
| 0.1uf          | 无极性贴片电容 | C6, C9, C10, C11, C12                              | 0402-c                            | C                       | 5        |
| 12pf           | 无极性贴片电容 | C7, C8                                             | 0402-c                            | C                       | 2        |
| 1N4148         |                | D3, D4                                             | SOD-123_L3.1-W1.7-LS3.7-RD        | 1N4148W_C727110         | 2        |
| HDR-1X3        | 3P接插件       | JP1, JP3                                           | HDR-1X3                           | Header 3                | 2        |
| HDR-1X2        | 2P接插件       | JP2                                                | 1.27-1X2                          | Header 2                | 1        |
| 2.2uh          | 小功率贴片电感 | L1                                                 | 0603-L                            | L                       | 1        |
| 绿色led        |                | LED1                                               | LED0402-RD                        | 16-213/SDRC/S530-A3/TR8 | 1        |
| SS8050         |                | Q1                                                 | SOT-23-3_L2.9-W1.3-P1.90-LS2.4-BR | SS8050                  | 1        |
| 0.5Ω           | 贴片电阻       | R1                                                 | 0402-r                            | 0402 0Ω (0R0) 1%        | 1        |
| 2kΩ            | 贴片电阻       | R2, R5, R6, R7, R11,  R12, R14, R15, R17, R18, R19 | 0402-r                            | 0402 0Ω (0R0) 1%        | 11       |
| 22kΩ           | 贴片电阻       | R3, R8, R9, R10, R16                               | 0402-r                            | 0402 0Ω (0R0) 1%        | 5        |
| 100kΩ          | 贴片电阻       | R4                                                 | 0402-r                            | 0402 0Ω (0R0) 1%        | 1        |
| 10Ω            | 贴片电阻       | R13                                                | 0402-r                            | 0402 0Ω (0R0) 1%        | 1        |
| 3x6x5          | 3x6轻触开关    | SW1, SW2, SW3, SW4                                 | SW-SMD_L4.7-W3.5-P3.35-EH         | TSW 3x6                 | 4        |
| SY8089A1AAC    |                | U1                                                 | SOT-23-5_L3.0-W1.7-P0.95-LS2.8-BL | SY8089A1AAC             | 1        |
| LTH7R.         |                | U2                                                 | SOT-23-5_L3.0-W1.7-P0.95-LS2.8-BL | LTH7R.                  | 1        |
| BME280         |                | U3                                                 | LGA-8_BME280_BR                   | BME280                  | 1        |
| 显示屏         |                | U4                                                 | tft-1.69寸                        | ZJY128R-IG01            | 1        |
| MPU-9250       |                | U5                                                 | QFN-24_L3.0-W3.0-P0.40-BL-EP      | MPU-9250                | 1        |
| CH340E         |                | U6                                                 | MSOP-10_L3.0-W3.0-P0.50-LS5.0-BL  | CH340E                  | 1        |
| MCU            |                | U7                                                 | QFN-48_L6.0-W6.0-P0.40-BL-EP4.2   | AT32F403ACGU7           | 1        |
| BM8563EMA      |                | U8                                                 | MSOP-8_L3.0-W3.0-P0.65-LS5.0-BL   | BM8563EMA               | 1        |
| TYPE-C-31-M-12 |                | USBC1                                              | USB-C_SMD-TYPE-C-31-M-12          | TYPE-C-31-M-12          | 1        |
| 32.768K        |                | X1                                                 | OSC-SMD_L3.2-W1.5                 | TFX-02S-32.768KJ21058   | 1        |

### 硬件成本

一共采购了两套设备（阻容器件不只两套），花了243元。