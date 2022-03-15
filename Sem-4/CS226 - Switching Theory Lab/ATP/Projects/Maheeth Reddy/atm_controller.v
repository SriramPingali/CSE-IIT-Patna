// Verilog module for ATM Machine Controller
module ATM_controller(keyboard,reset,enter,fail,pass,bal_disp);
	// Pin Declaration
    input [5:0] keyboard;               // Gives inputs to controller for A/c Number, PIN, Amount
    input reset,enter;                  // Reset pin, Enter pin
    output reg fail,pass;               // Indicator for service facilitation
    output reg [5:0] bal_disp;          // Output pin to display Balance
    
    reg [3:0] state;                    // State Register
    
    reg [4:0] curAcc = 5'bx;            // Display waveform for Account Number
    reg [3:0] inputPIN = 4'bx;          // Display waveform for PIN entered
    reg [3:0] validPIN = 4'bx;          // Display waveform for valid PIN
    
    reg [17:0] acc1,acc2,acc3,acc4;     // Account Details
    reg [2:0] upd1,upd2,upd3;           // Initiate changes to account details

    reg [11:0] temp;                    // Register for PIN authentication
    reg [3:0] temp1;                    // Register for confirming new PIN
    reg [5:0] temp2;                    // Register for confirming cash deposit
    
    initial begin
		// Initialize the FSM
        state <= 4'b0;
        fail <= 0;
        pass <= 0;
        upd1 <= 3'b000;
        upd2 <= 3'b000;
        upd3 <= 3'b000;
        bal_disp <= 6'b000000;
        
		// Details of all Accounts
        acc4[17:0] = 18'b00001_0001_010100_00_1;
        acc1[17:0] = 18'b00011_0011_001100_01_1;
        acc2[17:0] = 18'b00111_0111_001110_10_1;
        acc3[17:0] = 18'b01111_1111_001111_10_1;
    end

    always@(posedge enter,posedge reset) begin
        // Reset FSM - Account Number Identification
        if(reset == 1) begin
            state <= 4'b0000;
            fail <= 0;
            pass <= 0;
            bal_disp <= 6'b000000;
            upd2 <= 3'b000;
            upd1 <= 3'b000;
            upd3 <= 3'b000;
        end
        else begin
            // State 0 - Account Number Identification
            if(state == 4'b0000) begin
                if(keyboard[5:1] == acc4[17:13] && acc4[0] == 1) begin
                    temp <= acc4[12:1];
                    fail <= 0;
                    pass <= 1;
                    state <= 4'b0001;
                end
                else if(keyboard[5:1] == acc1[17:13] && acc1[0] == 1) begin
                    temp <= acc1[12:1];
                    state <= 4'b0001;
                    fail <= 0;
                    pass <= 1;
                end
                else if(keyboard[5:1] == acc2[17:13] && acc2[0] == 1) begin
                    temp <= acc2[12:1];
                    fail <= 0;
                    pass <= 1;
                    state <= 4'b0001;
                end
                else if(keyboard[5:1] == acc3[17:13] && acc3[0] == 1) begin
                    temp <= acc3[12:1];
                    fail <= 0;
                    pass <= 1;
                    state <= 4'b0001;
                end
                else begin
                    temp <= 12'bx;
                    inputPIN <= 4'bx;
                    state <= 4'b0101;
                    fail <= 1;
                    pass <= 0;
                end
            end

            // State 1 - Enter PIN - Attempt 1
            if(state == 4'b0001) begin
                if(keyboard[5:2] == temp[11:8]) begin
                    fail <= 0;
                    pass <= 1;
                    state <= 4'b0100;
                end
                else begin
                    fail <= 1;
                    pass <= 0;
                    state <= 4'b0010;
                end
            end
            
            // State 2 - Enter PIN - Attempt 2
            if(state == 4'b0010) begin
                if(keyboard[5:2] == temp[11:8]) begin
                    fail <= 0;
                    pass <= 1;
                    state <= 4'b0100;
                end
                else begin
                    fail <= 1;
                    pass <= 0;
                    state <= 4'b0011;
                end
            end
            
            // State 3 - Enter PIN - Attempt 3
            if(state == 4'b0011) begin
                if(keyboard[5:2] == temp[11:8]) begin
                    fail <= 0;
                    pass <= 1;
                    state <= 4'b0100;
                end
                else begin
                    upd1 <= {1'b1,temp[1:0]};
                    state <= 4'b1011;
                    fail <= 1;
                    pass <= 0;
                end
            end
            
            // State 11 - Account Locked
            if(state == 4'b1011) begin
                inputPIN <= 4'bx;
                state <= 4'b0000;
                fail <= 1;
                pass <= 0;
            end
            
            // State 5 - Error State
            if(state == 4'b0101) begin
                state <= 4'b0000;
                fail <= 0;
                pass <= 0;
            end

            // State 4 - Select Banking Service
            if(state == 4'b0100) begin
                if(keyboard[5:4] == 2'b00) begin
                    state <= 4'b1100;
                    bal_disp <= temp[7:2];
                end
                else if(keyboard[5:4] == 2'b01) begin
                    state <= 4'b0110;
                end
                else if(keyboard[5:4] == 2'b10) begin
                    state <= 4'b0111;
                end
                else if(keyboard[5:4] == 2'b11) begin
                    state <= 4'b1000;
                end
            end
            
            // State 6 - Enter New PIN
            if(state==4'b0110) begin
                temp1<=keyboard[5:2];
                state<=1001;
            end

            // State 9 - Confirm New PIN
            if(state==4'b1001) begin
                if(keyboard[5:2]==temp1) begin
                    fail<=0;
                    pass<=1;
                    upd2<={1'b1,temp[1:0]};
                    state<=4'b1100;
                end
                else begin
                    state<=4'b0110;
                    fail<=1;
                    pass<=0;
                end
            end

            // State 12 - Success State
            if(state==4'b1100) begin
                pass<=0;
                fail<=0;
                state<=4'b0000;
                bal_disp<=6'b000000;
            end

            // State 7 - Cash Withdrawal - Enter Amount
            if(state==4'b0111) begin
                if(keyboard[5:0]<temp[7:2]) begin
                    temp[7:2]<=temp[7:2]-keyboard[5:0];
                    bal_disp[5:0]<=(temp[7:2]-keyboard[5:0]);
                    upd3<={1'b1,temp[1:0]};
                    state<=4'b1100;
                    pass<=1;
                    fail<=0;
                end
                else begin
                    state<=4'b0101;
                    pass<=0;
                    fail<=1;
                end
            end

            // State 8 - Cash Deposit - Give Cash
            if(state==4'b1000) begin
                temp2<=keyboard[5:0];
                state<=4'b1010;
            end

            // State 10 - Cash Deposit - Confirm Cash
            if(state==4'b1010) begin
                if(keyboard==temp2) begin
                    temp[7:2] <= temp[7:2]+keyboard[5:0];
                    upd3 <= {1'b1,temp[1:0]};
                    state <= 4'b1100;
                    bal_disp <= temp[7:2]+keyboard[5:0];
                    fail <= 0;
                    pass <= 1;
                end
                else begin
                    state<=4'b0101;
                    pass<=0;
                    fail<=1;
                end
            end
        end
    end

    // Waveforms for account number, entered PIN
    always@(keyboard) begin
        if(state == 4'b0000) begin
            curAcc = keyboard[5:1];
        end

        if(state == 4'b0001 || state == 4'b0010 || state == 4'b0011) begin
            inputPIN <= keyboard[5:2];
        end

        if(state == 4'b1011) begin
            inputPIN <= 4'bx;
        end
    end

    // Update Account State - To Lock an Account
    always@(upd1) begin
        if(upd1 == 3'b100) begin
            acc4[0] = 0;
        end
        else if(upd1[1:0] == 2'b01) begin
            acc1[0] = 0;
        end
        else if(upd1[1:0] == 2'b10) begin
            acc2[0] = 0;
        end
        else if(upd1[1:0] == 2'b11) begin
            acc3[0] = 0;
        end
    end

    // Update PIN for an account
    always@(upd2) begin
        if(upd2 == 3'b100) begin
            acc4[12:9] <= temp1;
        end
        else if(upd2[1:0] == 2'b01) begin
            acc1[12:9] <= temp1;
        end
        else if(upd2[1:0] == 2'b10) begin
            acc2[12:9] <= temp1;
        end
        else if(upd2[1:0] == 2'b11) begin
            acc3[12:9] <= temp1;
        end
    end

    // Update Balance when cash is withdrawn or deposited
    always@(upd3) begin
        if(upd3 == 3'b100) begin
            acc4[8:3] <= temp[7:2];
        end
        else if(upd3[1:0] == 2'b01) begin
            acc1[8:3] <= temp[7:2];
        end
        else if(upd3[1:0] == 2'b10) begin
            acc2[8:3] <= temp[7:2];
        end
        else if(upd3[1:0]==2'b11) begin
            acc3[8:3] <= temp[7:2];
        end
    end
endmodule

