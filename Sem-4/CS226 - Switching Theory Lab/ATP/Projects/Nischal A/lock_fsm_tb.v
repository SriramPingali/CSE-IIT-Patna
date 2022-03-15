module test_lock_fsm();
reg clk = 0;
reg [2:0] lock_key = 3'b110;
wire valid;
reg reset, enter, keyin;
wire unlock, error;
wire [3:0] state;
//reding for validity
read_valid read(valid);
//actual program
lock_fsm lock_obj(enter, keyin, reset,
		 valid, lock_key, clk,
		 unlock, error, state);
always #5 clk = ~clk;
initial
begin
reset = 1;
#10;
reset = 0; enter = 1;
#10;
keyin = 1; enter = 0;
#10;
enter = 1;
#10;
keyin = 1; enter = 0;
#10;
enter = 1;
#10;
keyin = 0; enter = 0;
#10;
enter = 1;
end
endmodule
