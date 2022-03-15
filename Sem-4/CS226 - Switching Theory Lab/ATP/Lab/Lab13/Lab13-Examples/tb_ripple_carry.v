
module tb_ripple_carry();
reg [3:0] A, B; // Declaration of two four-bit inputs
reg cin; // and the one-bit input carry
wire [3:0] s; // Declaration of the five-bit outputs
wire  cout;  // internal carry wires
ripple_carry DUT (s,cout,cin,A,B);

initial
begin
#10 A=4'b0000;B=4'b0001;cin=1'b0;
#10 A=4'b0001;B=4'b0001;cin=1'b0;
#10 A=4'b0010;B=4'b0001;cin=1'b1;
#10 A=4'b0100;B=4'b0001;cin=1'b1;
end
endmodule


