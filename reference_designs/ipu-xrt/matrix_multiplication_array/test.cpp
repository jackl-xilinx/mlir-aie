//===- test.cpp -------------------------------------------000---*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (C) 2023, Advanced Micro Devices, Inc.
//
//===----------------------------------------------------------------------===//

#include <bits/stdc++.h>
#include <boost/program_options.hpp>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdfloat>

#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_kernel.h"

constexpr int M = 512;
constexpr int K = 512;
constexpr int N = 512;

constexpr int A_VOLUME = M * K;
constexpr int B_VOLUME = N * K;
constexpr int C_VOLUME = M * N;

using A_DATATYPE = std::bfloat16_t;
using B_DATATYPE = std::bfloat16_t;
using C_DATATYPE = std::bfloat16_t;

constexpr int A_SIZE = (A_VOLUME * sizeof(A_DATATYPE));
constexpr int B_SIZE = (B_VOLUME * sizeof(B_DATATYPE));
constexpr int C_SIZE = (C_VOLUME * sizeof(C_DATATYPE));

constexpr bool VERIFY = true;

namespace po = boost::program_options;

void check_arg_file_exists(po::variables_map &vm_in, std::string name) {
  if (!vm_in.count(name)) {
    throw std::runtime_error("Error: no " + name + " file was provided\n");
  } else {
    std::ifstream test(vm_in[name].as<std::string>());
    if (!test) {
      throw std::runtime_error("The " + name + " file " +
                               vm_in[name].as<std::string>() +
                               " does not exist.\n");
    }
  }
}

std::vector<uint32_t> load_instr_sequence(std::string instr_path) {
  std::ifstream instr_file(instr_path);
  std::string line;
  std::vector<uint32_t> instr_v;
  while (std::getline(instr_file, line)) {
    std::istringstream iss(line);
    uint32_t a;
    if (!(iss >> std::hex >> a)) {
      throw std::runtime_error("Unable to parse instruction file\n");
    }
    instr_v.push_back(a);
  }
  return instr_v;
}

static inline std::int16_t random_int16_t() {
  // return ((std::int16_t)rand() % 0x10000);
  return std::int16_t(rand() % 0x10);
}

static inline std::bfloat16_t random_bfloat16_t() {
  std::default_random_engine gen;
  std::uniform_real_distribution<float> distribution(0.0, 1.0);
  return std::bfloat16_t(distribution(gen));
  return std::bfloat16_t(1.0);
  // return std::bfloat16_t((float)(rand()) / (float)(RAND_MAX));
}

template <typename Tin, typename Tout>
// void matmul(std::vector<Tin> a, std::vector<Tin> b, std::vector<Tout> &c) {
//   for (int row = 0; row < M; row++) {
//     for (int col = 0; col < N; col++) {
//       float running_sum = 0;
//       for (int i = 0; i < K; i++) {
//         running_sum += float(a[row * K + i]) * float(b[i * N + col]);
//       }
//       c[row * N + col] = Tout(running_sum);
//     }
//   }
// }
void matmul(std::vector<Tin> a, std::vector<Tin> b, std::vector<Tout> &c) {
  const int B = 64;
  for (int row = 0; row < M; row++) {
    for (int col = 0; col < N; col++) {
      for (int k = 0; k < K / B; k++) {
        float running_sum = 0;
        for (int i = 0; i < B; i++)
          running_sum +=
              float(a[row * K + k * B + i]) * float(b[i * N + k * B + col]);
        c[row * N + col] += Tout(running_sum);
      }
    }
  }
}

