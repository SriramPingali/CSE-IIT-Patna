himalayan(a).
himalayan(b).
himalayan(c).
himalayan(A):-mountain(A);ski(A).

like(a, rain).
like(a, snow).
notlike(b, B):-like(a, B).
mountain(C):-notlike(C, rain).
ski(D):-like(D,snow).

g(D):-himalayan(D),mountain(D),not(ski(D)),!.
