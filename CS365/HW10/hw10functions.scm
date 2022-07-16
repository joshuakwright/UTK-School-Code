#!/Users/local/bin/guile -s
!#

;Joshua Wright jwrig117

; returns cond with a and b where the smaller of the two is first
(define (swap a b)
	(cond
	  [(> a b) (cons b a)]
	  [(< a b) (cons a b)]))

; Returns a float for the average of the list
(define (avg avgList)
  	(cond 
	  [(eq? (list-length avgList) 1) (car avgList)]
	  [else (/ (avg-helper avgList 0) (* (list-length avgList) 1.0))]))

; recursively adds elements in a list
(define (avg-helper avgList avgVal) 
  	(cond 
	  [(null? avgList) avgVal]
	  [else (avg-helper (cdr avgList) (+ (car avgList) avgVal))]))

; returns the maximum element in the list
(define (max maxList) 
  	(cond
	  [(eq? (list-length maxList) 1) (car maxList)]
	  [else (max-helper maxList 0)]))

; recursively determines the maximum element in the passed list
(define (max-helper maxList maxVal) 
  	(cond
	  [(null? maxList) maxVal]
	  [(< maxVal (car maxList)) (max-helper (cdr maxList) (car maxList))]
	  [else (max-helper (cdr maxList) maxVal)]))

; returns true if the list is in increasing order and false otherwise
(define (increasing . incList) 
  (cond
    [(eq? (list-length incList) 1) #t]
    [else (apply < incList)]))

; returns a to the power of b
(define (apowb a b)
  (cond
    [(eq? b 0) 1]
    [(eq? b 1) a]
    [else (apowb-helper a a b)]))

; recursively multiplies the powNum, multTimes, which calculates powNum to the power of multTIme
(define (apowb-helper origNum powNum multTime) 
  (cond 
    [(eq? multTime 1) powNum]
    [else (apowb-helper origNum (* origNum powNum) (- multTime 1))]))

; returns a list that adheres to the specified function
(define (filter func lambdaList) 
  (filter-helper func lambdaList '()))

; builds list of elements that are returned from the func function
(define (filter-helper func lambdaList outputList) 
  (cond
    [(null? lambdaList) outputList]
    [else (filter-helper func (cdr lambdaList)
	(cond 
	  [(func (car lambdaList)) (append outputList (list (car lambdaList)))]
	  [else outputList]))]))

; returns the length of a list
(define (list-length lst) 
  (cond 
    [(null? lst) 0]
    [else (+ 1 (list-length (cdr lst)))]))
