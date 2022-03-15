// ALU with 32 Operations
module ALU (out,A,B,OPR);
    input [7:0] A,B;        // Input numbers
    input [4:0] OPR;        // Operation Code
    output reg [7:0] out;   // ALU Output

    // Operation Code for Addition is 5'b0_0000
    always @(OPR or A or B) begin
        case(OPR)
            5'd0: out = A+B;            5'd1: out = A-B;            5'd2: out = A&B;        5'd3: out = A|B;
            5'd4: out = ~(A&B);         5'd5: out = ~(A|B);         5'd6: out = A^B;        5'd7: out = ~(A^B);
            5'd8: out = A>B;            5'd9: out = A==B;           5'd10: out = A<B;       5'd11: out = A*B;
            5'd12: out = A/B;           5'd13: out = -A;            5'd14: out = -B;        5'd15: out = A<<2;
            5'd16: out = A>>2;          5'd17: out = B<<2;          5'd18: out = B>>2;      5'd19: out = ~A;
            5'd20: out = ~B;            5'd21: out = ~A & B;        5'd22: out = A & ~B;    5'd23: out = ~A | B;
            5'd24: out = A | ~B;        5'd25: out = A;             5'd26: out = B;         5'd27: out = A^(8'hff);
            5'd28: out = B^(8'hff);     5'd29: out = A^(8'hff)+1;   5'd30: out = B^(8'hff)+1;
            5'd31: out = {A[7],A[6],A[5],A[4],A[3],A[2],A[1],A[0]};
        endcase
    end
endmodule

// 8:1 Multiplexer
module MUX8_1 (out, ip0, ip1, ip2, ip3, ip4, ip5, ip6, ip7, sel);
    // MUX Inputs
    input [7:0] ip0,ip1,ip2,ip3,ip4,ip5,ip6,ip7;
    input [2:0] sel;        // MUX Select
    output [7:0] out;       // MUX Output

    assign out = (sel == 3'd0) ? ip0 :
                 (sel == 3'd1) ? ip1 :
                 (sel == 3'd2) ? ip2 :
                 (sel == 3'd3) ? ip3 :
                 (sel == 3'd4) ? ip4 :
                 (sel == 3'd5) ? ip5 :
                 (sel == 3'd6) ? ip6 : ip7;
endmodule

// Verilog Model for given block
module p3_b (Output, Input, SELA, SELB, SELD, OPR, Clock, Clear);
    input [7:0] Input;              // Input Line
    input [2:0] SELA, SELB, SELD;   // Select Lines
    input [4:0] OPR;                // Operation Code
    input Clock;                    // Clock
    input Clear;                    // Clear data from registers
    output [7:0] Output;            // Output Line

    reg [7:0] R [7:1];              // Registers 1-7
    wire [7:0] A_bus, B_bus;        // A bus and B bus

    // Instantiate MUX and ALU submodules
    MUX8_1 muxA (A_bus, Input, R[1], R[2], R[3], R[4], R[5], R[6], R[7], SELA);
    MUX8_1 muxB (B_bus, Input, R[1], R[2], R[3], R[4], R[5], R[6], R[7], SELB);
    ALU     alu (Output, A_bus, B_bus, OPR);

    // Initialize all Register Data to Zero
    integer i,j;
    initial begin
        for (i=1; i<=7; i=i+1) begin
            R[i] = 8'b0;
        end
    end

    // Store Output in Registers
    always @(posedge Clock) begin
        // if Clear = 1 Erase Data from all Registers
        if (Clear) begin
            for (j=1; j<=7; j=j+1) begin
                R[j] = 8'b0;
            end
        end
        else R[SELD] = Output;
        // Store in Register selected by SELD
    end
endmodule

// FSM to add 2 numbers from Registers R1-R7 of given block
module p3_c (Output, Input, Clock, Clear, Reset);
    input [7:0] Input;                  // Input Line
    input Clock;                        // Clock
    input Clear;                        // Clear data from registers
    input Reset;                        // Back to Initial State
    output [7:0] Output;                // Output Lines

    // State Encoding
    parameter READ  = 2'd0;             // Read from registers
    parameter CALC  = 2'd1;             // Calculate Output
    parameter WRITE = 2'd2;             // Write to register

    reg [7:0] R [7:1];                  // Registers 1-7
    reg [4:0] OPR;                      // Operation Code
    reg [1:0] state = READ;             // State Register
    reg [2:0] SELA, SELB, SELD;         // Select Lines
    wire [7:0] A_bus, B_bus;            // A bus and B bus

    // Instantiate MUX and ALU submodules
    MUX8_1 muxA (A_bus, Input, R[1], R[2], R[3], R[4], R[5], R[6], R[7], SELA);
    MUX8_1 muxB (B_bus, Input, R[1], R[2], R[3], R[4], R[5], R[6], R[7], SELB);
    ALU     alu (Output, A_bus, B_bus, OPR);

    integer i,j;
    initial begin
        for (i=1; i<=7; i=i+1) begin
            R[i] = 8'b0;
        end
        // Initial Values in R1, R2, R7
        R[1] = 8'd125;
        R[7] = 8'd144;
    end

    always @(posedge Clock) begin
        // State Transition
        // If Reset = 1, go back to Read State
        if (Reset) state = READ;
        else state = (state+1)%3;
        //Else, move to next state

        // Clear Data in all Registers
        if (Clear) begin
            for (j=1; j<=7; j=j+1) begin
                R[j] = 8'b0;
            end
        end
    end

    always @(state) begin
        case(state)
        READ: begin
            // Input line and R1 will be added
            SELA = 3'd0;
            SELB = 3'd1;
        end
        CALC: begin
            // Add Input line and R1
            OPR = 5'd0;
        end
        WRITE: begin
            // Output is stored in Register 7
            SELD = 3'd7;
            R[SELD] = Output;
        end
        endcase
    end
endmodule
