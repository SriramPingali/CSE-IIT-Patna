module tb_mux_behav();  
                        
reg in1,in2,sel;
wire out;  

mux_behav UUT (out,in1,in2,sel);

initial
begin
in1 = 1'b0;                   // here we apply inputs to the logic
in2 = 1'b0;
sel = 1'b0;
#10;
in1 = 1'b0; 
in2 = 1'b1;
sel = 1'b0;
#10;
in1 = 1'b1;
in2 = 1'b0;
sel = 1'b0;
#10;
in1 = 1'b1;
in2 = 1'b1;
sel = 1'b0;
#10;
in1 = 1'b0;                   
in2 = 1'b0;
sel = 1'b1;
#10;
in1 = 1'b0; 
in2 = 1'b1;
sel = 1'b1;
#10;
in1 = 1'b1;
in2 = 1'b0;
sel = 1'b1;
#10;
in1 = 1'b1;
in2 = 1'b1;
sel = 1'b1;
#10;
 
end

// set up the monitoring
initial
begin
$monitor("sel=%b, in1=%b, in2=%b, out=%b, time=%t\n", sel, in1,in1, out, $time);
end


endmodule
