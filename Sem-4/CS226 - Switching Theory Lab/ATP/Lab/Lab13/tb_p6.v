// Testbench for given model
module tb_p6_decade_counter();
    // Mode declaration
    reg clk;
    wire [3:0] q;

    // Instantiate given model
    decade_counter UUT (q,clk);

    initial begin
        clk = 0;
        // Assume initial value is zero
        UUT.q = 4'b0000;
    end

    always #10 clk = ~clk;

    // Display all parameters
    initial begin
        $display("time\t clk q");
        $monitor("%g\t  %b  %h",$time,clk,q);
    end
endmodule

// Testbench for given model
module tb_p6_decoded_counter();
    // Mode Declaration
    reg clk;
    wire ctrl;
    wire [3:0] count_value;

    // Instantiate given model
    decoded_counter DUT (ctrl,clk);

    assign count_value = DUT.count_value;
    initial begin
        clk = 0;
        // Assume initial value is zero
        DUT.count_value = 4'b0000;
    end

    always #10 clk = ~clk;

    // Display all parameters
    initial begin
        $display("time\t clk count_value ctrl");
        $monitor("%g\t   %b      %d       %b",$time,clk,DUT.count_value,ctrl);
    end
endmodule