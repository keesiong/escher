import booleans.es ;
import lists.es ;

generate : a -> (a -> a) -> Int -> (List a) ;
(generate x y z) = (generate2 x y 0 z) ;

generate2 : a -> (a -> a) -> Int -> Int -> (List a) ;
(generate2 x y w_SV/CONST/ z) = if (= w_SV z) then [] 
	       	      	      	else (# x (generate2 (y x) y (add w_SV 1) z)) ;

succ : Int -> Int ;
(succ x_SV/CONST/) = (add x_SV 1) ;

firstInts : Int -> (List Int) ;
(firstInts x) = (generate 0 succ x) ;

-- : (firstInts 1) ; : (firstInts 20) ;

Blue, Green : Colour ;
type Ball = (Int * Colour) ;

succBall : Ball -> Ball ;
(succBall (x,Blue)) = (x,Green) ;
(succBall (x,Green)) = ((add x 1),Blue) ;

mysetOfBalls : Int -> (List Ball) ;
(mysetOfBalls n) = (generate (1,Blue) succBall n) ;

-- : (mysetOfBalls 1) ; : (mysetOfBalls 10) ;

succSetBalls : (List Ball) -> (List Ball) ;
(succSetBalls []) = [ (1,Blue) ] ;
(succSetBalls (# x y)) = (concat ((# x y), [ (succBall (last (# x y))) ])) ;

firstSetBalls : Int -> (List (List Ball)) ;
(firstSetBalls n) = (generate [] succSetBalls n) ;

-- : (firstSetBalls 1) ; : (firstSetBalls 10) ;

setToList : (a -> Bool) -> (List a) ;
(setToList \x.False) = [] ;
(setToList \x.(= x y)) = [ y ] ;
(setToList \x.(|| w_SV v_SV)) = 
	   (concat ((setToList \x.w_SV),(setToList \x.v_SV))) ; Eager ;

select : (List a) -> (List Int) -> (List a) ;
(select [] []) = [] ;
(select (# x1 x2) (# y1 y2)) = if (= y1 1) then (# x1 (select x2 y2))
	      	     	       else (select x2 y2) ;

bitvector : Int -> Int -> Int -> (List Int) ;
(bitvector m n x) = 
	   if (> x m) then []
	   else if (&& (<= x n) (<= n m)) then (# 1 (bitvector m n (add x 1)))
	   else (# 0 (bitvector m n (add x 1))) ;

-- this produces all combinations of size $n$ from $m$ balls, not what we need.
ballsDrawnSet : (List Ball) -> Int -> Int -> (List (List Ball)) ;
(ballsDrawnSet s m n) = 
	 (map (select s) 
	      (rmduplicates (setToList \x.(permute ((bitvector m n 1),x))))) ;

-- : (ballsDrawnSet (setOfBalls 4) 4 2) ;

-- *********************************************************
-- This routine returns all the possible ways of picking 
-- $n$ elements with replacement from a set of $m$ elements.
-- *********************************************************
selectWithReplacement : (List Int) -> Int -> (List (List Int)) ;
(selectWithReplacement [] x) = [] ;
(selectWithReplacement (# x y) n_SV/CONST/) = 
		          (selectWithReplacement2 (# x y) n_SV n_SV) ;
           
selectWithReplacement2 : (List Int) -> Int -> Int -> (List (List Int)) ;
(selectWithReplacement2 s n_SV/CONST/ m_SV/CONST/) = 
	if (= m_SV 1) then (expandByOne [] s)
	else (expandByOneRep s (selectWithReplacement2 s n_SV (sub m_SV 1))) ;

expandByOneRep : (List Int) -> (List (List Int)) -> (List (List Int)) ;
(expandByOneRep s []) = [] ;
(expandByOneRep s (# x y)) = (concat ((expandByOne x s), (expandByOneRep s y)));

expandByOne : (List Int) -> (List Int) -> (List (List Int)) ;
(expandByOne [] []) = [] ;
(expandByOne x (# x2 [])) = (# (# x2 x) []) ;
(expandByOne x (# x2 (# y2 z2))) = (# (# x2 x) (expandByOne x (# y2 z2))) ;

-- : (expandByOne [] [1,2,3,4]) ; : (expandByOne [1,2] [1,2,3,4]) ;
-- : (expandByOneRep [1,2,3] [[1],[2],[3]]) ;
-- : (selectWithReplacement [1,2,3] 1) ; : (selectWithReplacement [1,2,3] 4) ;

-- ballsDrawnRepl : (List Ball) -> Int -> Int -> (List (List Ball)) ;
-- (ballsDrawnRepl s m n) = 
-- 	(map (map \x.(elem x s)) (selectWithReplacement (ints 1 m) n)) ;

allProducts : (List a) -> Int -> Int -> (List (List a)) ;
(allProducts s m n) = 
	(map (map \x.(elem x s)) (selectWithReplacement (ints 1 m) n)) ;

-- : (allProducts [Green,Blue] 2 4) ;

-- **********************************
-- set of all balls of a certain size
-- **********************************

setOfAllBalls : Int -> (List (List Ball)) ;
(setOfAllBalls n) = (map (zip (ints 1 n)) (allProducts [Green,Blue] 2 n)) ;

-- : (setOfAllBalls 3) ;

-- *******************************************************
-- all possible draws with replacement from a set of balls
-- *******************************************************
setOfAllDraws : (List Ball) -> Int -> (List (List Ball)) ;
(setOfAllDraws s n) = (allProducts s (length s) n) ;

setOfAllDrawsAugmented : (List Ball) -> Int -> (List ((List Ball) * (List Ball))) ;
(setOfAllDrawsAugmented s n) = (setOfAllDrawsAugmented2 s n (setOfAllDraws s n)) ; Eager ;

setOfAllDrawsAugmented2 : (List Ball) -> Int -> (List (List Ball)) -> (List ((List Ball) * (List Ball))) ;
(setOfAllDrawsAugmented2 s n x) = (zipWith s x) ; Eager ;

-- : (setOfAllDrawsAugmented (head (setOfAllBalls 3)) 1) ;

-- ***********************************************
-- set of all ((List Ball) * (List Ball)) pairs
-- ***********************************************

swap : a -> b -> c ;
(swap x y) = (setOfAllDrawsAugmented y x) ;

setOfAllPairs : Int -> Int -> (List ((List Ball) * (List Ball))) ;
(setOfAllPairs m n) = (setOfAllPairs2 m n (setOfAllBalls m)) ;

setOfAllPairs2 : Int -> Int -> (List (List Ball)) -> (List ((List Ball) * (List Ball))) ;
(setOfAllPairs2 m n s) = (fold concat2 [] (map (swap n) s)) ; Eager ;

-- : (setOfAllPairs 3 4) ;
: (setOfAllBalls 6) ;
: (setOfAllPairs 6 3) ;

-- **********************
-- Urns and balls example
-- **********************
numOfBalls : Int -> Float ;
numOfBalls = \x.(poisson 6 x) ;

exp : Float ;
exp = 2.71828 ;

poisson : Int -> Int -> Float ;
(poisson x) = \y.(div (power (x,y)) (mul (power (exp,x)) (fac y))) ;

setOfBalls : Int -> (List Ball) -> Float ;
(setOfBalls n) = \x.if (= (getBallNumbers x) (ints 1 n)) then (colourProb x)
	       	    else 0.0 ;

getBallNumbers : (List Ball) -> (List Int) ;
(getBallNumbers []) = [] ;
(getBallNumbers (# (y,z) w)) = (# y (getBallNumbers w)) ;

colourProb : (List Ball) -> Float ;
(colourProb []) = 1.0 ;
(colourProb (# (y,z) w)) = if (= z Blue) then (mul 0.6 (colourProb w))
	       	     	   else (mul 0.4 (colourProb w)) ;

ballDrawn : (List Ball) -> Ball -> Float ;
(ballDrawn s (x,y)) = if (<= x (length s)) then (div 1.0 (length s)) else 0 ;

obsColour : Colour -> Colour -> Float ;
(obsColour x) = \y.if (= x y) then 0.8 else 0.2 ;
-- (obsColour x) = \y.if (= x y) then 1.0 else 0.0 ;

jointDistr : (Int * (List Ball) * (List Ball) * (List Colour)) -> Float ;
(jointDistr (n,s,x,y)) = 
     if (|| (/= (length s) n) (/= (length x) (length y))) then 0.0
     else (mul (numOfBalls n) (mul (setOfBalls n s) (jointDistr2 s x y))) ;

jointDistr2 : (List Ball) -> (List Ball) -> (List Colour) -> Float ;
(jointDistr2 s [] []) = 1.0 ;
(jointDistr2 s (# (x1,x2) y) (# w z)) =
	(mul (mul (ballDrawn s (x1,x2)) (obsColour x2 w))
	     (jointDistr2 s y z)) ;

-- : (jointDistr (3,[(1,Blue),(2,Blue),(3,Green)],[(1,Blue),(1,Blue),(3,Green)],
--		 [Blue,Blue,Green])) ;

swapJD : Int -> (List Colour) -> ((List Ball) * (List Ball)) -> Float ;
(swapJD x1 x2 (x3,x4)) = (jointDistr (x1,x3,x4,x2)) ;

posterior : (List Colour) -> Int -> Float ;
(posterior x) = \y.(fold add 0.0 
	      		 (map (swapJD y x) (setOfAllPairs y (length x)))) ;

{-
: (posterior [Blue,Blue,Blue,Blue] 4) ;

: (posterior [Blue,Green,Blue] 1) ;
: (posterior [Blue,Green,Blue] 2) ;
: (posterior [Blue,Green,Blue] 3) ;
: (posterior [Blue,Green,Blue] 4) ;
: (posterior [Blue,Green,Blue] 5) ;
: (posterior [Blue,Green,Blue] 6) ;
-}


   

