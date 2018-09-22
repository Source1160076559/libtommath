#include "tommath_private.h"
#ifdef BN_MP_TC_AND_C
/* LibTomMath, multiple-precision integer library -- Tom St Denis
 *
 * LibTomMath is a library that provides multiple-precision
 * integer arithmetic as well as number theoretic functionality.
 *
 * The library was designed directly after the MPI library by
 * Michael Fromberger but has been written from scratch with
 * additional optimizations in place.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */

/* two complement and */
int mp_tc_and(const mp_int *a, const mp_int *b, mp_int *c)
{
   int res = MP_OKAY, bits;
   int as = mp_isneg(a), bs = mp_isneg(b), s = 0;
   mp_int *mx = 0, _mx, acpy, bcpy;

   if (as || bs) {
      bits = MAX(mp_count_bits(a), mp_count_bits(b));
      res = mp_init_set_int(&_mx, 1);
      if (res != MP_OKAY) {
         goto end;
      }

      mx = &_mx;
      res = mp_mul_2d(mx, bits + 1, mx);
      if (res != MP_OKAY) {
         goto end;
      }

      if (as) {
         res = mp_init(&acpy);
         if (res != MP_OKAY) {
            goto end;
         }

         res = mp_add(mx, a, &acpy);
         if (res != MP_OKAY) {
            mp_clear(&acpy);
            goto end;
         }
         a = &acpy;
      }
      if (bs) {
         res = mp_init(&bcpy);
         if (res != MP_OKAY) {
            goto end;
         }

         res = mp_add(mx, b, &bcpy);
         if (res != MP_OKAY) {
            mp_clear(&bcpy);
            goto end;
         }
         b = &bcpy;
      }
   }

   res = mp_and(a, b, c);
   s = as & bs;

   if (s && res == MP_OKAY) {
      res = mp_sub(c, mx, c);
   }

end:
   if (a == &acpy) {
      mp_clear(&acpy);
   }

   if (b == &bcpy) {
      mp_clear(&bcpy);
   }

   if (mx == &_mx) {
      mp_clear(mx);
   }

   return res;
}
#endif

/* ref:         $Format:%D$ */
/* git commit:  $Format:%H$ */
/* commit time: $Format:%ai$ */