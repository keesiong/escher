import numbers.es ;

union : (a -> Bool) -> (a -> Bool) -> (a -> Bool) ;
(union s t) = \x.(|| (s x) (t x)) ;

intersect : (a -> Bool) -> (a -> Bool) -> (a -> Bool) ;
(intersect s t) = \x.(&& (s x) (t x)) ;

minus : (a -> Bool) -> (a -> Bool) -> (a -> Bool) ;
(minus s t) = \x.(&& (s x) (= (t x) False)) ;

subset : (a -> Bool) -> (a -> Bool) -> Bool ;
(subset s t) = (pi \x.(implies (s x) (t x))) ;

powerset : (a -> Bool) -> ((a -> Bool) -> Bool) ;
(powerset \x.False)  = \s.(= s \x.False) ;
(powerset \x.if u_SV then True else v_SV) =
           \s.(sigma \t.(sigma \r.(&& ((powerset \x.u_SV) t)
                    (&& ((powerset \x.v_SV) r) ((= s) (union t r)))))) ;
(powerset \x.if u_SV then False else v_SV) = (powerset \x.(&& (not u_SV) v_SV)) ;
(powerset \x.(= x t)) = \s.(|| (= s \y.False) (= s \x.(= x t))) ;
(powerset \x.(|| u_SV v_SV)) =
           \s.(sigma \t.(sigma \r.(&& ((powerset \x.u_SV) t)
                    (&& ((powerset \x.v_SV) r) (= s (union t r)))))) ;

linearise : (a -> Bool) -> (a -> Bool) ;
(linearise \x.False) = \x.False ;
(linearise \x.if u_SV then True else v_SV) =  
          (union (linearise \x.u_SV) (linearise \x.v_SV)) ;
(linearise \x.if u_SV then False else v_SV) =
          (linearise \x.(&& (not u_SV) v_SV)) ;
(linearise \x.(= x t)) = \x.if (= x t) then True else False ;
(linearise \x.(|| u_SV v_SV)) = (union (linearise \x.u_SV) (linearise \x.v_SV)) ;

rmdup : (a -> Bool) -> (a -> Bool) ;
(rmdup \x.t_SV) = \x.(rmdup2 t_SV) ;

rmdup2 : Bool -> Bool ;
(rmdup2 False) = False ;
(rmdup2 True) = True ;
(rmdup2 (= x t_SV)) = (= x t_SV) ;
(rmdup2 if (= x t_SV) then True else False) = 
                 if (= x t_SV) then True else False ;
(rmdup2 if (= x t_SV) then True else u) =
        if (= x t_SV) then True else (rmdup2 (&& (/= x t_SV) u)) ; Eager ;
(rmdup2 (|| (= x t_SV) u)) = (|| (= x t_SV) (rmdup2 (&& (/= x t_SV) u))) ;
(rmdup2 (|| (|| (= x t_SV) u) v)) = (rmdup2 (|| (= x t_SV) (|| u v))) ;

rmdupCustom : (a -> a -> Bool) -> (a -> Bool) -> (a -> Bool) ;
(rmdupCustom p \x.t_SV) = \x.(rmdupCustom2 p t_SV) ;

rmdupCustom2 : (a -> a -> Bool) -> Bool -> Bool ;
(rmdupCustom2 p False) = False ;
(rmdupCustom2 p if (= x t_SV) then True else False) = 
                       if (= x t_SV) then True else False ;
(rmdupCustom2 p if (= x t_SV) then True else u) =
 if (= x t_SV) then True else (rmdupCustom2 p (&& (not (p x t_SV)) u)) ; Eager;

card : (a -> Bool) -> Int ;
(card s) = (card2 (rmdup s)) ;

card2 : (a -> Bool) -> Int ;
(card2 \x.(= x u)) = 1 ;
(card2 \x.(= u x)) = 1 ;
(card2 \x.(|| (= x u) v_SV)) = (add 1 (card2 \x.v_SV)) ;
(card2 \x.(|| (= u x) v_SV)) = (add 1 (card2 \x.v_SV)) ;
(card2 \x.(|| u_SV v_SV)) = (add (card2 \x.u_SV) (card2 \x.v_SV)) ;
(card2 \x.False) = 0 ;
(card2 \x.if (= x u_SV) then True else v_SV) = (add 1 (card2 \x.v_SV)) ;
(card2 \x.if (&& (<= u_SV/CONST/ x) (<= x v_SV/CONST/)) then True else t_SV) =
       (add (sub v_SV u_SV) (card2 \x.t_SV)) ;
(card2 \x.if (>= x u_SV/CONST/) then True else t_SV) = Infinity ;
(card2 \x.if (<= x u_SV/CONST/) then True else t_SV) = Infinity ;

(card2 \x.if x_SV then True else v_SV) = (add (card2 \x.x_SV) (card2 \x.v_SV)) ;

-- typeof(x) - which x? may need to use occurrence.
-- (card2 \x.(&& (= (proj1 x) u_SV/CONST/)
--           (= (proj2 x) v_SV/CONST/))) = 1 ; typeof(19) ~ (a * b) -> Bool;

mapFn : (a -> b) -> (a -> Bool) -> (b -> Bool) ;
(mapFn t s) = \x.\exists y.(&& (s y) (= (t y) x)) ;

