// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "element_wise_ops.h"
#include "core/providers/cpu/math/element_wise_ops.h"

namespace onnxruntime {
namespace contrib {

ONNX_CPU_OPERATOR_KERNEL(
    Affine,
    1,
    KernelDefBuilder().TypeConstraint("T", DataTypeImpl::GetTensorType<float>()),
    Affine<float>);

template <>
Status Affine<float>::Compute(OpKernelContext* ctx) const {
  auto& X = *ctx->Input<Tensor>(0);
  auto& Y = *ctx->Output(0, X.Shape());
  MakeEigenArrayMap<float>(Y) = alpha_ * MakeEigenArrayMap<float>(X) + beta_;
  return Status::OK();
}

ONNX_CPU_OPERATOR_KERNEL(
    Scale,
    1,
    KernelDefBuilder().TypeConstraint("T", DataTypeImpl::GetTensorType<float>()),
    Scale<float>);
template <typename T>
auto EigenMap(Tensor& t) -> EigenVectorMap<T> {
  return EigenVectorMap<T>(t.template MutableData<T>(), t.Shape().Size());
}
template <typename T>
auto EigenMap(const Tensor& t) -> ConstEigenVectorMap<T> {
  return ConstEigenVectorMap<T>(t.template Data<T>(), t.Shape().Size());
}

template <>
Status Scale<float>::Compute(OpKernelContext* ctx) const {
  auto& X = *ctx->Input<Tensor>(0);
  auto& Y = *ctx->Output(0, X.Shape());
  EigenMap<float>(Y) = scale_ * EigenMap<float>(X);
  return Status::OK();
}

}  // namespace contrib
}  // namespace onnxruntime
