(define (list-length x)
    (if (null? x)
       0
       (+ 1 (list-length (cdr x)))))
