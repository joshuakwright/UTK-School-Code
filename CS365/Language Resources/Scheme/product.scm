(define (product x)
    (if (null? x)
       1
       (* (car x) (product (cdr x)))))