filterSet : (a -> Bool) -> (a -> Bool) -> (a -> Bool) ;
(filterSet p \x.False) = \x.False ;
(filterSet p \x.(= x v)) = if (p v) then \x.(= x v) else \x.False ;
(filterSet p \x.(|| u_SV v_SV)) = (union (filterSet p \x.u_SV)
                                         (filterSet p \x.v_SV)) ;
(filterSet p \x.if (= x v) then True else v_SV) =
           if (p v) then (union \x.(= x v) (filterSet p \x.v_SV))
           else (filterSet p \x.v_SV) ;

pickAnElement : (a -> Bool) -> a ;
(pickAnElement \x.(= x u)) = u ;
(pickAnElement \x.if (= x u) then True else v_SV) = u ;

switch : Int -> Bool -> Bool -> Bool -> Bool ;
(switch 1 t1 t2 t3) = t1 ;
(switch 2 t1 t2 t3) = t2 ;
(switch 3 t1 t2 t3) = t3 ;

compare : a -> a -> Int ;
(compare x y) = if (= x y) then 1 else if (< x y) then 2 else 3 ;

makeBTree : (a -> Bool) -> (a -> Bool) ;
-- we expect the first argument to be in list form
(makeBTree \x.s_SV) = \x.(makeBTree2 (sortIte s_SV)) ;

makeBTree2 : Bool -> Bool ;
(makeBTree2 False) = False ;
(makeBTree2 if (= x y) then True else False) = (= x y) ;
(makeBTree2 if (= x y) then True else v2) = 
         (switch (compare x (midEle if (= x y) then True else v2)) True
                 (makeBTree2 (lessthan (midEle if (= x y) then True else v2)
                                         if (= x y) then True else v2))
                 (makeBTree2 (greaterthan (midEle if (= x y) then True else v2)
                                          if (= x y) then True else v2))) ;

sortIte : Bool -> Bool ;
(sortIte False) = False ;
(sortIte if (= x y) then True else v) = (insIte (= x y) (sortIte v)) ;

insIte : Bool -> Bool -> Bool ;
(insIte (= x y) False) = if (= x y) then True else False ;
(insIte (= x_SV y) if (= z_SV/EQUAL,x_SV/ y2) then True else v) =
         if (< y y2) 
         then if (= x_SV y) then True else if (= z_SV y2) then True else v
         else if (= z_SV y2) then True else (insIte (= x_SV y) v) ;

cardBool : Bool -> Int ;
(cardBool False) = 0 ;
(cardBool if (= x y) then True else v) = (add 1 (cardBool v)) ;

get : Float -> Bool -> Bool ;
(get 1.0 if (= x y) then True else v) = y ;
(get n_SV/CONST/ if (= x y) then True else z) = (get (sub n_SV 1.0) z) ;

midEle : Bool -> a ;
(midEle s) = (get (integer (cardBool s) 2) s) ;

lessthan : a -> Bool -> Bool ;
(lessthan z False) = False ;
(lessthan z if (= x y) then True else v2) = 
                   if (< y z) then if (= x y) then True else (lessthan z v2)
                   else (lessthan z v2) ;

greaterthan : a -> Bool -> Bool ;
(greaterthan z False) = False ;
(greaterthan z if (= x y) then True else v2) = 
                  if (> y z) then if (= x y) then True else (greaterthan z v2)
                  else (greaterthan z v2) ;

removeBound : (a -> Bool) -> Bool ;
(removeBound \x.x_SV) = x_SV ;

simplify2D : ((a * a) -> Bool) -> ((a * a) -> Bool) ;
(simplify2D \x.(&& (= (proj1 x) v1) (= (proj2 x) v2))) = \x.(= x (v1,v2)) ;
(simplify2D \x.(|| u_SV v_SV)) = (union (simplify2D \x.u_SV) 
                                        (simplify2D \x.v_SV)) ;

msetunion : (a -> Int) -> (a -> Int) -> (a -> Int) ;
(msetunion \x.0 m) = m ;
(msetunion \x.if (= x t) then v else w_SV m) =
     \x.if (= x t) then (add v (m t)) 
        else ((msetunion \x.w_SV (remove \x.(= x t) m)) x) ;

msetdiff : (a -> Int) -> (a -> Int) -> (a -> Int) ;
(msetdiff \x.0 m) = \x.0 ;
(msetdiff \x.if (= x t) then v else w_SV m) =
     \x.if (= x t) then (monus v (m t)) else ((msetdiff \x.w_SV m) x) ;

msetmax : (a -> Int) -> (a -> Int) -> (a -> Int) ;
(msetmax \x.0 m) = m ;
(msetmax \x.if (= x t) then v else w_SV m) =
     \x.if (= x t) then (max v (m t)) 
        else ((msetmax \x.w_SV (remove \x.(= x t) m)) x) ;

msetmin : (a -> Int) -> (a -> Int) -> (a -> Int) ;
(msetmin \x.0 m) = \x.0 ;
(msetmin \x.if (= x t) then v else w_SV m) =
     \x.if (= x t) then (min v (m t)) else ((msetmin \x.w_SV m) x) ;

msetinc : (a -> Int) -> (a -> Int) -> Bool ;
(msetinc \x.0 m) = True ;
(msetinc \x.if u_SV then v else w_SV m) =
     (&& (pi \x.(implies u_SV (<= v (m x))))
          (msetinc (remove \x.u_SV \x.w_SV) m)) ;

msetmember : a -> (a -> Int) -> Bool ;
(msetmember x m) = (< 0 (m x)) ;
