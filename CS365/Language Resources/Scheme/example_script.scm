#!/usr/local/bin/guile -s
!#

(define (sum x)
   (if (null? x)
           0
           (+ (car x) (sum (cdr x)))))

(define lst '(0 2 3 4))
(display lst)
(newline)
(display (car lst))
(newline)
(display (cdr lst))
(newline)

(display(sum lst))
(newline)
