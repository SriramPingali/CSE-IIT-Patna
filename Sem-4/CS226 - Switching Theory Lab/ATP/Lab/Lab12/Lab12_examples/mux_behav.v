module mux_behav(out, in1,in2,sel );  //you list all inputs and outputs, by convention outputs go first
output out;                          // this tells the compile which lines are inputs and outputs 
input in1,in2,sel;
reg out;
always@(in1 or in2 or sel)
begin
	if (sel) 
out =in1;
           else	
	out=in2;
end

endmodule
