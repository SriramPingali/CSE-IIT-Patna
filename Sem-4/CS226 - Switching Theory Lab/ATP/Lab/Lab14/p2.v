// 4-bit Up Counter with
// additional output 'upper'
module p2(count,upper,clk);
    // 4-bit register for counting
    output reg [3:0] count = 4'b0;
    output upper;      // output -> upper
    input clk;         // Clock

    always @(posedge clk)
        // At rising edge of clock
        // Increase count by 1
        count = count + 1;
        
        // Upper is High when count is
        // within upper half of counter range
        // i.e., 8-15
        assign upper = count[3];
endmodule
