import booleans.es ;
import lists.es ;

TLC, WOT, TRC, WOL, SAA, WTR, BLC, WOB, BRC : Observation ;
Up, Right, Down, Left : Action ;

type Individual = (List (Observation * Action)) ;

projObs : (Observation * Action) -> Observation ;
(projObs (t1,t2)) = t1 ;

projAction : (Observation * Action) -> Action ;
(projAction (t1,t2)) = t2 ;

isAction : a -> Bool ;
isAction = \x.(|| (|| (|| (= x Up) (= x Right)) (= x Down)) (= x Left)) ;

lastTransition : Individual -> (Observation * Action) ;
(lastTransition x) = (last x) ;

last2Transitions : Individual -> Individual ;
(last2Transitions (# x (# y []))) = (# x (# y [])) ;
(last2Transitions (# w (# x (# y z)))) = (last2Transitions (# x (# y z))) ;

last3Transitions : Individual -> Individual ;
(last3Transitions (# x (# y (# z [])))) = (# x (# y (# z []))) ;
(last3Transitions (# v (# w (# x (# y z))))) = (last3Transitions (# w (# x (# y z)))) ;

transition : Individual -> Observation ;
(transition x) = if (&& (= (projObs (lastTransition x)) SAA)
	       	    	(= (projAction (lastTransition x)) Left)) then WOL
		 else if (&& (= (projObs (lastTransition x)) SAA)
	       	    	(= (projAction (lastTransition x)) Down)) then WOB	
	         else if (= (projObs (lastTransition x)) WOT) then WOT
		 else SAA ;

realign : Observation -> (List (Action * Observation)) -> Action -> Individual ;
(realign x [] w) = [(x,w)] ;
(realign x (# (y1,y2) []) w) = (# (x,y1) (# (y2,w) [])) ;
(realign x (# (y1,y2) z) w) = (# (x,y1) (realign y2 z w)) ;

extend : Observation -> (List (Action * Observation)) -> 
       	 	     	(List (Action * Observation)) -> Bool ;
(extend s0 x1 x2) = 
	\exists x.
	     (&& (isAction x)
	      	 (= x2 (concat (x1, [(x, (transition (realign s0 x1 x)))])))) ;

: (extend SAA [(Left,SAA),(Right,SAA)] x) ;




