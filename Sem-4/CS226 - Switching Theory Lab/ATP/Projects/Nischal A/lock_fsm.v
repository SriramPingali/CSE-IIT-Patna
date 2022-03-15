module lock_fsm (enter, keyin, reset,
		 valid, lock_key, clk,
		 unlock, error, state);
input clk;
input [2:0] lock_key;
input reset, enter, keyin, valid;
output unlock, error;
output [3:0] state;
reg [3:0] state, next_state;
parameter [3:0] START = 4'b0000;
parameter [3:0] cmp0 = 4'b0001;
parameter [3:0] idle0 = 4'b0010;
parameter [3:0] idle0x = 4'b0011;
parameter [3:0] error1 = 4'b0100;
parameter [3:0] cmp1 = 4'b0101;
parameter [3:0] idle1 = 4'b0110;
parameter [3:0] idle1x = 4'b0111;
parameter [3:0] error2 = 4'b1000;
parameter [3:0] cmp2 = 4'b1001;
parameter [3:0] idle2 = 4'b1010;
parameter [3:0] idle2x = 4'b1011;
parameter [3:0] error3 = 4'b1100;
parameter [3:0] done = 4'b1110;
always @ (posedge clk)
begin
if (reset & valid)
state <= START;
else if (valid)
state <= next_state;
end

not n_ent (n_enter, enter);
not n_res (n_reset, reset);

always @ *
begin
case (state)
START:
if (reset  & n_enter)
next_state <= START;
else if (n_reset & enter)
next_state <= cmp0;
cmp0:
if (keyin == lock_key[2])
next_state <= idle0;
else
next_state <= idle0x;
idle0:
if (n_enter)
next_state <= idle0;
else
next_state <= cmp1;
idle0x:
if (n_enter)
next_state <= idle0x;
else
next_state <= error1;
error1:
next_state <= idle1x;
cmp1:
if (keyin == lock_key[1])
next_state <= idle1;
else
next_state <= idle1x;
idle1:
if (n_enter)
next_state <= idle1;
else
next_state <= cmp2;
idle1x:
if (n_enter)
next_state <= idle1x;
else
next_state <= error2;
error2:
next_state <= idle2x;
cmp2:
if (keyin == lock_key[0])
next_state <= idle2;
else
next_state <= idle2x;
idle2:
if (n_enter)
next_state <= idle2;
else
next_state <= done;
idle2x:
if (n_enter)
next_state <= idle2x;
else
next_state <= error3;
endcase
end
assign unlock = (state == done);
assign error = (state == error3);
endmodule
