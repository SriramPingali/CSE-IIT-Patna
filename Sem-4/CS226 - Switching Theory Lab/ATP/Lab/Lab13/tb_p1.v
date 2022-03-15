// Testbench for p1.v
module tb_p1();
    // Mode Declaration
    reg [3:0] num;
    wire [3:0] op;

    p1 UUT (op,num);    // Instantiate p1

    // Testcase: All 4-bit binary numbers
    initial begin
        num = 4'b0000;
        repeat(15) begin
            #10 num = num + 1;
        end
    end
    
    initial begin
        $display("binary    gray");
        $monitor(" %b      %b",num,op);
    end
endmodule