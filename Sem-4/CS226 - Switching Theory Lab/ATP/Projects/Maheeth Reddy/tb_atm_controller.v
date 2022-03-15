// Testbench for ATM Controller module
// Simulation Time : 950ns
module tb_ATM_controller();
	// Inputs
	reg [5:0] keyboard;
	reg reset,enter;

	// Outputs
	wire pass,fail;
	wire [5:0] bal_disp;

	// Instantiate ATM Controller Module
	ATM_controller UUT (
		.keyboard(keyboard),
        .reset(reset),
        .enter(enter),
        .fail(fail),
        .pass(pass),
        .bal_disp(bal_disp)
	);

    // Waveform display
	wire [3:0] state = UUT.state;
	wire [4:0] curAcc = UUT.curAcc;
	wire [3:0] validPIN = UUT.temp[11:8];
	wire [3:0] inputPIN = UUT.inputPIN;
    
	// 1 clock cycle is 40ns
    always #20 enter = ~enter;
	
	initial begin
		// Initialize Inputs
		keyboard = 0;
		reset = 0;
		enter = 0;

		#10;	//10
		// Account Number
		keyboard = {5'b00001,1'b0};
		#40;	//50
		// PIN - Attempt 1
		keyboard = {4'b0001,2'b0};
		#40;	//90
		// Authentication Successful
		// Select Banking Service
		keyboard = {2'b00,4'b0};
		// Balance is displayed
		#40;	//130
		

		// Account number
		keyboard = {5'b00001,1'b0};
		#60;	//190
		// PIN - Attempt 1
		keyboard = {4'b1000,2'b0};
		#40;	//230
		// Authentication Failed
		// PIN - Attempt 2
		keyboard = {4'b0001,2'b0};
		#40;	//270
		// Authentication Successful
		// Select Banking Service
		keyboard = {2'b10,4'b0};
		#40;	//310
		// Cash Withdrawal - Enter Amount
		keyboard = 6'b000011;
		#100;	//410
		// Amount is deducted
		// New Balance is displayed


		// Account number
		keyboard = {5'b00001,1'b0};
		#40;	//450
		// PIN - Attempt 1
		keyboard = {4'b0001,2'b0};
		#40;	//490
		// Authentication Successful
		// Select Banking Service
		keyboard = {2'b01,4'b0};
		#40;	//530
		// Change PIN - Enter New PIN
		keyboard = {4'b0110,2'b0};
		#40;	//570
		// Confirm New PIN
		keyboard = {4'b0110,2'b0};
		#60;	//630


		// Account Number 
		keyboard = {5'b10001,1'b0};
		#80;	//710
		// Account not in database

		
		// Account Number
		keyboard = {5'b00001,1'b0};
		#60;	//770
		// Enter PIN - Attempt 1
		keyboard = {4'b0110,2'b0};
		#40;	//810
		// Authentication Successful
		// Select Banking Service
		keyboard = {2'b11,4'b0};
		#40;	//850
		// Cash Deposit - Give Cash
		keyboard = 6'b000101;
		#40;	//890
		// Confirm Cash
		keyboard = 6'b000101;
		#40;	//930
		// Amount is deposited
		// New Balance is displayed
	end
endmodule

