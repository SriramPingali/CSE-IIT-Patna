// Testbench for p2.v
module tb_p2();
    // Mode Declaration
    wire Y, S1, S2;
    reg S, clk;

    p2 UUT (Y, S, clk);   //Instantiate p2

    always #9 clk = ~clk;

    // Assign S1 and S2
    assign S1 = UUT.S_prev[2];
    assign S2 = UUT.S_prev[1];

    initial begin
        clk = 0;
        UUT.S_prev = 2'b00;

        S = 1; #10;
        S = 0; #40;
        S = 1; #60;
        S = 0; #80;
        S = 1; #40;
        S = 0; #20;
        S = 1; #20;
        S = 0; #20;
    end

    // Display all parameters
    initial begin
        $monitor("time=%g clk=%b  S=%b  S1=%b  S2=%b  Y=%b",$time,clk,S,S1,S2,Y);
    end
endmodule