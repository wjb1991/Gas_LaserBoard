;;#############################################################################
;;! \file source/vector/mean_SP_CV_2.asm
;;!
;;! \brief C-Callable mean of real and imag parts of a complex vector.
;;! \author David M. Alter
;;! \date   07/06/11
;;
;; HISTORY:
;;   07/20/11 - original (D. Alter)
;;
;; DESCRIPTION: C-Callable mean of real and imag parts of a complex vector.
;;              y_re = sum(x_re)/N
;;              y_im = sum(x_im)/N
;; FUNCTION:    
;;   extern complex_float mean_SP_CV_2(const complex_float *x, const Uint16 N)
;;
;; USAGE:       y = mean_SP_CV_2(x, N);
;;
;; PARAMETERS:  complex_float *x = input array
;;              Uint16 N = length of x array
;;
;; RETURNS:     complex_float y = result
;;
;; BENCHMARK:   2*N + 34 cycles (including the call and return)
;;
;; NOTES:
;;   1) N must be even, and a minimum of 4.
;;   2) The type 'complex_float' is defined as
;;
;;      typedef struct {
;;         float32 dat[2];
;;      } complex_float;
;;
;;      Element dat[0] is the real part, dat[1] is the imag part.
;;   3) The algorithm is mean_re = sum(re)/N
;;                     mean_im = sum(im)/N
;;      1/N is first computed and stored off.  The summations of the real
;;      and imaginary parts are then computed, and multiplied by 1/N.  The
;;      first summation of each is a just a load of the first element.
;;      These loads are interleaved at the tail-end of the 1/N computation
;;      to fill delay slots, and is why the loop epilog only shows one
;;      additional summation, and not two).  Register choices for the
;;      summation loop have been carefully chosen (R0H - R3H) to allow this.
;;
;;  Group:            C2000
;;  Target Family:    C28x+FPU32
;;
;; Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/ 
;; ALL RIGHTS RESERVED
;;#############################################################################
;;$TI Release: C28x Floating Point Unit Library V1.50.00.00 $
;;$Release Date: Jun 2, 2015 $
;;#############################################################################

        .global _mean_SP_CV_2
        .text
_mean_SP_CV_2:
        MOV         *SP++, AL       ;store N on stack for later use
        LSR         AL, #1          ;divide N by 2

;Note: I believe the UI16TOF32 will incur a 1 cycle stall below due to SP 
;write/read usage. But, there are no filler instructions we can move up here 
;anyway, so we'll take the stall.

;Compute 1/N
        UI16TOF32   R1H, *--SP      ;retrieve N
        ADDB        AL, #-2         ;subtract 2 from N (RPTB is 'n-1' times, 
                                    ;and one unrolled loop iteration done 
                                    ;separately)
        EINVF32     R2H, R1H        ;R2H = Ye = Estimate(1/B)
        NOP                         ;delay slot
        MPYF32      R3H, R2H, R1H   ;R3H = Ye*B
        NOP                         ;delay slot
        SUBF32      R3H, #2.0, R3H  ;R3H = 2.0 - Ye*B       
        NOP                         ;delay slot
        MPYF32      R2H, R2H, R3H   ;R2H = Ye = Ye*(2.0 - Ye*B)
        NOP                         ;delay slot
        MPYF32      R3H, R2H, R1H   ;R3H = Ye*B
        MOV32       R0H, *XAR4++    ;load first real part (fill delay slot)
        SUBF32      R3H, #2.0, R3H  ;R3H = 2.0 - Ye*B
        MOV32       R1H, *XAR4++    ;load first imag part (fill delay slot)
        MPYF32      R2H, R2H, R3H   ;R2H = Ye = Ye*(2.0 - Ye*B) = 1/B
        MOV32       R3H, *XAR4++    ;load next real part (fill delay slot)
 
        MOV32       *SP++, R2H      ;store 1/B on stack for later use

;---Main loop - unrolled once
        RPTB        end_loop, @AL

        ADDF32      R0H, R0H, R3H   ;add next real part
        || MOV32    R2H, *XAR4++    ;load next imag part

        ADDF32      R1H, R1H, R2H   ;add next imag part
        || MOV32    R3H, *XAR4++    ;load next real part

        ADDF32      R0H, R0H, R3H   ;add next real part
        || MOV32    R2H, *XAR4++    ;load next imag part

        ADDF32      R1H, R1H, R2H   ;add next imag part
        || MOV32    R3H, *XAR4++    ;load next real part

end_loop:

;Last iteration done seperately to avoid XAR4 pointer overrun into possibly 
;non-defined memory
        ADDF32      R0H, R0H, R3H   ;add last real part
        || MOV32    R2H, *XAR4      ;load last imag part
        
        ADDF32      R1H, R1H, R2H   ;add last imag part

;Divide the summations by N (multiply by 1/N) and store back to memory
        MOV32       R2H, *--SP      ;retrieve 1/N from stack

        MPYF32      R0H, R0H, R2H   ;real part
        MPYF32      R1H, R1H, R2H   ;imag part

        MOV32       *XAR6++, R0H    ;store real part
        MOV32       *XAR6, R1H      ;store imag part

;Finish up
        LRETR                       ;return

;end of function mean_SP_CV_2()
;*********************************************************************

       .end
;;#############################################################################
;;  End of File
;;#############################################################################
