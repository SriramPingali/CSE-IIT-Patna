
module tb_add_sub();
reg [3:0] A, B; // Declaration of two four-bit inputs
reg sel; // and the one-bit input carry
wire [3:0] s; // Declaration of the five-bit outputs
wire  v;  // internal carry wires
add_sub DUT (V,s,cout,sel,A,B);

initial
begin
#10 A=4'b0000;B=4'b0001;sel=1'b0;
#10 A=4'b0001;B=4'b0001;sel=1'b0;
#10 A=4'b0010;B=4'b0001;sel=1'b1;
#10 A=4'b0100;B=4'b0001;sel=1'b1;
#10 A=4'b0010;B=4'b0001;sel=1'b0;
#10 A=4'b0100;B=4'b0001;sel=1'b0;
#10 A=4'b0111;B=4'b0111;sel=1'b0;  //overflow case
end
endmodule



