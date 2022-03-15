// Module for given Sequential Circuit
module p2 (Y,S,clk);
    output Y;       // Output Y
    input S, clk;
    
    wire S1,S2;
    reg [2:1] S_prev;  // Stores previous values of S

    assign S2 = S_prev[2];   // Value of S before 2 clock cycles
    assign S1 = S_prev[1];   // Value of S before 1 clock cycle
    
    assign Y = (S & S1 & S2) | (~S & ~S1 & ~S2);
    
    always @ (posedge clk) begin
        S_prev = {S, S_prev[2]};
    end
endmodule