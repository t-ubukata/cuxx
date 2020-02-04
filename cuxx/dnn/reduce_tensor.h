#ifndef CUXX_DNN_REDUCE_TENSOR_H_
#define CUXX_DNN_REDUCE_TENSOR_H_

#include "cudnn.h"
#include "cuxx/util.h"

#include "cuxx/dnn/common.h"

namespace cuxx {
namespace dnn {

// FactorT must be float or double.
template<typename TensorT, typename FactorT>
class ReduceTensor {
 public:
  ReduceTensor(cudnnReduceTensorOp_t op, cudnnDataType_t dtype,
               cudnnNanPropagation_t nan_opt,
               cudnnReduceTensorIndices_t reduces_indices,
               cudnnIndicesType_t indices_type) {
    CUXX_DNN_CHECK(cudnnCreateReduceTensorDescriptor(&desc_));
    CUXX_DNN_CHECK(cudnnSetReduceTensorDescriptor(desc_, op, dtype, nan_opt,
                                                  reduces_indices,
                                                  indices_type));
  }

  ~ReduceTensor() {
    CUXX_DNN_CHECK(cudnnDestroyReduceTensorDescriptor(desc_));
  }

  ReduceTensor(const ReduceTensor&) = delete;
  ReduceTensor operator=(const ReduceTensor&) = delete;

  cudnnReduceTensorDescriptor_t desc() const {return desc_;}

  void Compute(const Handle& handle,
               void* indices, size_t indices_size_in_bytes,
               void* workspace, size_t workspace_size_in_bytes,
               FactorT alpha, const Tensor<TensorT>& a,
               FactorT beta, Tensor<TensorT>* c) const {
    CUXX_DNN_CHECK(cudnnReduceTensor(handle.raw_handle(), desc_,
                                     indices, indices_size_in_bytes,
                                     workspace, workspace_size_in_bytes,
                                     &alpha, a.desc(), a.dev_mem(),
                                     &beta, c->desc(), c->dev_mem()));
  }

 private:
  cudnnReduceTensorDescriptor_t desc_;
};

}  // namespace dnn
}  // namespace cuxx

#endif  // CUXX_DNN_REDUCE_TENSOR_H_
