// Testbench for p7.v
module tb_p7();
  reg s1, s0, d3, d2, d1, d0;
  wire op1,op2,op3,op4,op5;
  
  // Initialize MUX's of all coding styles
  mux1 uut1(op1, s1, s0, d3, d2, d1, d0);
  mux2 uut2(op2, s1, s0, d3, d2, d1, d0);
  mux3 uut3(op3, s1, s0, d3, d2, d1, d0);
  mux4 uut4(op4, s1, s0, d3, d2, d1, d0);
  mux5 uut5(op5, s1, s0, d3, d2, d1, d0);
  
  initial 
    begin
		d0 = 0;
        d1 = 0;
        d2 = 0;
        d3 = 0;
		
        s1 = 0;
		s0 = 0;
		#10;
        d0 = 1;
        #10;
        s1 = 0;
        s0 = 1;
        #10
        d1 = 1;
        #10;
        s1 = 1;
        s0 = 0;
        #10;
        d2 = 1;
        #10;
        s1 = 1;
        s0 = 1;
        #10;
        d3 = 1;
	end
    
    // Display MUXes output for all cases
    initial begin
        $monitor("d3=%b d2=%b d1=%b d0=%b s1=%b s0=%b\nop1=%b op2=%b op3=%b op4=%b op5=%b\n--------",d3,d2,d1,d0,s1,s0,op1,op2,op3,op4,op5);
    end
endmodule