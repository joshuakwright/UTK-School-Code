; concatenate function - using inductive construction
(define (concat l1 l2)
   (if (null? l1)
           l2
           (cons (car l1) (concat (cdr l1) l2))))

; find minimum number in the list - using inductive construction
(define (min lst)
   (if (null? (cdr lst))
           (car lst)
           (if (< (car lst) (min (cdr lst)))
                   (car lst)
                   (min (cdr lst)))))

; plus function - using tail recursion without continuation
(define (plus x . xs)
    (if (null? xs)
	    x
		(apply plus (cons (+ x (car xs)) (cdr xs)))))

; factorial - using inductive construction (start with base case, have solution for t, extend solution to t+1)
; Get solution for n-l and then multiply by n to get actual factorial
(define (fact1 n)
	(if (< n 2)
		1
		(* n (fact1 (- n 1)))))

; factorial - using tail recursion with continuation (recursive call last thing being executed in a function and additional computation does not follow the recursive call)
; Do product first and then pass down with each call to factorial function
; Hiding helper functions with letrec - making it local to fact function
(define (fact n)
   (letrec 
	 ((factHelper (lambda (n productThusFar)
		(if (< n 2)
			productThusFar
			(factHelper (- n 1) (* n productThusFar))))))
	 (factHelper n 1)))

; sum - using inductive construction
; Base case 0, have solution for n element list, get solution for n+1 element list (add n+1 element to sum of first n elements
(define (sum1 x)
    (if (null? x)
       0
       (+ (car x) (sum1 (cdr x)))))

; sum - using tail recursion
(define (sum x) (sum-helper x 0))

(define (sum-helper x sumSoFar)
    (if (null? x)
       sumSoFar
	   (sum-helper (cdr x) (+ (car x) sumSoFar))))

