// Structural Description for given schematic
module p6(op1, op2, A, B, C);
    input A,B,C;    // Inputs
    output op1,op2; // Outputs

    /*  x1 */ nor U2a(x1,A,B);
    /*  x2 */ nand U1b(x2,A,B);
    /*  x3 */ not U6(x3,x2);
    /*  x4 */ not U3(x4,C);
    /*  x5 */ nor U2b(x5,x1,x3);
    /*  x6 */ not U8(x6,x4);
    /*  x7 */ not U9(x7,x6);
    /*  x8 */ or U5(x8,x1,x7);
    /*  x9 */ xnor U4(x9,x5,x6);
    /* op1 */ nand U1a(op1,x2,x8);
    /* op2 */ not U7(op2,x9);

endmodule
