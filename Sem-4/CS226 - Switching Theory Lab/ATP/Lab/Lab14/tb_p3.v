// Testbench for p3_b in p3.v
module tb_p3_b();
    // Mode Declaration
    reg [7:0] Input;
    reg [2:0] SELA, SELB, SELD;
    reg [4:0] OPR;
    reg Clock = 0;
    wire [7:0] Output;

    // Instantiate p3_b
    p3_b UUT (Output, Input, SELA, SELB, SELD, OPR, Clock);

    wire [7:0] val = UUT.R[SELD];   // Value Stored in desired register

    // 1 Clock Cycle = 10ns
    always #5 Clock = ~Clock;

    initial begin
        // Addition of Input=24 and R1=0, store result=24 in R1
        Input = 8'd24;
        SELA = 3'd0;
        SELB = 3'd1;
        SELD = 3'd1;
        OPR = 5'd0;
        #10;
        
        // Addition of R1=24 and R1=24, store result=48 in R2
        SELA = 3'd1;
        SELB = 3'd1;
        SELD = 3'd2;
        OPR = 5'd0;
        #10;
        
        // Subtraction of R1=24 and Input=53, store result=29 in R3
        Input = 8'd53;
        SELA = 3'd0;
        SELB = 3'd1;
        SELD = 3'd3;
        OPR = 5'd1;
        #10;
        
        // Bitwise AND of R1=24 and R3=29, store result=20 in R4
        SELA = 3'd1;
        SELB = 3'd3;
        SELD = 3'd4;
        OPR = 5'd2;
        #10;
        
        // Bitwise XOR of R4=20 and Input=255, store result=235 in R7
        Input = 8'd255;
        SELA = 3'd4;
        SELB = 3'd0;
        SELD = 5'd7;
        OPR = 5'd6;
        #10;

        // End Simulation
        Input = 8'bx;
        SELA = 3'bx;
        SELB = 3'bx;
        SELD = 5'bx;
        OPR = 5'bx;
    end

    // Display all parameters
    initial begin
        $monitor("time=%g\tClock=%b Input=%d SELA=%d A_Bus=%d SELB=%d B_bus=%d OPR=%d Output=%d SELD=%d Value_Stored=%d",$time,Clock,Input,SELA,UUT.R[SELA],SELB,UUT.R[SELB],OPR,Output,SELD,val);
    end
endmodule

// Testbench for p3_c (FSM) in p3.v
module tb_p3_c();
    // Mode Declaration
    reg [7:0] Input;
    reg Clear = 0, Clock = 0, Reset = 0;
    wire [7:0] Output;

    // Instantiate p3_c
    p3_c UUT (Output, Input, Clock, Clear, Reset);

    // Parameters of p3_c for simulation waveform
    wire [2:0] SELA = UUT.SELA;
    wire [2:0] SELB = UUT.SELB;
    wire [2:0] SELD = UUT.SELD;
    wire [4:0] OPR = UUT.OPR;
    wire [7:0] A_bus = UUT.A_bus;
    wire [7:0] B_bus = UUT.B_bus;
    wire [7:0] val = UUT.R[3'd7];   // Value stored in desired register
    wire [1:0] state = UUT.state;   // State of FSM

    // 1 Clock Cycle = 10ns
    always #5 Clock = ~Clock;

    initial begin
        // Add Input=24 and R1=125, store result=149 in R7
        Input = 8'd24;
        #25;
        // Add Input=53 and R1=125, store result=178 in R7
        Input = 8'd53;
    end

    // Display all parameters
    initial begin
        $monitor("time=%g\tClock=%b State=%d Input=%d SELA=%d A_Bus=%d SELB=%d B_bus=%d OPR=%d Output=%d SELD=%d Value_Stored=%d",$time,Clock,state,Input,UUT.SELA,UUT.A_bus,UUT.SELB,B_bus,UUT.OPR,Output,UUT.SELD,val);
    end
endmodule