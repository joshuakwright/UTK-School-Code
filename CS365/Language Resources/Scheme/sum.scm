(define (sum x)
    (if (null? x)
       0
       (+ (car x) (sum (cdr x)))))
