      DOUBLE PRECISION FUNCTION DNRM2( N, X, INCX )

      INTEGER             INCX, N
      DOUBLE PRECISION    X(*)

      DOUBLE PRECISION    TEMP

      CALL DNRM2_SUB( N, X, INCX, TEMP )
      DNRM2 = TEMP

      RETURN
      END
