// FSM to detect the pattern 1101
module p1(Y,A,clk);
    // A is input bit
    input A,clk;
    // clk is Clock
    output reg Y;   // Y is the output
    // Y is high when the last four input bits are 1101

    reg [2:0] state = 3'b000;   // State Register
    wire s2 = state[2];
    wire s1 = state[1];
    wire s0 = state[0];

    always @(posedge clk) begin
        // Combinational Logic for output
        assign Y = state[2];

        // Combinational Logic for Next State
        state = {(s1 & s0 & A),                                 // state[0]
                 (~s1 & s0 & A) + (s1 & ~s0) + (s2 & A),        // state[1]
                 (~s2 & ~s1 & ~s0 & A) + (s1 & ~s0 & ~A)};      // state[2]
    end
endmodule
