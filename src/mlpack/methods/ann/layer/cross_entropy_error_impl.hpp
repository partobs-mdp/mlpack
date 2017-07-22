/**
 * @file cross_entropy_error_impl.hpp
 * @author Konstantin Sidorov
 *
 * Implementation of the cross-entropy performance function.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_CROSS_ENTROPY_ERROR_IMPL_HPP
#define MLPACK_METHODS_ANN_LAYER_CROSS_ENTROPY_ERROR_IMPL_HPP

// In case it hasn't yet been included.
#include "cross_entropy_error.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

template<typename InputDataType, typename OutputDataType>
CrossEntropyError<InputDataType, OutputDataType>::CrossEntropyError()
{
  // Nothing to do here.
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
double CrossEntropyError<InputDataType, OutputDataType>::Forward(
    const arma::Mat<eT>&& input, const arma::Mat<eT>&& target)
{
  return -arma::accu(target % arma::trunc_log(input) +
                     (1. - target) % arma::trunc_log(1. - input));
}

template<typename InputDataType, typename OutputDataType>
template<typename eT>
void CrossEntropyError<InputDataType, OutputDataType>::Backward(
    const arma::Mat<eT>&& input,
    const arma::Mat<eT>&& target,
    arma::Mat<eT>&& output)
{
  output = (1. - target) / (1. - input + 1e-2) - target / (input + 1e-2);
}

template<typename InputDataType, typename OutputDataType>
template<typename Archive>
void CrossEntropyError<InputDataType, OutputDataType>::Serialize(
    Archive& /* ar */,
    const unsigned int /* version */)
{
  // Nothing to do here.
}

} // namespace ann
} // namespace mlpack

#endif