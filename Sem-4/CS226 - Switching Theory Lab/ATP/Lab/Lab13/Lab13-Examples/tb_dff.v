

module tb_dff();
reg d,reset; // Declaration of input
reg clk=0;

dff  DUT (q,d,clk,reset); 

always #5 clk = ~clk;

initial
begin
reset=1'b0;
#1 d=1'b1; reset=1'b1;
#5 d=1'b1; reset=1'b1;
#5 d=1'b0; reset=1'b1;
#6  d=1'b1; reset=1'b1;
#10  d=1'b0; reset=1'b1;
end
endmodule

