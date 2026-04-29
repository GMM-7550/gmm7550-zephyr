/*
 * Copyright (c)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/i2c.h>

#define PD_CONTROL_SRC_MASK 0x00000001
#define PD_CONTROL_DRN_MASK 0x00000002
#define PD_CONTROL_REG *((uint32_t *)0xf0003000)

#define I2C_INST DT_NODELABEL(i2c0)
#define FUSB303B_ADDRESS 0x21

#ifdef CONFIG_USB3_PD
static void usb_pd_source_set(bool on)
{
  uint32_t pd_control = PD_CONTROL_REG;

  if (on) {
    pd_control |= PD_CONTROL_SRC_MASK;
  } else {
    pd_control &= ~PD_CONTROL_SRC_MASK;
  }
  PD_CONTROL_REG = pd_control;
}

static int usb_pd_source_handler(const struct shell *sh,
                                 size_t argc, char **argv)
{
  if (!strcmp(argv[1], "on")) {
    usb_pd_source_set(true);
    shell_print(sh, "Power source: ON\n");
  } else if (!strcmp(argv[1], "off")) {
    usb_pd_source_set(false);
    shell_print(sh, "Power source: OFF\n");
  } else {
    shell_error(sh, "Unknown power state, should be 'on' or 'off'");
    return -EINVAL;
  }
  return 0;
}

const struct device *const i2c = DEVICE_DT_GET(I2C_INST);

static int fusb303b_read_reg(const struct device *dev, const uint8_t reg)
{
  uint8_t wr_buf = reg;
  uint8_t rd_buf;
  i2c_write_read(i2c, FUSB303B_ADDRESS,
                 (void *)&wr_buf, 1, (void *)&rd_buf, 1);
  return rd_buf;
}

#if 0
static int fusb303b_write_reg(const struct device *dev,
                              const uint8_t reg, const uint8_t data)
{
  return 0;
}
#endif

static int usb_pd_status_handler(const struct shell *sh,
                                 size_t argc, char **argv)
{
  uint8_t reg;

  if (!device_is_ready(i2c)) {
    printf("i2c is not ready!\n");
    return -EINVAL;
  }

  shell_print(sh, "USB PD Status:");

  reg = fusb303b_read_reg(i2c, 0x03);
  shell_print(sh, "    Port Role: %02x", reg);
  shell_print(sh, "                     TRY: %d", (reg>>4) & 3);
  shell_print(sh, "                     DRP: %d", (reg>>2) & 1);
  shell_print(sh, "                     SNK: %d", (reg>>1) & 1);
  shell_print(sh, "                     SRC: %d", (reg)    & 1);

  shell_print(sh, "      Control: %02x", fusb303b_read_reg(i2c, 0x04));
  shell_print(sh, "    Control 1: %02x", fusb303b_read_reg(i2c, 0x05));
  shell_print(sh, "       Manual: %02x", fusb303b_read_reg(i2c, 0x09));

  reg = fusb303b_read_reg(i2c, 0x11);
  shell_print(sh, "       Status: %02x", reg);
  shell_print(sh, "                Auto SNK: %d", (reg>>7) & 1);
  shell_print(sh, "                 Vsafe0V: %d", (reg>>6) & 1);
  shell_print(sh, "                  Orient: %d", (reg>>4) & 3);
  shell_print(sh, "                  VbusOK: %d", (reg>>3) & 1);
  shell_print(sh, "                  BC LVL: %d", (reg>>1) & 3);
  shell_print(sh, "                  Attach: %d", (reg)    & 1);

  reg = fusb303b_read_reg(i2c, 0x12);
  shell_print(sh, "     Status 1: %02x", reg);
  shell_print(sh, "                Fault: %d", (reg>>1) & 1);
  shell_print(sh, "               Remedy: %d", (reg) & 1);

  reg = fusb303b_read_reg(i2c, 0x13);
  shell_print(sh, "         Type: %02x", reg);
  shell_print(sh, "               Debug SRC: %d", (reg>>6) & 1);
  shell_print(sh, "               Debug SNK: %d", (reg>>5) & 1);
  shell_print(sh, "                    Sink: %d", (reg>>4) & 1);
  shell_print(sh, "                  Source: %d", (reg>>3) & 1);
  shell_print(sh, "            Active Cable: %d", (reg>>2) & 1);
  shell_print(sh, "              Audio Vbus: %d", (reg>>1) & 1);
  shell_print(sh, "                   Audio: %d", (reg) & 1);

  shell_print(sh, "    Interrupt: %02x", fusb303b_read_reg(i2c, 0x14));
  shell_print(sh, "  Interrupt 1: %02x", fusb303b_read_reg(i2c, 0x15));
  return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(usb3_pd_cmds,
                               SHELL_CMD_ARG(status,
                                             NULL, "USB PD Status",
                                             usb_pd_status_handler, 1, 0),
                               SHELL_CMD_ARG(source,
                                             NULL, "Power source on|off",
                                             usb_pd_source_handler, 2, 0),
                               SHELL_SUBCMD_SET_END);
#endif

#ifdef CONFIG_USB3_TEST_SHELL

SHELL_STATIC_SUBCMD_SET_CREATE(usb3_cmds,
                               SHELL_COND_CMD_ARG(CONFIG_USB3_PD,
                                                  pd, &usb3_pd_cmds, "Power Delivery commands",
                                                  NULL, 0, 0),
                               SHELL_COND_CMD_ARG(CONFIG_USB3_FS,
                                                  fs, NULL, "Full Speed commands",
                                                  NULL, 0, 0),
                               SHELL_COND_CMD_ARG(CONFIG_USB3_HS,
                                                  hs, NULL, "High Speed commands",
                                                  NULL, 0, 0),
                               SHELL_COND_CMD_ARG(CONFIG_USB3_SS,
                                                  ss, NULL, "Super Speed commands",
                                                  NULL, 0, 0),
                               SHELL_SUBCMD_SET_END
                               );

#endif

int main(void)
{
  SHELL_COND_CMD_REGISTER(CONFIG_USB3_TEST_SHELL,
                          usb3, &usb3_cmds, "USB 3 test and debug commands",
                          NULL);

  return 0;
}
