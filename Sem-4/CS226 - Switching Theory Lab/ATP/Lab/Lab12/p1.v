// Model for given Logic Function
module p1(z,d3,d2,d1,d0,s1,s0);
    
    // Variable Declaration
    output z;
    input d3,d2,d1,d0;
    input s1,s0;

    wire _s0, _s1, z0, z1, z2, z3;

    // Inverted Select Lines
    not n0 (_s0,s0);
    not n1 (_s1,s1);

    // Generating Inputs for NOR gate
    and a0 (z0, _s1, _s0, d0);
    and a1 (z1, _s1,  s0, d1);
    and a2 (z2,  s1, _s0, d2);
    and a3 (z3,  s1,  s0, d3);
    
    // Final Output z
    nor nor_0(z,z0,z1,z2,z3);

endmodule