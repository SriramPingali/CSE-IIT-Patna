module tb_or_gate();   // Test bench for and_gate.v
reg a,b;                         // a reg, to allow us to assign the input, and a wire to receive the output
wire out;
or_gate uut (out,a,b); // this instantiates a and gate, uut is a label 
initial
begin
a = 1'b0;                   // here we apply inputs to the gate
b = 1'b0;
#10;
a = 1'b0;
b = 1'b1;
#10;
a = 1'b1;
b = 1'b1;
#10;
a = 1'b1;
b = 1'b0;
#10;
end
// set up the monitoring
initial
begin
$display("a   b   out time=%t",$time);
//$monitor("a=%b, b=%b, out=%b, time=%t\n", a, b, out, $time);
$monitor("%b, %b, %b, time=%t\n", a, b, out, $time);
end
endmodule
