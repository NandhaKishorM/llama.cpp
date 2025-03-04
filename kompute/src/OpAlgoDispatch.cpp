// SPDX-License-Identifier: Apache-2.0

/**
 * Copyright (c) 2023 Nomic, Inc. All rights reserved.
 *
 * This software is licensed under the terms of the Software for Open Models License (SOM),
 * version 1.0, as detailed in the LICENSE_SOM.txt file. A copy of this license should accompany
 * this software. Except as expressly granted in the SOM license, all rights are reserved by Nomic, Inc.
 */

#include "kompute/operations/OpAlgoDispatch.hpp"

namespace kp {

OpAlgoDispatch::~OpAlgoDispatch()
{
    KP_LOG_DEBUG("Kompute OpAlgoDispatch destructor started");

    if (this->mPushConstantsData) {
        KP_LOG_DEBUG("Kompute freeing push constants data");
        free(this->mPushConstantsData);
    }
}

void
OpAlgoDispatch::record(const vk::CommandBuffer& commandBuffer)
{
    KP_LOG_DEBUG("Kompute OpAlgoDispatch record called");

    // Barrier to ensure the data is finished writing to buffer memory
    for (const std::shared_ptr<Tensor>& tensor :
         this->mAlgorithm->getTensors()) {
        tensor->recordPrimaryBufferMemoryBarrier(
          commandBuffer,
          vk::AccessFlagBits::eShaderWrite,
          vk::AccessFlagBits::eShaderRead,
          vk::PipelineStageFlagBits::eComputeShader,
          vk::PipelineStageFlagBits::eComputeShader);
    }

    if (this->mPushConstantsSize) {
        this->mAlgorithm->setPushConstants(
          this->mPushConstantsData,
          this->mPushConstantsSize,
          this->mPushConstantsDataTypeMemorySize);
    }

    this->mAlgorithm->recordBindCore(commandBuffer);
    this->mAlgorithm->recordBindPush(commandBuffer);
    this->mAlgorithm->recordDispatch(commandBuffer);
}

void
OpAlgoDispatch::preEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpAlgoDispatch preEval called");
}

void
OpAlgoDispatch::postEval(const vk::CommandBuffer& /*commandBuffer*/)
{
    KP_LOG_DEBUG("Kompute OpAlgoDispatch postSubmit called");
}

}
