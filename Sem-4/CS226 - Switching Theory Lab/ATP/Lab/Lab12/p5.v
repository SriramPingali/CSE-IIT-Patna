/* Verilog Model for
   Alarm in Mueseum */
module p5(A,m0,m1,m2);
    
    input m0,m1,m2; // motion sensor in each room
    output A;       // alarm

    /* alarm sounds when there is
     motion in more than one room at a time */
    assign A = (m0 & m1) | (m1 & m2) | (m2 & m0);
endmodule