// 4 : 1 Multiplexer
module MUX_4to1(op,s1,s0,d3,d2,d1,d0);
    input s1,s0;        // select lines
    input d3,d2,d1,d0;  // inputs
    output op;          // output

    and (line0,~s1,~s0,d0);
    and (line1,~s1,s0,d1);
    and (line2,s1,~s0,d2);
    and (line3,s1,s0,d3);
    or (op,line0,line1,line2,line3);
endmodule

// 16 : 1 Multiplexer
module p8(op,s3,s2,s1,s0,d15,d14,d13,d12,d11,d10,d9,d8,d7,d6,d5,d4,d3,d2,d1,d0);
    input s3,s2,s1,s0;                      // select lines
    input d7,d6,d5,d4,d3,d2,d1,d0;          // inputs 0-7
    input d15,d14,d13,d12,d11,d10,d9,d8;    // inputs 8-15
    output op;                              // output

    // We use 4 MUX's to provide 16 input lines
    // Connect each of their output lines to 5th MUX
    // s1, s0 are select lines for 4 MUX's
    // s3, s2 are select lines for 5th MUX
    MUX_4to1 mux0 (line0,s1,s0,d3,d2,d1,d0);
    MUX_4to1 mux1 (line1,s1,s0,d7,d6,d5,d4);
    MUX_4to1 mux2 (line2,s1,s0,d11,d10,d9,d8);
    MUX_4to1 mux3 (line3,s1,s0,d15,d14,d13,d12);
    MUX_4to1 muxF (op,s3,s2,line3,line2,line1,line0);
endmodule