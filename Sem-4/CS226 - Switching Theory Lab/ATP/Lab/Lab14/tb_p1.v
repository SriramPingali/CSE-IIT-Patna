// Testbench for module p1 in p1.v
module tb_p1();
    // Mode Declaration
    reg A,clk;  // Inputs
    wire Y;     // Outputs

    p1 UUT (Y,A,clk);   // Instantiate p1

    wire [2:0] state = UUT.state;

    // Test Sequence: 11101101001001101101
    initial begin
        clk = 1;

        A = 1'b1;  #30;
        A = 1'b0;  #10;
        A = 1'b1;  #20;
        A = 1'b0;  #10;
        A = 1'b1;  #10;
        A = 1'b0;  #20;
        A = 1'b1;  #10;
        A = 1'b0;  #20;
        A = 1'b1;  #20;
        A = 1'b0;  #10;
        A = 1'b1;  #20;
        A = 1'b0;  #10;
        A = 1'b1;  #10;
    end

    // 1 Clock Cycle = 10ns
    always #5 clk = ~clk;

    // Display all parameters
    initial begin
        $monitor("time=%g\tclk=%b A=%b state=%b Y=%b",$time,clk,A,state,Y);
    end
endmodule
