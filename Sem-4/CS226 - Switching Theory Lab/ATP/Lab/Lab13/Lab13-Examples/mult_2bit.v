module mult_2bit(p,x,y); 
parameter N = 2;
input  [N-1:0] x, y;
output [2*N-1:0] p;
wire a1_o,a2_o, a3_o,a4_o;
and a1 (p[0],x[0],y[0]);
and a2 (a2_o,x[1],y[0]);
and a3 (a3_o, x[0],y[1]);
and a4 (a4_o,x[1],y[1]);

half_adder H1(p[1],h1_o, a2_o, a3_o);
half_adder H2(p[2],p[3], h1_o,a4_o);
 endmodule



module half_adder(s,c,a,b);
input a,b;
output s,c;

assign s= a ^b;
assign c= a &b;
endmodule



















































