/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#ifndef CORE_COMPAT_XPU_LEGACY_H
#define CORE_COMPAT_XPU_LEGACY_H

#include <xpu/host.h>

namespace xpu
{

  inline constexpr auto host_to_device = xpu::h2d;
  inline constexpr auto device_to_host = xpu::d2h;

  template<typename T>
  class hd_buffer {

  public:
    hd_buffer() = default;
    hd_buffer(size_t size) : m_buffer(size, xpu::buf_io) {}

    T* h() { return xpu::h_view(m_buffer).begin(); }
    T* d() { return m_buffer.get(); }

    xpu::buffer<T>& underlying() { return m_buffer; }

  private:
    xpu::buffer<T> m_buffer;
  };

  template<typename T>
  class d_buffer {

  public:
    d_buffer() = default;
    d_buffer(size_t size) : m_buffer(size, xpu::buf_device) {}

    T* d() { return m_buffer.get(); }

    xpu::buffer<T>& underlying() { return m_buffer; }

  private:
    xpu::buffer<T> m_buffer;
  };


  template<typename T>
  void copy(hd_buffer<T>& buf, direction dir)
  {
    static xpu::queue _Q;
    _Q.copy(buf.underlying(), dir);
    _Q.wait();
  }

  enum class side
  {
    host,
    device
  };

  template<typename T, side S>
  struct cmem_io {
    using type = T*;
  };

  template<typename T>
  struct cmem_io<T, side::host> {
    using type = hd_buffer<T>;
  };

  template<typename T, side S>
  using cmem_io_t = typename cmem_io<T, S>::type;

  template<typename T, side S>
  struct cmem_device {
    using type = T*;
  };

  template<typename T>
  struct cmem_device<T, side::host> {
    using type = d_buffer<T>;
  };

  template<typename T, side S>
  using cmem_device_t = typename cmem_device<T, S>::type;

}  // namespace xpu

#define XPU_BLOCK_SIZE_1D(...)

#define XPU_EXPORT_KERNEL(Image, Kernel, ...) XPU_EXPORT_KERNEL_II(Image, Kernel, xpu::no_smem, 64, ##__VA_ARGS__)

#define XPU_EXPORT_KERNEL_II(Image, Kernel, SMEM, BlockSize, ...)                                                      \
  struct Kernel : xpu::kernel<Image> {                                                                                 \
    using block_size = xpu::block_size<BlockSize>;                                                                     \
    using context    = xpu::kernel_context<SMEM>;                                                                      \
    XPU_D void operator()(context& ctx, ##__VA_ARGS__);                                                                \
  }

#define XPU_KERNEL(Kernel, smemIgnored, ...)                                                                           \
  XPU_EXPORT(Kernel);                                                                                                  \
  XPU_D void Kernel::operator()(context& ctx, ##__VA_ARGS__)

#endif
