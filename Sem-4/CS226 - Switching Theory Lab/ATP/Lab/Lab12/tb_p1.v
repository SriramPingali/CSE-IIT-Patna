// Testbench for p2 module
module tb_p1();
    
    // Registers to assign inputs, Wire for output
    reg d3,d2,d1,d0,s1,s0;
    wire z;

    // Instantiate module p1 with above variables
    p1 UUT (z,d3,d2,d1,d0,s1,s0);

    // All Test Cases
    initial begin
        d0 = 1'b0;
        d1 = 1'b1;
        d2 = 1'b0;
        d3 = 1'b1;

        s1 = 1'b0;
        s0 = 1'b0;
        #10;
        s1 = 1'b0;
        s0 = 1'b0;
        d0 = 1;
        #10;
        
        s1 = 1'b0;
        s0 = 1'b1;
        #10;
        s1 = 1'b0;
        s0 = 1'b1;
        d1 = 0;
        #10;
        
        s1 = 1'b1;
        s0 = 1'b0;
        #10;
        s1 = 1'b1;
        s0 = 1'b0;
        d2 = 1;
        #10;
        
        s1 = 1'b1;
        s0 = 1'b1;
        #10;
        s1 = 1'b1;
        s0 = 1'b1;
        d3 = 0;
    end

    // print test cases with respective outputs
    initial begin
        $monitor("d3=%b d2=%b d1=%b d0=%b s1=%b s0=%b z=%b",d3,d2,d1,d0,s1,s0,z);
    end
endmodule