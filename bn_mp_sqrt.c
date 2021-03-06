#include "tommath_private.h"
#ifdef BN_MP_SQRT_C
/* LibTomMath, multiple-precision integer library -- Tom St Denis */
/* SPDX-License-Identifier: Unlicense */

/* this function is less generic than mp_n_root, simpler and faster */
mp_err mp_sqrt(const mp_int *arg, mp_int *ret)
{
   mp_err res;
   mp_int t1, t2;

   /* must be positive */
   if (arg->sign == MP_NEG) {
      return MP_VAL;
   }

   /* easy out */
   if (MP_IS_ZERO(arg)) {
      mp_zero(ret);
      return MP_OKAY;
   }

   if ((res = mp_init_copy(&t1, arg)) != MP_OKAY) {
      return res;
   }

   if ((res = mp_init(&t2)) != MP_OKAY) {
      goto E2;
   }

   /* First approx. (not very bad for large arg) */
   mp_rshd(&t1, t1.used/2);

   /* t1 > 0  */
   if ((res = mp_div(arg, &t1, &t2, NULL)) != MP_OKAY) {
      goto E1;
   }
   if ((res = mp_add(&t1, &t2, &t1)) != MP_OKAY) {
      goto E1;
   }
   if ((res = mp_div_2(&t1, &t1)) != MP_OKAY) {
      goto E1;
   }
   /* And now t1 > sqrt(arg) */
   do {
      if ((res = mp_div(arg, &t1, &t2, NULL)) != MP_OKAY) {
         goto E1;
      }
      if ((res = mp_add(&t1, &t2, &t1)) != MP_OKAY) {
         goto E1;
      }
      if ((res = mp_div_2(&t1, &t1)) != MP_OKAY) {
         goto E1;
      }
      /* t1 >= sqrt(arg) >= t2 at this point */
   } while (mp_cmp_mag(&t1, &t2) == MP_GT);

   mp_exch(&t1, ret);

E1:
   mp_clear(&t2);
E2:
   mp_clear(&t1);
   return res;
}

#endif
