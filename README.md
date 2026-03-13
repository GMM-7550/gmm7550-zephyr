# Zephyr OS on GMM-7550 module and LiteX-VexRiscv

Out-of-tree board files to run Zephyr on GMM-7550

## Build instructions

Build dependencies:

- Zephyr OS sources (https://github.com/zephyrproject-rtos/zephyr)
  and RISC-V SDK
- `west` Zephyr's meta-tool (https://docs.zephyrproject.org/latest/develop/west/index.html)
- `uv` Python package and project manager (https://github.com/astral-sh/uv)

```
    git clone https://github.com/GMM-7550/gmm7550-zephyr.git
    cd gmm7550-zephyr
    make
```
