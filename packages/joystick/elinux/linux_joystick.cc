// Copyright 2021 Sony Group Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <errno.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <stdio.h>
#include <unistd.h>

extern "C" __attribute__((visibility("default"))) __attribute__((used)) int
joystick_open(const char* device) {
  int fd = open(device, O_NONBLOCK);
  if (fd < 0) {
    fprintf(stderr, "Failed to open %s (%d)\n", device, errno);
  }
  return fd;
}

extern "C" __attribute__((visibility("default"))) __attribute__((used)) int
joystick_read(int fd, js_event* ev) {
  int bytes = read(fd, ev, sizeof(*ev));
  if (bytes < 0) {
    return -1;
  }
  return bytes == sizeof(*ev);
}
