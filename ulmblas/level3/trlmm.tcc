#ifndef ULMBLAS_LEVEL3_TRLMM_TCC
#define ULMBLAS_LEVEL3_TRLMM_TCC 1

#include <ulmblas/auxiliary/memorypool.h>
#include <ulmblas/config/blocksize.h>
#include <ulmblas/level1extensions/gescal.h>
#include <ulmblas/level3/pack/gepack.h>
#include <ulmblas/level3/pack/trlpack.h>
#include <ulmblas/level3/mgemm.h>
#include <ulmblas/level3/mtrlmm.h>
#include <ulmblas/level3/trlmm.h>

namespace ulmBLAS {

template <typename IndexType, typename Alpha, typename TA, typename TB>
void
trlmm(IndexType    m,
      IndexType    n,
      const Alpha  &alpha,
      bool         unitDiag,
      const TA     *A,
      IndexType    incRowA,
      IndexType    incColA,
      TB           *B,
      IndexType    incRowB,
      IndexType    incColB)
{
    typedef decltype(Alpha(0)*TA(0)*TB(0))  T;

    const IndexType MC = BlockSize<T>::MC;
    const IndexType NC = BlockSize<T>::NC;

    const IndexType mb = (m+MC-1) / MC;
    const IndexType nb = (n+NC-1) / NC;

    const IndexType _mc = m % MC;
    const IndexType _nc = n % NC;

    static MemoryPool<T> memoryPool;

    if (alpha==Alpha(0)) {
        gescal(m, n, Alpha(0), B, incRowB, incColB);
        return;
    }

    T  *_A = memoryPool.allocate(MC*MC);
    T  *_B = memoryPool.allocate(MC*NC);

    for (IndexType j=0; j<nb; ++j) {
        IndexType nc = (j!=nb-1 || _nc==0) ? NC : _nc;

        for (IndexType l=mb-1; l>=0; --l) {
            IndexType kc = (l!=mb-1 || _mc==0) ? MC   : _mc;

            gepack_B(kc, nc,
                     &B[l*MC*incRowB+j*NC*incColB], incRowB, incColB,
                     _B);

            trlpack(kc, unitDiag,
                    &A[l*MC*(incRowA+incColA)], incRowA, incColA,
                    _A);

            mtrlmm(kc, nc, alpha, _A, _B,
                   &B[l*MC*incRowB+j*NC*incColB], incRowB, incColB);

            for (IndexType i=l+1; i<mb; ++i) {
                IndexType mc = (i!=mb-1 || _mc==0) ? MC : _mc;

                gepack_A(mc, kc,
                         &A[i*MC*incRowA+l*MC*incColA], incRowA, incColA,
                         _A);

                mgemm(mc, nc, kc, alpha, _A, _B, T(1),
                      &B[i*MC*incRowB+j*NC*incColB], incRowB, incColB);
            }
        }
    }

    memoryPool.release(_A);
    memoryPool.release(_B);
}

} // namespace ulmBLAS

#endif // ULMBLAS_LEVEL3_TRLMM_TCC