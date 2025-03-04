// SPDX-License-Identifier: Apache-2.0

/**
 * Copyright (c) 2023 Nomic, Inc. All rights reserved.
 *
 * This software is licensed under the terms of the Software for Open Models License (SOM),
 * version 1.0, as detailed in the LICENSE_SOM.txt file. A copy of this license should accompany
 * this software. Except as expressly granted in the SOM license, all rights are reserved by Nomic, Inc.
 */

#include "kompute/operations/OpTensorSyncDevice.hpp"

namespace kp {

OpTensorSyncDevice::OpTensorSyncDevice(
  const std::vector<std::shared_ptr<Tensor>>& tensors)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncDevice constructor with params");

    if (tensors.size() < 1) {
        throw std::runtime_error(
          "Kompute OpTensorSyncDevice called with less than 1 tensor");
    }

    this->mTensors = tensors;
}

OpTensorSyncDevice::~OpTensorSyncDevice()
{
    KP_LOG_DEBUG("Kompute OpTensorSyncDevice destructor started");

    this->mTensors.clear();
}

void
OpTensorSyncDevice::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncDevice record called");

    for (size_t i = 0; i < this->mTensors.size(); i++) {
        if (this->mTensors[i]->tensorType() == Tensor::TensorTypes::eDevice) {
            this->mTensors[i]->recordCopyFromStagingToDevice(commandBuffer);
        }
    }
}

void
OpTensorSyncDevice::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncDevice preEval called");
}

void
OpTensorSyncDevice::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpTensorSyncDevice postEval called");
}

}
