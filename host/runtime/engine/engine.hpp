/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#pragma once

#include "axcl_rt.h"
#include "axcl_rt_type_internal.h"

#include "package/host/runtime/engine/engine_request.hpp"
#include "package/host/runtime/engine/engine_response.hpp"

#define TAG "engine"

namespace axcl::rt {

class engine {
public:
    engine() = default;

    [[nodiscard]] axclError init(axclrtEngineVNpuKind kind) const;
    [[nodiscard]] axclError get_vnpu_kind(axclrtEngineVNpuKind *kind) const;
    [[nodiscard]] axclError finalize() const;

    [[nodiscard]] axclError load(const void *model, uint64_t size, uint64_t *id) const;
    [[nodiscard]] axclError unload(uint64_t id) const;

    [[nodiscard]] const char *get_model_version(uint64_t id) const;
    [[nodiscard]] axclError get_usage(uint64_t id, int64_t *sysSize, int64_t *cmmSize) const;
    [[nodiscard]] axclError get_model_type(uint64_t id, axclrtEngineModelKind *type) const;

    [[nodiscard]] axclError get_model_info(uint64_t id, axclrtEngineIOInfo *info) const;
    [[nodiscard]] static axclError destroy_model_info(axclrtEngineIOInfo info);
    [[nodiscard]] static axclError get_shape_group_count(axclrtEngineIOInfo info, int32_t *count);
    [[nodiscard]] static int32_t get_input_count(axclrtEngineIOInfo info);
    [[nodiscard]] static int32_t get_output_count(axclrtEngineIOInfo info);
    [[nodiscard]] static axclError get_input_dims(axclrtEngineIOInfo info, uint32_t group, uint32_t index, axclrtEngineIODims *dims);
    [[nodiscard]] static axclError get_output_dims(axclrtEngineIOInfo info, uint32_t group, uint32_t index, axclrtEngineIODims *dims);
    [[nodiscard]] static uint64_t get_input_size(axclrtEngineIOInfo info, uint32_t group, uint32_t index);
    [[nodiscard]] static uint64_t get_output_size(axclrtEngineIOInfo info, uint32_t group, uint32_t index);
    [[nodiscard]] static const char *get_input_name(axclrtEngineIOInfo info, uint32_t index);
    [[nodiscard]] static const char *get_output_name(axclrtEngineIOInfo info, uint32_t index);
    [[nodiscard]] static int32_t get_input_data_type(axclrtEngineIOInfo info, uint32_t index, axclrtEngineDataType *type);
    [[nodiscard]] static int32_t get_output_data_type(axclrtEngineIOInfo info, uint32_t index, axclrtEngineDataType *type);
    [[nodiscard]] static int32_t get_input_data_layout(axclrtEngineIOInfo info, uint32_t index, axclrtEngineDataLayout *layout);
    [[nodiscard]] static int32_t get_output_data_layout(axclrtEngineIOInfo info, uint32_t index, axclrtEngineDataLayout *layout);
    [[nodiscard]] static int32_t get_input_index(axclrtEngineIOInfo info, const char *name);
    [[nodiscard]] static int32_t get_output_index(axclrtEngineIOInfo info, const char *name);

    [[nodiscard]] static axclError create_io(axclrtEngineIOInfo info, axclrtEngineIO *io);
    [[nodiscard]] static axclError destroy_io(axclrtEngineIO io);
    [[nodiscard]] static axclError set_input(axclrtEngineIO io, uint32_t index, const void *buffer, uint64_t buffer_size);
    [[nodiscard]] static axclError set_input(axclrtEngineIO io, const char *name, const void *buffer, uint64_t buffer_size);
    [[nodiscard]] static axclError set_output(axclrtEngineIO io, uint32_t index, const void *buffer, uint64_t buffer_size);
    [[nodiscard]] static axclError set_output(axclrtEngineIO io, const char *name, const void *buffer, uint64_t buffer_size);
    [[nodiscard]] static axclError get_input(axclrtEngineIO io, uint32_t index, void **buffer, uint64_t *buffer_size);
    [[nodiscard]] static axclError get_input(axclrtEngineIO io, const char *name, void **buffer, uint64_t *buffer_size);
    [[nodiscard]] static axclError get_output(axclrtEngineIO io, uint32_t index, void **buffer, uint64_t *buffer_size);
    [[nodiscard]] static axclError get_output(axclrtEngineIO io, const char *name, void **buffer, uint64_t *buffer_size);
    [[nodiscard]] static axclError set_batch(axclrtEngineIO io, uint32_t batch_size);
    [[nodiscard]] static axclError get_batch(axclrtEngineIO io, uint32_t *batch_size);

    [[nodiscard]] axclError create_context(uint64_t id, uint64_t *context) const;

    [[nodiscard]] axclError set_affinity(uint64_t id, axclrtEngineSet set) const;
    [[nodiscard]] axclError get_affinity(uint64_t id, axclrtEngineSet *set) const;

    [[nodiscard]] axclError start_p2p(std::vector<float>& elapsed, uint32_t mode, uint64_t size) const;

    [[nodiscard]] axclError execute(uint64_t id, uint64_t context, uint32_t group, axclrtEngineIO io, axclrtStream axclStream, bool has_resp=true) const;

protected:
    [[nodiscard]] static axclError get_runtime_environment(axclrtStream *stream, axclrtEnvironment &env);
    [[nodiscard]] axclError invoke(axclrtStream stream, const axclrtEnvironment &env, int32_t timeout, bool has_resp=true) const;

private:
    pkg::host::engine_request m_req;
    pkg::host::engine_response m_rsp;
};

}  // namespace axcl::rt
