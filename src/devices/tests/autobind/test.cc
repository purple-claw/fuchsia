// Copyright 2021 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fidl/fuchsia.io/cpp/wire.h>
#include <lib/device-watcher/cpp/device-watcher.h>
#include <lib/fdio/directory.h>

#include <gtest/gtest.h>

TEST(AutobindTest, DriversExist) {
  fbl::unique_fd out;
  ASSERT_EQ(ZX_OK, device_watcher::RecursiveWaitForFile("/dev/sys/test/autobind", &out));

  // We want to make sure autobind doesn't bind to itself, so try to connect
  // and assert that it was closed.
  zx::channel client, server;
  ASSERT_EQ(ZX_OK, zx::channel::create(0, &client, &server));
  ASSERT_EQ(ZX_OK, fdio_open("/dev/sys/test/autobind/autobind",
                             static_cast<uint32_t>(fuchsia_io::wire::OpenFlags::kRightReadable),
                             server.release()));
  ASSERT_EQ(ZX_OK, client.wait_one(ZX_CHANNEL_PEER_CLOSED, zx::time::infinite(), nullptr));
}
