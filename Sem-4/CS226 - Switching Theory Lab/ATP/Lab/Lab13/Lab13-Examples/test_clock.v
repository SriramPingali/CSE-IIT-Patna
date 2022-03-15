
module test_clock();
parameter half_cycle = 50; 
reg clock;
initial 
begin: clock_label // Note: clock_loop is a named block of statements 
//clock = 0; 
forever 
begin 
#half_cycle clock = 1; 
#half_cycle clock = 0; 
end 
end 
// always #100 clock = ~clock;  another way of declaring clock  
endmodule
