belong(a).
belong(b).
belong(c).
belong(X):-notmountclimb(X),notskier(X),!, fail.
/*contraposition to have horn clause */
belong(X).
like(a,rain). /*Declearing that a likes rain*/
like(a,snow). /*Declearing that a likes snow*/
like(a,X) :- notlike(b,X). /*showing that A likes what B notlikes*/
like(b,X) :- like(a,X),!,fail. /*showing that A notlikes what B likes*/
like(b,X).
mountclimb(X):-like(X,rain),!,fail. /*No mountain climber likes rain*/
mountclimb(X).
notskier(X):- notlike(X,snow). /*contraposition to have horn clause */
notmountclimb(X):- mountclimb(X),!,fail.
notmountclimb(X).
notlike(M,N):- like(M,N),!,fail.
notlike(M,N).
q(X):-belong(X),mountclimb(X),notskier(X),!. /* ! means no backtrack*/
