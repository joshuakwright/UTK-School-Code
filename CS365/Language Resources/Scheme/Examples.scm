;;Create a simple helloWorld procedure
(define (helloWorld) (display "Hello World\n"))

;;Find the sum of natural numbers from 1 to the passed value (inclusive)
(define (sumOf max) (* max (/ (+ max 1) 2)))

;;Create a list of numbers up to the passed value (exclusive) and then find the sum
(define (sumAddUpList max) (apply + (iota max)))

;;Returns the sum of natural numbers from 1 to the pass value and a list of those values
(define (sumWithLoop max) 
    (do (
        (i 1 (1+ i)) 
        (p '() (append p (list i))) 
        (sum 0 (+ sum i))) 
            ((> i max) 
                (list sum p))))

;;Find the sum of natural numbers from 1 to the passed value (inclusive) using recursion
(define (sumOfRecursive max) 
    (if (eq? max 0) 
        0 
        (+ max (sumOfRecursive (- max 1)))))

;;Add the first and last numbers in the sequence, repeating this process until there are no pairs left
(define (sumAddByPairs list) 
    (if (eq? (length list) 0) 
        0 
        (if (eq? (length list) 1) 
            (car list) 
            (+ (car list) (car (last-pair list)) (sumAddByPairs (list-tail (list-head list (- (length list) 1)) 1))))))

;;Create a list of length n containing random values between 1 and m
(define (genList n m) 
    (if (eq? 0 n) 
        '() 
        (append (genList (- n 1) m) (list(random m)))))
        
;;Returns true if a given list is in order, otherwise returns false
(define (isSorted list)
    (if (eq? (length list) 1)
        #t
        (if (< (car list) (car (cdr list)))
            (isSorted (list-tail list 1))
            #f)))
;;The main bogoSort procedure that checks if the list is in order yet
(define (bogoSort list)
    (if (isSorted list)
        list
        (bogoSort (bogoHelper list (random (length list))))))
        
;;The helper function for the bogoSort which randomizes the values of the original list
(define (bogoHelper oldList k)
    (if (eq? (length oldList) 1)
        (list (car oldList))
        (append (list(list-ref oldList k)) (bogoHelper (append (list-head oldList k) (list-tail oldList (+ k 1))) (random (- (length oldList) 1))))
        ))
