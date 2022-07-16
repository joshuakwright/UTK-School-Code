(define sum (lambda (x)
    (if (null? x)
       0
       (+ (car x) (sum (cdr x))))))
