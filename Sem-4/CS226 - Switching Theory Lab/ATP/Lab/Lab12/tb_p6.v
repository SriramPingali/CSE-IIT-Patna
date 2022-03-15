// Testbench for module p6
module tb_p6();
    reg A,B,C;      // Inputs
    wire op1,op2;   // Outputs

    p6 UUT (op1,op2,A,B,C);

    // Loop through all combinations of A,B,C
    initial begin
        A = 0;
        B = 0;
        C = 0;

        while ((A & B & C) != 1) begin
            #10;
            A = A^(B & C);
            B = B^C;
            C = C^1;
        end
    end

    // Print outputs for all possible inputs
    initial begin
        $monitor("A=%b B=%b C=%b op1=%b op2=%b",A,B,C,op1,op2);
    end
endmodule
