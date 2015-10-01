import booleans.es ;
import lists.es ;
import sets.es ;
import data.es ;

-- : (isort us6) ; -- (2855, 0.467)

-- : (qsort us6) ; -- (18249, 2.007)

-- : (= () ()) ; -- (1)
-- : (= prod0 prod1) ; -- (8)
-- : (= prod2 prod1) ; -- (10)
-- : (= list3 list2) ; -- (6)
-- : (= list88 list8) ; -- (27)    -- 0.010

-- : ((less \pve2.((= pve2) 1)) \pve3.((= pve3) 1)) ;

-- : (= set3 set4) ; -- (22)
-- : (= set3 set5) ; -- (15)
-- : (= set1 set2) ; -- (66) -- 0.021

-- There is a difference because of simplifyConjunction2
-- : (append (x, y, list2 )) ; -- (21)
-- : (append (u, v, list2 )) ; -- (23)

-- : (append (x, y, list8)) ; -- (150)
-- : (append (u, v, list8)) ; -- (128)
-- : (append (list9, z, (concat (list8, list6)))) ; -- (91)
-- : (append (list6, v, (concat (list8, list6)))) ; -- (78)
-- : (append (list9, (concat (list8, list6)), x)) ; -- (149) -- var capture
-- : (append (list9, (concat (list8, list6)), w)) ; -- (155) -- 0.164

