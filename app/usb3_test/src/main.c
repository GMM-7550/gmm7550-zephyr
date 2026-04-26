/*
 * Copyright (c)
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#define PD_CONTROL_SRC_MASK 0x00000001
#define PD_CONTROL_DRN_MASK 0x00000002
#define PD_CONTROL_REG *((uint32_t *)0xf0003000)

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

static int usb_pd_status_handler(const struct shell *sh,
                                 size_t argc, char **argv)
{
  shell_print(sh, "USB PD Status:\n");
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
