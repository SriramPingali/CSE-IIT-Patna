module counter (C_OUT,CLK,reset);	
  output 	[3: 0] 	C_OUT;
  input 	CLK,reset;
  reg 	[3:0] 	C_OUT;

  always @ (posedge CLK)
  begin
  if (reset) C_OUT <= 4'b0000;
  else 
  C_OUT <= C_OUT + 4'b0001;
  end
  endmodule
  
  
  