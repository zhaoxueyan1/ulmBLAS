#ifndef ULMBLAS_LEVEL2_SPLMV_H
#define ULMBLAS_LEVEL2_SPLMV_H 1

namespace ulmBLAS {

template <typename IndexType, typename Alpha, typename TA, typename TX,
          typename Beta, typename TY>
    void
    splmv(IndexType    n,
          const Alpha  &alpha,
          const TA     *A,
          const TX     *x,
          IndexType    incX,
          const Beta   &beta,
          TY           *y,
          IndexType    incY);

} // namespace ulmBLAS

#endif // ULMBLAS_LEVEL2_SPLMV_H

#include <ulmblas/level2/splmv.tcc>
