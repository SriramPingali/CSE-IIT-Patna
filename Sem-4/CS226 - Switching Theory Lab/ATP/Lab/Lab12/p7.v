// Style 1
module mux1(op,s1,s0,d3,d2,d1,d0);
    // input s1,s0;        // Select Lines
    input s1,s0,d3,d2,d1,d0;  // Inputs
    output op;          // Output

    and (op0,~s1,~s0,d0);     // Line 0
    and (op1,~s1,s0,d1);      // Line 1
    and (op2,s1,~s0,d2);      // Line 2
    and (op3,s1,s0,d3);       // Line 3
    or (op,op0,op1,op2,op3);  // Output
endmodule

// Style 2
module mux2(op,s1,s0,d3,d2,d1,d0);
    // input s1,s0;        // Select Lines
    input s1,s0,d3,d2,d1,d0;  // Inputs
    output op;          // Output

    and (op0,~s1,~s0,d0);               // Line 0
    and (op1,~s1,s0,d1);                // Line 1
    and (op2,s1,~s0,d2);                // Line 2
    and (op3,s1,s0,d3);                 // Line 3
    assign op = (op0|op1|op2|op3);      // Output
endmodule

// Style 3
module mux3(op,s1,s0,d3,d2,d1,d0);
    // input s1,s0;        // Select Lines
    input s1,s0,d3,d2,d1,d0;  // Inputs
    output op;          // Output
    assign op = (~s1 & ~s0 & d0) | (~s1 & s0 & d1) | (s1 & ~s0 & d2) | (s1 & s0 & d3);
endmodule

// Style 4
module mux4(op,s1,s0,d3,d2,d1,d0);
            // Select Lines
    input s1,s0,d3,d2,d1,d0;  // Inputs
    output op;          // Output
    assign op = s1 ? (s0 ? d3 : d2) : (s0 ? d1 : d0);
endmodule

// Style 5
module mux5(op,s1,s0,d3,d2,d1,d0);
    // input s1,s0;        // Select Lines
    input s1,s0,d3,d2,d1,d0;  // Inputs
    output op;          // Output
    reg op;

    always @(s1 or s0 or d3 or d2 or d1 or d0)
    begin
        if(s1==0 & s0==0)
            assign op = d0;
        else if(s1==0 & s0==1)
            assign op = d1;
        else if(s1==1 & s0==0)
            assign op = d2;
        else
            assign op = d3;
    end
endmodule