int main(int argc, const char *argv[]) {

  // Program arguments parsing
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")(
      "xclbin,x", po::value<std::string>()->required(),
      "the input xclbin path")(
      "kernel,k", po::value<std::string>()->required(),
      "the kernel name in the XCLBIN (for instance PP_PRE_FD)")(
      "verbosity,v", po::value<int>()->default_value(0),
      "the verbosity of the output")(
      "instr,i", po::value<std::string>()->required(),
      "path of file containing userspace instructions to be sent to the LX6");
  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
    }
  } catch (const std::exception &ex) {
    std::cerr << ex.what() << "\n\n";
    std::cerr << "Usage:\n" << desc << "\n";
    return 1;
  }

  check_arg_file_exists(vm, "xclbin");
  check_arg_file_exists(vm, "instr");

  srand(time(NULL));

  std::vector<uint32_t> instr_v =
      load_instr_sequence(vm["instr"].as<std::string>());

  int verbosity = vm["verbosity"].as<int>();
  if (verbosity >= 1)
    std::cout << "Sequence instr count: " << instr_v.size() << "\n";

  // Start the XRT test code
  // Get a device handle
  unsigned int device_index = 0;
  auto device = xrt::device(device_index);

  // Load the xclbin
  if (verbosity >= 1)
    std::cout << "Loading xclbin: " << vm["xclbin"].as<std::string>() << "\n";
  auto xclbin = xrt::xclbin(vm["xclbin"].as<std::string>());

  if (verbosity >= 1)
    std::cout << "Kernel opcode: " << vm["kernel"].as<std::string>() << "\n";
  std::string Node = vm["kernel"].as<std::string>();

  // Get the kernel from the xclbin
  auto xkernels = xclbin.get_kernels();
  auto xkernel = *std::find_if(xkernels.begin(), xkernels.end(),
                               [Node](xrt::xclbin::kernel &k) {
                                 auto name = k.get_name();
                                 std::cout << "Name: " << name << std::endl;
                                 return name.rfind(Node, 0) == 0;
                               });
  auto kernelName = xkernel.get_name();

  if (verbosity >= 1)
    std::cout << "Registering xclbin: " << vm["xclbin"].as<std::string>()
              << "\n";

  device.register_xclbin(xclbin);

  // get a hardware context
  if (verbosity >= 1)
    std::cout << "Getting hardware context.\n";
  xrt::hw_context context(device, xclbin.get_uuid());

  // get a kernel handle
  if (verbosity >= 1)
    std::cout << "Getting handle to kernel:" << kernelName << "\n";
  auto kernel = xrt::kernel(context, kernelName);

  auto bo_instr = xrt::bo(device, instr_v.size() * sizeof(int),
                          XCL_BO_FLAGS_CACHEABLE, kernel.group_id(0));
  auto bo_a =
      xrt::bo(device, A_SIZE, XRT_BO_FLAGS_HOST_ONLY, kernel.group_id(2));
  auto bo_b =
      xrt::bo(device, B_SIZE, XRT_BO_FLAGS_HOST_ONLY, kernel.group_id(3));
  auto bo_c =
      xrt::bo(device, C_SIZE, XRT_BO_FLAGS_HOST_ONLY, kernel.group_id(4));

  if (verbosity >= 1)
    std::cout << "Writing data into buffer objects.\n";
  srand(static_cast<unsigned>(time(0)));
  A_DATATYPE *bufA = bo_a.map<A_DATATYPE *>();
  std::vector<A_DATATYPE> AVec;
  for (int i = 0; i < A_VOLUME; i++)
    AVec.push_back(random_bfloat16_t());
  memcpy(bufA, AVec.data(), (AVec.size() * sizeof(A_DATATYPE)));
  B_DATATYPE *bufB = bo_b.map<B_DATATYPE *>();
  std::vector<B_DATATYPE> BVec;
  for (int i = 0; i < B_VOLUME; i++)
    BVec.push_back(random_bfloat16_t());
  memcpy(bufB, BVec.data(), (BVec.size() * sizeof(B_DATATYPE)));
  C_DATATYPE *bufC = bo_c.map<C_DATATYPE *>();
  std::vector<C_DATATYPE> CVec;
  for (int i = 0; i < C_VOLUME; i++)
    CVec.push_back(0);
  memcpy(bufC, CVec.data(), (CVec.size() * sizeof(C_DATATYPE)));

  void *bufInstr = bo_instr.map<void *>();
  memcpy(bufInstr, instr_v.data(), instr_v.size() * sizeof(int));

  bo_instr.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  bo_a.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  bo_b.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  bo_c.sync(XCL_BO_SYNC_BO_TO_DEVICE);

  if (verbosity >= 1)
    std::cout << "Running Kernel.\n";

  int num_iter = 10;
  float npu_time_total = 0;
  float npu_time_min = 9999999;
  float npu_time_max = 0;

  int errors = 0;
  float macs = 2.0 * float(M) * float(K) * float(N);

  for (unsigned iter=0; iter<num_iter; iter++) {

    auto start = std::chrono::high_resolution_clock::now();
    auto run = kernel(bo_instr, instr_v.size(), bo_a, bo_b, bo_c);
    run.wait();
    auto stop = std::chrono::high_resolution_clock::now();

    bo_c.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    C_DATATYPE *bufOut = bo_c.map<C_DATATYPE *>();

    int max_errors = 100;

    if (VERIFY) {
      std::cout << "Verifying against reference matmul ..." << std::endl;
      auto vstart = std::chrono::system_clock::now();
      std::vector<C_DATATYPE> output_ref0;
      for (uint32_t i = 0; i < C_VOLUME; i++)
        output_ref0.push_back(0);
      // output_ref0.push_back(K);
      matmul(AVec, BVec, output_ref0);

      const float absTol = std::abs(0.1);
      // const float absTol = std::abs(5);
      for (int row = 0; row < M; row++) {
        for (int col = 0; col < N; col++) {
          if (std::abs((float)bufOut[row * N + col] -
                      (float)output_ref0[row * N + col]) > absTol) {
            errors++;
            if (errors < max_errors) {
              std::cout << "\nerror, row: " << row << " col: " << col
                        << " expected "
                        << std::to_string((float)output_ref0[row * N + col])
                        << ", got "
                        << std::to_string((float)bufOut[row * N + col]) << "\n";
            }
          }
        }
      }
      auto vstop = std::chrono::system_clock::now();
      float vtime =
          std::chrono::duration_cast<std::chrono::seconds>(vstop - vstart)
              .count();
      std::cout << "Verify time: " << vtime << "secs." << std::endl;
    } else {
      if (verbosity >= 1)
        std::cout << "WARNING: matmul results not verified." << std::endl;
    }

    float npu_time =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
            .count();

    npu_time_total += npu_time;
    npu_time_min = (npu_time < npu_time_min) ? npu_time : npu_time_min;
    npu_time_max = (npu_time > npu_time_max) ? npu_time : npu_time_max;
  }

  std::cout << std::endl
            << "Avg NPU matmul time: " << npu_time_total/num_iter << "us." << std::endl;
  std::cout << "Avg NPU gflops: " << macs / (1000 * npu_time_total/num_iter) << std::endl;

  std::cout << std::endl
            << "Min NPU matmul time: " << npu_time_min << "us." << std::endl;
  std::cout << "Min NPU gflops: " << macs / (1000 * npu_time_min) << std::endl;

  std::cout << std::endl
            << "Max NPU matmul time: " << npu_time_max << "us." << std::endl;
  std::cout << "Max NPU gflops: " << macs / (1000 * npu_time_max) << std::endl;


  if (VERIFY && !errors) {
    std::cout << "\nPASS!\n\n";
    return 0;
  } else {
    std::cout << "\nerror count: " << errors << "\n\n";
    std::cout << "\nfailed.\n\n";
    return 1;
  }
}
