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

## Funding

This project received funding through [NGI0 Commons
Fund](https://nlnet.nl/commonsfund), a fund established by
[NLnet](https://nlnet.nl) with financial support from the European
Commission's [Next Generation Internet](https://ngi.eu) program. Learn
more at the [NLnet project page](https://nlnet.nl/project/GateMate-USB3-PHY).

<img src="https://nlnet.nl/logo/banner.png" alt="NLnet foundation
logo" width="20%" />](https://nlnet.nl) [<img
src="https://nlnet.nl/image/logos/NGI0_tag.svg" alt="NGI Zero Logo"
width="20%" />](https://nlnet.nl/commonsfund)