-- : (delete ( 1, ((# 2) ((# 1) [])), ((# 2) []))) ; -- (14)
-- : (delete ( 12, ((# 2) ((# 1) [])), ((# 2) []))) ; -- (16)
-- : (delete ( 2, ((# 2) ((# 1) ((# 2) [])  )), x )) ; -- (28) -- 0.012

-- : (permute (((# 1) []) , x)) ; -- (19)
-- : (permute ( ((# 2) ((# 1) [])), x)) ; -- (85) -- 0.022

-- : (permute ( ((# 3) ((# 2) ((# 1) []))), x)) ; -- (292, 0.064)
-- : (permute ( (# 10 (# 3 (# 2 (# 1 [])))), x)) ; -- (1328, 0.440)
-- : (permute ( (# 12 (# 10 (# 3 (# 2 (# 1 []))))), x)) ; -- (6305, 7.061)

-- : (sorted list7) ; -- (74)
-- : (sorted list8) ; -- (71)
-- : (sorted (isort us6)) ; -- (8668) -- 1.809


-- crickettennis = \x.if (= x Cricket) then True
--                    else if (= x Tennis) then True else False ;
-- : \x.(pi \y.(implies (crickettennis y) (likes (x,y)))) -- (159)
-- : \x.(pi \y.(implies (favourite y) (likes (x,y)))) -- (129) -- 0.038
-- 
-- 
-- 
-- : (powerset \x.((|| ((= x) 1)) ((= x) 2))) -- (41)
-- : (linearise \x.((|| ((= x) 1)) ((= x) 2))) -- (8)
-- : ((union \x.((|| ((= x) 1)) ((= x) 2))) \x.((|| ((= x) 1)) ((= x) 3))) -- (3)
-- 
-- : (intersect set12 set13) -- (31)
-- -- 0.021
-- 
-- : (msetunion mset1 mset2) -- (52)
-- : (msetdiff mset1 mset2) -- (22)
-- : (msetmax mset1 mset2) -- (52)
-- : (msetmin mset1 mset2) -- (19) -- 0.015
-- 
-- : (msetinc mset0 mset2) -- (34)
-- : (msetmember F mset1) -- (8)
-- : (msetmember A mset1) -- (6) -- 0.0012
-- 
-- bunch = \x.if (= x (Abloy,3,Short,Normal)) then True
--            else if (= x (Abloy,4,Medium,Broad)) then True else False ;
-- 
-- (projmake (x1,x2,x3,x4)) = x1
-- (projlength (x1,x2,x3,x4)) = x3
-- 
-- -- cond : Key -> Bool
-- cond = \x.(&& (= Abloy (projmake x)) (= Medium (projlength x)))
-- 
-- (setexists1 p t) = (sigma \x.(&& (t x) (p x)))
-- 
-- : (setexists1 cond bunch) -- (27, 0.0010)
-- 

Avon , Bedfordshire , Berkshire , 
Buckinghamshire , Cambridgeshire , Cornwall ,
Devon , Dorset , Essex , Gloucestershire ,
Hampshire , Herefordshire , Hertfordshire ,
Kent , London , Northamptonshire , Oxfordshire ,
Somerset , Surrey , Sussex , Warwickshire ,        
Wiltshire , Worcestershire : County ;

Bath , Bournemouth , Bristol , Cheltenham ,
Cirencester , Dorchester , Exeter , Gloucester ,
Penzance , Plymouth , Salisbury , Shaftesbury ,
Sherbourne , Taunton , Torquay , Truro , 
Winchester : City ;

neighbours : (County * County) -> Bool ;
neighbours = 
  \x.if (= x (Devon, Cornwall)) then True
     else if (= x (Devon, Dorset)) then True
     else if (= x (Devon, Somerset)) then True
     else if (= x (Avon, Somerset)) then True
     else if (= x (Avon, Wiltshire)) then True
     else if (= x (Avon, Gloucestershire)) then True
     else if (= x (Dorset, Wiltshire)) then True
     else if (= x (Somerset, Wiltshire)) then True
     else if (= x (Gloucestershire, Wiltshire)) then True
     else if (= x (Dorset, Somerset)) then True
     else if (= x (Dorset, Hampshire)) then True
     else if (= x (Hampshire, Wiltshire)) then True
     else if (= x (Hampshire, Berkshire)) then True
     else if (= x (Hampshire, Sussex)) then True
     else if (= x (Hampshire, Surrey)) then True
     else if (= x (Sussex, Surrey)) then True
     else if (= x (Sussex, Kent)) then True
     else if (= x (London, Surrey)) then True
     else if (= x (London, Kent)) then True
     else if (= x (London, Essex)) then True
     else if (= x (London, Hertfordshire)) then True
     else if (= x (London, Buckinghamshire)) then True
     else if (= x (Surrey, Buckinghamshire)) then True
     else if (= x (Surrey, Kent)) then True
     else if (= x (Surrey, Berkshire)) then True
     else if (= x (Oxfordshire, Berkshire)) then True
     else if (= x (Oxfordshire, Wiltshire)) then True
     else if (= x (Oxfordshire, Gloucestershire)) then True
     else if (= x (Oxfordshire, Warwickshire)) then True
     else if (= x (Oxfordshire, Northamptonshire)) then True
     else if (= x (Oxfordshire, Buckinghamshire)) then True
     else if (= x (Berkshire, Wiltshire)) then True
     else if (= x (Berkshire, Buckinghamshire)) then True
     else if (= x (Gloucestershire, Worcestershire)) then True
     else if (= x (Worcestershire, Herefordshire)) then True
     else if (= x (Worcestershire, Warwickshire)) then True
     else if (= x (Bedfordshire, Buckinghamshire)) then True
     else if (= x (Bedfordshire, Northamptonshire)) then True
     else if (= x (Bedfordshire, Cambridgeshire)) then True
     else if (= x (Bedfordshire, Hertfordshire)) then True
     else if (= x (Hertfordshire, Essex)) then True
     else if (= x (Hertfordshire, Cambridgeshire)) then True
     else if (= x (Hertfordshire, Buckinghamshire)) then True
     else if (= x (Buckinghamshire, Northamptonshire)) then True else False ;

distance : (City * City * Int) -> Bool ;
distance = 
  \x.if (= x (Plymouth, Exeter, 42)) then True
     else if (= x (Exeter, Bournemouth, 82)) then True
     else if (= x (Bristol, Taunton, 43)) then True
     else if (= x (Bristol, Gloucester, 35)) then True
     else if (= x (Torquay, Exeter, 23)) then True
     else if (= x (Plymouth, Torquay, 24)) then True
     else if (= x (Bristol, Bath, 13)) then True
     else if (= x (Exeter, Taunton, 34)) then True
     else if (= x (Penzance, Plymouth, 78)) then True
     else if (= x (Taunton, Bournemouth, 70)) then True
     else if (= x (Bournemouth, Salisbury, 28)) then True
     else if (= x (Taunton, Salisbury, 64)) then True
     else if (= x (Salisbury, Bath, 40)) then True
     else if (= x (Bath, Gloucester, 39)) then True
     else if (= x (Bournemouth, Bath, 65)) then True
     else if (= x (Truro, Penzance, 26)) then True
     else if (= x (Plymouth, Truro, 52)) then True
     else if (= x (Shaftesbury, Salisbury, 20)) then True
     else if (= x (Sherbourne, Shaftesbury, 16)) then True
     else if (= x (Dorchester, Bournemouth, 28)) then True
     else if (= x (Salisbury, Winchester, 24)) then True
     else if (= x (Exeter, Sherbourne, 53)) then True
     else if (= x (Sherbourne, Taunton, 29)) then True
     else if (= x (Bath, Cirencester, 32)) then True
     else if (= x (Cirencester, Cheltenham, 16)) then True
     else if (= x (Cheltenham, Gloucester, 9)) then True
     else if (= x (Dorchester, Sherbourne, 19)) then True
     else if (= x (Bath, Shaftesbury, 33)) then True
     else if (= x (Winchester, Bournemouth, 41)) then True
     else if (= x (Exeter, Dorchester, 53)) then True else False ;

isin : (City * County) -> Bool ;
isin = 
  \x.if (= x (Bristol, Avon)) then True
     else if (= x (Taunton, Somerset)) then True
     else if (= x (Salisbury, Wiltshire)) then True
     else if (= x (Bath, Avon)) then True
     else if (= x (Bournemouth, Dorset)) then True
     else if (= x (Gloucester, Gloucestershire)) then True
     else if (= x (Torquay, Devon)) then True
     else if (= x (Penzance, Cornwall)) then True
     else if (= x (Plymouth, Devon)) then True
     else if (= x (Exeter, Devon)) then True
     else if (= x (Winchester, Hampshire)) then True
     else if (= x (Dorchester, Dorset)) then True
     else if (= x (Cirencester, Gloucestershire)) then True
     else if (= x (Truro, Cornwall)) then True
     else if (= x (Cheltenham, Gloucestershire)) then True
     else if (= x (Shaftesbury, Dorset)) then True
     else if (= x (Sherbourne, Dorset)) then True else False ;

-- : \x.(sigma \y.(&& (|| (distance (Bristol, x, y)) 
--                        (distance (x, Bristol, y)))  
--                    (< y 40)) ) ; -- (582, 0.070) 

-- : \x.\y.(sigma \z.(&& (distance (x,y,z)) (< z 20))) ; -- (239, 0.043)

-- : \x.(|| (neighbours (Oxfordshire,x)) (neighbours (x, Oxfordshire))) ;
-- -- (395, 0.059) 

-- : \x.(sigma \y.(&& (isin (x,y)) (/= y Wiltshire))) ; -- (158, 0.037)

-- : \x.(sigma \y.(&& (|| (neighbours (Oxfordshire,y)) 
--                       (neighbours (y, Oxfordshire))) (isin (x,y)))) ;
-- -- (1174, 0.150)
 
-- : \x.(sigma \y.(&& (isin (x,y)) (|| (neighbours (Oxfordshire,y)) 
--             (neighbours (y, Oxfordshire)))  )) ; -- (9446, 1.369)
 
-- westcountry : County -> Bool ;
-- westcountry = \x.if (= x Devon) then True else if (= x Cornwall) then True 
--                  else if (= x Somerset) then True  
--                  else if (= x Avon) then True else False ;
-- : \x.(sigma \y.(&& (westcountry y) (isin (x,y)))) ; -- (293, 0.054)

-- : \x.(sigma \y.(sigma \z.(&& (|| (distance (Bristol, y, z)) 
--     (distance (y, Bristol, z)) )  (&& (< z 50) (isin (y,x))) ))) ; -- (915, 0.130)

-- : (pi \x.(implies (|| (neighbours (Avon,x)) (neighbours (x,Avon))) 
--       (sigma \y.(isin (y,x))) )) ; -- (740, 0.364)
-- 
-- : (sigma \x.(&& (isin (Bristol, x))
--                 (pi \z.(implies (sigma \y.(&& (|| (distance (Bristol, z, y)) 
--                                        (distance (z, Bristol, y))) (< y 40)))
--                                  (isin (z, x)))))) ; -- (335, 0.073)
