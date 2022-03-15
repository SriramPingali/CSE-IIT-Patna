// Testbench for module comparators in p4
module tb_p4();
    parameter N = 8;
    reg [N-1:0] a, b;
    wire eq, neq, lt, lte, gt, gte;

    comparators uut(eq, neq, lt, lte, gt, gte, a, b);
    
    initial begin
        a = 8'b0001_1111; // DEC 31
        b = 8'b0000_1111; // DEC 15
        #10;
        a = 8'b0101_1000; // DEC 88
        b = 8'b0101_1000; // DEC 88
        #10;
        a = 8'b0100_1001; // DEC 73
        b = 8'b0111_0101; // DEC 117
    end

    // Display Comparator Outputs
    initial begin
        $monitor("For a = %b, and b = %b\n\teq = %b\n\tneq = %b\n\tlt = %b\n\tlte = %b\n\tgt = %b\n\tgte = %b\n",a,b,eq,neq,lt,lte,gt,gte);
    end
endmodule