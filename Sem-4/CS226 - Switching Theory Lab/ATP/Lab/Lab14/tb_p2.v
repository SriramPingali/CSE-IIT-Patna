// Testbench for p2 in p2.v
module tb_p2();
    // Mode Declaration
    reg clk = 1'b0;     // Clock
    wire [3:0] count;   // Counter value
    wire upper;         // output 'upper'

    p2 UUT (count,upper,clk);   // Instantiate p2

    // 1 Clock Cycle = 20ns
    always #10 clk = ~clk;

    // Display all parameters
    initial begin
        $monitor("time=%g\tclk=%b count=%h upper=%b",$time,clk,count,upper);
    end
endmodule
