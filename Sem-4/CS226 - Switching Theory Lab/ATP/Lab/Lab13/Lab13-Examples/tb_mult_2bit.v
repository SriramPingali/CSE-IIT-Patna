

module tb_mult_2bit();
parameter N = 2;
reg [N-1:0] x, y;
wire [2*N-1:0] p;
mult_2bit uut(p,x,y); //wire the multiplier
initial
begin
x=2'b00;y=2'b00; #5;
x=2'b01;y=2'b01; #5;
x=2'b10;y=2'b10; #5;
x=2'b11;y=2'b11; #5;
end
endmodule



















































